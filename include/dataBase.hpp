#include "Arduino.h"
#include "baseNode.hpp"

class DataBase
{
public:
    BaseNode GLOBAl_OPScreen;
    BaseNode SCR1_BatteryState;
    BaseNode SCR1_MilkTempC;
    BaseNode SCR1_TasksCount;
    BaseNode SCR1_TaskDescription;
    BaseNode SCR1_TaskProgress_SCR1n2_Description;
    BaseNode SCR1_TaskStepByStepDescription;
    BaseNode SCR3_BlowGunVolume;
    BaseNode SCR6_GETSET_Day;
    BaseNode SCR6_GETSET_Month;
    BaseNode SCR6_GETSET_Year;
    BaseNode SCR6_GETSET_RTCHH;
    BaseNode SCR6_GETSET_RTCMM;
    BaseNode SCR6_GETSET_AlarmHH;
    BaseNode SCR6_GETSET_AlarmMM;
    BaseNode SCR6_AlarmMode;
    BaseNode SCR9_GETSET_LCDfreeq;
    BaseNode SCR10_GETSET_PasteurDuratMM;
    BaseNode SCR10_GETSET_HeatingDuratMM;
    BaseNode SCR10_GETSET_PasteurTempC;
    BaseNode SCR10_GETSET_HeatingTempC;
    BaseNode SCR11_GETSET_BlowGunCalibMS;
    BaseNode SCR1_COMM_ESC_ToCancelScreen;
    BaseNode SCR1_COMM_ALM_ONOFFMixer;
    BaseNode SCR2_COMM_9_AcceptCancelling;
    BaseNode SCR3_COMM_UP_BlowgunPlus250;
    BaseNode SCR3_COMM_DOWN_BlowgunMin250;
    BaseNode SCR3_COMM_FreeButt_StartBlowgun;
    BaseNode SCR4_COMM_2_StartWashingProgramm;
    BaseNode SCR4_COMM_1_StartPasteurProgramm;
    BaseNode SCR6_COMM_UPDOWN_SwitchAlarmMode;
    BaseNode SCR6_COMM_SET_AcceptNewOPDateTime;
    BaseNode SCR9_COMM_SET_AcceptNewDataFreeq;
    BaseNode SCR10_COMM_SET_AcceptNewPHParams;
    BaseNode SCR11_COMM_SET_AcceptBlowgunCalib;
    BaseNode SCR11_COMM_TestBlowgunCalib;

    DataBase();
    void init();
};