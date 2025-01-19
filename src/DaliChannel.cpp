#include "DaliChannel.h"
#include "OpenKNX.h"

DaliChannel::DaliChannel(MessageQueue &queue) : _queue(queue) {}

DaliChannel::~DaliChannel() {}

const std::string DaliChannel::name()
{
    if (_isStaircase)
        if (_isGroup)
            return "StaircaseChannel_G";
        else
            return "StaircaseChannel_A";

    if (_isGroup)
        return "StandardChannel_G";
    return "StandardChannel_A";
}

const bool DaliChannel::isConfigured()
{
    return _isConfigured;
}

const bool DaliChannel::isGroup()
{
    return _isGroup;
}

bool DaliChannel::hasError()
{
    return _errorState;
}

void DaliChannel::init(uint8_t channelIndex, bool ig)
{
    _channelIndex = channelIndex;
    _isGroup = ig;
}

// will be called once
// only if knx.configured == true
void DaliChannel::setup()
{
    if (_isGroup)
    {
        _isConfigured = ParamGRP_deviceType != PT_groupType_none;
        if (!_isConfigured)
            return;

        _isStaircase = ParamGRP_type;
        _min = 0;
        _max = 100;
        if (_isStaircase)
            interval = ParamGRP_stairtime;
        _onDay = DaliHelper::percentToArc((float)ParamGRP_onDay);
        _onNight = DaliHelper::percentToArc((float)ParamGRP_onNight);
        _dimmStatusInterval = ParamGRP_dimmStateInterval;
        _dimmInterval = (uint8_t)(ParamGRP_dimmRelDuration / 2.55);
        if(ParamGRP_hcl)
        {
            _hclCurve = ParamGRP_hclCurve;
            _hclIsAlsoOn = ParamGRP_hclStart;
        }
    }
    else
    {
        _isConfigured = ParamADR_deviceType != PT_deviceType_Deaktiviert;
        if (!_isConfigured)
            return;

        _isStaircase = ParamADR_type;
        _min = ParamADR_min;
        _max = ParamADR_max;
        if (_isStaircase)
            interval = ParamADR_stairtime;
        _onDay = DaliHelper::percentToArc((float)ParamADR_onDay);
        _onNight = DaliHelper::percentToArc((float)ParamADR_onNight);
        _getError = ParamADR_error;
        _queryInterval = ParamADR_queryTime;
        _dimmStatusInterval = ParamADR_dimmStateInterval;
        _dimmInterval = (uint8_t)(ParamADR_dimmRelDuration / 2.55);
        if(ParamADR_hcl)
        {
            _hclCurve = ParamADR_hclCurve;
            _hclIsAlsoOn = ParamADR_hclStart;
        }
    }
    _min = DaliHelper::percentToArc(_min);
    _max = DaliHelper::percentToArc(_max);
    logDebugP("Min/Max %i/%i | D/N %i/%i (%.2f/%.2f) | TRH %is | Err %i | Q %is | Dimm %i/%i", _min, _max, _onDay, _onNight, DaliHelper::arcToPercentFloat(_onDay), DaliHelper::arcToPercentFloat(_onNight), interval, _getError, _queryInterval, _dimmInterval, _dimmStatusInterval);
}

void DaliChannel::loop()
{
    if(_hclIsAutoMode != _hclLastState)
    {
        _hclLastState = _hclIsAutoMode;
        logDebugP("HCL Mode: %s", _hclIsAutoMode ? "Auto" : "Manu");
    }
}

void DaliChannel::loop1()
{
    if (!_isConfigured)
        return;

    loopStaircase();
    loopDimming();
    loopError();
    loopQueryLevel();
}

void DaliChannel::loopStaircase()
{
    if (_isStaircase && currentState)
    {
        if (millis() - startTime > interval * 1000)
        {
            logDebugP("Zeit abgelaufen");
            currentState = false;
            sendArc(0x00);
            setSwitchState(false);
        }
    }
}

