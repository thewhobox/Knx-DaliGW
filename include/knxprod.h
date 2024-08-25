#pragma once

#define paramDelay(time) (uint32_t)( \
            (time & 0xC000) == 0xC000 ? (time & 0x3FFF) * 100 : \
            (time & 0xC000) == 0x0000 ? (time & 0x3FFF) * 1000 : \
            (time & 0xC000) == 0x4000 ? (time & 0x3FFF) * 60000 : \
            (time & 0xC000) == 0x8000 ? ((time & 0x3FFF) > 1000 ? 3600000 : \
                                            (time & 0x3FFF) * 3600000 ) : 0 )
#define PT_deviceType_Deaktiviert 0
#define PT_deviceType_DT0 1
#define PT_deviceType_DT1 2
#define PT_deviceType_DT2 3
#define PT_deviceType_DT3 4
#define PT_deviceType_DT4 5
#define PT_deviceType_DT5 6
#define PT_deviceType_DT6 7
#define PT_deviceType_DT7 8
#define PT_deviceType_DT8 9
#define PT_lock_no 0
#define PT_lock_on 1
#define PT_lock_off 2
#define PT_lock_value 3
#define PT_groupType_none 0
#define PT_groupType_switch 1
#define PT_groupType_dimm 2
#define PT_groupType_color 3
#define PT_scenetype_none 0
#define PT_scenetype_address 1
#define PT_scenetype_group 2
#define PT_scenetype_broadcast 3
#define PT_colorType_HSV 0
#define PT_colorType_RGB 1
#define PT_colorType_TW 2
#define PT_colorType_XYY 3
#define PT_colorSpace_rgb 1
#define PT_colorSpace_xy 0
#define PT_clickAction_none 0
#define PT_clickAction_on 1
#define PT_clickAction_off 2
#define PT_clickAction_toggle 3
#define PT_clickAction_lock 4
#define PT_clickAction_unlock 5
#define PT_clickAction_lock_toggle 6
#define PT_clickAction_identify 7
#define PT_hclType_none 0
#define PT_hclType_sun 1
#define PT_hclType_time 2
#define PT_dimmLock_none 0
#define PT_dimmLock_noOn 1
#define PT_dimmLock_noOff 2
#define PT_dimmLock_noBoth 3
#define PT_offset_no 0
#define PT_offset_plus 1
#define PT_offset_minus 2
//--------------------Allgemein---------------------------
#define MAIN_OpenKnxId 0xA4
#define MAIN_ApplicationNumber 0x01
#define MAIN_ApplicationVersion 0x09
#define MAIN_OrderNumber "REG1-Dali"
#define MAIN_ParameterSize 2106
#define MAIN_MaxKoNumber 1602


#define APP_daynight		0x0000
// Offset: 0, Size: 1 Bit, Text: Tag/Nacht Objekt
#define ParamAPP_daynight knx.paramBit(APP_daynight, 0)
#define APP_funcBtn		0x0000
#define APP_funcBtn_Shift	4
#define APP_funcBtn_Mask	0x0007
// Offset: 0, BitOffset: 1, Size: 3 Bit, Text: Func Aktion Klick
#define ParamAPP_funcBtn ((uint)((knx.paramByte(APP_funcBtn) >> APP_funcBtn_Shift) & APP_funcBtn_Mask))
#define APP_funcBtnDbl		0x0000
#define APP_funcBtnDbl_Shift	1
#define APP_funcBtnDbl_Mask	0x0007
// Offset: 0, BitOffset: 4, Size: 3 Bit, Text: Func Aktion Doppelklick
#define ParamAPP_funcBtnDbl ((uint)((knx.paramByte(APP_funcBtnDbl) >> APP_funcBtnDbl_Shift) & APP_funcBtnDbl_Mask))
#define APP_funcBtnLong		0x0001
#define APP_funcBtnLong_Shift	5
#define APP_funcBtnLong_Mask	0x0007
// Offset: 1, Size: 3 Bit, Text: Func Aktion Langer Klick
#define ParamAPP_funcBtnLong ((uint)((knx.paramByte(APP_funcBtnLong) >> APP_funcBtnLong_Shift) & APP_funcBtnLong_Mask))
//!< Number: 1, Text: Broadcast, Function: Schalten
#define APP_Kobroadcast_switch 1
#define KoAPP_broadcast_switch knx.getGroupObject(APP_Kobroadcast_switch)
//!< Number: 2, Text: Broadcast, Function: Dimmen Absolut
#define APP_Kobroadcast_dimm 2
#define KoAPP_broadcast_dimm knx.getGroupObject(APP_Kobroadcast_dimm)
//!< Number: 3, Text: Allgemein, Function: Tag/Nacht
#define APP_Kodaynight 3
#define KoAPP_daynight knx.getGroupObject(APP_Kodaynight)
//!< Number: 4, Text: Allgemein, Function: Einschaltwert Tag
#define APP_KoonValue 4
#define KoAPP_onValue knx.getGroupObject(APP_KoonValue)
//!< Number: 5, Text: Allgemein, Function: Szene
#define APP_Koscene 5
#define KoAPP_scene knx.getGroupObject(APP_Koscene)

//---------------------Modules----------------------------

//-----Module specific starts
#define BASE_Share_ParamBlockOffset 2
#define BASE_Share_ParamBlockSize 45
#define SCE_ParamBlockOffset 47
#define SCE_ParamBlockSize 4
#define ADR_ParamBlockOffset 303
#define ADR_ParamBlockSize 23
#define GRP_ParamBlockOffset 1775
#define GRP_ParamBlockSize 19
#define HCL_ParamBlockOffset 2079
#define HCL_ParamBlockSize 9
#define BASE_Share_KoOffset 6
#define BASE_Share_KoBlockSize 6
#define SCE_KoOffset 12
#define SCE_KoBlockSize 0
#define ADR_KoOffset 12
#define ADR_KoBlockSize 20
#define GRP_KoOffset 1292
#define GRP_KoBlockSize 19
#define HCL_KoOffset 1596
#define HCL_KoBlockSize 2

