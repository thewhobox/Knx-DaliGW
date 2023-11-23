#include "DaliModule.h"

const std::string DaliModule::name()
{
    return "Dali";
}

//You can also give it a version
//will be displayed in Command Infos 
const std::string DaliModule::version()
{
    return "";
}

void DaliModule::setCallback(EventHandlerReceivedDataFuncPtr callback)
{
    dali->setCallback(callback);
}

//will be called once
//only if knx.configured == true
void DaliModule::setup(bool conf)
{
    queue = new MessageQueue();

    if(!conf) return;

    for(int i = 0; i < 64; i++)
    {
        DaliChannel *ch = new DaliChannel(i, queue, false);
        ch->setup();
        channels[i] = ch;
    }

    
    for(int i = 0; i < 16; i++)
    {
        DaliChannel *ch = new DaliChannel(i, queue, true);
        ch->setup();
        groups[i] = ch;
    }
}

void DaliModule::setup1(bool conf)
{
    dali = new DaliClass();
	dali->begin(DALI_TX, DALI_RX);
}

void DaliModule::loop()
{
    if(_adrState != AddressingState::None) return;

    if(!_gotInitData)
    {
        if(millis() > 1000)
            loopInitData();
        return;
    }

    for(int i = 0; i < 64; i++)
    {
        channels[i]->loop();
    }
    for(int i = 0; i < 16; i++)
    {
        groups[i]->loop();
    }
}

void DaliModule::loop1(bool configured)
{
    if(_adrState != AddressingState::None)
    {
        loopAddressing();
        loopMessages();
        return;
    }

    loopMessages();
    loopBusState();

    if(!configured) return;

    for(int i = 0; i < 64; i++)
    {
        channels[i]->loop1();
    }
    for(int i = 0; i < 16; i++)
    {
        groups[i]->loop1();
    }

    if(_lastChangedGroup != 255)
    {
        if(_lastChangedGroup > 15)
        {
            _lastChangedGroup -= 16;
            for(int i = 0; i < 64; i++)
                channels[i]->setGroupState(_lastChangedGroup, _lastChangedValue == 1);
        } else {
            for(int i = 0; i < 64; i++)
                channels[i]->setGroupState(_lastChangedGroup, _lastChangedValue);
        }

        _lastChangedGroup = 255;
    }
}

void DaliModule::loopInitData()
{
    DaliChannel* channel = channels[_adrFound];
    _adrFound++;

    if(channel->isConfigured())
    {
        if(_adrFound == 0)
            sendArc(0xFF, 170, 1); //turn on all at 10%

        uint16_t groups = 0;
        int16_t resp = getInfo(channel->channelIndex(), DaliCmd::QUERY_GROUPS_0_7);
        if(resp < 0)
        {
            logErrorP("Dali Error %i: Code %i", _adrFound-1, resp);
            return;
        }
        groups = resp;

        resp = getInfo(channel->channelIndex(), DaliCmd::QUERY_GROUPS_8_15);
        if(resp < 0)
        {
            logErrorP("Dali Error %i: Code %i", _adrFound-1, resp);
            return;
        }
        groups |= resp << 8;
        channel->setGroups(groups);

        sendCmd(channel->channelIndex(), DaliCmd::OFF, channel->isGroup());
    }

    if(_adrFound > 63)
    {
        _adrFound = 0;
        _gotInitData = true;
        _daliStateLast = 1;
        logInfoP("Finished init");
    }
}

int16_t DaliModule::getInfo(byte address, DaliCmd command, uint8_t additional)
{
    _daliStateLast = millis();
    uint8_t respId = sendMsg(MessageType::Cmd, address, command | additional, 0, true);
    bool gotResponse = false;
    int16_t resp = queue->getResponse(respId);
    
    while(!gotResponse)
    {
        resp = queue->getResponse(respId);

        if(resp == -1)
        {
            //error
            gotResponse = true;
            logErrorP("Got no response from channel %i", address);
        } else if(resp >= 0)
        {
            gotResponse = true;
        } else if(millis() - _daliStateLast > 2000)
        {
            logErrorP("Got no response from channel %i (2)", address);
            gotResponse = true;
        }
    }
    return (int16_t)resp;
}

