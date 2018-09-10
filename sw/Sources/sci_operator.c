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
 *    notice, this list of conditions and the following disclaimer in thef
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

 
 
//! \file   solutions/instaspin_motion/src/ctrl.c
//! \brief  Contains the various functions related to the controller (CTRL) object
//!
//! (C) Copyright 2011, Texas Instruments, Inc.


// **************************************************************************
// the includes

#include <math.h>


// drivers


// modules


// platforms
#include "include/sci_operator.h"
#include "include/sci_message.h"
#include "include/user_data.h"
#include "include/main.h"


#define KEY_DELAY_500MS	(500/8)
#define KEY_DELAY_200MS	(200/8)
#define KEY_DELAY_100MS	(100/8)
#define KEY_DELAY_50MS	(50/8)

//extern  MOTOR_Vars_t gMotorVars;
extern CTRL_Obj *gpCtlObj;   // for autotune

OPERATOR_Obj *g_pOperObj;
int32_t 	g_i32EditValue, g_i32EditMaxValue, g_i32EditMinValue, g_i32DeltaValue;

OPERATOR_Handle OPERATOR_init(void *pMemory,const size_t numBytes)
{
	OPERATOR_Handle operHandle;

	if (numBytes < sizeof(OPERATOR_Obj))
		return((OPERATOR_Handle)NULL);

	// assign the handle
	operHandle = (OPERATOR_Handle)pMemory;

	return(operHandle);
}

void OPERATOR_setup(OPERATOR_Handle operHandle, DM_Handle dmHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	uint_least8_t i,j;

	pOperator->dmHandle = dmHandle;
	pOperator-> OpCommStatus = OPERATOR_INIT1;
	g_pOperObj = pOperator;

	pOperator->u16Key = 0;
	pOperator->u8LEDBlink = 0;
	pOperator->u8LEDOnOff = 0;

	for (j=0; j<2; j++)
	{
		pOperator->u16LCDBlink[j] =0;
		for (i=0; i<16; i++)
			pOperator->ai8LCD[j][i]= ' ';
	}


	OPERATOR_runMenuKey(operHandle);
}



void OPERATOR_setLEDBlink(OPERATOR_Handle operHandle,uint_least8_t u8BlinkData)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	pOperator->u8LEDBlink |= u8BlinkData;
	pOperator->u8LEDOnOff |= u8BlinkData;
}

void OPERATOR_setLEDOn(OPERATOR_Handle operHandle,uint_least8_t u8OnData)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	pOperator->u8LEDOnOff |= u8OnData;
	pOperator->u8LEDBlink &= (~u8OnData);
	pOperator->u8LEDBlink &= 0x00ff;

}

void OPERATOR_setLEDOff(OPERATOR_Handle operHandle,uint_least8_t u8OffData)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	pOperator->u8LEDOnOff &= (~u8OffData);
	pOperator->u8LEDOnOff &= 0x00ff;
	pOperator->u8LEDBlink &= (~u8OffData);
	pOperator->u8LEDBlink &= 0x00ff;
}



void OPERATOR_setupLEDs(OPERATOR_Handle operHandle)
{
    OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
    DM_Handle dmHandle = OPERATOR_getDMHandle(operHandle);


    if (DM_isInverterTrip(dmHandle))
    {
        OPERATOR_setLEDBlink(operHandle,  OPERATOR_STOP_LED | OPERATOR_RUN_LED | OPERATOR_DRIVE_LED | OPERATOR_REV_LED |
                                              OPERATOR_FWD_LED  | OPERATOR_SEQ_LED | OPERATOR_SEQ_LED | OPERATOR_REF_LED);
    }
    else
    {
        _iq iqFreqOut;
        if (pOperator->u8ModeIndex == 0)
            OPERATOR_setLEDOn(operHandle, OPERATOR_DRIVE_LED);
        else
            OPERATOR_setLEDOff(operHandle, OPERATOR_DRIVE_LED);


        if (DM_isLocalMode(dmHandle))
            OPERATOR_setLEDOff(operHandle, OPERATOR_SEQ_LED | OPERATOR_REF_LED);
        else
        {
            //Remote Mode
            if (DM_isRefSelectRemote(dmHandle))
                OPERATOR_setLEDOn(operHandle,OPERATOR_REF_LED);
            else
                OPERATOR_setLEDBlink(operHandle,OPERATOR_REF_LED);

            if (DM_isRunSelectRemote(dmHandle))
                OPERATOR_setLEDOn(operHandle,OPERATOR_SEQ_LED);
            else
                OPERATOR_setLEDBlink(operHandle,OPERATOR_SEQ_LED);
            //OPERATOR_setLEDOn(operHandle, OPERATOR_SEQ_LED | OPERATOR_REF_LED);
        }

        iqFreqOut = DM_getFreqOutPu(dmHandle);
        if (DM_isRunStatus(dmHandle))
        {
            OPERATOR_setLEDOff(operHandle, OPERATOR_STOP_LED);
            if (DM_isFreqOutLowSpeed(dmHandle))
                 OPERATOR_setLEDBlink(operHandle, OPERATOR_RUN_LED);
            else
                 OPERATOR_setLEDOn(operHandle, OPERATOR_RUN_LED);

            if (   iqFreqOut >= 0)
            {
                OPERATOR_setLEDOn (operHandle, OPERATOR_FWD_LED);
                OPERATOR_setLEDOff(operHandle, OPERATOR_REV_LED);
            }
            else
            {
                OPERATOR_setLEDOff(operHandle, OPERATOR_FWD_LED);
                OPERATOR_setLEDOn (operHandle, OPERATOR_REV_LED);
            }
        }
        else
        {
            OPERATOR_setLEDOff(operHandle, OPERATOR_RUN_LED);

            if (DM_isFreqOutLowSpeed(dmHandle))
            {
                OPERATOR_setLEDOn(operHandle, OPERATOR_STOP_LED);
                OPERATOR_setLEDOff(operHandle, OPERATOR_FWD_LED | OPERATOR_REV_LED );
            }
            else
                OPERATOR_setLEDBlink(operHandle, OPERATOR_STOP_LED);

        }

    }

}

void OPERATOR_setLCDBlinkOn(OPERATOR_Handle operHandle,uint_least8_t u8LineNo, uint16_t u16OnData)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	pOperator->u16LCDBlink[u8LineNo] |= u16OnData;
}

void OPERATOR_setLCDBlinkOff(OPERATOR_Handle operHandle,uint_least8_t u8LineNo, uint16_t u16OffData)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	pOperator->u16LCDBlink[u8LineNo] &= (~u16OffData);
}

void OPERATOR_setLCDChar(OPERATOR_Handle operHandle, uint_least8_t u8LineNo, uint_least8_t u8PosNo, int_least8_t i8CharValue)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	pOperator->ai8LCD[u8LineNo][u8PosNo] = i8CharValue;
}

void OPERATOR_clearLCDChar(OPERATOR_Handle operHandle, uint_least8_t u8LineNo, uint_least8_t u8PosNo)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	pOperator->ai8LCD[u8LineNo][u8PosNo] = ' ';
}