//-----Module: adresse
#define ADR_deviceType		0x0000
#define ADR_deviceType_Shift	4
#define ADR_deviceType_Mask	0x000F
// Offset: 0, Size: 4 Bit, Text: Gerätetyp
#define ParamADR_deviceTypeIndex(X) ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_deviceType)) >> ADR_deviceType_Shift) & ADR_deviceType_Mask))
// Offset: 0, Size: 4 Bit, Text: Gerätetyp
#define ParamADR_deviceType ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_deviceType)) >> ADR_deviceType_Shift) & ADR_deviceType_Mask))
#define ADR_type		0x0000
// Offset: 0, BitOffset: 4, Size: 1 Bit, Text: Betriebsart
#define ParamADR_typeIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_type), 4)
// Offset: 0, BitOffset: 4, Size: 1 Bit, Text: Betriebsart
#define ParamADR_type knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_type), 4)
#define ADR_error		0x0000
// Offset: 0, BitOffset: 5, Size: 1 Bit, Text: Fehlerstatus auslesen
#define ParamADR_errorIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_error), 5)
// Offset: 0, BitOffset: 5, Size: 1 Bit, Text: Fehlerstatus auslesen
#define ParamADR_error knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_error), 5)
#define ADR_min		0x0001
// Offset: 1, Size: 16 Bit (2 Byte), Text: 
#define ParamADR_minIndex(X) knx.paramFloat((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_min), Float_Enc_DPT9)
// Offset: 1, Size: 16 Bit (2 Byte), Text: 
#define ParamADR_min knx.paramFloat((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_min), Float_Enc_DPT9)
#define ADR_max		0x0003
// Offset: 3, Size: 16 Bit (2 Byte), Text: 
#define ParamADR_maxIndex(X) knx.paramFloat((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_max), Float_Enc_DPT9)
// Offset: 3, Size: 16 Bit (2 Byte), Text: 
#define ParamADR_max knx.paramFloat((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_max), Float_Enc_DPT9)
#define ADR_nachtriggern		0x0000
// Offset: 0, BitOffset: 6, Size: 1 Bit, Text: Nachtriggern erlauben
#define ParamADR_nachtriggernIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_nachtriggern), 6)
// Offset: 0, BitOffset: 6, Size: 1 Bit, Text: Nachtriggern erlauben
#define ParamADR_nachtriggern knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_nachtriggern), 6)
#define ADR_stairtime		0x0005
#define ADR_stairtime_Shift	2
#define ADR_stairtime_Mask	0x3FFF
// Offset: 5, Size: 14 Bit, Text: Nachlaufzeit
#define ParamADR_stairtimeIndex(X) ((uint)((knx.paramWord((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_stairtime)) >> ADR_stairtime_Shift) & ADR_stairtime_Mask))
// Offset: 5, Size: 14 Bit, Text: Nachlaufzeit
#define ParamADR_stairtime ((uint)((knx.paramWord((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_stairtime)) >> ADR_stairtime_Shift) & ADR_stairtime_Mask))
#define ADR_manuoff		0x0000
// Offset: 0, BitOffset: 7, Size: 1 Bit, Text: Manuelles ausschalten
#define ParamADR_manuoffIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_manuoff), 7)
// Offset: 0, BitOffset: 7, Size: 1 Bit, Text: Manuelles ausschalten
#define ParamADR_manuoff knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_manuoff), 7)
#define ADR_lockbehave		0x0007
#define ADR_lockbehave_Shift	6
#define ADR_lockbehave_Mask	0x0003
// Offset: 7, Size: 2 Bit, Text: Verhalten bei Sperren
#define ParamADR_lockbehaveIndex(X) ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_lockbehave)) >> ADR_lockbehave_Shift) & ADR_lockbehave_Mask))
// Offset: 7, Size: 2 Bit, Text: Verhalten bei Sperren
#define ParamADR_lockbehave ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_lockbehave)) >> ADR_lockbehave_Shift) & ADR_lockbehave_Mask))
#define ADR_lockvalue		0x0008
#define ADR_lockvalue_Shift	1
#define ADR_lockvalue_Mask	0x007F
// Offset: 8, Size: 7 Bit, Text: 
#define ParamADR_lockvalueIndex(X) ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_lockvalue)) >> ADR_lockvalue_Shift) & ADR_lockvalue_Mask))
// Offset: 8, Size: 7 Bit, Text: 
#define ParamADR_lockvalue ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_lockvalue)) >> ADR_lockvalue_Shift) & ADR_lockvalue_Mask))
#define ADR_unlockbehave		0x0007
#define ADR_unlockbehave_Shift	4
#define ADR_unlockbehave_Mask	0x0003
// Offset: 7, BitOffset: 2, Size: 2 Bit, Text: Verhalten bei Entsperren
#define ParamADR_unlockbehaveIndex(X) ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_unlockbehave)) >> ADR_unlockbehave_Shift) & ADR_unlockbehave_Mask))
// Offset: 7, BitOffset: 2, Size: 2 Bit, Text: Verhalten bei Entsperren
#define ParamADR_unlockbehave ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_unlockbehave)) >> ADR_unlockbehave_Shift) & ADR_unlockbehave_Mask))
#define ADR_unlockvalue		0x0009
#define ADR_unlockvalue_Shift	1
#define ADR_unlockvalue_Mask	0x007F
// Offset: 9, Size: 7 Bit, Text: 
#define ParamADR_unlockvalueIndex(X) ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_unlockvalue)) >> ADR_unlockvalue_Shift) & ADR_unlockvalue_Mask))
// Offset: 9, Size: 7 Bit, Text: 
#define ParamADR_unlockvalue ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_unlockvalue)) >> ADR_unlockvalue_Shift) & ADR_unlockvalue_Mask))
#define ADR_onDay		0x000A
// Offset: 10, Size: 16 Bit (2 Byte), Text: Einschaltwert Tag
#define ParamADR_onDayIndex(X) knx.paramFloat((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_onDay), Float_Enc_DPT9)
// Offset: 10, Size: 16 Bit (2 Byte), Text: Einschaltwert Tag
#define ParamADR_onDay knx.paramFloat((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_onDay), Float_Enc_DPT9)
#define ADR_onNight		0x000C
// Offset: 12, Size: 16 Bit (2 Byte), Text: Einschaltwert Nacht
#define ParamADR_onNightIndex(X) knx.paramFloat((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_onNight), Float_Enc_DPT9)
// Offset: 12, Size: 16 Bit (2 Byte), Text: Einschaltwert Nacht
#define ParamADR_onNight knx.paramFloat((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_onNight), Float_Enc_DPT9)
#define ADR_locknegate		0x0007
// Offset: 7, BitOffset: 4, Size: 1 Bit, Text: Sperren bei
#define ParamADR_locknegateIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_locknegate), 4)
// Offset: 7, BitOffset: 4, Size: 1 Bit, Text: Sperren bei
#define ParamADR_locknegate knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_locknegate), 4)
#define ADR_colorType		0x0007
#define ADR_colorType_Shift	1
#define ADR_colorType_Mask	0x0003
// Offset: 7, BitOffset: 5, Size: 2 Bit, Text: Farbe ansteuern per
#define ParamADR_colorTypeIndex(X) ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_colorType)) >> ADR_colorType_Shift) & ADR_colorType_Mask))
// Offset: 7, BitOffset: 5, Size: 2 Bit, Text: Farbe ansteuern per
#define ParamADR_colorType ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_colorType)) >> ADR_colorType_Shift) & ADR_colorType_Mask))
#define ADR_colorSpace		0x0007
// Offset: 7, BitOffset: 7, Size: 1 Bit, Text: Farbe übertragen per
#define ParamADR_colorSpaceIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_colorSpace), 7)
// Offset: 7, BitOffset: 7, Size: 1 Bit, Text: Farbe übertragen per
#define ParamADR_colorSpace knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_colorSpace), 7)
#define ADR_tempMin		0x000E
// Offset: 14, Size: 16 Bit (2 Byte), Text: Farbtemperatur Min
#define ParamADR_tempMinIndex(X) ((uint)((knx.paramWord((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_tempMin)))))
// Offset: 14, Size: 16 Bit (2 Byte), Text: Farbtemperatur Min
#define ParamADR_tempMin ((uint)((knx.paramWord((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_tempMin)))))
#define ADR_tempMax		0x0010
// Offset: 16, Size: 16 Bit (2 Byte), Text: Farbtemperatur Max
#define ParamADR_tempMaxIndex(X) ((uint)((knx.paramWord((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_tempMax)))))
// Offset: 16, Size: 16 Bit (2 Byte), Text: Farbtemperatur Max
#define ParamADR_tempMax ((uint)((knx.paramWord((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_tempMax)))))
#define ADR_hcl		0x0008
// Offset: 8, BitOffset: 7, Size: 1 Bit, Text: HCL aktivieren
#define ParamADR_hclIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_hcl), 7)
// Offset: 8, BitOffset: 7, Size: 1 Bit, Text: HCL aktivieren
#define ParamADR_hcl knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_hcl), 7)
#define ADR_queryTime		0x0012
// Offset: 18, Size: 16 Bit (2 Byte), Text: Dimmwert abfragen (0 = deaktiviert)
#define ParamADR_queryTimeIndex(X) ((uint)((knx.paramWord((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_queryTime)))))
// Offset: 18, Size: 16 Bit (2 Byte), Text: Dimmwert abfragen (0 = deaktiviert)
#define ParamADR_queryTime ((uint)((knx.paramWord((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_queryTime)))))
#define ADR_xyIgnore		0x0009
// Offset: 9, BitOffset: 7, Size: 1 Bit, Text: Helligkeit ignorieren? (nur xy verwenden)
#define ParamADR_xyIgnoreIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_xyIgnore), 7)
// Offset: 9, BitOffset: 7, Size: 1 Bit, Text: Helligkeit ignorieren? (nur xy verwenden)
#define ParamADR_xyIgnore knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_xyIgnore), 7)
#define ADR_dimmStateInterval		0x0014
#define ADR_dimmStateInterval_Shift	4
#define ADR_dimmStateInterval_Mask	0x000F
// Offset: 20, Size: 4 Bit, Text: Dimmstatus Interval bei relativ
#define ParamADR_dimmStateIntervalIndex(X) ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_dimmStateInterval)) >> ADR_dimmStateInterval_Shift) & ADR_dimmStateInterval_Mask))
// Offset: 20, Size: 4 Bit, Text: Dimmstatus Interval bei relativ
#define ParamADR_dimmStateInterval ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_dimmStateInterval)) >> ADR_dimmStateInterval_Shift) & ADR_dimmStateInterval_Mask))
#define ADR_dimmRelDuration		0x0015
// Offset: 21, Size: 8 Bit (1 Byte), Text: Dimmzeit von 0-100 % bei relativ
#define ParamADR_dimmRelDurationIndex(X) ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_dimmRelDuration)))))
// Offset: 21, Size: 8 Bit (1 Byte), Text: Dimmzeit von 0-100 % bei relativ
#define ParamADR_dimmRelDuration ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_dimmRelDuration)))))
#define ADR_hclCurve		0x0014
#define ADR_hclCurve_Shift	2
#define ADR_hclCurve_Mask	0x0003
// Offset: 20, BitOffset: 4, Size: 2 Bit, Text: Verwende
#define ParamADR_hclCurveIndex(X) ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_hclCurve)) >> ADR_hclCurve_Shift) & ADR_hclCurve_Mask))
// Offset: 20, BitOffset: 4, Size: 2 Bit, Text: Verwende
#define ParamADR_hclCurve ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_hclCurve)) >> ADR_hclCurve_Shift) & ADR_hclCurve_Mask))
#define ADR_hclStart		0x0014
// Offset: 20, BitOffset: 6, Size: 1 Bit, Text: HCL anwenden
#define ParamADR_hclStartIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_hclStart), 6)
// Offset: 20, BitOffset: 6, Size: 1 Bit, Text: HCL anwenden
#define ParamADR_hclStart knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_hclStart), 6)
#define ADR_dimmLock		0x0016
#define ADR_dimmLock_Shift	6
#define ADR_dimmLock_Mask	0x0003
// Offset: 22, Size: 2 Bit, Text: Einschalten bei relativ
#define ParamADR_dimmLockIndex(X) ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_dimmLock)) >> ADR_dimmLock_Shift) & ADR_dimmLock_Mask))
// Offset: 22, Size: 2 Bit, Text: Einschalten bei relativ
#define ParamADR_dimmLock ((uint)((knx.paramByte((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_dimmLock)) >> ADR_dimmLock_Shift) & ADR_dimmLock_Mask))
#define ADR_hcl_manu_bri		0x0014
// Offset: 20, BitOffset: 7, Size: 1 Bit, Text: Helligkeit geändert wird
#define ParamADR_hcl_manu_briIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_hcl_manu_bri), 7)
// Offset: 20, BitOffset: 7, Size: 1 Bit, Text: Helligkeit geändert wird
#define ParamADR_hcl_manu_bri knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_hcl_manu_bri), 7)
#define ADR_hcl_manu_col		0x0016
// Offset: 22, BitOffset: 2, Size: 1 Bit, Text: Farbe/Temperatur geändert wird
#define ParamADR_hcl_manu_colIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_hcl_manu_col), 2)
// Offset: 22, BitOffset: 2, Size: 1 Bit, Text: Farbe/Temperatur geändert wird
#define ParamADR_hcl_manu_col knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_hcl_manu_col), 2)
#define ADR_hcl_auto_off		0x0016
// Offset: 22, BitOffset: 3, Size: 1 Bit, Text: das EVG ausgeschaltet wird
#define ParamADR_hcl_auto_offIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_hcl_auto_off), 3)
// Offset: 22, BitOffset: 3, Size: 1 Bit, Text: das EVG ausgeschaltet wird
#define ParamADR_hcl_auto_off knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_hcl_auto_off), 3)
#define ADR_hcl_auto_day		0x0016
// Offset: 22, BitOffset: 4, Size: 1 Bit, Text: der Tag vorbei ist
#define ParamADR_hcl_auto_dayIndex(X) knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * X + ADR_hcl_auto_day), 4)
// Offset: 22, BitOffset: 4, Size: 1 Bit, Text: der Tag vorbei ist
#define ParamADR_hcl_auto_day knx.paramBit((ADR_ParamBlockOffset + ADR_ParamBlockSize * channelIndex() + ADR_hcl_auto_day), 4)
//!< Number: 0, Text: A{{argChan}} {{0}}, Function: Schalten
#define ADR_Koswitch 0
#define KoADR_switchIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Koswitch)
#define KoADR_switch knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Koswitch)
//!< Number: 1, Text: A{{argChan}} {{0}}, Function: Schalten Status
#define ADR_Koswitch_state 1
#define KoADR_switch_stateIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Koswitch_state)
#define KoADR_switch_state knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Koswitch_state)
//!< Number: 2, Text: A{{argChan}} {{0}}, Function: Dimmen Relativ
#define ADR_Kodimm_relative 2
#define KoADR_dimm_relativeIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kodimm_relative)
#define KoADR_dimm_relative knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kodimm_relative)
//!< Number: 3, Text: A{{argChan}} {{0}}, Function: Dimmen Absolut
#define ADR_Kodimm_absolute 3
#define KoADR_dimm_absoluteIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kodimm_absolute)
#define KoADR_dimm_absolute knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kodimm_absolute)
//!< Number: 4, Text: A{{argChan}} {{0}}, Function: Dimmen Status
#define ADR_Kodimm_state 4
#define KoADR_dimm_stateIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kodimm_state)
#define KoADR_dimm_state knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kodimm_state)
//!< Number: 5, Text: A{{argChan}} {{0}}, Function: Sperren
#define ADR_Kolock 5
#define KoADR_lockIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kolock)
#define KoADR_lock knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kolock)
//!< Number: 19, Text: A{{argChan}} {{0}}, Function: Fehler
#define ADR_Koerror 19
#define KoADR_errorIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Koerror)
#define KoADR_error knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Koerror)
//!< Number: 6, Text: A{{argChan}} {{0}}, Function: RGB Farbe
#define ADR_Kocolor 6
#define KoADR_colorIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kocolor)
#define KoADR_color knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kocolor)
//!< Number: 7, Text: A{{argChan}} {{0}}, Function: RGB Farbe Status
#define ADR_Kocolor_rgb_state 7
#define KoADR_color_rgb_stateIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kocolor_rgb_state)
#define KoADR_color_rgb_state knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kocolor_rgb_state)
//!< Number: 8, Text: A{{argChan}} {{0}}, Function: Rot Relativ
#define ADR_Kocolor_red_relative 8
#define KoADR_color_red_relativeIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kocolor_red_relative)
#define KoADR_color_red_relative knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kocolor_red_relative)
//!< Number: 9, Text: A{{argChan}} {{0}}, Function: Rot Absolut
#define ADR_Kocolor_red_absolute 9
#define KoADR_color_red_absoluteIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kocolor_red_absolute)
#define KoADR_color_red_absolute knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kocolor_red_absolute)
//!< Number: 10, Text: A{{argChan}} {{0}}, Function: Rot Status
#define ADR_Kocolor_red_state 10
#define KoADR_color_red_stateIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kocolor_red_state)
#define KoADR_color_red_state knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kocolor_red_state)
//!< Number: 11, Text: A{{argChan}} {{0}}, Function: Grün Relativ
#define ADR_Kocolor_green_relative 11
#define KoADR_color_green_relativeIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kocolor_green_relative)
#define KoADR_color_green_relative knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kocolor_green_relative)
//!< Number: 12, Text: A{{argChan}} {{0}}, Function: Grün Absolut
#define ADR_Kocolor_green_absolute 12
#define KoADR_color_green_absoluteIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kocolor_green_absolute)
#define KoADR_color_green_absolute knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kocolor_green_absolute)
//!< Number: 13, Text: A{{argChan}} {{0}}, Function: Grün Status
#define ADR_Kocolor_green_state 13
#define KoADR_color_green_stateIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kocolor_green_state)
#define KoADR_color_green_state knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kocolor_green_state)
//!< Number: 14, Text: A{{argChan}} {{0}}, Function: Blau Relativ
#define ADR_Kocolor_blue_relative 14
#define KoADR_color_blue_relativeIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kocolor_blue_relative)
#define KoADR_color_blue_relative knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kocolor_blue_relative)
//!< Number: 15, Text: A{{argChan}} {{0}}, Function: Blau Absolut
#define ADR_Kocolor_blue_absolute 15
#define KoADR_color_blue_absoluteIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kocolor_blue_absolute)
#define KoADR_color_blue_absolute knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kocolor_blue_absolute)
//!< Number: 16, Text: A{{argChan}} {{0}}, Function: Blau Status
#define ADR_Kocolor_blue_state 16
#define KoADR_color_blue_stateIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kocolor_blue_state)
#define KoADR_color_blue_state knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kocolor_blue_state)
//!< Number: 17, Text: A{{argChan}} {{0}}, Function: HCL Kurve
#define ADR_Kohcl_curve 17
#define KoADR_hcl_curveIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Kohcl_curve)
#define KoADR_hcl_curve knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Kohcl_curve)
//!< Number: 18, Text: A{{argChan}} {{0}}, Function: Szene
#define ADR_Koscene 18
#define KoADR_sceneIndex(X) knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * X + ADR_Koscene)
#define KoADR_scene knx.getGroupObject(ADR_KoOffset + ADR_KoBlockSize * channelIndex() + ADR_Koscene)