void DaliModule::loopMessages()
{
    Message *msg = queue->pop();
    if(msg == nullptr) return;

    switch(msg->type)
    {
        case MessageType::Arc:
        {
            int16_t resp = dali->sendArcWait(msg->para1, msg->para2, msg->addrtype);
            if(msg->wait)
                queue->setResponse(msg->id, resp);
            break;
        }

        case MessageType::Cmd:
        {
            logDebugP("sending Command %i %i", msg->para2, static_cast<DaliCmd>(msg->para2));
            int16_t resp = dali->sendCmdWait(msg->para1, static_cast<DaliCmd>(msg->para2), msg->addrtype);
            if(msg->wait)
                queue->setResponse(msg->id, resp);
            break;
        }

        case MessageType::SpecialCmd:
        {
            int16_t resp = dali->sendSpecialCmdWait(msg->para1, msg->para2);
            if(msg->wait)
                queue->setResponse(msg->id, resp);
            break;
        }
    }

    delete[] msg;
}

void DaliModule::loopAddressing()
{
    switch(_adrState)
    {
        case AddressingState::Randomize_Wait:
        {
            if(millis() - _adrTime > DALI_WAIT_RANDOMIZE)
            {
                _adrState = AddressingState::Search;
                logInfoP("RandomizeWait finished");
            }
            break;
        }

        case AddressingState::Search:
        {
            if(_adrNoRespCounter == 2)
            {
                _adrState = AddressingState::Finish;
                break;
            }

            byte high = _adrHigh >> 16;
            byte middle = (_adrHigh >> 8) & 0xFF;
            byte low = _adrHigh & 0xFF;
            sendCmdSpecial(DaliSpecialCmd::SEARCHADDRH, high);
            sendCmdSpecial(DaliSpecialCmd::SEARCHADDRM, middle);
            sendCmdSpecial(DaliSpecialCmd::SEARCHADDRL, low);
            _adrResp = sendCmdSpecial(DaliSpecialCmd::COMPARE, 0x00, true);
            _adrTime = millis();
            _adrState = AddressingState::SearchWait;
            break;
        }

        case AddressingState::SearchWait:
        {
            int16_t response = queue->getResponse(_adrResp);
            
            if(response == -200 || response == -1)
            {
                if(response == -1 || millis() - _adrTime > DALI_WAIT_SEARCH)
                {
                    _adrLow = _adrHigh + 1;
                    _adrHigh = _adrHighLast;
                    _adrState = AddressingState::Search;
                    _adrNoRespCounter++;
                }
            } else if(response >= 0) {
                if(_adrLow == _adrHigh)
                {
                    logInfoP("Found Ballast at %X", _adrLow);
                    if(_adrAssign)
                    {
                        uint8_t addr = 0;
                        while(addresses[addr]){
                            addr++;
                        }
                        addresses[addr] = true;
                        logInfoP("Assinging Address %i", addr);
                        sendCmdSpecial(DaliSpecialCmd::PROGRAMSHORT, _adrLow);
                        sendCmdSpecial(DaliSpecialCmd::WITHDRAW);
                        ballasts[_adrFound].high = (_adrLow >> 16) & 0xFF;
                        ballasts[_adrFound].middle = (_adrLow >> 8) & 0xFF;
                        ballasts[_adrFound].low = _adrLow & 0xFF;
                        ballasts[_adrFound].address = response >> 1;
                        _adrFound++;

                        _adrLow = 0;
                        _adrHigh = 0xFFFFFF;
                        _adrHighLast = 0xFFFFFF;
                        _adrNoRespCounter = 1;
                        sendCmdSpecial(DaliSpecialCmd::WITHDRAW);
                        _adrState = AddressingState::Search;
                    } else {
                        _adrResp = sendCmdSpecial(DaliSpecialCmd::QUERY_SHORT, 0, true);
                        _adrTime = millis();
                        _adrState = AddressingState::Found;
                    }
                } else {
                    //logInfoP("Range has ballast");
                    _adrHighLast = _adrHigh;
                    _adrHigh = (_adrLow + _adrHigh) / 2;
                    _adrNoRespCounter = 0;
                    _adrState = AddressingState::Search;
                }
            } else {
                logErrorP("Dali Error %i, aborting addressing", response);
                _adrState = AddressingState::Finish;
            }
            break;
        }

        case AddressingState::Found:
        {
            int16_t response = queue->getResponse(_adrResp);
            if(response == -200 || response == -1)
            {
                if(response == -1 || millis() - _adrTime > DALI_WAIT_SEARCH)
                {
                    logErrorP("Found ballast not answering");
                    _adrState = AddressingState::Finish;
                    ballasts[_adrFound].address = 255;
                    _adrFound++;
                }
            } else if(response >= 0) {
                logInfoP("Ballast has Short Address %i", response >> 1);

                ballasts[_adrFound].high = (_adrLow >> 16) & 0xFF;
                ballasts[_adrFound].middle = (_adrLow >> 8) & 0xFF;
                ballasts[_adrFound].low = _adrLow & 0xFF;
                ballasts[_adrFound].address = response >> 1;
                _adrFound++;

                _adrLow = 0;
                _adrHigh = 0xFFFFFF;
                _adrHighLast = 0xFFFFFF;
                _adrNoRespCounter = 1;
                sendCmdSpecial(DaliSpecialCmd::WITHDRAW);
                _adrState = AddressingState::Search;
            } else {
                logErrorP("Dali Error %i, aborting addressing", response);
                _adrState = AddressingState::Finish;
            }
            break;
        }

        case AddressingState::Finish:
        {
            logErrorP("Found %i ballasts", _adrFound);
            sendCmdSpecial(DaliSpecialCmd::TERMINATE);
            _adrState = AddressingState::None;
            break;
        }

        case AddressingState::Query_Wait:
        {
            int16_t resp = queue->getResponse(_adrResp);

            if(resp == -1 || millis() - _adrTime > DALI_WAIT_SEARCH)
            {
                _adrState = AddressingState::Withdraw_Others;
                logErrorP("Adresse wird nicht verwendet");
                return;
            }

            if(resp == -200) return;

            if(resp >= 0)
            {
                logErrorP("Adresse wird bereits verwendet");
                _assState = AssigningState::Failed_Exists;
                _adrState = AddressingState::None;
                return;
            } else {
                logErrorP("Bus Error %i", resp);
                _assState = AssigningState::Failed_Bus;
                _adrState = AddressingState::None;
                return;
            }
            break;
        }

        case AddressingState::Withdraw_Others:
        {
            sendCmdSpecial(DaliSpecialCmd::INITIALISE, 0);
            sendCmdSpecial(DaliSpecialCmd::INITIALISE, 0);
            if(_adrHigh > 0)
            {
                logInfoP("Verwerfe alle mit niedrigerer Long Address");
                _adrHigh--;
                byte high = _adrHigh >> 16;
                byte middle = (_adrHigh >> 8) & 0xFF;
                byte low = _adrHigh & 0xFF;
                sendCmdSpecial(DaliSpecialCmd::SEARCHADDRH, high);
                sendCmdSpecial(DaliSpecialCmd::SEARCHADDRM, middle);
                sendCmdSpecial(DaliSpecialCmd::SEARCHADDRL, low);
                sendCmdSpecial(DaliSpecialCmd::WITHDRAW);
                _adrHigh++;
            }
            byte high = _adrHigh >> 16;
            byte middle = (_adrHigh >> 8) & 0xFF;
            byte low = _adrHigh & 0xFF;
            sendCmdSpecial(DaliSpecialCmd::SEARCHADDRH, high);
            sendCmdSpecial(DaliSpecialCmd::SEARCHADDRM, middle);
            sendCmdSpecial(DaliSpecialCmd::SEARCHADDRL, low);
            _adrResp =  sendCmdSpecial(DaliSpecialCmd::COMPARE, 0, true);
            _adrTime = millis();
            _adrState = AddressingState::Check_Address;
            break;
        }

        case AddressingState::Check_Address:
        {
            if(millis() - _adrTime > DALI_WAIT_SEARCH)
            {
                logInfoP("Gerät antwortet nicht");
                _assState = AssigningState::Failed_No_Answer;
                _adrState = AddressingState::None;
                return;
            }

            int16_t resp = queue->getResponse(_adrResp);

            if(resp == -200) return;

            if(resp >= 0)
            {
                logInfoP("Long Address exists");
                logInfoP("CMD_PROGRAMSHORT to %i", _adrLow);
                sendCmdSpecial(DaliSpecialCmd::PROGRAMSHORT, _adrLow);
                _adrState = AddressingState::Confirm_Address;
                _adrResp = sendCmdSpecial(DaliSpecialCmd::QUERY_SHORT, 0, true);
                _adrTime = millis();
                logInfoP("Frage Short Address ab");
                return;
            } else if(resp == -1) {
                logInfoP("Long Address dont exists");
                _assState = AssigningState::Failed_Exists_Not;
                _adrState = AddressingState::None;
            } else {
                logInfoP("Bus Error %i", resp);
                _assState = AssigningState::Failed_Bus;
                _adrState = AddressingState::None;
            }
            break;
        }

        case AddressingState::Confirm_Address:
        {
            if(millis() - _adrTime > DALI_WAIT_SEARCH)
            {
                logInfoP("Gerät antwortet nicht");
                _assState = AssigningState::Failed_No_Answer;
                _adrState = AddressingState::None;
                return;
            }

            int16_t resp = queue->getResponse(_adrResp);

            if(resp == -200) return;

            if(resp >= 0)
            {
                logInfoP("Got resp %i", resp);
                if(resp == _adrLow)
                {
                    logInfoP("Adresse erfolgreich gesetzt");
                    _assState = AssigningState::Success;
                } else {
                    logInfoP("Adresse wurde nicht übernommen");
                    _assState = AssigningState::Failed_Confirm;
                }
                _adrState = AddressingState::None;
                return;
            } else {
                logInfoP("Bus Error %i", resp);
                _assState = AssigningState::Failed_Bus;
                _adrState = AddressingState::None;
                return;
            }
            break;
        }

        case AddressingState::SearchAdr:
        {
            if(_adrLow == 64)
            {
                sendCmdSpecial(DaliSpecialCmd::INITIALISE, 255);
                sendCmdSpecial(DaliSpecialCmd::INITIALISE, 255);
                sendCmdSpecial(DaliSpecialCmd::RANDOMISE);
                sendCmdSpecial(DaliSpecialCmd::RANDOMISE);
                _adrState = AddressingState::Randomize_Wait;
                _adrLow = 0;
                _adrHigh = 0xFFFFFF;
                _adrHighLast = 0xFFFFFF;
                break;
            }
                
            _adrState = AddressingState::SearchAdrWait;
            _assState = AssigningState::Working;
            _adrResp = sendCmd(_adrLow, DaliCmd::QUERY_STATUS, DaliAddressTypes::SHORT, true);
            _adrTime = millis();
            break;
        }
        
        case AddressingState::SearchAdrWait:
        {
            if(millis() - _adrTime > DALI_WAIT_SEARCH)
            {
                logInfoP("Address %i dont exists", _adrLow);
                _adrState = AddressingState::SearchAdr;
                _adrLow++;
                return;
            }

            int16_t resp = queue->getResponse(_adrResp);

            if(resp == -200) return;

            if(resp >= 0)
            {
                logInfoP("Address %i exists", _adrLow);
                ballasts[_adrFound].address = _adrLow;
                _adrState = AddressingState::SearchAdr;
                _adrLow++;
            } else if(resp == -1) {
                logInfoP("Address %i dont exists", _adrLow);
                _adrState = AddressingState::SearchAdr;
                _adrLow++;
            } else {
                logInfoP("Bus Error %i", resp);
                _assState = AssigningState::Failed_Bus;
                _adrState = AddressingState::None;
            }
            break;
        }

    }
}