void OPERATOR_setLCDString(OPERATOR_Handle operHandle, uint_least8_t u8LineNo, uint_least8_t u8PosNo, char *pi8Data)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	int_least8_t *pi8LCDBuff = &pOperator->ai8LCD[u8LineNo][u8PosNo];
	uint_least8_t u8Value;
	while ( (u8Value = *pi8Data++) )
		*pi8LCDBuff++ = u8Value;
}

void OPERATOR_setCenterLCDString(OPERATOR_Handle operHandle, int_least8_t u8LineNo, char *pi8String)
{
	int_least8_t i8Len= 0;
	while (pi8String[i8Len]) i8Len++;
	OPERATOR_setLCDString(operHandle, u8LineNo, (16-i8Len)>>1, pi8String);

}

void OPERATOR_clearLCDLine(OPERATOR_Handle operHandle,uint_least8_t u8LineNo)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	int_least8_t *pi8LCDBuff = &pOperator->ai8LCD[u8LineNo][0];
	uint_least8_t i;
	for (i=0; i< 16; i++)
		*pi8LCDBuff++ = ' ';
	pOperator->u16LCDBlink[u8LineNo] = 0;
}

void OPERATOR_clearLCDAll(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	uint_least8_t i,j;
	for (j=0; j<2; j++)
	{
		pOperator->u16LCDBlink[j] = 0;
		for (i=0; i< 16; i++)
			pOperator->ai8LCD[j][i] = ' ';
	}
}

bool OPERATOR_isKey(OPERATOR_Handle operHandle, OPERRATOR_Key_e operKey)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	return (pOperator->u16Key & ((uint16_t) operKey));
}

bool OPERATOR_showEmergyTrip(OPERATOR_Handle operHandle)
{
    DM_Handle dmHandle = OPERATOR_getDMHandle(operHandle);
    if (DM_isTripDisplay(dmHandle))
    {
        char ai8Line1[20], ai8Line2[20];
        uint16_t u16TripType;
        u16TripType = DM_getInverterTripType(dmHandle);
        DM_outInverterTripName1(u16TripType, ai8Line1);
        DM_outInverterTripName2(u16TripType, ai8Line2);

        OPERATOR_setLEDBlink(operHandle,  OPERATOR_STOP_LED | OPERATOR_RUN_LED | OPERATOR_DRIVE_LED | OPERATOR_REV_LED |
                                          OPERATOR_FWD_LED  | OPERATOR_SEQ_LED | OPERATOR_SEQ_LED | OPERATOR_REF_LED);
        OPERATOR_clearLCDAll(operHandle);
        OPERATOR_setCenterLCDString(operHandle,0,ai8Line1);
        OPERATOR_setCenterLCDString(operHandle,1,ai8Line2);
        //DM_clrFirstDisplayTrip(dmHandle);
        return true;
    }
    else
        return false;
}

void OPERATOR_showModeLevel(OPERATOR_Handle operHandle)
{
	char *pi8ModeName;
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;

	OPERATOR_clearLCDAll(operHandle);
	OPERATOR_setCenterLCDString(operHandle,0, " * Main Menu * ");

	pi8ModeName = (char *) DM_getModeName(OPERATOR_getDMHandle(operHandle), pOperator->u8ModeIndex);
	OPERATOR_setCenterLCDString(operHandle, 1, pi8ModeName);
}

void OPERATOR_showGroupLevel(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	char ai8Line1[20], ai8Line2[20];

	DM_outGroup(OPERATOR_getDMHandle(operHandle),
				pOperator->u8ModeIndex, pOperator->u8GroupIndex,
				ai8Line1, ai8Line2);
	OPERATOR_clearLCDAll(operHandle);
	OPERATOR_setCenterLCDString(operHandle, 0, ai8Line1);
	OPERATOR_setCenterLCDString(operHandle, 1, ai8Line2);


}

void OPERATOR_showFunLevel(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	//DM_Handle dmHandle = pOperator->dmHandle;
	char ai8Line1[20], ai8Line2[20];

	DM_outFunction(OPERATOR_getDMHandle(operHandle),
				   pOperator->u8ModeIndex, pOperator->u8GroupIndex, pOperator->u8FunIndex,
				   ai8Line1, ai8Line2);
	OPERATOR_clearLCDAll(operHandle);
	OPERATOR_setCenterLCDString(operHandle, 0, ai8Line1);
	OPERATOR_setCenterLCDString(operHandle, 1, ai8Line2);

}

void OPERATOR_showCellLevel(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	DM_Handle dmHandle = OPERATOR_getDMHandle(operHandle);
	char ai8Line1[20], ai8Line2[20];

	switch (pOperator->u8ModeIndex)
	{
		case OP_MODE_OPERATION:
			if (pOperator->u16SpecialIndex == 0xffff)
			{
				uint_least8_t u8ItemNo = DM_getOperModeSizeNo(dmHandle);
				if (pOperator->u8CellIndex == u8ItemNo)
					DM_outFunction(dmHandle,
								   OP_MODE_OPERATION, OP_GROUP_U,1,
								   ai8Line1, ai8Line2);
				//else if (pOperator->u8CellIndex == u8ItemNo+1)
				//	DM_outFunction(dmHandle,
				//					OP_MODE_OPERATION, OP_GROUP_U,2,
				//					ai8Line1, ai8Line2);
				else if (pOperator->u8CellIndex == u8ItemNo+1)
					DM_outFunction(dmHandle,
								   OP_MODE_OPERATION, OP_GROUP_U,0,
								   ai8Line1, ai8Line2);
				else
					DM_outFunCell(dmHandle,
								  OP_MODE_OPERATION, OP_GROUP_U,0,pOperator->u8CellIndex,
								  ai8Line1, ai8Line2);
			}
			else
				DM_outFunCell(dmHandle,
						      OP_MODE_OPERATION, OP_GROUP_U, pOperator->u8FunIndex,  pOperator->u8CellIndex,
							  ai8Line1, ai8Line2);
			break;
		case OP_MODE_INIT:
			if (pOperator->u16SpecialIndex == 0xffff)
			{
				uint_least8_t u8ItemNo = DM_getCellSizeNo(dmHandle, OP_MODE_INIT, OP_GROUP_A,0);
				if (pOperator->u8CellIndex == u8ItemNo)
					DM_outFunction(dmHandle,
								    OP_MODE_INIT, OP_GROUP_A,1,
									 ai8Line1, ai8Line2);
				else
					DM_outFunCell(dmHandle,
							      OP_MODE_INIT, OP_GROUP_A,0,pOperator->u8CellIndex,
								ai8Line1, ai8Line2);
			}
			else
				DM_outFunCell(dmHandle,
							  OP_MODE_INIT, OP_GROUP_A, 1,  pOperator->u8CellIndex,
							  ai8Line1, ai8Line2);
			break;
		case OP_MODE_PROG:
			DM_outFunCell(dmHandle,
						  pOperator->u8ModeIndex, pOperator->u8GroupIndex, pOperator->u8FunIndex,  pOperator->u8CellIndex,
						  ai8Line1, ai8Line2);
			break;
	}

	OPERATOR_clearLCDAll(operHandle);
	OPERATOR_setCenterLCDString(operHandle, 0, ai8Line1);
	OPERATOR_setCenterLCDString(operHandle, 1, ai8Line2);
}