//-----Module: group
#define GRP_deviceType		0x0000
#define GRP_deviceType_Shift	6
#define GRP_deviceType_Mask	0x0003
// Offset: 0, Size: 2 Bit, Text: Gerätetyp
#define ParamGRP_deviceTypeIndex(X) ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_deviceType)) >> GRP_deviceType_Shift) & GRP_deviceType_Mask))
// Offset: 0, Size: 2 Bit, Text: Gerätetyp
#define ParamGRP_deviceType ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_deviceType)) >> GRP_deviceType_Shift) & GRP_deviceType_Mask))
#define GRP_type		0x0000
// Offset: 0, BitOffset: 2, Size: 1 Bit, Text: Betriebsart
#define ParamGRP_typeIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_type), 2)
// Offset: 0, BitOffset: 2, Size: 1 Bit, Text: Betriebsart
#define ParamGRP_type knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_type), 2)
#define GRP_nachtriggern		0x0000
// Offset: 0, BitOffset: 3, Size: 1 Bit, Text: Nachtriggern erlauben
#define ParamGRP_nachtriggernIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_nachtriggern), 3)
// Offset: 0, BitOffset: 3, Size: 1 Bit, Text: Nachtriggern erlauben
#define ParamGRP_nachtriggern knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_nachtriggern), 3)
#define GRP_stairtime		0x0001
#define GRP_stairtime_Shift	2
#define GRP_stairtime_Mask	0x3FFF
// Offset: 1, Size: 14 Bit, Text: Nachlaufzeit
#define ParamGRP_stairtimeIndex(X) ((uint)((knx.paramWord((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_stairtime)) >> GRP_stairtime_Shift) & GRP_stairtime_Mask))
// Offset: 1, Size: 14 Bit, Text: Nachlaufzeit
#define ParamGRP_stairtime ((uint)((knx.paramWord((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_stairtime)) >> GRP_stairtime_Shift) & GRP_stairtime_Mask))
#define GRP_manuoff		0x0000
// Offset: 0, BitOffset: 4, Size: 1 Bit, Text: Manuelles ausschalten
#define ParamGRP_manuoffIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_manuoff), 4)
// Offset: 0, BitOffset: 4, Size: 1 Bit, Text: Manuelles ausschalten
#define ParamGRP_manuoff knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_manuoff), 4)
#define GRP_lockbehave		0x0000
#define GRP_lockbehave_Shift	1
#define GRP_lockbehave_Mask	0x0003
// Offset: 0, BitOffset: 5, Size: 2 Bit, Text: Verhalten bei Sperre
#define ParamGRP_lockbehaveIndex(X) ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_lockbehave)) >> GRP_lockbehave_Shift) & GRP_lockbehave_Mask))
// Offset: 0, BitOffset: 5, Size: 2 Bit, Text: Verhalten bei Sperre
#define ParamGRP_lockbehave ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_lockbehave)) >> GRP_lockbehave_Shift) & GRP_lockbehave_Mask))
#define GRP_lockvalue		0x0003
#define GRP_lockvalue_Shift	1
#define GRP_lockvalue_Mask	0x007F
// Offset: 3, Size: 7 Bit, Text: 
#define ParamGRP_lockvalueIndex(X) ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_lockvalue)) >> GRP_lockvalue_Shift) & GRP_lockvalue_Mask))
// Offset: 3, Size: 7 Bit, Text: 
#define ParamGRP_lockvalue ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_lockvalue)) >> GRP_lockvalue_Shift) & GRP_lockvalue_Mask))
#define GRP_unlockbehave		0x0004
#define GRP_unlockbehave_Shift	6
#define GRP_unlockbehave_Mask	0x0003
// Offset: 4, Size: 2 Bit, Text: Verhalten bei Entsperren
#define ParamGRP_unlockbehaveIndex(X) ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_unlockbehave)) >> GRP_unlockbehave_Shift) & GRP_unlockbehave_Mask))
// Offset: 4, Size: 2 Bit, Text: Verhalten bei Entsperren
#define ParamGRP_unlockbehave ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_unlockbehave)) >> GRP_unlockbehave_Shift) & GRP_unlockbehave_Mask))
#define GRP_unlockvalue		0x0005
#define GRP_unlockvalue_Shift	1
#define GRP_unlockvalue_Mask	0x007F
// Offset: 5, Size: 7 Bit, Text: 
#define ParamGRP_unlockvalueIndex(X) ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_unlockvalue)) >> GRP_unlockvalue_Shift) & GRP_unlockvalue_Mask))
// Offset: 5, Size: 7 Bit, Text: 
#define ParamGRP_unlockvalue ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_unlockvalue)) >> GRP_unlockvalue_Shift) & GRP_unlockvalue_Mask))
#define GRP_onDay		0x0006
// Offset: 6, Size: 16 Bit (2 Byte), Text: Einschaltwert Tag
#define ParamGRP_onDayIndex(X) knx.paramFloat((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_onDay), Float_Enc_DPT9)
// Offset: 6, Size: 16 Bit (2 Byte), Text: Einschaltwert Tag
#define ParamGRP_onDay knx.paramFloat((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_onDay), Float_Enc_DPT9)
#define GRP_onNight		0x0008
// Offset: 8, Size: 16 Bit (2 Byte), Text: Einschaltwert Nacht
#define ParamGRP_onNightIndex(X) knx.paramFloat((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_onNight), Float_Enc_DPT9)
// Offset: 8, Size: 16 Bit (2 Byte), Text: Einschaltwert Nacht
#define ParamGRP_onNight knx.paramFloat((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_onNight), Float_Enc_DPT9)
#define GRP_locknegate		0x0000
// Offset: 0, BitOffset: 7, Size: 1 Bit, Text: Sperren bei
#define ParamGRP_locknegateIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_locknegate), 7)
// Offset: 0, BitOffset: 7, Size: 1 Bit, Text: Sperren bei
#define ParamGRP_locknegate knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_locknegate), 7)
#define GRP_colorType		0x0004
#define GRP_colorType_Shift	4
#define GRP_colorType_Mask	0x0003
// Offset: 4, BitOffset: 2, Size: 2 Bit, Text: Farbe ansteuern per
#define ParamGRP_colorTypeIndex(X) ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_colorType)) >> GRP_colorType_Shift) & GRP_colorType_Mask))
// Offset: 4, BitOffset: 2, Size: 2 Bit, Text: Farbe ansteuern per
#define ParamGRP_colorType ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_colorType)) >> GRP_colorType_Shift) & GRP_colorType_Mask))
#define GRP_colorSpace		0x0003
// Offset: 3, BitOffset: 7, Size: 1 Bit, Text: Farbe übertragen per
#define ParamGRP_colorSpaceIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_colorSpace), 7)
// Offset: 3, BitOffset: 7, Size: 1 Bit, Text: Farbe übertragen per
#define ParamGRP_colorSpace knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_colorSpace), 7)
#define GRP_tempMin		0x000A
// Offset: 10, Size: 16 Bit (2 Byte), Text: Farbtemperatur Min
#define ParamGRP_tempMinIndex(X) ((uint)((knx.paramWord((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_tempMin)))))
// Offset: 10, Size: 16 Bit (2 Byte), Text: Farbtemperatur Min
#define ParamGRP_tempMin ((uint)((knx.paramWord((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_tempMin)))))
#define GRP_tempMax		0x000C
// Offset: 12, Size: 16 Bit (2 Byte), Text: Farbtemperatur Max
#define ParamGRP_tempMaxIndex(X) ((uint)((knx.paramWord((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_tempMax)))))
// Offset: 12, Size: 16 Bit (2 Byte), Text: Farbtemperatur Max
#define ParamGRP_tempMax ((uint)((knx.paramWord((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_tempMax)))))
#define GRP_hcl		0x0004
// Offset: 4, BitOffset: 4, Size: 1 Bit, Text: HCL aktivieren
#define ParamGRP_hclIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_hcl), 4)
// Offset: 4, BitOffset: 4, Size: 1 Bit, Text: HCL aktivieren
#define ParamGRP_hcl knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_hcl), 4)
#define GRP_xyIgnore		0x0004
// Offset: 4, BitOffset: 5, Size: 1 Bit, Text: Helligkeit ignorieren? (nur xy verwenden)
#define ParamGRP_xyIgnoreIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_xyIgnore), 5)
// Offset: 4, BitOffset: 5, Size: 1 Bit, Text: Helligkeit ignorieren? (nur xy verwenden)
#define ParamGRP_xyIgnore knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_xyIgnore), 5)
#define GRP_dimmStateInterval		0x000E
#define GRP_dimmStateInterval_Shift	4
#define GRP_dimmStateInterval_Mask	0x000F
// Offset: 14, Size: 4 Bit, Text: Dimmstatus Interval bei relativ
#define ParamGRP_dimmStateIntervalIndex(X) ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_dimmStateInterval)) >> GRP_dimmStateInterval_Shift) & GRP_dimmStateInterval_Mask))
// Offset: 14, Size: 4 Bit, Text: Dimmstatus Interval bei relativ
#define ParamGRP_dimmStateInterval ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_dimmStateInterval)) >> GRP_dimmStateInterval_Shift) & GRP_dimmStateInterval_Mask))
#define GRP_queryTime		0x000F
// Offset: 15, Size: 16 Bit (2 Byte), Text: Dimmwert abfragen (0 = deaktiviert)
#define ParamGRP_queryTimeIndex(X) ((uint)((knx.paramWord((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_queryTime)))))
// Offset: 15, Size: 16 Bit (2 Byte), Text: Dimmwert abfragen (0 = deaktiviert)
#define ParamGRP_queryTime ((uint)((knx.paramWord((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_queryTime)))))
#define GRP_dimmRelDuration		0x0011
// Offset: 17, Size: 8 Bit (1 Byte), Text: Dimmzeit von 0-100% bei relativ
#define ParamGRP_dimmRelDurationIndex(X) ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_dimmRelDuration)))))
// Offset: 17, Size: 8 Bit (1 Byte), Text: Dimmzeit von 0-100% bei relativ
#define ParamGRP_dimmRelDuration ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_dimmRelDuration)))))
#define GRP_hclCurve		0x0004
#define GRP_hclCurve_Mask	0x0003
// Offset: 4, BitOffset: 6, Size: 2 Bit, Text: Verwende
#define ParamGRP_hclCurveIndex(X) ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_hclCurve))) & GRP_hclCurve_Mask))
// Offset: 4, BitOffset: 6, Size: 2 Bit, Text: Verwende
#define ParamGRP_hclCurve ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_hclCurve))) & GRP_hclCurve_Mask))
#define GRP_hclStart		0x0005
// Offset: 5, BitOffset: 7, Size: 1 Bit, Text: HCL anwenden
#define ParamGRP_hclStartIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_hclStart), 7)
// Offset: 5, BitOffset: 7, Size: 1 Bit, Text: HCL anwenden
#define ParamGRP_hclStart knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_hclStart), 7)
#define GRP_dimmLock		0x000E
#define GRP_dimmLock_Shift	2
#define GRP_dimmLock_Mask	0x0003
// Offset: 14, BitOffset: 4, Size: 2 Bit, Text: Einschalten bei rlativ
#define ParamGRP_dimmLockIndex(X) ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_dimmLock)) >> GRP_dimmLock_Shift) & GRP_dimmLock_Mask))
// Offset: 14, BitOffset: 4, Size: 2 Bit, Text: Einschalten bei rlativ
#define ParamGRP_dimmLock ((uint)((knx.paramByte((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_dimmLock)) >> GRP_dimmLock_Shift) & GRP_dimmLock_Mask))
#define GRP_hcl_manu_bri		0x000E
// Offset: 14, BitOffset: 6, Size: 1 Bit, Text: Helligkeit geändert wird
#define ParamGRP_hcl_manu_briIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_hcl_manu_bri), 6)
// Offset: 14, BitOffset: 6, Size: 1 Bit, Text: Helligkeit geändert wird
#define ParamGRP_hcl_manu_bri knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_hcl_manu_bri), 6)
#define GRP_hcl_manu_col		0x000E
// Offset: 14, BitOffset: 7, Size: 1 Bit, Text: Farbe/Temperatur geändert wird
#define ParamGRP_hcl_manu_colIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_hcl_manu_col), 7)
// Offset: 14, BitOffset: 7, Size: 1 Bit, Text: Farbe/Temperatur geändert wird
#define ParamGRP_hcl_manu_col knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_hcl_manu_col), 7)
#define GRP_hcl_auto_off		0x0012
// Offset: 18, Size: 1 Bit, Text: das EVG ausgeschaltet wird
#define ParamGRP_hcl_auto_offIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_hcl_auto_off), 0)
// Offset: 18, Size: 1 Bit, Text: das EVG ausgeschaltet wird
#define ParamGRP_hcl_auto_off knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_hcl_auto_off), 0)
#define GRP_hcl_auto_day		0x0012
// Offset: 18, BitOffset: 1, Size: 1 Bit, Text: der Tag vorbei ist
#define ParamGRP_hcl_auto_dayIndex(X) knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * X + GRP_hcl_auto_day), 1)
// Offset: 18, BitOffset: 1, Size: 1 Bit, Text: der Tag vorbei ist
#define ParamGRP_hcl_auto_day knx.paramBit((GRP_ParamBlockOffset + GRP_ParamBlockSize * channelIndex() + GRP_hcl_auto_day), 1)
//!< Number: 0, Text: G{{argChan}} {{0}}, Function: Schalten
#define GRP_Koswitch 0
#define KoGRP_switchIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Koswitch)
#define KoGRP_switch knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Koswitch)
//!< Number: 1, Text: G{{argChan}} {{0}}, Function: Schalten Status
#define GRP_Koswitch_state 1
#define KoGRP_switch_stateIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Koswitch_state)
#define KoGRP_switch_state knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Koswitch_state)
//!< Number: 2, Text: G{{argChan}} {{0}}, Function: Dimmen Relativ
#define GRP_Kodimm_relative 2
#define KoGRP_dimm_relativeIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kodimm_relative)
#define KoGRP_dimm_relative knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kodimm_relative)
//!< Number: 3, Text: G{{argChan}} {{0}}, Function: Dimmen Absolut
#define GRP_Kodimm_absolute 3
#define KoGRP_dimm_absoluteIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kodimm_absolute)
#define KoGRP_dimm_absolute knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kodimm_absolute)
//!< Number: 4, Text: G{{argChan}} {{0}}, Function: Dimmen Status
#define GRP_Kodimm_state 4
#define KoGRP_dimm_stateIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kodimm_state)
#define KoGRP_dimm_state knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kodimm_state)
//!< Number: 5, Text: G{{argChan}} {{0}}, Function: Sperren
#define GRP_Kolock 5
#define KoGRP_lockIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kolock)
#define KoGRP_lock knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kolock)
//!< Number: 6, Text: G{{argChan}} {{0}}, Function: RGB Farbe
#define GRP_Kocolor 6
#define KoGRP_colorIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kocolor)
#define KoGRP_color knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kocolor)
//!< Number: 7, Text: G{{argChan}} {{0}}, Function: RGB Farbe Status
#define GRP_Kocolor_state 7
#define KoGRP_color_stateIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kocolor_state)
#define KoGRP_color_state knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kocolor_state)
//!< Number: 8, Text: G{{argChan}} {{0}}, Function: Rot Relativ
#define GRP_Kocolor_red_rel 8
#define KoGRP_color_red_relIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kocolor_red_rel)
#define KoGRP_color_red_rel knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kocolor_red_rel)
//!< Number: 9, Text: G{{argChan}} {{0}}, Function: Rot Absolut
#define GRP_Kocolor_red_abs 9
#define KoGRP_color_red_absIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kocolor_red_abs)
#define KoGRP_color_red_abs knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kocolor_red_abs)
//!< Number: 10, Text: G{{argChan}} {{0}}, Function: Rot Status
#define GRP_Kocolor_red_state 10
#define KoGRP_color_red_stateIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kocolor_red_state)
#define KoGRP_color_red_state knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kocolor_red_state)
//!< Number: 11, Text: G{{argChan}} {{0}}, Function: Grün Relativ
#define GRP_Kocolor_green_rel 11
#define KoGRP_color_green_relIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kocolor_green_rel)
#define KoGRP_color_green_rel knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kocolor_green_rel)
//!< Number: 12, Text: G{{argChan}} {{0}}, Function: Grün Absolut
#define GRP_Kocolor_green_abs 12
#define KoGRP_color_green_absIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kocolor_green_abs)
#define KoGRP_color_green_abs knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kocolor_green_abs)
//!< Number: 13, Text: G{{argChan}} {{0}}, Function: Grün Status
#define GRP_Kocolor_green_state 13
#define KoGRP_color_green_stateIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kocolor_green_state)
#define KoGRP_color_green_state knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kocolor_green_state)
//!< Number: 14, Text: G{{argChan}} {{0}}, Function: Blau Relativ
#define GRP_Kocolor_blue_rel 14
#define KoGRP_color_blue_relIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kocolor_blue_rel)
#define KoGRP_color_blue_rel knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kocolor_blue_rel)
//!< Number: 15, Text: G{{argChan}} {{0}}, Function: Blau Absolut
#define GRP_Kocolor_blue_abs 15
#define KoGRP_color_blue_absIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kocolor_blue_abs)
#define KoGRP_color_blue_abs knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kocolor_blue_abs)
//!< Number: 16, Text: G{{argChan}} {{0}}, Function: Blau Status
#define GRP_Kocolor_blue_state 16
#define KoGRP_color_blue_stateIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kocolor_blue_state)
#define KoGRP_color_blue_state knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kocolor_blue_state)
//!< Number: 17, Text: G{{argChan}} {{0}}, Function: HCL Kurve
#define GRP_Kohcl_curve 17
#define KoGRP_hcl_curveIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Kohcl_curve)
#define KoGRP_hcl_curve knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Kohcl_curve)
//!< Number: 18, Text: G{{argChan}} {{0}}, Function: Szene
#define GRP_Koscene 18
#define KoGRP_sceneIndex(X) knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * X + GRP_Koscene)
#define KoGRP_scene knx.getGroupObject(GRP_KoOffset + GRP_KoBlockSize * channelIndex() + GRP_Koscene)