void DaliModule::loopBusState()
{
    bool state = !digitalRead(DALI_RX);
    if(state != _daliBusStateToSet)
    {
        _daliBusStateToSet = state;
        _daliStateLast = millis();
        if(_daliStateLast == 0) _daliStateLast = 1;
    } else if(_daliStateLast != 0 && millis() - _daliStateLast > 1000)
    {
        _daliStateLast = 0;
        if(_daliBusState != _daliBusStateToSet)
        {
            _daliBusState = _daliBusStateToSet;
            if(_daliBusState)
                logInfoP("Dali Busspg. vorhanden");
            else
                logInfoP("Dali Busspg. nicht vorhanden");
        }
    }
}

bool DaliModule::getDaliBusState()
{
    return _daliBusState;
}

void DaliModule::processInputKo(GroupObject &ko)
{
    if(_adrState != AddressingState::None) return;

    int koNum = ko.asap();
    if(koNum >= ADR_KoOffset && koNum < ADR_KoOffset + ADR_KoBlockSize * 64)
    {
        int index = floor((koNum - ADR_KoOffset) / ADR_KoBlockSize);
        channels[index]->processInputKo(ko);
        return;
    }

    if(koNum >= GRP_KoOffset && koNum < GRP_KoOffset + GRP_KoBlockSize * 16)
    {
        int index = floor((koNum - GRP_KoOffset) / GRP_KoBlockSize);
        int chanIndex = (ko.asap() - GRP_KoOffset) % GRP_KoBlockSize;
        groups[index]->processInputKo(ko);

        if(chanIndex == GRP_Koswitch_state)
        {
            _lastChangedGroup = index + 16;
            _lastChangedValue = ko.value(Dpt(1,1));
        }
        if(chanIndex == GRP_Kodimm_state)
        {
            _lastChangedGroup = index;
            _lastChangedValue = ko.value(Dpt(5,1));
        }
        
        return;
    }

    switch(koNum)
    {
        //broadcast switch
        case APP_Kobroadcast_switch:
            koHandleSwitch(ko);
            break;

        //broadcast dimm absolute
        case APP_Kobroadcast_dimm:
            koHandleDimm(ko);
            break;

        //Tag/Nacht Objekt
        case APP_Kodaynight:
            koHandleDayNight(ko);
            break;

        //Set OnValue Day
        case APP_KoonValue:
            koHandleOnValue(ko);
            break;

        case APP_Koscene:
            koHandleScene(ko);
            break;
    }
}