void DaliChannel::loopDimming()
{
    if (_dimmDirection != DimmDirection::None)
    {
        if(millis() - _dimmLast > _dimmInterval)
        {
            if (_dimmDirection == DimmDirection::Up)
            {
                if (currentDimmType == DimmType::Brigthness)
                    sendCmd(DaliCmd::ON_AND_STEP_UP);
                *currentDimmValue = *currentDimmValue + 1;
                if (*currentDimmValue == 254)
                {
                    logDebugP("Dimm Stop at 254");
                    _dimmDirection = DimmDirection::None;
                    updateCurrentDimmValue();
                }else if (*currentDimmValue >= _max)
                {
                    logDebugP("Dimm Stop at max %i (%i)", _max, *currentDimmValue);
                    _dimmDirection = DimmDirection::None;
                    updateCurrentDimmValue();
                }
            }
            else if (_dimmDirection == DimmDirection::Down)
            {
                if (currentDimmType == DimmType::Brigthness)
                {
                    uint8_t dimmLock = _isGroup ? ParamGRP_dimmLock : ParamADR_dimmLock;
                    if(dimmLock == PT_dimmLock_noBoth || dimmLock == PT_dimmLock_noOn)
                    {
                        sendCmd(DaliCmd::STEP_DOWN);
                    } else {
                        sendCmd(DaliCmd::STEP_DOWN_AND_OFF);
                    }
                }
                    
                *currentDimmValue = *currentDimmValue - 1;

                if (*currentDimmValue == 0)
                {
                    logDebugP("Dimm Stop at 0");
                    _dimmDirection = DimmDirection::None;
                    updateCurrentDimmValue();
                }
                if (*currentDimmValue <= _min)
                {
                    logDebugP("Dimm Stop at min %i", _min);
                    _dimmDirection = DimmDirection::None;
                    updateCurrentDimmValue();
                }
            }
            
            _dimmLast = millis();
        }

        if(_dimmStatusInterval != 0 && millis() - _dimmLastStatus > (_dimmStatusInterval*100))
        {
            _dimmLastStatus = millis();
            updateCurrentDimmValue();
        }
    }
}

void DaliChannel::loopError()
{
    if (!_isGroup && _getError)
    {
        if (_errorResp != 300)
        {
            int16_t resp = _queue.getResponse(_errorResp);
            if (resp == -200)
                return;
            if (resp == -1)
                resp = 1;
                logErrorP("EVG hat Anwtort %i %i", resp, _errorResp);
            _errorResp = 300;
            if (resp < 0)
                return;

            resp = resp & 0b11;
            bool val = knx.getGroupObject(calcKoNumber(ADR_Koerror)).value(Dpt(1, 1));
            _errorState = val != 0;
            if (val != resp)
                knx.getGroupObject(calcKoNumber(ADR_Koerror)).value((val != 0), DPT_Switch);

            if(_errorState)
            {
                logErrorP("EVG hat ein Fehler");
            }
        }

        if (millis() - _lastError > 60000)
        {
            _errorResp = sendCmd(DaliCmd::QUERY_STATUS, true);
            _lastError = millis();
            logDebugP("EVG abfragen %i", _errorResp);
        }
    }
}

void DaliChannel::loopQueryLevel()
{
    if(_queryInterval == 0) return;

    if(_queryId == 0 && (millis() - _lastValueQuery) > (_queryInterval*1000))
    {
        logDebugP("Query actual level");
        _lastValueQuery = millis();
        if(_lastValueQuery == 0) _lastValueQuery++;

        _queryId = sendCmd(DaliCmd::QUERY_ACTUAL_LEVEL, true);
        logDebugP("id: %i", _queryId);
        return;
    }
    if(_queryId != 0)
    {
        int16_t resp = _queue.getResponse(_queryId);
        if(millis() - _lastValueQuery > 500)
        {
            logErrorP("Got no answer %i", _queryId);
            _queryId = 0;
            return;
        }
        if(resp == -200) return;
        logDebugP("answer: %i", resp);
        _queryId = 0;
        if(resp < 0) return;
        logDebugP("Got new actual level %i-%i", DaliHelper::arcToPercent(resp), resp);
        setDimmState(resp, false, true);
    }
}

uint16_t DaliChannel::calcKoNumber(int asap)
{
    if (_isGroup)
        return asap + (GRP_KoBlockSize * _channelIndex) + GRP_KoOffset;

    return asap + (ADR_KoBlockSize * _channelIndex) + ADR_KoOffset;
}

uint8_t DaliChannel::sendArc(byte v)
{
    Message *msg = new Message();
    msg->id = _queue.getNextId();
    msg->type = MessageType::Arc;
    msg->para1 = _channelIndex;
    msg->para2 = v;
    msg->addrtype = _isGroup;
    return _queue.push(msg);
}

