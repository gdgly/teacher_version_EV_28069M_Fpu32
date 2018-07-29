/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
#ifndef _USER_DATA_H_
#define _USER_DATA_H_

//! \file   solutions/instaspin_motion/src/ctrl.h
//! \brief Contains the public interface, object and function definitions for 
//!        various functions related to the CTRL object 
//!
//! (C) Copyright 2011, Texas Instruments, Inc.


// **************************************************************************
// the includes

#include "sw/modules/types/src/types.h"
#include "sw/drivers/sci/src/32b/f28x/f2806x/sci.h"
#include "sw/drivers/gpio/src/32b/f28x/f2806x/gpio.h"
#include "sw/modules/hal/boards/hvkit_rev1p1/f28x/f2806x/src/hal.h"

#include "sw/modules/filter/src/32b/filter_fo.h"

//#include "main.h"
#include "sci_message.h"
#include "i2c_24lc32.h"
#include "filter.h"





#ifdef __cplusplus
extern "C" {
#endif

//#define DM_OPERATION_RUN_BITS            (1 << 0)
//#define DM_OPERATION_FORWARD_BITS		 (1 << 1)
//#define DM_OPEARTION_EXTERNAL_FAULT		 (1 << 4)
//#define DM_OPEARTION_FAULT_RESET		 (1 << 5)
#define DM_FOR_TERM1     (1 << 0)
#define DM_REV_TERM2     (1 << 1)

typedef enum
{
	CTL_FOC_Open = 0,
	CTL_FOC_w_Encoder1 = 1,
	CTL_FOC_w_Encoder2 = 2,
	CTL_FOC_w_Hall1 =3,
} CTL_METHOD_e;

typedef enum
{
	CTL_Current_Loop = 0,
	CTL_Speed_Loop = 1,
	CTL_Position_Loop = 2
} CTL_LOOP_e;

typedef enum
{
    REF_DigialOperator = 0,
    REF_Terminal = 1,
    REF_SerinalCommum = 2,
    REF_OptionPCB =3,
} REF_SELECTION_e;


typedef enum
{
    STOP_Ramp = 0,
    STOP_Coast = 1,
    STOP_DCInjection =2,
    STOP_Timer  = 3
} STOP_SELECTION_e;

typedef enum
{
    FaultType_NoFault = (0<< 0),
    FaultType_EEProm24LC32 = (1 << 0),
    FaultType_IoExpander = (1 << 1),
} HARDWARE_FaultType_e;

typedef enum
{
    TripType_NoTrip   = (0 << 0),
    TripType_Hardware = (1 << 0),
    TripType_UV       = (1 << 1),
    TripType_OV       = (1 << 2),
    TripType_OC       = (1 << 3),
    TripType_OH       = (1 << 4),
} INVERTER_TripType_e;

typedef enum
{
    Signal_Stop = (0 << 0),
    Signal_Run = (1 << 0),
    Signal_Reverse = (1 << 1),
    Signal_Fault = (1 << 2),
    Signal_Reset = (1 << 3),
} Operation_Signal_e;

typedef enum
{
    OP_ACCESS_MONITOR = 0x00,
    OP_ACCESS_USER    = 0x01,
    OP_ACCESS_QUICK   = 0x02,
    OP_ACCESS_BASIC   = 0x03,
    OP_ACCESS_ADVANCE = 0x04
} OPERATOR_ACCESS_e;


typedef enum
{
	DM_TYPE_Int16 = 0x00,
	DM_TYPE_UInt16 = 0x01,
	DM_TYPE_Int8 = 0x02,
	DM_TYPE_UInt8 = 0x03
} DM_TYPE_e;




typedef enum
{
	DM_ATTRIBUTE_Read 	= 0x0001,
	DM_ATTRIBUTE_Write 	= 0x0002,
	DM_ATTRIBUTE_Write_Run = 0x0004,


	DM_ATTRIBUTE_Point0 = 0x0000,
	DM_ATTRIBUTE_Point1 = 0x0010,
	DM_ATTRIBUTE_Point2 = 0x0020,
	DM_ATTRIBUTE_Point3 = 0x0030,
	DM_ATTRIBUTE_Hex2	= 0x0040,
	DM_ATTRIBUTE_Hex4	= 0x0050,
	DM_ATTRIBUTE_Sts    = 0x0060,

	DM_ATTRIBUTE_Int16  = 0x0100,
	DM_ATTRIBUTE_UInt16 = 0x0200,
	DM_ATTRIBUTE_Int8   = 0x0300,
	DM_ATTRIBUTE_UInt8  = 0x0400

	//DM_ATTRIBUTE_Value = 0x1000
} DM_ATTRIBUTE_e;




typedef enum
{
	DM_UNIT_None = 0x0001,
	DM_UNIT_Hz	 = 0x0002,
	DM_UNIT_RPM	 = 0x0003,
	DM_UNIT_Amp	 = 0x0004,
	DM_UNIT_Volt = 0x0005,
	DM_UNIT_Power = 0x0006,
	DM_UNIT_Percent = 0x0007,
	DM_UNIT_Sec	 = 0x0008,
	DM_UNIT_Ntm  = 0x0009,
	DM_UNIT_Temp = 0x0010,
	DM_UNIT_TRIP = 0x0011
} DM_UNIT_e;

#define I2C_ADDRESS_INVALID		0xffff
#define MODBUS_ADDRESS_INVALID	0xffff



typedef struct _DM_Obj_ *DM_Handle;
//typedef struct _DM_Cell_ DM_Cell;

//typedef void (*setCallbackFunction)(DM_Handle handle, const struct _DM_Cell_ *pdmCell);
//typedef int32_t (*getCallbackFunction)(DM_Handle handle);

typedef struct _DM_Cell_
{
	void			*pValue;
	uint16_t		u16EEPromAddr, u16ModbusAdd;
	uint16_t		u16DefValue, u16MaxValue, u16MinValue;
	uint16_t 		u16Attribute, u16Unit;

	//int32_t (*CallbackValue)(const struct _DM_Cell_ *pdmCell);
	int32_t (*getCallbackFunction)(DM_Handle handle);
	void (*setCallbackFunction)(DM_Handle handle, const struct _DM_Cell_ *pdmCell);
	//CallbackValue	CallbackValue;
} DM_Cell;

//typedef int32_t (*CallbackValue)(const DM_Cell *pdmCell);
//typedef int32_t (*CallbackValue)(void);

typedef struct _DM_FunCell_
{
	const DM_Cell	*pdmCell;
	uint_least8_t	u8Index;
	char			ai8CellName[20];
	const char*		pai8DescArray;
} DM_FunCell;


typedef struct _DM_Function_
{
	const DM_FunCell*	*pdmFunCell;
	uint_least8_t	u8ArraySize;
	char			ai8FunSym[3];
	char			ai8FunName[20];
} DM_Function;


typedef struct _DM_Group_
{
	const DM_Function*	*pdmFunction;
	uint_least8_t	u8ArraySize;
	char			ai8GroupSym[2];
	char			ai8GroupName[20];
} DM_Group;


typedef struct _DM_Mode_
{
	const DM_Group*	*pdmGroup;
	uint_least8_t	u8ArraySize;
	char			ai8ModeName[20];
} DM_Mode;

#define MAX_TRIP_TRACE  5
typedef struct _DM_Obj_
{
    EEPROM_Handle eepromHandle;

	const DM_Mode* *pdmMode;
	uint_least8_t	u8ArraySize;
	char			ai8ObjName[20];

	FILTER_Obj     filterDCBus, filterOutFreq, filterOutEncoder, filterOutCurrent;
	FILTER_Handle  filterHandleDCBus, filterHandleOutFreq, filterHandleOutEncoder, filterHandleOutCurrent;

	_iq            iqOutVoltageU, iqOutVoltageV, iqOutVoltageW, iqOutCurrentU, iqOutCurrentV, iqOutCurrentW;

	bool                    bLocalMode, bAutotuneMode,bReverseProhibit;
	uint_least8_t           u8OperationSignal;
	HARDWARE_FaultType_e    hardwareFaultType;
	//INVERTER_TripType_e     inverterTripType;
	INVERTER_TripType_e		tripTypeTrace[5];


	OPERATOR_ACCESS_e       operatorAccess;
	CTL_METHOD_e            ctlMethod;
	CTL_LOOP_e   			ctlLoop;
	REF_SELECTION_e         refSelect, runSelect;
	STOP_SELECTION_e        stopSelect;
	int16_t                 i16RefFreqHz, i16RefCurrent;
	uint16_t                u16OutputTerm, u16AccTime;
	uint16_t				u16DCBusMax, u16DCBusMin, u16CurrentMax, u16CurrentMin;
} DM_Obj;




extern DM_Obj gdmObj;
extern DM_Handle gdmHandle;

extern uint16_t g_u16DelayCnt;

extern uint16_t DATA_min(uint16_t u16Val1, uint16_t u16Val2);
extern void Data_SetDelayCnt(uint16_t u16Delay);
extern uint16_t Data_GetDelayCnt();		// internal
extern uint16_t DATA_ConvertToCnt(uint16_t u16Delay);
extern void DATA_Delay(uint16_t u16Delay);
extern uint16_t DATA_MakeWord(uint_least8_t u8HiWord, uint_least8_t u8LoWord );
extern uint_least8_t DATA_HiByte(uint16_t u16Value);
extern uint_least8_t DATA_LoByte(uint16_t u16Value);
extern uint16_t DATA_CalCRC16(uint_least8_t au8Buff[], uint_least8_t u8TotNo);
extern uint16_t DATA_InRange(DM_TYPE_e dmType, uint16_t u16Value, uint16_t u16DefValue, uint16_t u16MaxValue, uint16_t u16MinValue);

extern DM_Handle DM_init(const void *pMemory,const size_t numBytes);
extern void DM_setup(DM_Handle dmHandle, EEPROM_Handle eepromHandle);
extern void DM_run(DM_Handle dmHandle, HAL_AdcData_t *pAdcData, _iq iqOutFreq, _iq iqOutCur, _iq OutEncoder);


inline bool DM_isInverterTrip(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return (pdmObj->hardwareFaultType != FaultType_NoFault);
}


inline bool DM_isHardwareFault(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return (pdmObj->tripTypeTrace[0] != TripType_NoTrip);
}



inline bool DM_isLocalMode(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return pdmObj->bLocalMode;
}

inline bool DM_isAutotuneMode(DM_Handle dmHandle)
{
	 DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	 return pdmObj->bAutotuneMode;
}

inline bool DM_isRunStatus(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return  (pdmObj->u8OperationSignal &  Signal_Run);
}

inline bool DM_isStopStatus(DM_Handle dmHandle)
{
   return ~DM_isRunStatus(dmHandle);
}

inline bool DM_isRefSelectRemote(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return (pdmObj->refSelect != REF_DigialOperator);
}

inline bool DM_isRunSelectRemote(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return (pdmObj->runSelect != REF_DigialOperator);
}

//REF_SELECTION_e refSelect, runSelect;

extern void DM_toggleLocalRemoteMode(DM_Handle dmHandle);

inline void DM_setRunStatus(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    //if (DM_isLocalMode(dmHandle))
    pdmObj->u8OperationSignal |=   Signal_Run;
}

inline void DM_setStopStatus(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    //if (DM_isLocalMode(dmHandle))
    pdmObj->u8OperationSignal &= (~Signal_Run);

}

/*inline bool DM_getLocalMode(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return pdmObj->bLocalMode;
}*/

inline void DM_setLocalRemoteMode(DM_Handle dmHandle, bool bFlag)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    pdmObj->bLocalMode = bFlag;
}