void DaliModule::koHandleSwitch(GroupObject & ko)
{
    bool value = ko.value(DPT_Switch);
    logDebugP("Broadcast Switch %i", value);
    dali->sendArcBroadcast(value ? 0xFE : 0x00);
}

void DaliModule::koHandleDimm(GroupObject & ko)
{
    uint8_t value = ko.value(Dpt(5,1));
    logDebugP("Broadcast Dimm %i", value);
    value = ((253/3)*(log10(value)+1)) + 1;
    value++;
    dali->sendArcBroadcast(value);
}

void DaliModule::koHandleDayNight(GroupObject & ko)
{
    bool value = ko.value(DPT_Switch);
    if(ParamAPP_daynight) value = !value;
    logDebugP("Broadcast Day/Night %i", value);
    if(ParamAPP_daynight) value = !value;

    for(int i = 0; i < 64; i++)
        channels[i]->isNight = value;
    for(int i = 0; i < 16; i++)
        groups[i]->isNight = value;
}

void DaliModule::koHandleOnValue(GroupObject & ko)
{
    uint8_t value = ko.value(Dpt(5,1));
    logDebugP("KO OnValue: %i", value);

    for(int i = 0; i < 64; i++)
        channels[i]->setOnValue(value);
    for(int i = 0; i < 16; i++)
        groups[i]->setOnValue(value);
}