void DetermineBlinkDig(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	uint_least8_t u8DigNo=0, u8LoopNo;
	while (pOperator->ai8LCD[1][u8DigNo] == ' ') u8DigNo++;	// Find first char != ' '
	while (pOperator->ai8LCD[1][u8DigNo] != ' ') u8DigNo++; // Find char == ' '
	switch (g_i32DeltaValue)
	{
		case 10:
			u8LoopNo = 2;
			break;
		case 100:
			u8LoopNo = 3;
			break;
		case 1000:
			u8LoopNo = 4;
			break;
		case 10000:
			u8LoopNo = 5;
			break;
		default:
			u8LoopNo = 1;
			break;
	}
	while (u8LoopNo--)
	{
		while (pOperator->ai8LCD[1][--u8DigNo] == '.');
	}
	pOperator->u16LCDBlink[1] = 1 << u8DigNo;
}

void OPERATOR_showCellEditLevel(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	char ai8Line1[20], ai8Line2[20];
	bool bFlag;

	bFlag = DM_outEditCell(OPERATOR_getDMHandle(operHandle),
						   pOperator->u8ModeIndex,pOperator->u8GroupIndex, pOperator->u8FunIndex,pOperator->u8CellIndex,
							g_i32EditValue,
							ai8Line1, ai8Line2);
	OPERATOR_clearLCDAll(operHandle);
	OPERATOR_setCenterLCDString(operHandle, 0, ai8Line1);
	OPERATOR_setCenterLCDString(operHandle, 1, ai8Line2);
	if (bFlag)
		DetermineBlinkDig(operHandle);
}

void OPERATOR_showEntryLevel(OPERATOR_Handle operHandle)
{
	//OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	char ai8Line1[20], ai8Line2[20];

	ai8Line1[0]=0;
	strcpy(ai8Line2,"Entry Accepted");
	OPERATOR_clearLCDAll(operHandle);
	OPERATOR_setCenterLCDString(operHandle, 0, ai8Line1);
	OPERATOR_setCenterLCDString(operHandle, 1, ai8Line2);
}


void OPERATOR_convertEstState(EST_State_e estState, char *pai8String)
{
    switch (estState)
    {
        case EST_State_Error:
            strcpy(pai8String, "State: Error");
            break;
        case EST_State_Idle:
            strcpy(pai8String, "State: Idle");
            break;
        case EST_State_RoverL:
            strcpy(pai8String, "State: R/L");
            break;
        case EST_State_Rs:
            strcpy(pai8String, "State: Rs");
            break;
        case EST_State_RampUp:
            strcpy(pai8String,"State: Rampup");
            break;
        case EST_State_IdRated:
            strcpy(pai8String,"State: Control Id");
            break;
        case EST_State_RatedFlux_OL:
            strcpy(pai8String,"State: Opn flux");
            break;
        case EST_State_RatedFlux:
            strcpy(pai8String,"State: Rate flux");
            break;
        case EST_State_RampDown:
            strcpy(pai8String,"State: Ramp down");
            break;
        case EST_State_LockRotor:
            strcpy(pai8String,"State: Lock rotor");
            break;
        case EST_State_Ls:
            strcpy(pai8String,"State: Ls");
            break;
        case EST_State_Rr:
            strcpy(pai8String,"State: Rr");
            break;
        case EST_State_MotorIdentified:
            strcpy(pai8String,"State: Identify");
            break;
        case EST_State_OnLine:
            strcpy(pai8String,"State: On line");
            break;
        case EST_numStates:
            strcpy(pai8String,"State: Num");
            break;
    }
}

void OPERATOR_showAutoTune(OPERATOR_Handle operHandle)
{
    OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
    static bool bSaveMotorFlag = false;
    char ai8Line1[20], ai8Line2[20];

    OPERATOR_clearLCDAll(operHandle);
    strcpy(ai8Line1, "AutoTuning");
    switch (pOperator->u8GroupIndex)
    {
        case 0:
            OPERATOR_setLEDOn(operHandle,   OPERATOR_DRIVE_LED | OPERATOR_REV_LED |
                                            OPERATOR_FWD_LED  | OPERATOR_SEQ_LED | OPERATOR_SEQ_LED | OPERATOR_REF_LED);

            strcpy(ai8Line2,"Prepare to start");
            bSaveMotorFlag = false;
            //pOperator->estStateAutotune = EST_State_Idle;
            break;
        case 1:
        {
        	EST_State_e estState;
        	estState = DM_getAutotuneState(pOperator->dmHandle);
            OPERATOR_convertEstState(estState, ai8Line2);
            OPERATOR_setLEDBlink(operHandle,   OPERATOR_DRIVE_LED | OPERATOR_REV_LED |
                                               OPERATOR_FWD_LED  | OPERATOR_SEQ_LED | OPERATOR_SEQ_LED | OPERATOR_REF_LED);

            break;
        }
        case 2:
        	OPERATOR_setLEDOn(operHandle,   OPERATOR_DRIVE_LED | OPERATOR_REV_LED |
        	                                OPERATOR_FWD_LED  | OPERATOR_SEQ_LED | OPERATOR_SEQ_LED | OPERATOR_REF_LED);

        	strcpy(ai8Line2,"Proc complete");
        	if (!bSaveMotorFlag)
        	{
        		bSaveMotorFlag = true;
        		DM_saveMotorParameters(pOperator->dmHandle);		//Including Bias
        		//DM_saveBiasParameters(pOperator->dmHandle);
        		DM_SavePIgains(pOperator->dmHandle);
        		//DM_calcPIgains(pOperator->dmHandle);
        	}
        	break;


    }

    OPERATOR_setCenterLCDString(operHandle, 0, ai8Line1);
    OPERATOR_setCenterLCDString(operHandle, 1, ai8Line2);
}

void OPERATOR_runMenuKey(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	pOperator->OpStatus = OP_STATUS_MODELEVEL1;
	pOperator->u8ModeIndex = pOperator->u8GroupIndex = pOperator->u8FunIndex = pOperator->u8CellIndex = 0;

	if (DM_isAutotuneMode(pOperator->dmHandle))
	{
		DM_autotuneEnd(pOperator->dmHandle);
		DM_setAutotuneMode(pOperator->dmHandle, false);
	}
	OPERATOR_showModeLevel(operHandle);
}


void OPERATOR_runUpKey_Level4Default(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	uint_least8_t u8ItemNo;
	u8ItemNo = DM_getCellSizeNo(OPERATOR_getDMHandle(operHandle),
								pOperator->u8ModeIndex,pOperator->u8GroupIndex, pOperator->u8FunIndex );
	pOperator->u8CellIndex =  (pOperator->u8CellIndex+1)% u8ItemNo;
	OPERATOR_showCellLevel(operHandle);
}