inline void DM_setAutotuneMode(DM_Handle dmHandle, bool bFlag)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    pdmObj->bAutotuneMode = bFlag;
}

inline REF_SELECTION_e DM_getRefSelect(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return pdmObj->refSelect;
}

inline void DM_setRefSelect(DM_Handle handle, REF_SELECTION_e refSelect)
{
    DM_Obj *pObj = (DM_Obj *)handle;
    pObj->refSelect = refSelect;
}

inline REF_SELECTION_e DM_getRunSelect(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return pdmObj->runSelect;
}

inline DM_setrunSelect(DM_Handle dmHandle,REF_SELECTION_e  opSelect)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    pdmObj->runSelect = opSelect;
}

inline CTL_METHOD_e DM_getCtlMethod(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return pdmObj->ctlMethod;
}

inline CTL_LOOP_e DM_getCtlLoop(DM_Handle dmHandle)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	return pdmObj->ctlLoop;
}

inline void DM_setCtlMethod(DM_Handle handle, CTL_METHOD_e ctlMethod)
{
    DM_Obj *pObj = (DM_Obj *)handle;
    pObj->ctlMethod = ctlMethod;
}


inline _iq DM_getFreqOutPu(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return FILTER_getValue(pdmObj->filterHandleOutFreq);
}