void DaliModule::koHandleScene(GroupObject & ko)
{
    uint8_t gotNumber = ko.value(DPT_SceneNumber);
    logDebugP("KO Scene: %i", gotNumber);
    for(int i = 0; i < 16; i++)
    {
        uint8_t dest = ParamSCE_typeIndex(i);
        logDebugP("KO Scene%i: Dest=%i", i, dest);
        if(dest == 0) continue;
        uint8_t number = ParamSCE_numberKnxIndex(i);
        logDebugP("KO Scene%i: Number=%i", i, number-1);
        if(gotNumber == number - 1)
        {
            bool isSave = ko.value(Dpt(18,1,0));
            logDebugP("KO Scene%i: Save=%i", i, isSave);
            if(isSave && !ParamSCE_saveIndex(i))
            {
                logDebugP("KO Scene%i: Save not allowed", i);
                continue;
            }

            uint8_t scene = ParamSCE_numberDaliIndex(i);
            logDebugP("KO Scene%i: Scene=%i", i, scene);
            uint8_t addr = 0;
            uint8_t type = 0;
            switch(dest)
            {
                //Address
                case 1:
                {
                    addr = ParamSCE_addressIndex(i);
                    logDebugP("KO Scene%i: Addr=%i", i, addr);
                    type = static_cast<uint8_t>(DaliAddressTypes::SHORT);
                    break;
                }

                //Group
                case 2:
                {
                    addr = ParamSCE_groupIndex(i);
                    logDebugP("KO Scene%i: Grou=%i", i, addr);
                    type = static_cast<uint8_t>(DaliAddressTypes::GROUP);
                    break;
                }

                //Broadcast
                case 3:
                {
                    addr = 0xFF;
                    logDebugP("KO Scene%i: Broadcast", i);
                    type = static_cast<uint8_t>(DaliAddressTypes::GROUP);
                    break;
                }
            }

            if(isSave)
            {
                sendCmd(addr, DaliCmd::ARC_TO_DTR, type);
                uint8_t temp = static_cast<uint8_t>(DaliCmd::DTR_AS_SCENE);
                temp |= scene;
                sendCmd(addr, static_cast<DaliCmd>(temp), type);
            } else {
                uint8_t temp = static_cast<uint8_t>(DaliCmd::GO_TO_SCENE);
                temp |= scene;
                sendCmd(addr, static_cast<DaliCmd>(temp), type);
            }
        }
    }
}