void OPERATOR_runUpKey(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	DM_Handle dmHandle = OPERATOR_getDMHandle(operHandle);
	switch (pOperator->OpStatus)
	{
		uint_least8_t u8ItemNo;
		case OP_STATUS_MODELEVEL1:
		{
			u8ItemNo = DM_getModeSizeNo(dmHandle);
			pOperator->u8ModeIndex = (pOperator->u8ModeIndex+1)% u8ItemNo;
			OPERATOR_showModeLevel(operHandle);
			break;
		}
		case OP_STATUS_GROUPLEVEL2:
		{
			u8ItemNo = DM_getGroupSizeNo(dmHandle, pOperator->u8ModeIndex);
			pOperator->u8GroupIndex = (pOperator->u8GroupIndex+1)% u8ItemNo;
			OPERATOR_showGroupLevel(operHandle);
			break;
		}
		case OP_STATUS_FUNLEVEL3:
		{
			uint_least8_t u8AccessLevel = DM_getAccessLevel(dmHandle);
			if (u8AccessLevel == OP_ACCESS_BASIC)
			{
				uint16_t u16ItemNo = DM_getTotalFunctionSizeNo(dmHandle);
				pOperator->u16SpecialIndex = (pOperator->u16SpecialIndex +1 ) % u16ItemNo;
				DM_findGroupFunctionIndex(dmHandle, pOperator->u16SpecialIndex,
										  &pOperator->u8GroupIndex, &pOperator->u8FunIndex);
			}
			else
			{
				u8ItemNo = DM_getFunctionSizeNo(dmHandle, pOperator->u8ModeIndex, pOperator->u8GroupIndex);
				pOperator->u8FunIndex = (pOperator->u8FunIndex+1)% u8ItemNo;
			}
			OPERATOR_showFunLevel(operHandle);
			break;
		}
		case OP_STAUS_CELLLEVEL4:
		{
			switch (pOperator->u8ModeIndex)
			{
				case OP_MODE_OPERATION:
					if (pOperator->u16SpecialIndex == 0xffff)
					{
						u8ItemNo = DM_getOperModeSizeNo(dmHandle) + 2;
						pOperator->u8CellIndex =  (pOperator->u8CellIndex+1)% u8ItemNo;
						OPERATOR_showCellLevel(operHandle);
					}
					else
						OPERATOR_runUpKey_Level4Default(operHandle);
					break;
				case OP_MODE_INIT:
					if (pOperator->u16SpecialIndex == 0xffff)
					{
						u8ItemNo = DM_getCellSizeNo(dmHandle, OP_MODE_INIT, OP_GROUP_A,0)+1;
						pOperator->u8CellIndex =  (pOperator->u8CellIndex+1)% u8ItemNo;
						OPERATOR_showCellLevel(operHandle);
					}
					else
						OPERATOR_runUpKey_Level4Default(operHandle);

					break;
				case OP_MODE_PROG:
				{
					uint_least8_t u8AccessLevel = DM_getAccessLevel(dmHandle);
					if (u8AccessLevel == OP_ACCESS_QUICK)
					{
						uint16_t u16ItemNo = DM_getTotalCellSizeNo(dmHandle);
						pOperator->u16SpecialIndex = (pOperator->u16SpecialIndex +1 ) % u16ItemNo;
						DM_findGroupFunctionCellIndex(dmHandle, pOperator->u16SpecialIndex,
													 &pOperator->u8GroupIndex, &pOperator->u8FunIndex, &pOperator->u8CellIndex);
						OPERATOR_showCellLevel(operHandle);
					}
					else
						OPERATOR_runUpKey_Level4Default(operHandle);
					break;
				}

			}
			break;

		}
		case OP_STATUS_EDITLEVEL5:
		{
			g_i32EditValue = g_i32EditValue + g_i32DeltaValue;
			if (g_i32EditValue > g_i32EditMaxValue )
				g_i32EditValue = g_i32EditMaxValue;
			OPERATOR_showCellEditLevel(operHandle);
			break;
		}
	}

}



void OPERATOR_runDownKey_Level4Default(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	uint_least8_t u8ItemNo;
	u8ItemNo = DM_getCellSizeNo(OPERATOR_getDMHandle(operHandle),
								pOperator->u8ModeIndex,pOperator->u8GroupIndex, pOperator->u8FunIndex );
	if (pOperator->u8CellIndex)
	   pOperator->u8CellIndex--;
	else
	   pOperator->u8CellIndex = u8ItemNo -1;
	OPERATOR_showCellLevel(operHandle);
}


void OPERATOR_runDownKey(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	DM_Handle dmHandle = OPERATOR_getDMHandle(operHandle);
	switch (pOperator->OpStatus)
	{
		uint_least8_t u8ItemNo;
		case OP_STATUS_MODELEVEL1:
		{
			u8ItemNo = DM_getModeSizeNo(dmHandle);
			if (pOperator->u8ModeIndex)
				pOperator->u8ModeIndex--;
			else
				pOperator->u8ModeIndex = u8ItemNo-1;
			OPERATOR_showModeLevel(operHandle);
			break;
		}
		case OP_STATUS_GROUPLEVEL2:
		{
			u8ItemNo = DM_getGroupSizeNo(dmHandle, pOperator->u8ModeIndex);
			if (pOperator->u8GroupIndex)
				pOperator->u8GroupIndex--;
			else
				pOperator->u8GroupIndex = u8ItemNo-1;
			OPERATOR_showGroupLevel(operHandle);
			break;

		}
		case OP_STATUS_FUNLEVEL3:
		{
			uint_least8_t u8AccessLevel = DM_getAccessLevel(dmHandle);
			if (u8AccessLevel == OP_ACCESS_BASIC)
			{
				uint16_t u16ItemNo = DM_getTotalFunctionSizeNo(dmHandle);
				if (pOperator->u16SpecialIndex)
					pOperator->u16SpecialIndex--;
				else
					pOperator->u16SpecialIndex = u16ItemNo -1;

				DM_findGroupFunctionIndex(dmHandle, pOperator->u16SpecialIndex,
										  &pOperator->u8GroupIndex, &pOperator->u8FunIndex);
			}
			else
			{
				u8ItemNo = DM_getFunctionSizeNo(dmHandle, pOperator->u8ModeIndex, pOperator->u8GroupIndex);
				if (pOperator->u8FunIndex)
					pOperator->u8FunIndex--;
				else
					pOperator->u8FunIndex = u8ItemNo-1;
			}
			OPERATOR_showFunLevel(operHandle);
			break;

		}
		case OP_STAUS_CELLLEVEL4:
		{
			switch (pOperator->u8ModeIndex)
			{
				case OP_MODE_OPERATION:
					if (pOperator->u16SpecialIndex == 0xffff)
					{
						u8ItemNo = DM_getOperModeSizeNo(dmHandle) + 2;
						if (pOperator->u8CellIndex)
							pOperator->u8CellIndex--;
						else
							pOperator->u8CellIndex =  u8ItemNo -1;
						OPERATOR_showCellLevel(operHandle);
					}
					else
						OPERATOR_runDownKey_Level4Default(operHandle);
					break;
				case OP_MODE_INIT:
					if (pOperator->u16SpecialIndex == 0xffff)
					{
						u8ItemNo = DM_getCellSizeNo(dmHandle, OP_MODE_INIT, OP_GROUP_A, 0)+1;	//A1
						if (pOperator->u8CellIndex)
							pOperator->u8CellIndex--;
						else
							pOperator->u8CellIndex =  u8ItemNo -1;
						OPERATOR_showCellLevel(operHandle);
					}
					else
						OPERATOR_runDownKey_Level4Default(operHandle);

					break;
				case OP_MODE_PROG:
				{
					uint_least8_t u8AccessLevel = DM_getAccessLevel(dmHandle);
					if (u8AccessLevel == OP_ACCESS_QUICK)
					{
						uint16_t u16ItemNo = DM_getTotalCellSizeNo(dmHandle);
						if (pOperator->u16SpecialIndex)
							pOperator->u16SpecialIndex--;
						else
							pOperator->u16SpecialIndex = u16ItemNo -1;

						DM_findGroupFunctionCellIndex(dmHandle, pOperator->u16SpecialIndex,
													&pOperator->u8GroupIndex, &pOperator->u8FunIndex, &pOperator->u8CellIndex);
						OPERATOR_showCellLevel(operHandle);
					}
					else
						OPERATOR_runDownKey_Level4Default(operHandle);
					break;
				}

			}
			break;

		}
		case OP_STATUS_EDITLEVEL5:
		{
			g_i32EditValue = g_i32EditValue - g_i32DeltaValue;
			if (g_i32EditValue < g_i32EditMinValue )
				g_i32EditValue = g_i32EditMinValue;
			OPERATOR_showCellEditLevel(operHandle);
			break;
		}



	}


}