uint8_t DaliChannel::sendCmd(byte cmd, bool wait)
{
    Message *msg = new Message();
    msg->id = _queue.getNextId();
    msg->type = MessageType::Cmd;
    msg->para1 = _channelIndex;
    msg->para2 = cmd;
    msg->addrtype = _isGroup;
    msg->wait = wait;
    return _queue.push(msg);
}

uint8_t DaliChannel::sendSpecialCmd(DaliSpecialCmd cmd, byte value)
{
    Message *msg = new Message();
    msg->id = _queue.getNextId();
    msg->type = MessageType::SpecialCmd;
    msg->para1 = static_cast<uint8_t>(cmd);
    msg->para2 = value;
    msg->addrtype = _isGroup;
    return _queue.push(msg);
}

void DaliChannel::processInputKo(GroupObject &ko)
{
    int chanIndex = 0;
    if (_isGroup)
    {
        chanIndex = (ko.asap() - GRP_KoOffset) % GRP_KoBlockSize;
        //logDebugP("Got GROUP KO %i", chanIndex);
    }
    else
    {
        chanIndex = (ko.asap() - ADR_KoOffset) % ADR_KoBlockSize;
        //logDebugP("Got SHORT KO %i", chanIndex);
    }

    switch (chanIndex)
    {
    // Schalten
    case ADR_Koswitch:
        koHandleSwitch(ko);
        break;

    // Schalten Status
    // case 1

    // Dimmen relativ
    case ADR_Kodimm_relative:
        koHandleDimmRel(ko);
        break;

    // Dimmen Absolut
    case ADR_Kodimm_absolute:
        koHandleDimmAbs(ko);
        break;

    // Dimmen Status
    // case 4

    // Sperren
    case ADR_Kolock:
        koHandleLock(ko);
        break;

    case ADR_Kocolor:
        koHandleColor(ko);
        break;

    // Farbe Status
    // case 7

    // Farbe Rot Dimmen relativ
    case ADR_Kocolor_red_relative:
        koHandleColorRel(ko, 0);
        break;

    // Farbe Rot Dimmen absolut
    case ADR_Kocolor_red_absolute:
        koHandleColorAbs(ko, 0);
        break;

    // Farbe Rot Status
    // case 10

    // Farbe Grün Dimmen relativ
    case ADR_Kocolor_green_relative:
        koHandleColorRel(ko, 1);
        break;

    // Farbe Grün Dimmen absolut
    case ADR_Kocolor_green_absolute:
        koHandleColorAbs(ko, 1);
        break;

    // Farbe Grün Status
    // case 13

    // Farbe Blau Dimmen relativ
    case ADR_Kocolor_blue_relative:
        koHandleColorRel(ko, 2);
        break;

    // Farbe Blau Dimmen absolut
    case ADR_Kocolor_blue_absolute:
        koHandleColorAbs(ko, 2);
        break;

    // Farbe Blau Status
    // case 16

    case ADR_Kohcl_curve:
        koHandleHclCurve(ko);
        break;

    case ADR_Koscene:
        koHandleScene(ko);
        break;
        
    // Error
    // case 19
    }
}

void DaliChannel::koHandleHclCurve(GroupObject &ko)
{
    uint8_t curve = ko.value(Dpt(5,1));
    if(curve > 2)
    {
        logErrorP("Setzen der HCL Kurve ignoriert, da zu hoch: %i, max %i", curve, 2);
        return;
    }
    _hclCurve = curve;
}

void DaliChannel::koHandleScene(GroupObject &ko)
{
    uint8_t number = ko.value(Dpt(17,1));
    logDebugP("Szene KNX %i to DALI %i", number + 1, number);
    if(number > 15)
    {
        logErrorP("Szene ignoriert, da zu hoch: %i, max 15", number);
        return;
    }

    sendCmd(DaliCmd::GO_TO_SCENE | number);
}