bool DaliModule::processFunctionProperty(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    if(objectIndex != 164) return false;

    switch(propertyId)
    {
        case 2:
            funcHandleType(data, resultData, resultLength);
            return true;

        case 3:
            funcHandleScan(data, resultData, resultLength);
            return true;

        case 4:
            funcHandleAssign(data, resultData, resultLength);
            return true;
        
        case 5:
            funcHandleAddress(data, resultData, resultLength);
            return true;

        case 10:
            funcHandleEvgWrite(data, resultData, resultLength);
            return true;

        case 111:
            funcHandleEvgRead(data, resultData, resultLength);
            return true;
    }


    return false;
}

void DaliModule::funcHandleType(uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    uint8_t resp = sendMsg(MessageType::Cmd, data[0], DaliCmd::QUERY_DEVICE_TYPE, DaliAddressTypes::SHORT, true);
    resultData[0] = 0x00;
    resultData[1] = resp;
    resultLength = 2;
}

void DaliModule::funcHandleScan(uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    ballasts = new Ballast[64];
    logInfoP("Starting Scan");
    _adrState = AddressingState::Init;
    _adrLow = 0;
    _adrHigh = 0xFFFFFF;
    _adrHighLast = 0xFFFFFF;
    _adrFound = 0;
    _adrAssign = false;
    
    sendCmdSpecial(DaliSpecialCmd::INITIALISE, data[0] ? 255 : 0);
    sendCmdSpecial(DaliSpecialCmd::INITIALISE, data[0] ? 255 : 0);

    if(data[1] == 1)
    {
        logInfoP("Do Randomize");
        sendCmdSpecial(DaliSpecialCmd::RANDOMISE);
        sendCmdSpecial(DaliSpecialCmd::RANDOMISE);
        _adrState = AddressingState::Randomize_Wait;
        _adrTime = millis();
        logInfoP("RandomizeWait");
    } else {
        logInfoP("Don't Randomize");
        _adrState = AddressingState::Search;
    }


    _adrNoRespCounter = 1;
    resultLength = 0;
}