//-----Module: scene
#define SCE_type		0x0000
#define SCE_type_Shift	6
#define SCE_type_Mask	0x0003
// Offset: 0, Size: 2 Bit, Text: Senden an
#define ParamSCE_typeIndex(X) ((uint)((knx.paramByte((SCE_ParamBlockOffset + SCE_ParamBlockSize * X + SCE_type)) >> SCE_type_Shift) & SCE_type_Mask))
// Offset: 0, Size: 2 Bit, Text: Senden an
#define ParamSCE_type ((uint)((knx.paramByte((SCE_ParamBlockOffset + SCE_ParamBlockSize * channelIndex() + SCE_type)) >> SCE_type_Shift) & SCE_type_Mask))
#define SCE_save		0x0000
// Offset: 0, BitOffset: 2, Size: 1 Bit, Text: Speichern erlauben
#define ParamSCE_saveIndex(X) knx.paramBit((SCE_ParamBlockOffset + SCE_ParamBlockSize * X + SCE_save), 2)
// Offset: 0, BitOffset: 2, Size: 1 Bit, Text: Speichern erlauben
#define ParamSCE_save knx.paramBit((SCE_ParamBlockOffset + SCE_ParamBlockSize * channelIndex() + SCE_save), 2)
#define SCE_numberKnx		0x0001
#define SCE_numberKnx_Shift	1
#define SCE_numberKnx_Mask	0x007F
// Offset: 1, Size: 7 Bit, Text: Szenennummer Knx
#define ParamSCE_numberKnxIndex(X) ((uint)((knx.paramByte((SCE_ParamBlockOffset + SCE_ParamBlockSize * X + SCE_numberKnx)) >> SCE_numberKnx_Shift) & SCE_numberKnx_Mask))
// Offset: 1, Size: 7 Bit, Text: Szenennummer Knx
#define ParamSCE_numberKnx ((uint)((knx.paramByte((SCE_ParamBlockOffset + SCE_ParamBlockSize * channelIndex() + SCE_numberKnx)) >> SCE_numberKnx_Shift) & SCE_numberKnx_Mask))
#define SCE_numberDali		0x0000
#define SCE_numberDali_Shift	1
#define SCE_numberDali_Mask	0x000F
// Offset: 0, BitOffset: 3, Size: 4 Bit, Text: Szenennummer Dali
#define ParamSCE_numberDaliIndex(X) ((uint)((knx.paramByte((SCE_ParamBlockOffset + SCE_ParamBlockSize * X + SCE_numberDali)) >> SCE_numberDali_Shift) & SCE_numberDali_Mask))
// Offset: 0, BitOffset: 3, Size: 4 Bit, Text: Szenennummer Dali
#define ParamSCE_numberDali ((uint)((knx.paramByte((SCE_ParamBlockOffset + SCE_ParamBlockSize * channelIndex() + SCE_numberDali)) >> SCE_numberDali_Shift) & SCE_numberDali_Mask))
#define SCE_address		0x0002
#define SCE_address_Shift	2
#define SCE_address_Mask	0x003F
// Offset: 2, Size: 6 Bit, Text: Dali Adresse
#define ParamSCE_addressIndex(X) ((uint)((knx.paramByte((SCE_ParamBlockOffset + SCE_ParamBlockSize * X + SCE_address)) >> SCE_address_Shift) & SCE_address_Mask))
// Offset: 2, Size: 6 Bit, Text: Dali Adresse
#define ParamSCE_address ((uint)((knx.paramByte((SCE_ParamBlockOffset + SCE_ParamBlockSize * channelIndex() + SCE_address)) >> SCE_address_Shift) & SCE_address_Mask))
#define SCE_group		0x0003
#define SCE_group_Shift	4
#define SCE_group_Mask	0x000F
// Offset: 3, Size: 4 Bit, Text: Dali Gruppe
#define ParamSCE_groupIndex(X) ((uint)((knx.paramByte((SCE_ParamBlockOffset + SCE_ParamBlockSize * X + SCE_group)) >> SCE_group_Shift) & SCE_group_Mask))
// Offset: 3, Size: 4 Bit, Text: Dali Gruppe
#define ParamSCE_group ((uint)((knx.paramByte((SCE_ParamBlockOffset + SCE_ParamBlockSize * channelIndex() + SCE_group)) >> SCE_group_Shift) & SCE_group_Mask))