inline bool DM_isFreqOutLowSpeed(DM_Handle dmHandle)
{
    return _IQabs(DM_getFreqOutPu(dmHandle) < _IQ(0.01));
}

extern void DM_runInputRef(DM_Handle dmHandle);
extern void DM_runOperStatus(DM_Handle dmHandle);
extern void DM_runRefLimit(DM_Handle dmHandle);


inline INVERTER_TripType_e DM_getInverterTripType(DM_Handle handle)
{
    DM_Obj *pdmObj = (DM_Obj *) handle;
    return pdmObj->tripTypeTrace[0];
}

inline bool DM_isInverterTripType(DM_Handle handle, INVERTER_TripType_e tripType)
{
    DM_Obj *pObj = (DM_Obj *)handle;
    return (pObj->tripTypeTrace[0] & tripType);
}

inline void DM_setInverterTripType(DM_Handle handle, INVERTER_TripType_e tripType)
{
    if (!DM_isInverterTripType(handle, tripType))
    {
        DM_Obj *pdmObj = (DM_Obj *) handle;
        uint_least8_t i;
        for (i=1; i< MAX_TRIP_TRACE ; i++)
        	pdmObj->tripTypeTrace[i] = pdmObj->tripTypeTrace[i-1];

        pdmObj->tripTypeTrace[0] |= tripType;
        //DM_setFirstDisplayTrip(handle);
    }
}