void OPERATOR_runEnterKey_Level4Default(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	if (DM_isWrite(OPERATOR_getDMHandle(operHandle),
				   pOperator->u8ModeIndex, pOperator->u8GroupIndex, pOperator->u8FunIndex, pOperator->u8CellIndex))
	{
		pOperator->OpStatus = OP_STATUS_EDITLEVEL5;
		DM_getToValueInt32(pOperator->dmHandle,
						   pOperator->u8ModeIndex, pOperator->u8GroupIndex, pOperator->u8FunIndex,  pOperator->u8CellIndex,
						   &g_i32EditValue, &g_i32EditMaxValue, &g_i32EditMinValue, &g_i32DeltaValue);
		OPERATOR_showCellEditLevel(operHandle);
	}
}

void OPERATOR_runEnterKey(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	switch (pOperator->OpStatus)
	{
		case OP_STATUS_MODELEVEL1:
		{
			switch (pOperator->u8ModeIndex)
			{
				case OP_MODE_OPERATION:
						pOperator->OpStatus = OP_STAUS_CELLLEVEL4;
						pOperator->u8GroupIndex = OP_GROUP_U;
						pOperator->u8FunIndex = pOperator->u8CellIndex = 0;
						pOperator->u16SpecialIndex = 0xffff;
						OPERATOR_showCellLevel(operHandle);
						break;
				case OP_MODE_INIT:
						pOperator->OpStatus = OP_STAUS_CELLLEVEL4;
						pOperator->u8GroupIndex = OP_GROUP_A;
						pOperator->u8FunIndex = pOperator->u8CellIndex = 0;
						pOperator->u16SpecialIndex = 0xffff;
						OPERATOR_showCellLevel(operHandle);
						break;
				case OP_MODE_PROG:
				    {
				        uint_least8_t u8AccessLevel = DM_getAccessLevel(pOperator->dmHandle);
				        if (u8AccessLevel == OP_ACCESS_ADVANCE)
				        {
				            pOperator->OpStatus = OP_STATUS_GROUPLEVEL2;
				            pOperator->u8GroupIndex = OP_GROUP_B;
				            pOperator->u8FunIndex = pOperator->u8CellIndex = 0;
				            OPERATOR_showGroupLevel(operHandle);
				        }
				        else if (u8AccessLevel == OP_ACCESS_BASIC)
				        {
				            pOperator->OpStatus = OP_STATUS_FUNLEVEL3;
				            pOperator->u8GroupIndex = OP_GROUP_B;
				            pOperator->u8FunIndex = pOperator->u8CellIndex = 0;
				            pOperator->u16SpecialIndex = 0;
				            OPERATOR_showFunLevel(operHandle);
				        }
				        else if (u8AccessLevel == OP_ACCESS_QUICK)
				        {
				            pOperator->OpStatus = OP_STAUS_CELLLEVEL4;
				            pOperator->u8GroupIndex = OP_GROUP_B;
				            pOperator->u8FunIndex = pOperator->u8CellIndex = 0;
				            pOperator->u16SpecialIndex = 0;
				            OPERATOR_showCellLevel(operHandle);
				        }
				        break;
				    }
				  case OP_MODE_AUTOTUNE:
				    {
				        //pOperator->u8AutoTuneIndex++;
				        pOperator->OpStatus = OP_STATUS_GROUPLEVEL2;
				        pOperator->u8GroupIndex = 0;
				        DM_autotunePrepStart(pOperator->dmHandle);
				        DM_setAutotuneMode(pOperator->dmHandle, true);
				        OPERATOR_showAutoTune(operHandle);

				    }

				    break;
			}
			break;
		}
		case OP_STATUS_GROUPLEVEL2:
		{
			if (DM_isAutotuneMode(pOperator->dmHandle))
			{
				if (pOperator->u8GroupIndex == 0)
				{
					pOperator->u8GroupIndex = 1;
					DM_autotuneStart(pOperator->dmHandle);
					OPERATOR_showAutoTune(operHandle);
				}
				else if (pOperator->u8GroupIndex == 2)
				{
					DM_setAutotuneMode(pOperator->dmHandle, false);
					pOperator->u8GroupIndex = 0;
					pOperator->OpStatus = OP_STATUS_MODELEVEL1;
					OPERATOR_showModeLevel(operHandle);
				}
			}
			else
			{
				pOperator->OpStatus = OP_STATUS_FUNLEVEL3;
				OPERATOR_showFunLevel(operHandle);
			}

			break;
		}
		case OP_STATUS_FUNLEVEL3:
		{
			pOperator->OpStatus = OP_STAUS_CELLLEVEL4;
			OPERATOR_showCellLevel(operHandle);
			break;
		}
		case OP_STAUS_CELLLEVEL4:
			switch (pOperator->u8ModeIndex)
			{
				case OP_MODE_OPERATION:
				{
					if (pOperator->u16SpecialIndex == 0xffff)
					{
						uint_least8_t u8ItemNo = DM_getOperModeSizeNo(pOperator->dmHandle);
						if (pOperator->u8CellIndex < u8ItemNo)
							OPERATOR_runEnterKey_Level4Default(operHandle);
						else
						{
							if (pOperator->u8CellIndex == u8ItemNo)
								pOperator->u8FunIndex = 1;	//U2
							else if (pOperator->u8CellIndex == u8ItemNo+1)
								pOperator->u8FunIndex = 2;	//U3
							else
								pOperator->u8FunIndex = 0;	//U1

							pOperator->u16SpecialIndex = pOperator->u8CellIndex;
							pOperator->u8CellIndex = 0;
							OPERATOR_showCellLevel(operHandle);
						}
					}
					else
						OPERATOR_runEnterKey_Level4Default(operHandle);
					break;
				}
				case OP_MODE_INIT:
				{
					if (pOperator->u16SpecialIndex == 0xffff)
					{
						uint_least8_t u8ItemNo = DM_getCellSizeNo(OPERATOR_getDMHandle(operHandle), OP_MODE_INIT, OP_GROUP_A,0);
						if (pOperator->u8CellIndex < u8ItemNo)
							OPERATOR_runEnterKey_Level4Default(operHandle);
						else
						{
							pOperator->u8FunIndex = 1;	//A2
							pOperator->u16SpecialIndex = pOperator->u8CellIndex;
							pOperator->u8CellIndex = 0;
							OPERATOR_showCellLevel(operHandle);
						}
					}
					else
						OPERATOR_runEnterKey_Level4Default(operHandle);
					break;
				}
				case OP_MODE_PROG:
					OPERATOR_runEnterKey_Level4Default(operHandle);
					break;
			}
			break;
		case OP_STATUS_EDITLEVEL5:
			pOperator->OpStatus = OP_STATUS_ENTRYLEVEL6;
			DM_setValue(OPERATOR_getDMHandle(operHandle),
						pOperator->u8ModeIndex,pOperator->u8GroupIndex, pOperator->u8FunIndex,  pOperator->u8CellIndex,
					    g_i32EditValue);
			OPERATOR_showEntryLevel(operHandle);
			break;
	}
	OPERATOR_setupLEDs(operHandle);
}