void DaliModule::funcHandleAssign(uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    logInfoP("Starting assigning address");
    _adrResp = sendCmd(data[0], DaliCmd::QUERY_STATUS, DaliAddressTypes::SHORT, true);
    _adrTime = millis();
    _adrState = AddressingState::Query_Wait;
    _assState = AssigningState::Working;

    _adrHigh = data[1] << 16;
    _adrHigh |= data[2] << 8;
    _adrHigh |= data[3];
    logInfoP("Long  Addr %X", _adrHigh);
    logInfoP("Short Addr %i", data[0]);
    _adrLow = (data[0] << 1) | 1;
    
    resultLength = 0;
}

void DaliModule::funcHandleAddress(uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    logInfoP("Starting addressing new auto");
    _adrLow = 0;
    _adrState = AddressingState::SearchAdr;
    _adrFound = 0;
    _adrAssign = true;
    ballasts = new Ballast[64];
    addresses = new bool[64];
    for(int i = 0; i < 64; i++) addresses[i] = false;
    resultLength = 0;
}

void DaliModule::funcHandleEvgWrite(uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    logInfoP("Starting setting up EVG");

    DaliChannel* channel = channels[data[0]];
    channel->setMinMax(data[1], data[2]);

    sendCmdSpecial(DaliSpecialCmd::SET_DTR, (data[1] == 255) ? 255 : DaliHelper::percentToArc(data[1]));
    sendCmd(data[0], DaliCmd::DTR_AS_MIN);
    sendCmdSpecial(DaliSpecialCmd::SET_DTR, (data[2] == 255) ? 255 : DaliHelper::percentToArc(data[2]));
    sendCmd(data[0], DaliCmd::DTR_AS_MAX);
    sendCmdSpecial(DaliSpecialCmd::SET_DTR, (data[3] == 255) ? 255 : DaliHelper::percentToArc(data[3]));
    sendCmd(data[0], DaliCmd::DTR_AS_POWER_ON);
    sendCmdSpecial(DaliSpecialCmd::SET_DTR, (data[4] == 255) ? 255 : DaliHelper::percentToArc(data[4]));
    sendCmd(data[0], DaliCmd::DTR_AS_FAIL);

    //fadetime
    //faderate

    uint16_t groups = data[7];
    groups |= data[8] << 8;
    channel->setGroups(groups);

    for(int i = 0; i < 16; i++)
    {
        if((groups >> i) & 0x1)
        {
            sendMsg(MessageType::Cmd, data[0], DaliCmd::ADD_TO_GROUP | i);
            sendMsg(MessageType::Cmd, data[0], DaliCmd::ADD_TO_GROUP | i);
        } else {
            sendMsg(MessageType::Cmd, data[0], DaliCmd::REMOVE_FROM_GROUP | i);
            sendMsg(MessageType::Cmd, data[0], DaliCmd::REMOVE_FROM_GROUP | i);
        }
    }

    for(int i = 0; i < 16; i++)
    {
        sendCmdSpecial(DaliSpecialCmd::SET_DTR, (data[i + 9] == 255) ? 255 : DaliHelper::percentToArc(data[i + 9]));
        sendMsg(MessageType::Cmd, data[0], DaliCmd::DTR_AS_SCENE | i);
    }
    resultLength = 1;
    resultData[0] = 0x00;
}