inline void DM_clrInverterTripType(DM_Handle handle, INVERTER_TripType_e tripType)
{
    if (DM_isInverterTripType(handle, tripType))
    {
        DM_Obj *pdmObj = (DM_Obj *) handle;
        uint_least8_t i;
        for (i=1; i< MAX_TRIP_TRACE ; i++)
          	pdmObj->tripTypeTrace[i] = pdmObj->tripTypeTrace[i-1];

        pdmObj->tripTypeTrace[0] &= (~tripType);
        //if (pdmObj->inverterTripType == TripType_NoTrip)
         //   DM_clrFirstDisplayTrip(handle);
    }
}
inline HARDWARE_FaultType_e DM_getHardwareFaultType(DM_Handle handle)
{
    DM_Obj *pdmObj = (DM_Obj *) handle;
    return pdmObj->hardwareFaultType;
}

inline bool DM_isHardwareFaultType(DM_Handle handle,HARDWARE_FaultType_e faultType)
{
    DM_Obj *pObj = (DM_Obj *)handle;
    return (pObj->hardwareFaultType & faultType);
}

inline void DM_setHardwareFaultType(DM_Handle handle,HARDWARE_FaultType_e faultType)
{
    if (!DM_isHardwareFaultType(handle,faultType))
    {
        DM_Obj *pObj = (DM_Obj *)handle;

        pObj->hardwareFaultType |= faultType;
        DM_setInverterTripType(handle, TripType_Hardware);
        //DM_setFirstDisplayTrip(handle);
    }

}

inline void DM_clrHardwareFaultType(DM_Handle handle,HARDWARE_FaultType_e faultType)
{

    if (DM_isHardwareFaultType(handle,faultType))
    {
        DM_Obj *pObj = (DM_Obj *)handle;

        pObj->hardwareFaultType &= (~faultType);
        if (pObj->hardwareFaultType == FaultType_NoFault )
            DM_clrInverterTripType(handle, TripType_Hardware);
    }

}



extern void DM_outHardwareFaultName(DM_Handle dmHandle, char *pi8String);
extern void DM_outInverterTripName1(DM_Handle dmHandle, char *pi8String);
extern void DM_outInverterTripName2(DM_Handle dmHandle, char *pi8String);
extern void DM_autotunePrepStart(DM_Handle dmHandle);
extern void DM_autotuneStart(DM_Handle dmHandle);
extern void DM_autotuneEnd(DM_Handle dmHandle);
extern EST_State_e DM_getAutotuneState(DM_Handle dmHandle);
extern bool DM_getRunIdentifySignal(DM_Handle dmHandle);



/*void DM_setRefFreq(DM_Handle handle, int16_t i16RefFreqHz)
{
    DM_Obj *pObj = (DM_Obj *)handle;
    pObj->i16RefFreqHz = i16RefFreqHz;
}*/

extern const char* DM_getModeName(DM_Handle dmHandle, uint_least8_t u8ModeNo);
extern uint_least8_t DM_getModeSizeNo(DM_Handle dmHandle);
extern uint_least8_t DM_getOperModeSizeNo(DM_Handle dmHandle);
//extern uint_least8_t DM_getInitModeLevel(DM_Handle dmHandle);
extern uint_least8_t DM_getAccessLevel(DM_Handle dmHandle);
extern int32_t DM_getCellValue(DM_Handle dmHandle, const DM_Cell  *pdmCell);