void OPERATOR_runEscKey(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	switch (pOperator->OpStatus)
	{
		case OP_STATUS_GROUPLEVEL2:

			if (DM_isAutotuneMode(pOperator->dmHandle))
			{
				if (pOperator->u8GroupIndex == 0)
				{
					pOperator->OpStatus = OP_STATUS_MODELEVEL1;
					DM_autotuneEnd(pOperator->dmHandle);
					DM_setAutotuneMode(pOperator->dmHandle, false);
					OPERATOR_showModeLevel(operHandle);
				}
				else if (pOperator->u8GroupIndex == 1)
				{
					DM_autotuneEnd(pOperator->dmHandle);
					pOperator->u8GroupIndex = 0;

					OPERATOR_showAutoTune(operHandle);
				}

			}

			break;
		case OP_STATUS_FUNLEVEL3:
		{
			uint_least8_t u8AccessLevel = DM_getAccessLevel(OPERATOR_getDMHandle(operHandle));
			if (u8AccessLevel == OP_ACCESS_BASIC)
			{
				pOperator->OpStatus = OP_STATUS_MODELEVEL1;
				OPERATOR_showModeLevel(operHandle);

			}
			else	//OP_ACCESS_ADVANCE
			{
				pOperator->OpStatus = OP_STATUS_GROUPLEVEL2;
				OPERATOR_showGroupLevel(operHandle);
			}
			break;
		}
		case OP_STAUS_CELLLEVEL4:
			switch (pOperator->u8ModeIndex)
			{
				case OP_MODE_OPERATION:
				case OP_MODE_INIT:
					if (pOperator->u16SpecialIndex == 0xffff)
					{
						pOperator->OpStatus = OP_STATUS_MODELEVEL1;
						OPERATOR_showModeLevel(operHandle);
					}
					else
					{
						pOperator->u8CellIndex = (uint_least8_t) pOperator->u16SpecialIndex;
						pOperator->u16SpecialIndex = 0xffff;
						OPERATOR_showCellLevel(operHandle);
					}
					break;

				case OP_MODE_PROG:
				{

					uint_least8_t u8AccessLevel = DM_getAccessLevel(OPERATOR_getDMHandle(operHandle));
					if (u8AccessLevel == OP_ACCESS_QUICK)
					{
						pOperator->OpStatus = OP_STATUS_MODELEVEL1;
						OPERATOR_showModeLevel(operHandle);
					}
					else	//OP_ACCESS_BASIC & ADVANCED
					{
						pOperator->OpStatus = OP_STATUS_FUNLEVEL3;
						OPERATOR_showFunLevel(operHandle);
					}
					break;

				}
			}
			break;
		case OP_STATUS_EDITLEVEL5:
			pOperator->OpStatus = OP_STAUS_CELLLEVEL4;
			OPERATOR_showCellLevel(operHandle);
			break;
		case OP_STATUS_ENTRYLEVEL6:
			pOperator->OpStatus = OP_STATUS_EDITLEVEL5;
			OPERATOR_showCellEditLevel(operHandle);
			break;
	}
}

void OPERATOR_runResetKey(OPERATOR_Handle operHandle)
{
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	switch (pOperator->OpStatus)
	{
		case OP_STATUS_EDITLEVEL5:	//Shift Left
			g_i32DeltaValue /= 10;
			if (g_i32DeltaValue == 0) g_i32DeltaValue = 1000;
			OPERATOR_showCellEditLevel(operHandle);
			break;

	}
	OPERATOR_setupLEDs(operHandle);
}

void OPERATOR_runLocalKey(OPERATOR_Handle operHandle)
{
	//OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;

	DM_toggleLocalRemoteMode(OPERATOR_getDMHandle(operHandle));
	//pOperator->bRemoteFlag ~= pOperator->bRemoteFlag;

	/*gMotorVars.bFlag_Run_Identify = false;
	if (gMotorVars.bFlag_enableSys)
	{
		gMotorVars.bFlag_enableSys = false;
	}
	else
	{
		gMotorVars.bFlag_enableSys = true;
	}*/

	OPERATOR_setupLEDs(operHandle);
}

void OPERATOR_runStartKey(OPERATOR_Handle operHandle)
{
    DM_Handle dmHandle = OPERATOR_getDMHandle(operHandle);

    if (DM_isLocalMode(dmHandle) || (DM_getRunSelect(dmHandle) == REF_DigialOperator))
    {
        DM_setRunStatus(dmHandle);
        //DM_runOperStatus(dmHandle);
    }

  	OPERATOR_setupLEDs(operHandle);

}

void OPERATOR_runStopKey(OPERATOR_Handle operHandle)
{
    DM_Handle dmHandle = OPERATOR_getDMHandle(operHandle);
    if (DM_isLocalMode(dmHandle) || (DM_getRunSelect(dmHandle) == REF_DigialOperator))
    {
        DM_setStopStatus(dmHandle);
    }
 	OPERATOR_setupLEDs(operHandle);

}