void DaliChannel::koHandleColorRel(GroupObject &ko, uint8_t index)
{
    logDebugP("Farbe relativ %i", index);
    if (currentIsLocked)
    {
        logErrorP("is locked");
        return;
    }

    uint8_t dimmLock = _isGroup ? ParamGRP_dimmLock : ParamADR_dimmLock;
    if(dimmLock == PT_dimmLock_noBoth || dimmLock == PT_dimmLock_noOn)
    {
        logDebugP("ignored due settings");
        return;
    }

    if(_isGroup ? ParamGRP_hcl_manu_col : ParamADR_hcl_manu_col)
        _hclIsAutoMode = false;

    _dimmStep = ko.value(Dpt(3, 7, 1));

    if (_dimmStep == 0)
    {
        logDebugP("Dimm Stop");
        _dimmDirection = DimmDirection::None;
        _dimmLast = 0;
        updateCurrentDimmValue();
        return;
    }

    currentDimmValue = &currentStep;
    currentDimmType = DimmType::Color;
    _dimmLastStatus = millis();
    _dimmDirection = ko.value(Dpt(3, 7, 0)) ? DimmDirection::Up : DimmDirection::Down;
    if (_dimmDirection == DimmDirection::Up)
    {
        logDebugP("Dimm Up Start %i/%i", currentStep, *currentDimmValue);
    }
    else if (_dimmDirection == DimmDirection::Down)
    {
        logDebugP("Dimm Down Start %i/%i", currentStep, *currentDimmValue);
    }
}

void DaliChannel::koHandleColorAbs(GroupObject &ko, uint8_t index)
{
    logDebugP("Farbe absolut %i", index);
    if (currentIsLocked)
    {
        logErrorP("is locked");
        return;
    }

    if(_isGroup ? ParamADR_hcl_manu_col : ParamADR_hcl_manu_col)
        _hclIsAutoMode = false;
        
    logDebugP("AutoSwitchConfig %i %i", ParamADR_hcl_manu_col, _hclIsAutoMode);

    currentColor[index] = ko.value(Dpt(5, 4));
    updateCurrentDimmValue();
    sendColor();
    logDebugP("AutoSwitchConfig2 %i %i", ParamADR_hcl_manu_col, _hclIsAutoMode);
}

void DaliChannel::koHandleSwitch(GroupObject &ko)
{
    logDebugP("Schalten");
    if (currentIsLocked)
    {
        logErrorP("is locked");
        return;
    }

    if (_isStaircase)
        handleSwitchStaircase(ko);
    else
        handleSwitchNormal(ko);
}

void DaliChannel::handleSwitchNormal(GroupObject &ko)
{
    bool value = ko.value(DPT_Switch);
    if (value)
    {
        uint8_t onValue = isNight ? _onNight : _onDay;
        if (onValue == 0)
            onValue = isNight ? _lastNightValue : _lastDayValue;
        logDebugP(isNight ? "Einschalten Nacht" : "Einschalten Tag");
        sendArc(onValue);
        if(_hclCurve != 255 && _hclIsAutoMode)
            setTemperature(_hclCurrentTemp);
        setDimmState(onValue, true);
    }
    else
    {
        logDebugP("Ausschalten");
        sendArc(0x00);
        setSwitchState(false);
    }
    currentState = value;
}

void DaliChannel::handleSwitchStaircase(GroupObject &ko)
{
    if (ko.value(DPT_Switch))
    {
        if (currentState)
        {
            logDebugP("ist bereits an");

            bool retrigger = _isGroup ? ParamGRP_manuoff : ParamADR_manuoff;
            if (retrigger)
            {
                logDebugP("wurde nachgetriggert");
                startTime = millis();
                return;
            }
            return;
        }
        logDebugP(isNight ? "Einschalten Nacht" : "Einschalten Tag");
        currentState = true;
        startTime = millis();
        logDebugP("interval %i", interval);

        uint8_t onValue = isNight ? _onNight : _onDay;
        if (onValue == 0)
            onValue = isNight ? _lastNightValue : _lastDayValue;
        sendArc(onValue);
        if(_hclCurve != 255 && _hclIsAutoMode)
            setTemperature(_hclCurrentTemp);
        setDimmState(DaliHelper::percentToArc(onValue));
    }
    else
    {
        bool manuOff = _isGroup ? ParamGRP_manuoff : ParamADR_manuoff;
        if (!manuOff)
        {
            logErrorP("no manuel off");
            return;
        }

        logDebugP("Ausschalten");
        sendArc(0x00);
        setSwitchState(false);
        currentState = false;
    }
}