extern uint_least8_t DM_getGroupSizeNo(DM_Handle dmHandle,uint_least8_t u8ModeIndex);
extern uint_least8_t DM_getFunctionSizeNo(DM_Handle dmHandle,uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex);
extern uint_least8_t DM_getCellSizeNo(DM_Handle dmHandle,uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex, uint_least8_t u8FunctionIndex);
extern uint16_t DM_getTotalFunctionSizeNo(DM_Handle dmHandle);
extern uint16_t DM_getTotalCellSizeNo(DM_Handle dmHandle);
extern void DM_findGroupFunctionIndex(DM_Handle dmHandle, uint16_t u16TotFunIndex,
							          uint_least8_t *pGroupIndex, uint_least8_t *pFunctionIndex);
extern void DM_findGroupFunctionCellIndex(DM_Handle dmHandle, uint16_t u16TotCellIndex,
							       uint_least8_t *pGroupIndex, uint_least8_t *pFunctionIndex, uint_least8_t *pCellIndex);


extern void DM_getToValueInt32(DM_Handle dmHandle,
		 	 	 	 	   uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
						   uint_least8_t u8FunIndex, uint_least8_t u8CellIndex,
						   int32_t *pi32Value, int32_t *pi32MaxValue, int32_t *pi32MinValue, int32_t *pi32IncValue);
extern void DM_setValue(DM_Handle dmHandle,
	 	   	   	   uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
	 	   	   	   	uint_least8_t u8FunIndex, uint_least8_t u8CellIndex,
	 	   	   	   	int32_t i32Value);

extern bool DM_isWrite(DM_Handle dmHandle,
			    		uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
			    		uint_least8_t u8FunIndex, uint_least8_t u8CellIndex);
extern bool DM_outEditCell(DM_Handle dmHandle,
				   	   	   uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
				   	   	   uint_least8_t u8FunIndex, uint_least8_t u8CellIndex,
				   	   	   int32_t i32Value,
				   	   	   char *pai8Line1, char *pai8Line2);

extern void DM_outFunCell(DM_Handle dmHandle,
				   uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
				   uint_least8_t u8FunIndex, uint_least8_t u8CellIndex,
				   char *pai8Line1, char *pai8Line2);
extern void DM_outGroup(DM_Handle dmHandle,
				   uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
				   char *pai8Line1, char *pai8Line2);
extern void DM_outFunction(DM_Handle dmHandle,
				   uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex, uint_least8_t u8FunIndex,
				   char *pai8Line1, char *pai8Line2);



extern void DM_setCallbackFreqRef(DM_Handle handle, const DM_Cell *pdmCell);
extern void DM_setCallbackCurrentRef(DM_Handle handle, const DM_Cell *pdmCell);

extern int32_t DM_getCallbackFreqOutHz(DM_Handle handle);
extern int32_t DM_getCallbackFreqOutRpm(DM_Handle handle);
extern int32_t DM_getCallbackEnc1OutRpm(DM_Handle handle);
extern int32_t DM_getCallbackEnc2OutRpm(DM_Handle handle);
extern int32_t DM_getCallbackHallOutRpm(DM_Handle handle);


extern int32_t DM_getCallbackCurrentOut(DM_Handle handle);
extern int32_t DM_getCallbackTorqueOut(DM_Handle handle);
extern int32_t DM_getCallbackPowerOut(DM_Handle handle);


extern int32_t DM_getCallbackDCBus(DM_Handle handle);
extern int32_t DM_getCallbackVoltageU(DM_Handle handle);
extern int32_t DM_getCallbackVoltageV(DM_Handle handle);
extern int32_t DM_getCallbackVoltageW(DM_Handle handle);
extern int32_t DM_getCallbackCurrentU(DM_Handle handle);
extern int32_t DM_getCallbackCurrentV(DM_Handle handle);
extern int32_t DM_getCallbackCurrentW(DM_Handle handle);
extern int32_t DM_getCallbackInputTerm(DM_Handle handle);
//extern int32_t DM_getCallbackOutputTerm(DM_Handle handle);
extern int32_t DM_getCallbackExtTemp(DM_Handle handle);
extern int32_t DM_getCallbackExtAd1(DM_Handle handle);
extern int32_t DM_getCallbackExtAd2(DM_Handle handle);
extern int32_t DM_getCallbackExtAd3(DM_Handle handle);
extern int32_t DM_getCallbackExtAd4(DM_Handle handle);
extern int32_t DM_getCallbackExtAd5(DM_Handle handle);
extern int32_t DM_getCallbackExtAd6(DM_Handle handle);
extern int32_t DM_getCallbackVersion(DM_Handle handle);

extern int32_t DM_getCallbackAccTime(DM_Handle handle);
extern void DM_setCallbackAccTime(DM_Handle handle, const DM_Cell *pdmCell);



#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _I2C_24LC32_H_ definition