uint16_t OPERATOR_runNoKey(OPERATOR_Handle operHandle)
{
	// For real-time update data
	OPERATOR_Obj *pOperator = (OPERATOR_Obj *) operHandle;
	DM_Handle dmHandle = OPERATOR_getDMHandle(operHandle);
	OPERATOR_setupLEDs(operHandle);
	switch (pOperator->OpStatus)
	{
	    case  OP_STATUS_GROUPLEVEL2:
	    {
	    	if (DM_isAutotuneMode(dmHandle))
	    	{
	    		if ((pOperator->u8GroupIndex == 1) &&
	    		    (DM_getRunIdentifySignal(dmHandle) == false))
	    		{
	    			DM_autotuneEnd(dmHandle);
	    			pOperator->u8GroupIndex = 2;
	    		}
	    		OPERATOR_showAutoTune(operHandle);
	    	}
	    	break;
	    }
		case OP_STAUS_CELLLEVEL4:
		{
			switch (pOperator->u8ModeIndex)
			{
				case OP_MODE_OPERATION:
					if (pOperator->u16SpecialIndex == 0xffff)
					{
						uint_least8_t u8ItemNo = DM_getOperModeSizeNo(dmHandle);
						if (pOperator->u8CellIndex == u8ItemNo)
							return 0;
											//DM_outFunction(dmHandle,
											//			   OP_MODE_OPERATION, OP_GROUP_U,1,
											//			   ai8Line1, ai8Line2);
						else if (pOperator->u8CellIndex == u8ItemNo+1)
							return 0;
											//DM_outFunction(dmHandle,
											//				OP_MODE_OPERATION, OP_GROUP_U,2,
											//				ai8Line1, ai8Line2);
					   else if (pOperator->u8CellIndex == u8ItemNo+2)
							return 0;
											//DM_outFunction(dmHandle,
											//			   OP_MODE_OPERATION, OP_GROUP_U,0,
											//			   ai8Line1, ai8Line2);
					   else
					   {

						    char  ai8Line1[20], ai8Line2[20];
						    DM_outFunCell( dmHandle,
										  OP_MODE_OPERATION, OP_GROUP_U,0,pOperator->u8CellIndex,
										  ai8Line1, ai8Line2);
							OPERATOR_clearLCDLine(operHandle,1);
							OPERATOR_setCenterLCDString(operHandle, 1, ai8Line2);
							return DATA_ConvertToCnt(KEY_DELAY_100MS);	// 100 ms

					   }
					}

			}
			break;

		}

	}

	return 0;
}



//OPERATOR_Handle		gOperHandle;
void OPERATOR_run(OPERATOR_Handle operHandle)
{
	static uint_least8_t u8LastKey = OPERATOR_NO_KEY;
	static uint16_t u16ContNo = 0;
	static uint16_t u16KeyDelayCnt=0;

	OPERATOR_Obj *pOperObj = operHandle;

	if (u16KeyDelayCnt)
		u16KeyDelayCnt--;
	else
	{
		if  (pOperObj->OpStatus ==OP_STATUS_ENTRYLEVEL6 )
		{
			pOperObj->OpStatus = OP_STAUS_CELLLEVEL4;
			OPERATOR_showCellLevel(operHandle);
			u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_500MS);	// 500 ms
			u8LastKey = OPERATOR_NO_KEY;
			u16ContNo = 0;
		}
		else
		{
		    bool bEmergyStatus;
		    bEmergyStatus = OPERATOR_showEmergyTrip(operHandle);

	        if (OPERATOR_isKey(operHandle, OPERATOR_UP_KEY))
	        {
	            if (!DM_isAutotuneMode(pOperObj->dmHandle) && !bEmergyStatus)
	            {
	                OPERATOR_runUpKey(operHandle);
	                if (u8LastKey == OPERATOR_UP_KEY)
	                    u16ContNo++;
	                if (u16ContNo > 15)
	                    u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_100MS);
	                else if (u16ContNo > 8)
	                    u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_200MS);
	                else
	                    u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_500MS);    // 100 ms
	                u8LastKey = OPERATOR_UP_KEY;
	            }
	        }
	        else if (OPERATOR_isKey(operHandle, OPERATOR_DOWN_KEY))
	        {
	            if (!DM_isAutotuneMode(pOperObj->dmHandle) && !bEmergyStatus)
	            {
	                OPERATOR_runDownKey(operHandle);
	                if (u8LastKey == OPERATOR_DOWN_KEY)
	                    u16ContNo++;
	                if (u16ContNo > 15)
	                    u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_100MS);
	                else if (u16ContNo > 8)
	                    u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_200MS);
	                else
	                    u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_500MS);   // 500 ms
	                u8LastKey = OPERATOR_DOWN_KEY;
	            }
	        }
	        else if (OPERATOR_isKey(operHandle, OPERATOR_ENTER_KEY))
	        {
	            if (bEmergyStatus)
	            {
	            	DM_setFirstDisplayTrip(pOperObj->dmHandle, false);
	                OPERATOR_runMenuKey(operHandle);
	            }
	            else
	                OPERATOR_runEnterKey(operHandle);

	            u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_500MS);    // 500 ms
	            u8LastKey = OPERATOR_NO_KEY;
	            u16ContNo = 0;
	        }
	        else if (OPERATOR_isKey(operHandle, OPERATOR_ESC_KEY))
	        {
	            if (bEmergyStatus)
	            {
	            	DM_setFirstDisplayTrip(pOperObj->dmHandle, false);
	                OPERATOR_runMenuKey(operHandle);
	            }
	            else
	                OPERATOR_runEscKey(operHandle);

	            u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_500MS);    // 500 ms
	            u8LastKey = OPERATOR_NO_KEY;
	            u16ContNo = 0;

	        }
	        else if (OPERATOR_isKey(operHandle, OPERATOR_RESET_KEY))
	        {
	        	if (!DM_isAutotuneMode(pOperObj->dmHandle))
	        	{
	        		 if (bEmergyStatus)
	        		 {
	        			 DM_setFirstDisplayTrip(pOperObj->dmHandle, false);
	        			 OPERATOR_runMenuKey(operHandle);
	        		 }
	        		 else
	        			 OPERATOR_runResetKey(operHandle);

	        	}

	            u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_500MS);    // 500 ms
	            u8LastKey = OPERATOR_NO_KEY;
	            u16ContNo = 0;

	        }
	        else if (OPERATOR_isKey(operHandle, OPERATOR_LOCAL_KEY))
	        {
	        	if (!DM_isAutotuneMode(pOperObj->dmHandle) && !bEmergyStatus)
	            {
	                OPERATOR_runLocalKey(operHandle);
	                u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_500MS);    // 500 ms
	                u8LastKey = OPERATOR_NO_KEY;
	                u16ContNo = 0;
	            }
	        }
	        else if (OPERATOR_isKey(operHandle, OPERATOR_STOP_KEY))
	        {
	        	if (!DM_isAutotuneMode(pOperObj->dmHandle) && !bEmergyStatus)
	            {
	                OPERATOR_runStopKey(operHandle);
	                u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_500MS);    // 500 ms
	                u8LastKey = OPERATOR_NO_KEY;
	                u16ContNo = 0;
	            }
	        }
	        else if (OPERATOR_isKey(operHandle, OPERATOR_RUN_KEY))
	        {
	        	 if (!DM_isAutotuneMode(pOperObj->dmHandle) && !bEmergyStatus)
	            {
	                OPERATOR_runStartKey(operHandle);
	                u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_500MS);    // 500 ms
	                u8LastKey = OPERATOR_NO_KEY;
	                u16ContNo = 0;
	            }
	        }
	        else if (OPERATOR_isKey(operHandle, OPERATOR_JOG_KEY))
	        {

	        }
	        else if (OPERATOR_isKey(operHandle, OPERATOR_MENU_KEY))
	        {
	            if (bEmergyStatus)
	            {
	            	DM_setFirstDisplayTrip(pOperObj->dmHandle, false);
	            	//OPERATOR_runMenuKey(operHandle);
	            }
	            //else
	            OPERATOR_runMenuKey(operHandle);

	            u16KeyDelayCnt = DATA_ConvertToCnt(KEY_DELAY_500MS);    // 500 ms
	            u8LastKey = OPERATOR_NO_KEY;
	            u16ContNo = 0;
	        }
	        else if (OPERATOR_isKey(operHandle, OPERATOR_FWR_KEY))
	        {

	        }
	        else
	        {
	        	if ( !bEmergyStatus)
	        	{
	        		u16KeyDelayCnt = OPERATOR_runNoKey(operHandle);
	        		u8LastKey = OPERATOR_NO_KEY;
	        		u16ContNo = 0;
	        	}
	        }
		}

	}

}