void DaliChannel::koHandleDimmRel(GroupObject &ko)
{
    logDebugP("Dimmen relativ");
    if (currentIsLocked)
    {
        logErrorP("is locked");
        return;
    }

    uint8_t dimmLock = _isGroup ? ParamGRP_dimmLock : ParamADR_dimmLock;
    if(dimmLock == PT_dimmLock_noBoth || dimmLock == PT_dimmLock_noOn)
    {
        logDebugP("ignored due settings");
        return;
    }

    if(_isGroup ? ParamGRP_hcl_manu_bri : ParamADR_hcl_manu_bri)
        _hclIsAutoMode = false;

    _dimmStep = ko.value(Dpt(3, 7, 1));

    if (_dimmStep == 0)
    {
        logDebugP("Dimm Stop");
        _dimmDirection = DimmDirection::None;
        _dimmLast = 0;
        updateCurrentDimmValue();
        return;
    }

    currentDimmValue = &currentStep;
    currentDimmType = DimmType::Brigthness;
    _dimmLastStatus = millis();
    _dimmDirection = ko.value(Dpt(3, 7, 0)) ? DimmDirection::Up : DimmDirection::Down;
    if (_dimmDirection == DimmDirection::Up)
    {
        if(!currentState)
        {
            *currentDimmValue = _min;
            logDebugP("starting with min %i", _min);
        }
        logDebugP("Dimm Up Start %i/%i", currentStep, *currentDimmValue);
    }
    else if (_dimmDirection == DimmDirection::Down)
    {
        logDebugP("Dimm Down Start %i/%i", currentStep, *currentDimmValue);
    }
}

void DaliChannel::koHandleDimmAbs(GroupObject &ko)
{
    logDebugP("Dimmen absolut");
    if (currentIsLocked)
    {
        logErrorP("is locked");
        return;
    }

    if(_isGroup ? ParamGRP_hcl_manu_bri : ParamADR_hcl_manu_bri)
        _hclIsAutoMode = false;

    uint8_t value = ko.value(Dpt(5, 1));
    logDebugP("Dimmen Absolut auf %i%%", value);
    uint8_t arc = DaliHelper::percentToArc(value);
    sendArc(arc);
    setDimmState(arc, true, true);
}

void DaliChannel::koHandleLock(GroupObject &ko)
{
    bool value = ko.value(Dpt(1, 1));

    if (_isGroup)
    {
        if (ParamGRP_locknegate)
            value = !value;
    }
    else
    {
        if (ParamADR_locknegate)
            value = !value;
    }

    if (currentIsLocked == value)
        return;
    currentIsLocked = value;
    uint8_t behave;
    uint8_t behavevalue;
    if (currentIsLocked)
    {
        logDebugP("Sperren");
        behave = ParamADR_lockbehave;
        behavevalue = ParamADR_lockvalue;
    }
    else
    {
        logDebugP("Entsperren");
        behave = ParamADR_unlockbehave;
        behavevalue = ParamADR_unlockvalue;
    }

    switch (behave)
    {
    // nothing
    case PT_lock_no:
        logDebugP("Nichts");
        return;

    // Ausschalten
    case PT_lock_off:
    {
        logDebugP("Ein");
        behavevalue = isNight ? _onNight : _onDay;
        break;
    }

    // Einschalten
    case PT_lock_on:
    {
        logDebugP("Aus");
        behavevalue = 0;
        break;
    }

    // Fester Wert
    case PT_lock_value:
    {
        logDebugP("Wert");
        break;
    }
    }
    logDebugP("%i - %i", behavevalue, DaliHelper::percentToArc(behavevalue));
    sendArc(behavevalue);
    setDimmState(DaliHelper::percentToArc(behavevalue));
}