//-----Module: hcl
#define HCL_type		0x0000
#define HCL_type_Shift	6
#define HCL_type_Mask	0x0003
// Offset: 0, Size: 2 Bit, Text: Ansteuerung über
#define ParamHCL_typeIndex(X) ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * X + HCL_type)) >> HCL_type_Shift) & HCL_type_Mask))
// Offset: 0, Size: 2 Bit, Text: Ansteuerung über
#define ParamHCL_type ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * channelIndex() + HCL_type)) >> HCL_type_Shift) & HCL_type_Mask))
#define HCL_min		0x0001
// Offset: 1, Size: 16 Bit (2 Byte), Text: Farbtemperatur Min
#define ParamHCL_minIndex(X) ((uint)((knx.paramWord((HCL_ParamBlockOffset + HCL_ParamBlockSize * X + HCL_min)))))
// Offset: 1, Size: 16 Bit (2 Byte), Text: Farbtemperatur Min
#define ParamHCL_min ((uint)((knx.paramWord((HCL_ParamBlockOffset + HCL_ParamBlockSize * channelIndex() + HCL_min)))))
#define HCL_max		0x0003
// Offset: 3, Size: 16 Bit (2 Byte), Text: Farbtemperatur Max
#define ParamHCL_maxIndex(X) ((uint)((knx.paramWord((HCL_ParamBlockOffset + HCL_ParamBlockSize * X + HCL_max)))))
// Offset: 3, Size: 16 Bit (2 Byte), Text: Farbtemperatur Max
#define ParamHCL_max ((uint)((knx.paramWord((HCL_ParamBlockOffset + HCL_ParamBlockSize * channelIndex() + HCL_max)))))
#define HCL_offsetRiseType		0x0000
#define HCL_offsetRiseType_Shift	4
#define HCL_offsetRiseType_Mask	0x0003
// Offset: 0, BitOffset: 2, Size: 2 Bit, Text: Verschiebe Sonnenaufgang
#define ParamHCL_offsetRiseTypeIndex(X) ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * X + HCL_offsetRiseType)) >> HCL_offsetRiseType_Shift) & HCL_offsetRiseType_Mask))
// Offset: 0, BitOffset: 2, Size: 2 Bit, Text: Verschiebe Sonnenaufgang
#define ParamHCL_offsetRiseType ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * channelIndex() + HCL_offsetRiseType)) >> HCL_offsetRiseType_Shift) & HCL_offsetRiseType_Mask))
#define HCL_offsetRiseMin		0x0005
// Offset: 5, Size: 8 Bit (1 Byte), Text: 
#define ParamHCL_offsetRiseMinIndex(X) ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * X + HCL_offsetRiseMin)))))
// Offset: 5, Size: 8 Bit (1 Byte), Text: 
#define ParamHCL_offsetRiseMin ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * channelIndex() + HCL_offsetRiseMin)))))
#define HCL_offsetSetType		0x0000
#define HCL_offsetSetType_Shift	2
#define HCL_offsetSetType_Mask	0x0003
// Offset: 0, BitOffset: 4, Size: 2 Bit, Text: Verschiebe Sonnenuntergang
#define ParamHCL_offsetSetTypeIndex(X) ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * X + HCL_offsetSetType)) >> HCL_offsetSetType_Shift) & HCL_offsetSetType_Mask))
// Offset: 0, BitOffset: 4, Size: 2 Bit, Text: Verschiebe Sonnenuntergang
#define ParamHCL_offsetSetType ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * channelIndex() + HCL_offsetSetType)) >> HCL_offsetSetType_Shift) & HCL_offsetSetType_Mask))
#define HCL_offsetSetMin		0x0006
// Offset: 6, Size: 8 Bit (1 Byte), Text: 
#define ParamHCL_offsetSetMinIndex(X) ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * X + HCL_offsetSetMin)))))
// Offset: 6, Size: 8 Bit (1 Byte), Text: 
#define ParamHCL_offsetSetMin ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * channelIndex() + HCL_offsetSetMin)))))
#define HCL_briMin		0x0007
#define HCL_briMin_Shift	1
#define HCL_briMin_Mask	0x007F
// Offset: 7, Size: 7 Bit, Text: Helligkeit Min
#define ParamHCL_briMinIndex(X) ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * X + HCL_briMin)) >> HCL_briMin_Shift) & HCL_briMin_Mask))
// Offset: 7, Size: 7 Bit, Text: Helligkeit Min
#define ParamHCL_briMin ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * channelIndex() + HCL_briMin)) >> HCL_briMin_Shift) & HCL_briMin_Mask))
#define HCL_briMax		0x0008
#define HCL_briMax_Shift	1
#define HCL_briMax_Mask	0x007F
// Offset: 8, Size: 7 Bit, Text: Helligkeit Max
#define ParamHCL_briMaxIndex(X) ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * X + HCL_briMax)) >> HCL_briMax_Shift) & HCL_briMax_Mask))
// Offset: 8, Size: 7 Bit, Text: Helligkeit Max
#define ParamHCL_briMax ((uint)((knx.paramByte((HCL_ParamBlockOffset + HCL_ParamBlockSize * channelIndex() + HCL_briMax)) >> HCL_briMax_Shift) & HCL_briMax_Mask))
#define HCL_checkTemperature		0x0000
// Offset: 0, BitOffset: 6, Size: 1 Bit, Text: Farbtemperatur aktivieren
#define ParamHCL_checkTemperatureIndex(X) knx.paramBit((HCL_ParamBlockOffset + HCL_ParamBlockSize * X + HCL_checkTemperature), 6)
// Offset: 0, BitOffset: 6, Size: 1 Bit, Text: Farbtemperatur aktivieren
#define ParamHCL_checkTemperature knx.paramBit((HCL_ParamBlockOffset + HCL_ParamBlockSize * channelIndex() + HCL_checkTemperature), 6)
#define HCL_checkBrightness		0x0000
// Offset: 0, BitOffset: 7, Size: 1 Bit, Text: Helligkeit aktivieren
#define ParamHCL_checkBrightnessIndex(X) knx.paramBit((HCL_ParamBlockOffset + HCL_ParamBlockSize * X + HCL_checkBrightness), 7)
// Offset: 0, BitOffset: 7, Size: 1 Bit, Text: Helligkeit aktivieren
#define ParamHCL_checkBrightness knx.paramBit((HCL_ParamBlockOffset + HCL_ParamBlockSize * channelIndex() + HCL_checkBrightness), 7)
//!< Number: 0, Text: HCL Kurve {{argChan}}, Function: Farbtemperatur
#define HCL_Kohcl_state 0
#define KoHCL_hcl_stateIndex(X) knx.getGroupObject(HCL_KoOffset + HCL_KoBlockSize * X + HCL_Kohcl_state)
#define KoHCL_hcl_state knx.getGroupObject(HCL_KoOffset + HCL_KoBlockSize * channelIndex() + HCL_Kohcl_state)
//!< Number: 1, Text: HCL Kurve {{argChan}}, Function: Helligkeit
#define HCL_Kobri_state 1
#define KoHCL_bri_stateIndex(X) knx.getGroupObject(HCL_KoOffset + HCL_KoBlockSize * X + HCL_Kobri_state)
#define KoHCL_bri_state knx.getGroupObject(HCL_KoOffset + HCL_KoBlockSize * channelIndex() + HCL_Kobri_state)