//g_pOperObj

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------


void OutInit1(SCIMessage_Handle sciMessageHandle)
{
	const uint_least8_t au8OutData[] =
			{0x00, 0x70, 0x00, 0x08, 0x20, 0x56, 0x55, 0x56,
	         0x78, 0x20, 0x54, 0x56, 0x55, 0x78, 0x20, 0x55,
	         0x55, 0x55, 0x78, 0x38, 0x56, 0x55, 0x56, 0x18,
	         0x38, 0x54, 0x56, 0x55, 0x18, 0x00, 0x48, 0x7a,
	         0x41, 0x00, 0x38, 0x45, 0xfd
			};
	 //uint_least8_t u8TotNo = 37;
	g_pOperObj-> OpCommStatus = OPERATOR_INIT2;
	SCIMessage_TxWrite(sciMessageHandle,37, au8OutData, OPERATOR_callback);

/*	uint_least8_t i;
	for (i=0; i<u8TotNo; i++)
		pSciMessage->au8TxBuffer[i] = au8OutData[i];
	pSciMessage->u8TxNumOfBytes = u8TotNo;
	pSciMessage->u8TxIndex = 0;

	SCI_enableTxFifoInt(pSciMessage->sciHandle);*/


}

void OutInit2(SCIMessage_Handle sciMessageHandle)
//void OutInit2(SCI_Message *pSciMessage)
{
	const uint_least8_t au8OutData[] =
			{0x46, 0x45, 0x38, 0x44, 0x44, 0x7e, 0x45, 0x44,
			 0x3c, 0x40, 0x42, 0x41, 0x3c, 0x3c, 0x40, 0x42,
			 0x21, 0x7c, 0x3c, 0x41, 0x40, 0x41, 0x3c, 0x20,
			 0x55, 0x56, 0x54, 0x78, 0x38, 0x55, 0x56, 0x54,
			 0x18, 0x38, 0x46, 0x45, 0x46, 0x38, 0x73, 0xdd,
			 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			};
	//const uint_least8_t u8TotNo = 46;
	g_pOperObj-> OpCommStatus = OPERATOR_START;
	SCIMessage_TxWrite(sciMessageHandle,46, au8OutData, NULL);

	/*
	uint_least8_t i;
	for (i=0; i<u8TotNo; i++)
		pSciMessage->au8TxBuffer[i] = au8OutData[i];
	pSciMessage->u8TxNumOfBytes = u8TotNo;
	pSciMessage->u8TxIndex = 0;
	SCI_enableTxFifoInt(pSciMessage->sciHandle);

	g_pOperObj-> OpCommStatus = OPERATOR_START;*/
}

void OutNormal(SCIMessage_Handle sciMessageHandle)
{
	uint_least8_t i;
	uint16_t	u16CRC;
	SCI_Message *pSciMessage = (SCI_Message *) sciMessageHandle;

	g_pOperObj->u16Key= DATA_MakeWord(pSciMessage->au8RxBuffer[2], pSciMessage->au8RxBuffer[3]);

	pSciMessage->au8TxBuffer[0] = 0;
	pSciMessage->au8TxBuffer[1] = 0x71;
	pSciMessage->au8TxBuffer[2] = g_pOperObj->u8LEDBlink;
	pSciMessage->au8TxBuffer[3] = g_pOperObj->u8LEDOnOff;

	pSciMessage->au8TxBuffer[4] = 0;
	pSciMessage->au8TxBuffer[5] = 0;
	pSciMessage->au8TxBuffer[6] = 0;
	pSciMessage->au8TxBuffer[7] = 0;

	pSciMessage->au8TxBuffer[8] = DATA_HiByte(g_pOperObj->u16LCDBlink[0]);
	pSciMessage->au8TxBuffer[9] = DATA_LoByte(g_pOperObj->u16LCDBlink[0]);
	pSciMessage->au8TxBuffer[10] = DATA_HiByte(g_pOperObj->u16LCDBlink[1]);
	pSciMessage->au8TxBuffer[11] = DATA_LoByte(g_pOperObj->u16LCDBlink[1]);

	for (i=0; i<16; i++)
		pSciMessage->au8TxBuffer[12+i] = (uint_least8_t)  g_pOperObj-> ai8LCD[0][i];

	for (i=0; i<16; i++)
		pSciMessage->au8TxBuffer[28+i] = (uint_least8_t) g_pOperObj-> ai8LCD[1][i];

	u16CRC = DATA_CalCRC16(pSciMessage->au8TxBuffer,44);
	pSciMessage->au8TxBuffer[44] = DATA_LoByte(u16CRC) ;
	pSciMessage->au8TxBuffer[45] = DATA_HiByte(u16CRC) ;

	SCIMessage_TxWrite0(sciMessageHandle, 46, NULL);

/*	pSciMessage->u8TxNumOfBytes = 46;
	pSciMessage->u8TxIndex = 0;
	SCI_enableTxFifoInt(pSciMessage->sciHandle);*/

}
//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void OPERATOR_callback(SCIMessage_Handle sciMessageHandle)
{
	SCI_Message *pSciMessage = (SCI_Message *) sciMessageHandle;
	if (pSciMessage->au8RxBuffer[1] == 0x70)
		OutInit1(sciMessageHandle);
	else
	{
		if (g_pOperObj-> OpCommStatus == OPERATOR_START)
			OutNormal(pSciMessage);
		else
			OutInit2(pSciMessage);
	}

}



// end of file