void DaliChannel::koHandleColor(GroupObject &ko)
{
    logDebugP("Farbe absolut");
    if (currentIsLocked)
    {
        logErrorP("is locked");
        return;
    }

    if(_isGroup ? ParamGRP_hcl_manu_col : ParamADR_hcl_manu_col)
        _hclIsAutoMode = false;

    logDebugP("AutoConf %i %i %i", _isGroup, _isGroup ? ParamGRP_hcl_manu_col : ParamADR_hcl_manu_col, _hclIsAutoMode);

    uint8_t colorType = _isGroup ? ParamGRP_colorType : ParamADR_colorType;

    switch(colorType)
    {
        //HSV
        case PT_colorType_HSV:
        {
            uint32_t value = ko.value(Dpt(232, 600));
            logDebugP("Got Color: %X", value);

            currentColor[0] = (value >> 16) & 0xFF;
            currentColor[1] = (value >> 8) & 0xFF;
            currentColor[2] = value & 0xFF;

            sendKoStateOnChange(ADR_Kocolor_rgb_state, value, Dpt(232, 600));
            sendKoStateOnChange(ADR_Kocolor_red_state, currentColor[0], Dpt(5, 4));
            sendKoStateOnChange(ADR_Kocolor_green_state, currentColor[1], Dpt(5, 4));
            sendKoStateOnChange(ADR_Kodimm_state, currentColor[2], Dpt(5, 4));

            sendColor();
            break;
        }

        //RGB
        case PT_colorType_RGB:
        {
            uint32_t value = ko.value(Dpt(232, 600));
            logDebugP("Got Color: %X", value);

            currentColor[0] = (value >> 16) & 0xFF;
            currentColor[1] = (value >> 8) & 0xFF;
            currentColor[2] = value & 0xFF;

            sendKoStateOnChange(ADR_Kocolor_rgb_state, value, Dpt(232, 600));
            sendKoStateOnChange(ADR_Kocolor_red_state, currentColor[0], Dpt(5, 4));
            sendKoStateOnChange(ADR_Kocolor_green_state, currentColor[1], Dpt(5, 4));
            sendKoStateOnChange(ADR_Kocolor_blue_state, currentColor[2], Dpt(5, 4));

            sendColor();
            break;
        }

        //TW
        case PT_colorType_TW:
        {
            uint16_t kelvin = ko.value(Dpt(7, 600));
            setTemperature(kelvin);
            break;
        }

        //xyY
        case PT_colorType_XYY:
        {
            uint8_t* data = ko.valueRef();
            /*
                <UnsignedInteger Id="DPST-242-600_F-1" Width="16" Name="x-axis" Unit="None" />
                <UnsignedInteger Id="DPST-242-600_F-2" Width="16" Name="y-axis" Unit="None" />
                <UnsignedInteger Id="DPST-242-600_F-3" Width="8" Name="brightness" Unit="%" />
                <Reserved Width="6" />
                <Bit Id="DPST-242-600_F-10" Cleared="invalid" Set="valid" Name="Validity xy" />
                <Bit Id="DPST-242-600_F-11" Cleared="invalid" Set="valid" Name="Validity brightness" />
            */
            uint16_t x = data[0] << 8 | data[1];
            uint16_t y = data[2] << 8 | data[3];
            uint8_t b = data[4];
            bool xyIgnore = _isGroup ? ParamGRP_xyIgnore : ParamADR_xyIgnore;
            if(xyIgnore)
            {
                pushWord(x, currentColor);
                pushWord(y, currentColor + 2);
                
                logDebugP("Got Color: %.4f %.4f", x / 65535.0, y / 65535.0);
            } else {
                ColorHelper::xyyToRGB(x, y, b, currentColor[0], currentColor[1], currentColor[2]);

                uint32_t value = currentColor[0] << 16 | currentColor[1] << 8 | currentColor[2];
                logDebugP("Got Color: %X", value);
            }


            sendColor();
            
            //TODO implement in Stack
            //sendKoStateOnChange(ADR_Kocolor_rgb_state, value, Dpt(242, 600), true);
            break;
        }
    }

    setDimmState(254, true, true); // TODO get real
    
    logDebugP("AutoConf %i %i %i", _isGroup, _isGroup ? ParamADR_hcl_manu_col : ParamGRP_hcl_manu_col, _hclIsAutoMode);
}

void DaliChannel::sendKoStateOnChange(uint16_t koNr, const KNXValue &value, const Dpt &type)
{
    GroupObject &ko = knx.getGroupObject(calcKoNumber(koNr));
    if(ko.valueNoSendCompare(value, type))
        ko.objectWritten();
}

void DaliChannel::setTemperature(uint16_t value)
{
    logDebugP("Set Kelvin: %i K", value);
    uint16_t mirek = 1000000.0 / value;
    //TODO check the colorType and then set RGB or TW or do nothing if it is no color Device
    sendSpecialCmd(DaliSpecialCmd::SET_DTR, mirek & 0xFF);
    //logDebugP("t5");
    sendSpecialCmd(DaliSpecialCmd::SET_DTR1, (mirek >> 8) & 0xFF);
    //logDebugP("t4");
    sendSpecialCmd(DaliSpecialCmd::ENABLE_DT, 8);
    //logDebugP("t3");
    sendCmd(DaliCmdExtendedDT8::SET_TEMP_COLOUR_TEMPERATURE);
    //logDebugP("t2");
    sendSpecialCmd(DaliSpecialCmd::ENABLE_DT, 8);
    //logDebugP("t1");
    sendCmd(DaliCmdExtendedDT8::ACTIVATE);

    sendKoStateOnChange(ADR_Kocolor_rgb_state, value, Dpt(7, 600));
}