//-----Module: Common Share
#define BASE_StartupDelayBase		0x0000
#define BASE_StartupDelayBase_Shift	6
#define BASE_StartupDelayBase_Mask	0x0003
// UnionOffset: 0, ParaOffset: 0, Size: 2 Bit, Text: Zeitbasis
#define ParamBASE_StartupDelayBase ((uint)((knx.paramByte((BASE_Share_ParamBlockOffset + BASE_StartupDelayBase)) >> BASE_StartupDelayBase_Shift) & BASE_StartupDelayBase_Mask))
#define BASE_StartupDelayTime		0x0000
#define BASE_StartupDelayTime_Mask	0x3FFF
// UnionOffset: 0, ParaOffset: 0, BitOffset: 2, Size: 14 Bit, Text: Zeit
#define ParamBASE_StartupDelayTime ((uint)((knx.paramWord((BASE_Share_ParamBlockOffset + BASE_StartupDelayTime))) & BASE_StartupDelayTime_Mask))
#define ParamBASE_StartupDelayTimeMS (paramDelay(ParamBASE_StartupDelayTime))
#define BASE_HeartbeatDelayBase		0x0002
#define BASE_HeartbeatDelayBase_Shift	6
#define BASE_HeartbeatDelayBase_Mask	0x0003
// UnionOffset: 0, ParaOffset: 2, Size: 2 Bit, Text: Zeitbasis
#define ParamBASE_HeartbeatDelayBase ((uint)((knx.paramByte((BASE_Share_ParamBlockOffset + BASE_HeartbeatDelayBase)) >> BASE_HeartbeatDelayBase_Shift) & BASE_HeartbeatDelayBase_Mask))
#define BASE_HeartbeatDelayTime		0x0002
#define BASE_HeartbeatDelayTime_Mask	0x3FFF
// UnionOffset: 0, ParaOffset: 2, BitOffset: 2, Size: 14 Bit, Text: Zeit
#define ParamBASE_HeartbeatDelayTime ((uint)((knx.paramWord((BASE_Share_ParamBlockOffset + BASE_HeartbeatDelayTime))) & BASE_HeartbeatDelayTime_Mask))
#define ParamBASE_HeartbeatDelayTimeMS (paramDelay(ParamBASE_HeartbeatDelayTime))
#define BASE_Timezone		0x0004
#define BASE_Timezone_Shift	3
#define BASE_Timezone_Mask	0x001F
// UnionOffset: 4, ParaOffset: 0, Size: 5 Bit, Text: Zeitzone
#define ParamBASE_Timezone ((uint)((knx.paramByte((BASE_Share_ParamBlockOffset + BASE_Timezone)) >> BASE_Timezone_Shift) & BASE_Timezone_Mask))
#define BASE_CombinedTimeDate		0x0004
// UnionOffset: 4, ParaOffset: 0, BitOffset: 5, Size: 1 Bit, Text: Empfangen über
#define ParamBASE_CombinedTimeDate knx.paramBit((BASE_Share_ParamBlockOffset + BASE_CombinedTimeDate), 5)
#define BASE_SummertimeAll		0x0004
#define BASE_SummertimeAll_Mask	0x0003
// UnionOffset: 4, ParaOffset: 0, BitOffset: 6, Size: 2 Bit, Text: Sommerzeit ermitteln durch
#define ParamBASE_SummertimeAll ((uint)((knx.paramByte((BASE_Share_ParamBlockOffset + BASE_SummertimeAll))) & BASE_SummertimeAll_Mask))
#define BASE_SummertimeDE		0x0004
#define BASE_SummertimeDE_Mask	0x0003
// UnionOffset: 4, ParaOffset: 0, BitOffset: 6, Size: 2 Bit, Text: Sommerzeit ermitteln durch
#define ParamBASE_SummertimeDE ((uint)((knx.paramByte((BASE_Share_ParamBlockOffset + BASE_SummertimeDE))) & BASE_SummertimeDE_Mask))
#define BASE_SummertimeWorld		0x0004
#define BASE_SummertimeWorld_Mask	0x0003
// UnionOffset: 4, ParaOffset: 0, BitOffset: 6, Size: 2 Bit, Text: Sommerzeit ermitteln durch
#define ParamBASE_SummertimeWorld ((uint)((knx.paramByte((BASE_Share_ParamBlockOffset + BASE_SummertimeWorld))) & BASE_SummertimeWorld_Mask))
#define BASE_SummertimeKO		0x0004
#define BASE_SummertimeKO_Mask	0x0003
// UnionOffset: 4, ParaOffset: 0, BitOffset: 6, Size: 2 Bit, Text: Sommerzeit ermitteln durch
#define ParamBASE_SummertimeKO ((uint)((knx.paramByte((BASE_Share_ParamBlockOffset + BASE_SummertimeKO))) & BASE_SummertimeKO_Mask))
#define BASE_Latitude		0x0005
// UnionOffset: 5, ParaOffset: 0, Size: 16 Bit (2 Byte), Text: Breitengrad
#define ParamBASE_Latitude knx.paramFloat((BASE_Share_ParamBlockOffset + BASE_Latitude), Float_Enc_IEEE754Single)
#define BASE_Longitude		0x0009
// UnionOffset: 5, ParaOffset: 4, Size: 16 Bit (2 Byte), Text: Längengrad
#define ParamBASE_Longitude knx.paramFloat((BASE_Share_ParamBlockOffset + BASE_Longitude), Float_Enc_IEEE754Single)
#define BASE_Diagnose		0x000D
// UnionOffset: 13, ParaOffset: 0, Size: 1 Bit, Text: Diagnoseobjekt anzeigen
#define ParamBASE_Diagnose knx.paramBit((BASE_Share_ParamBlockOffset + BASE_Diagnose), 0)
#define BASE_Watchdog		0x000D
// UnionOffset: 13, ParaOffset: 0, BitOffset: 1, Size: 1 Bit, Text: Watchdog aktivieren
#define ParamBASE_Watchdog knx.paramBit((BASE_Share_ParamBlockOffset + BASE_Watchdog), 1)
#define BASE_ReadTimeDate		0x000D
// UnionOffset: 13, ParaOffset: 0, BitOffset: 2, Size: 1 Bit, Text: Bei Neustart vom Bus lesen
#define ParamBASE_ReadTimeDate knx.paramBit((BASE_Share_ParamBlockOffset + BASE_ReadTimeDate), 2)
#define BASE_HeartbeatExtended		0x000D
// UnionOffset: 13, ParaOffset: 0, BitOffset: 3, Size: 1 Bit, Text: Erweitertes "In Betrieb"
#define ParamBASE_HeartbeatExtended knx.paramBit((BASE_Share_ParamBlockOffset + BASE_HeartbeatExtended), 3)
#define BASE_ManualSave		0x000D
#define BASE_ManualSave_Mask	0x0007
// UnionOffset: 13, ParaOffset: 0, BitOffset: 5, Size: 3 Bit, Text: Manuelles speichern
#define ParamBASE_ManualSave ((uint)((knx.paramByte((BASE_Share_ParamBlockOffset + BASE_ManualSave))) & BASE_ManualSave_Mask))
#define BASE_PeriodicSave		0x000E
// UnionOffset: 13, ParaOffset: 1, Size: 8 Bit (1 Byte), Text: Zyklisches speichern
#define ParamBASE_PeriodicSave ((uint)((knx.paramByte((BASE_Share_ParamBlockOffset + BASE_PeriodicSave)))))
//!< Number: 0, Text: In Betrieb, Function: Zyklisch
#define BASE_KoHeartbeat 0
#define KoBASE_Heartbeat knx.getGroupObject(BASE_KoHeartbeat + BASE_Share_KoOffset)
//!< Number: 1, Text: Uhrzeit/Datum, Function: Eingang
#define BASE_KoTime 1
#define KoBASE_Time knx.getGroupObject(BASE_KoTime + BASE_Share_KoOffset)
//!< Number: 2, Text: Datum, Function: Eingang
#define BASE_KoDate 2
#define KoBASE_Date knx.getGroupObject(BASE_KoDate + BASE_Share_KoOffset)
//!< Number: 3, Text: Diagnose, Function: Diagnoseobjekt
#define BASE_KoDiagnose 3
#define KoBASE_Diagnose knx.getGroupObject(BASE_KoDiagnose + BASE_Share_KoOffset)
//!< Number: 4, Text: Sommerzeit aktiv, Function: Eingang
#define BASE_KoIsSummertime 4
#define KoBASE_IsSummertime knx.getGroupObject(BASE_KoIsSummertime + BASE_Share_KoOffset)
//!< Number: 5, Text: Speichern, Function: Eingang
#define BASE_KoManualSave 5
#define KoBASE_ManualSave knx.getGroupObject(BASE_KoManualSave + BASE_Share_KoOffset)