void DaliModule::funcHandleEvgRead(uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    logInfoP("Starting reading EVG settings");

    
    DaliChannel* channel = channels[data[0]];
    resultData[1] = channel->getMin();
    resultData[2] = channel->getMax();
    int16_t resp = getInfo(data[0], DaliCmd::QUERY_POWER_ON_LEVEL);
    if(resp < 0)
    {
        logErrorP("Dali Error (POWER): Code %i", resp);
        resultData[0] = 0x01;
        resultLength = 1;
        return;
    }
    resultData[3] = resp;

    resp = getInfo(data[0], DaliCmd::QUERY_POWER_FAILURE);
    if(resp < 0)
    {
        logErrorP("Dali Error (FAILURE): Code %i", resp);
        resultData[0] = 0x02;
        resultLength = 1;
        return;
    }
    resultData[4] = resp;
    
    //fadetime
    //faderate

    resultData[7] = channel->getGroups() & 0xFF;
    resultData[8] = (channel->getGroups() >> 8) & 0xFF;

    for(int i = 0; i < 16; i++)
    {
        resp = getInfo(data[0], DaliCmd::QUERY_SCENE_LEVEL, i);
        if(resp < 0)
        {
            logErrorP("Dali Error (SCENE%i): Code %i", i, resp);
            resultData[0] = i+3;
            resultLength = 1;
            return;
        }
        if(resp == 255)
            resultData[i+9] = resp;
        else
            resultData[i+9] = DaliHelper::arcToPercent(resp);
    }
    resultLength = 25;
}

bool DaliModule::processFunctionPropertyState(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    if(objectIndex != 164) return false;

    switch(propertyId)
    {
        case 1:
            stateHandleType(data, resultData, resultLength);
            return true;

        case 3:
        case 5:
            stateHandleScanAndAddress(data, resultData, resultLength);
            return true;

        case 4:
            stateHandleAssign(data, resultData, resultLength);
            return true;
    }
    return false;
}

void DaliModule::stateHandleType(uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    int16_t resp = queue->getResponse(data[0]);
    logInfoP("Check Response %i = %i", data[0], resp);

    if(resp == -255)
    {
        resultData[0] = 0x00;
        resultLength = 1;
    } else {
        logInfoP("Send Response %i", resp);
        if(resp >= 0)
        {
            resultData[0] = 0x01;
            resultData[1] = (uint8_t)resp;
        } else {
            resultData[0] = 0x02;
            resultData[1] = (uint8_t)(resp*-1);
        }
        resultLength = 2;
    }
}

void DaliModule::stateHandleAssign(uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    resultData[0] = (uint8_t)_assState;
    resultLength = 1;
}

void DaliModule::stateHandleScanAndAddress(uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    resultData[0] = data[0] < _adrFound;
    resultData[1] = (data[0] >= _adrFound) && _adrState == AddressingState::None;
    if(data[0] < _adrFound)
    {
        resultData[2] = ballasts[data[0]].high;
        resultData[3] = ballasts[data[0]].middle;
        resultData[4] = ballasts[data[0]].low;
        resultData[5] = ballasts[data[0]].address;
        resultLength = 6;
    } else {
        resultLength = 2;
    }

    if(resultData[1])
    {
        delete[] ballasts;
        delete[] addresses;
    }
}

uint8_t DaliModule::sendArc(byte addr, byte value, byte type)
{
    Message *msg = new Message();
    msg->id = queue->getNextId();
    msg->type = MessageType::Arc;
    msg->para1 = addr;
    msg->para2 = value;
    msg->addrtype = type;
    return queue->push(msg);
}

uint8_t DaliModule::sendCmd(byte addr, DaliCmd value, byte type, bool wait)
{
    Message *msg = new Message();
    msg->id = queue->getNextId();
    msg->type = MessageType::Cmd;
    msg->para1 = addr;
    msg->para2 = value;
    msg->addrtype = type;
    msg->wait = wait;
    return queue->push(msg);
}

uint8_t DaliModule::sendCmdSpecial(DaliSpecialCmd command, byte value, bool wait)
{
    Message *msg = new Message();
    msg->id = queue->getNextId();
    msg->type = MessageType::SpecialCmd;
    msg->para1 = static_cast<uint8_t>(command);
    msg->para2 = value;
    msg->addrtype = 0;
    msg->wait = wait;
    return queue->push(msg);
}

uint8_t DaliModule::sendMsg(MessageType t, byte p1, byte p2, byte type, bool wait)
{
    Message *msg = new Message();
    msg->id = queue->getNextId();
    msg->type = t;
    msg->para1 = p1;
    msg->para2 = p2;
    msg->addrtype = type;
    msg->wait = wait;
    return queue->push(msg);
}

DaliModule openknxDaliModule;