void DaliChannel::setBrightness(uint8_t value)
{
    logDebugP("Set Brightness: %i %%", value);
    //TODO implement it...
    sendArc(DaliHelper::percentToArc(value));
    sendKoStateOnChange(ADR_Kodimm_state, value, Dpt(5, 1));
}

void DaliChannel::sendColor()
{
    // TODO senden nur alle 100? ms
    uint8_t r, g, b;
    uint8_t colorType = _isGroup ? ParamGRP_colorType : ParamADR_colorType;
    if (colorType == PT_colorType_HSV)
    {
        ColorHelper::hsvToRGB(currentColor[0], currentColor[1], currentColor[2], r, g, b);
    }
    else
    {
        r = currentColor[0];
        g = currentColor[1];
        b = currentColor[2];
    }

    logDebugP("RGB: %i %i %i", r, g, b);
    if (r == 255) r--;
    if (g == 255) g--;
    if (b == 255) b--;
    logDebugP("Send: %i %i %i", r, g, b);

    uint8_t sendType = _isGroup ? ParamGRP_colorSpace : ParamADR_colorSpace;
    switch (sendType)
    {
    // send as rgb
    case PT_colorSpace_rgb:
    {
        sendSpecialCmd(DaliSpecialCmd::SET_DTR, r);
        sendSpecialCmd(DaliSpecialCmd::SET_DTR1, g);
        sendSpecialCmd(DaliSpecialCmd::SET_DTR2, b);
        sendSpecialCmd(DaliSpecialCmd::ENABLE_DT, 8);
        sendCmd(DaliCmdExtendedDT8::SET_TEMP_RGB_LEVEL);
        sendSpecialCmd(DaliSpecialCmd::ENABLE_DT, 8);
        sendCmd(DaliCmdExtendedDT8::ACTIVATE);
        break;
    }

    // send as xy
    case PT_colorSpace_xy:
    {
        uint16_t x;
        uint16_t y;
        
        bool xyIgnore = _isGroup ? ParamGRP_xyIgnore : ParamADR_xyIgnore;
        if(xyIgnore)
        {
            popWord(x, currentColor);
            popWord(y, currentColor + 2);
        } else {
            ColorHelper::rgbToXY(r, g, b, x, y);
        }

        sendSpecialCmd(DaliSpecialCmd::SET_DTR, x & 0xFF);
        sendSpecialCmd(DaliSpecialCmd::SET_DTR1, (x >> 8) & 0xFF);
        sendSpecialCmd(DaliSpecialCmd::ENABLE_DT, 8);
        sendCmd(DaliCmdExtendedDT8::SET_COORDINATE_X);

        sendSpecialCmd(DaliSpecialCmd::SET_DTR, y & 0xFF);
        sendSpecialCmd(DaliSpecialCmd::SET_DTR1, (y >> 8) & 0xFF);
        sendSpecialCmd(DaliSpecialCmd::ENABLE_DT, 8);
        sendCmd(DaliCmdExtendedDT8::SET_COORDINATE_Y);

        sendSpecialCmd(DaliSpecialCmd::ENABLE_DT, 8);
        sendCmd(DaliCmdExtendedDT8::ACTIVATE);
        break;
    }
    }
}

void DaliChannel::setSwitchState(bool value, bool isSwitchCommand)
{
    if (isSwitchCommand)
    {
        uint8_t toSet = 0;
        if (value)
        {
            toSet = DaliHelper::percentToArc(isNight ? _onNight : _onDay);
        }
        else
        {
            toSet = 0;
        }

        setDimmState(toSet);
        currentStep = toSet;
    }

    logDebugP("AutoConfSwitch %i %i %i", value, ParamADR_hcl_auto_off, _hclIsAutoMode);
    if(!value && (_isGroup ? ParamGRP_hcl_auto_off : ParamADR_hcl_auto_off))
        _hclIsAutoMode = true;

    logDebugP("AutoConfSwitch %i %i %i", value, ParamADR_hcl_auto_off, _hclIsAutoMode);

    bool currentState = knx.getGroupObject(calcKoNumber(_isGroup ? GRP_Koswitch_state : ADR_Koswitch_state)).value(DPT_Switch);
    // if (value == currentState)
    //     return;
    knx.getGroupObject(calcKoNumber(_isGroup ? GRP_Koswitch_state : ADR_Koswitch_state)).value(value, DPT_Switch);
}

void DaliChannel::setDimmState(uint8_t value, bool isDimmCommand, bool isLastCommand)
{
    if (_dimmDirection == DimmDirection::None && value > 0)
    {
        if (isNight)
            _lastNightValue = value;
        else
            _lastDayValue = value;
    }

    if (isDimmCommand)
    {
        setSwitchState(value > 0, false);
        currentStep = value;
    }

    float perc = DaliHelper::arcToPercentFloat(value);

    GroupObject& ko = knx.getGroupObject(calcKoNumber(ADR_Kodimm_state));
    if(ko.valueNoSendCompare(perc, Dpt(5, 1)))
    {
        logDebugP("SetDimmState %.1f/%i", perc, value);
        ko.objectWritten();
    }
}

void DaliChannel::updateCurrentDimmValue()
{
    if (currentDimmType == DimmType::Color)
        sendColor();

    switch (currentDimmType)
    {
        case DimmType::Brigthness:
        {
            logDebugP("current dimm val %i", *currentDimmValue);
            setDimmState(*currentDimmValue, true, false);
            break;
        }

        case DimmType::Color:
        {
            uint32_t value = currentColor[2];
            value |= currentColor[1] << 8;
            value |= currentColor[0] << 16;
            GroupObject& ko = knx.getGroupObject(calcKoNumber(ADR_Kocolor_rgb_state));
            if(ko.valueNoSendCompare(value, Dpt(232, 600)))
                ko.objectWritten();
            
            ko = knx.getGroupObject(calcKoNumber(ADR_Kocolor_red_state));
            if(ko.valueNoSendCompare(currentColor[0], Dpt(5, 1)))
                ko.objectWritten();
                
            ko = knx.getGroupObject(calcKoNumber(ADR_Kocolor_green_state));
            if(ko.valueNoSendCompare(currentColor[1], Dpt(5, 1)))
                ko.objectWritten();
                
            ko = knx.getGroupObject(calcKoNumber(ADR_Kocolor_blue_state));
            if(ko.valueNoSendCompare(currentColor[2], Dpt(5, 1)))
                ko.objectWritten();
            break;
        }
    }
}

void DaliChannel::setOnValue(uint8_t value)
{
    _onDay = value;
}

void DaliChannel::setGroups(uint16_t groups)
{
    _groups = groups;
}

void DaliChannel::setGroupState(uint16_t group, bool state)
{
    if (group == 0xFFFF || _groups & 1 << group)
        setSwitchState(state, false);
}

void DaliChannel::setGroupState(uint16_t group, uint8_t value)
{
    if (group == 0xFFFF || _groups & (1 << group))
        setDimmState(value, true, true);
}

void DaliChannel::setMinArc(uint8_t min)
{
    _min = min;
}

void DaliChannel::setMaxArc(uint8_t max)
{
    _max = max;
}

void DaliChannel::setHcl(uint8_t curve, uint16_t value)
{
    //TODO add ComObject for selecting curve
    if(_hclCurve == 255) return;
    logDebugP("Temp curve %i | state %i | isAlsoOn %i", _hclCurve, currentState, _hclIsAlsoOn);
    if(!_hclIsAutoMode)
    {
        logDebugP("Ignored because mode is manu");
        return;
    }

    if(curve == _hclCurve)
        _hclCurrentTemp = value;

    if(curve == _hclCurve && currentState)
    {
        if(_hclIsAlsoOn)
        {
            logDebugP("Setting HCL");
            setTemperature(value);
        } else {
            logDebugP("Ignored. Only apply on turning on");
        }
    }
}

void DaliChannel::setHcl(uint8_t curve, uint8_t value)
{
    if(_hclCurve == 255) return;
    logDebugP("Bri curve %i | state %i | isAlsoOn %i", _hclCurve, currentState, _hclIsAlsoOn);
    if(!_hclIsAutoMode)
    {
        logDebugP("Ignored because mode is manu");
        return;
    }

    if(curve == _hclCurve)
        _hclCurrentBri = value;

    if(curve == _hclCurve && currentState)
    {
        _hclCurrentBri = value;
        if(_hclIsAlsoOn)
        {
            logDebugP("Setting HCL");
            setBrightness(value);
        } else {
            logDebugP("Ignored. Only apply on turning on");
        }
    }
}

uint8_t DaliChannel::getMin()
{
    return DaliHelper::arcToPercent(_min);
}

uint8_t DaliChannel::getMax()
{
    return DaliHelper::arcToPercent(_max);
}

uint16_t DaliChannel::getGroups()
{
    return _groups;
}