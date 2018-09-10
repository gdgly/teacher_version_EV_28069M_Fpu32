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
//! \file   solutions/instaspin_motion/src/user.c
//! \brief Contains the function for setting initialization data to the CTRL, HAL, and EST modules
//!
//! (C) Copyright 2012, Texas Instruments, Inc.


// **************************************************************************
// the includes
#include <stdio.h>

#include "sw/modules/user/src/32b/user.h"

#include "include/user_data.h"
#include "include/sci_operator.h"
#include "include/main.h"

//#include "sw/modules/hal/boards/hvkit_rev1p1/f28x/f2806x/src/hal.h"

#define	VERSION	1807

#define HAL_Gpio_DriveEnable    GPIO_Number_24

extern MOTOR_Vars_t         gMotorVars;

#define MAX_DESCRIPTION_LENGTH		17
uint16_t g_u16Reserved;
uint16_t g_u16DelayCnt;



DM_Obj gdmObj;
DM_Handle gdmHandle;

const char g_ai8DescCtrlMethod[][MAX_DESCRIPTION_LENGTH] = {"Open Loop FOC", "FOC w Enc#1", "FOC w Enc#2", "FOC w Hall", "Mix BLDC"};
const char g_ai8DescCtrlLoop[][MAX_DESCRIPTION_LENGTH] = {"Current", "Speed", "Position"};
const char g_ai8DescTripTrace[][MAX_DESCRIPTION_LENGTH] =
			{ "None Fault", "EEProm", "IO Expander","Under Volt", "Over Volt", "Over Curr",
			  "Over Heat", "Exetrn Term", "Unknown"
			};



//const char g_ai8DescLanguage[][MAX_DESCRIPTION_LENGTH] ={"English","Chinese"};
const char g_ai8DescInitial[][MAX_DESCRIPTION_LENGTH] ={"No Initial","User Initial","2-wire Initial","3-wire Initial"};

const char g_ai8DescAccessLevel[][MAX_DESCRIPTION_LENGTH] ={"Monitor Only","User Program", "Quick-start","Basic","Advanced"};
const char g_ai8DescReferSelect[][MAX_DESCRIPTION_LENGTH]={"Digital Operator", "Terminal","Serial Commun","Optional PCB"};
const char g_ai8DescStopMethod[][MAX_DESCRIPTION_LENGTH]={"Ramp to Stop","Coast to Stop","DC Injection","Coast with Timer"};
const char g_ai8DescEnable[][MAX_DESCRIPTION_LENGTH]={"Enable","Disable"};

const DM_Cell g_dmCellReserved    	= {&g_u16Reserved,  I2C_ADDRESS_INVALID, MODBUS_ADDRESS_INVALID,
										0x0000,0x0000, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point0,
										DM_UNIT_None, NULL, NULL};
const DM_FunCell g_dmFunCellReserved 	= {&g_dmCellReserved,  1, "Reserved",NULL};

const DM_Cell g_dmCellFreqRef    	= {&gdmObj.i16RefFreqHz,  I2C_ADDRESS_INVALID, 0x0020,
										0x0000,0x7fff, 0x8000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point2 ,
										DM_UNIT_Hz, NULL, NULL};
const DM_Cell g_dmCellFreqOut    	= {NULL,   I2C_ADDRESS_INVALID, 0x0021,
										 0x0000,0x7fff, 0x8000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point2,
										DM_UNIT_Hz, DM_getCallbackFreqOutHz, NULL};
const DM_Cell g_dmCellCurrentOut  	= {NULL, I2C_ADDRESS_INVALID, 0x0022,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point2,
										DM_UNIT_Amp, DM_getCallbackCurrentOut, NULL};
const DM_Cell g_dmCellCtlLoop       = {&gdmObj.ctlLoop, 0x10, 0x0023,
										0x0001,0x0002, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Point0,
										DM_UNIT_None, NULL, DM_setCallbackCtlLoop};
const DM_Cell g_dmCellCtlMethod  	= {&gdmObj.ctlMethod, 0x12, 0x0024,
										0x0000,0x0004, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Point0,
										DM_UNIT_None, NULL, DM_setCallbackCtlLoop};

const DM_Cell g_dmCellCurrentRef  	= {&gdmObj.i16RefCurrent, I2C_ADDRESS_INVALID, 0x0025,
										0x0000,0x7fff, 0x8000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_Int16  | DM_ATTRIBUTE_Point2,
										DM_UNIT_Amp, NULL, NULL};


const DM_Cell g_dmCellTorqueOut  	= {NULL, I2C_ADDRESS_INVALID, 0x0026,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point2,
										DM_UNIT_Ntm, DM_getCallbackTorqueOut, NULL};
const DM_Cell g_dmCellPowerOut  	= {NULL, I2C_ADDRESS_INVALID, 0x0027,
										 0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point3,
										DM_UNIT_Power, DM_getCallbackPowerOut, NULL};
const DM_Cell g_dmCellDCBus  	    = {NULL, I2C_ADDRESS_INVALID, 0x0028 ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_UInt16| DM_ATTRIBUTE_Point1,
										DM_UNIT_Volt, DM_getCallbackDCBus, NULL};

const DM_Cell g_dmCellMotorSpd  	= {NULL, I2C_ADDRESS_INVALID, 0x0029,
										0x0000,0x7fff, 0x8000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point0,
										DM_UNIT_RPM, DM_getCallbackFreqOutRpm, NULL};

const DM_Cell g_dmCellMotorSpdEnc1 	= {NULL, I2C_ADDRESS_INVALID, 0x002A,
										0x0000,0x7fff, 0x8000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point0,
										DM_UNIT_RPM, DM_getCallbackEnc1OutRpm, NULL};

const DM_Cell g_dmCellMotorSpdEnc2 	= {NULL, I2C_ADDRESS_INVALID, 0x002B,
										0x0000,0x7fff, 0x8000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point0,
										DM_UNIT_RPM, DM_getCallbackEnc2OutRpm, NULL};

const DM_Cell g_dmCellMotorSpdHall 	= {NULL, I2C_ADDRESS_INVALID, 0x002C,
										0x0000,0x7fff, 0x8000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point0,
										DM_UNIT_RPM, DM_getCallbackHallOutRpm, NULL};


const DM_Cell g_dmCellInputTerm  	= {NULL, I2C_ADDRESS_INVALID, 0x002D,
										0x0000,0x00ff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Sts ,
										DM_UNIT_None, DM_getCallbackInputTerm, NULL};
const DM_Cell g_dmCellOutputTerm  	= {&gdmObj.u16OutputTerm, I2C_ADDRESS_INVALID, 0x002A,
										0x0000,0x00ff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write| DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Sts ,
										DM_UNIT_None, NULL, NULL};

const DM_Cell g_dmCellVoltageU  	= {NULL, I2C_ADDRESS_INVALID, 0x002E ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point1,
										DM_UNIT_Volt, DM_getCallbackVoltageU, NULL};
const DM_Cell g_dmCellVoltageV  	= {NULL, I2C_ADDRESS_INVALID, 0x002F ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point1,
										DM_UNIT_Volt, DM_getCallbackVoltageV, NULL};
const DM_Cell g_dmCellVoltageW  	= {NULL, I2C_ADDRESS_INVALID, 0x0030 ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point1,
										DM_UNIT_Volt, DM_getCallbackVoltageW, NULL};

const DM_Cell g_dmCellCurrentU  	= {NULL, I2C_ADDRESS_INVALID, 0x0031 ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point2,
										DM_UNIT_Amp, DM_getCallbackCurrentU, NULL};

const DM_Cell g_dmCellCurrentV  	= {NULL, I2C_ADDRESS_INVALID, 0x0032 ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point2,
										DM_UNIT_Amp, DM_getCallbackCurrentV, NULL};

const DM_Cell g_dmCellCurrentW  	= {NULL, I2C_ADDRESS_INVALID, 0x0033 ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point2,
										DM_UNIT_Amp, DM_getCallbackCurrentW, NULL};

const DM_Cell g_dmCellExtTemp	= {NULL, I2C_ADDRESS_INVALID, 0x0034 ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_UInt16| DM_ATTRIBUTE_Point1,
										DM_UNIT_Temp, DM_getCallbackExtTemp, NULL};
const DM_Cell g_dmCellExtAd1	= {NULL, I2C_ADDRESS_INVALID, 0x0035 ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point2,
										DM_UNIT_Volt, DM_getCallbackExtAd1, NULL};
const DM_Cell g_dmCellExtAd2	= {NULL, I2C_ADDRESS_INVALID, 0x0036 ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point2,
										DM_UNIT_Volt, DM_getCallbackExtAd2, NULL};
const DM_Cell g_dmCellExtAd3	= {NULL, I2C_ADDRESS_INVALID, 0x0037 ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point2,
										DM_UNIT_Volt, DM_getCallbackExtAd3, NULL};
const DM_Cell g_dmCellExtAd4	= {NULL, I2C_ADDRESS_INVALID, 0x0038 ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point2,
										DM_UNIT_Volt, DM_getCallbackExtAd4, NULL};
const DM_Cell g_dmCellExtAd5	= {NULL, I2C_ADDRESS_INVALID, 0x0039 ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point2,
										DM_UNIT_Volt, DM_getCallbackExtAd5, NULL};
const DM_Cell g_dmCellExtAd6	= {NULL, I2C_ADDRESS_INVALID, 0x003A ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Int16| DM_ATTRIBUTE_Point2,
										DM_UNIT_Volt, DM_getCallbackExtAd6, NULL};
const DM_Cell g_dmCellVersion	= {NULL, I2C_ADDRESS_INVALID, 0x003B ,
										0x0000,0xffff, 0x0000,
										DM_ATTRIBUTE_Read | DM_TYPE_UInt16 | DM_ATTRIBUTE_Point2,
										DM_UNIT_None, DM_getCallbackVersion, NULL};


const DM_FunCell g_dmFunCellU1_01 			= {&g_dmCellFreqRef, 	1, "Frequency Ref", NULL};
const DM_FunCell g_dmFunCellU1_02 			= {&g_dmCellFreqOut, 	2, "Output Freq", NULL};
const DM_FunCell g_dmFunCellU1_03 			= {&g_dmCellCurrentOut, 3, "Output Current", NULL};
const DM_FunCell g_dmFunCellU1_04 			= {&g_dmCellCtlLoop,  4, "Control Loop", &g_ai8DescCtrlLoop[0][0]};
const DM_FunCell g_dmFunCellU1_05 			= {&g_dmCellCtlMethod,  5, "Control Method", &g_ai8DescCtrlMethod[0][0]};
const DM_FunCell g_dmFunCellU1_06 			= {&g_dmCellCurrentRef, 	6, "Torque Ref", NULL};

const DM_FunCell g_dmFunCellU1_07 			= {&g_dmCellTorqueOut,  7, "Output Torque", NULL};
const DM_FunCell g_dmFunCellU1_08 			= {&g_dmCellPowerOut, 	8, "Output Power", NULL};
const DM_FunCell g_dmFunCellU1_09 			= {&g_dmCellDCBus, 		9, "DC Bus Voltage", NULL};

const DM_FunCell g_dmFunCellU1_10 			= {&g_dmCellMotorSpd,   10, "Motor Speed", NULL};
const DM_FunCell g_dmFunCellU1_11 			= {&g_dmCellMotorSpdEnc1,   11, "Enc#1 Speed", NULL};
const DM_FunCell g_dmFunCellU1_12 			= {&g_dmCellMotorSpdEnc2,   12, "Enc#2 Speed", NULL};
const DM_FunCell g_dmFunCellU1_13 			= {&g_dmCellMotorSpdHall,   13, "Hall Speed", NULL};

const DM_FunCell g_dmFunCellU1_14 			= {&g_dmCellInputTerm, 	14, "Input Term Sts", NULL};
const DM_FunCell g_dmFunCellU1_15 			= {&g_dmCellOutputTerm, 15, "Output Term Sts", NULL};

const DM_FunCell g_dmFunCellU1_16 			= {&g_dmCellVoltageU, 	16, "Voltage-U", NULL};
const DM_FunCell g_dmFunCellU1_17 			= {&g_dmCellVoltageV, 	17, "Voltage-V", NULL};
const DM_FunCell g_dmFunCellU1_18 			= {&g_dmCellVoltageW, 	18, "Voltage-W", NULL};
const DM_FunCell g_dmFunCellU1_19 			= {&g_dmCellCurrentU, 	19, "Current-U", NULL};
const DM_FunCell g_dmFunCellU1_20 			= {&g_dmCellCurrentV, 	20, "Current-V", NULL};
const DM_FunCell g_dmFunCellU1_21 			= {&g_dmCellCurrentW, 	21, "Current-W", NULL};
const DM_FunCell g_dmFunCellU1_22 			= {&g_dmCellExtTemp, 	22, "Ext. Temp", NULL};
const DM_FunCell g_dmFunCellU1_23 			= {&g_dmCellExtAd1, 	23, "Ext. AD#1", NULL};
const DM_FunCell g_dmFunCellU1_24 			= {&g_dmCellExtAd2, 	24, "Ext. AD#2", NULL};
const DM_FunCell g_dmFunCellU1_25 			= {&g_dmCellExtAd3, 	25, "Ext. AD#3", NULL};
const DM_FunCell g_dmFunCellU1_26 			= {&g_dmCellExtAd4, 	26, "Ext. AD#4", NULL};
const DM_FunCell g_dmFunCellU1_27 			= {&g_dmCellExtAd5, 	27, "Ext. AD#5", NULL};
const DM_FunCell g_dmFunCellU1_28 			= {&g_dmCellExtAd6, 	28, "Ext. AD#6", NULL};
const DM_FunCell g_dmFunCellU1_29 			= {&g_dmCellVersion, 	29, "Version", NULL};


const DM_Cell g_dmTripTrace1	= {&gdmObj.au16TripTypeTrace[0], I2C_ADDRESS_INVALID, 0x003C ,
									0x0000,0xffff, 0x0000,
									DM_ATTRIBUTE_Read | DM_TYPE_UInt16,
									DM_UNIT_TRIP, NULL, NULL};
const DM_Cell g_dmTripTrace2	= {&gdmObj.au16TripTypeTrace[1], I2C_ADDRESS_INVALID, 0x003D ,
									0x0000,0xffff, 0x0000,
									DM_ATTRIBUTE_Read | DM_TYPE_UInt16,
									DM_UNIT_TRIP, NULL, NULL};
const DM_Cell g_dmTripTrace3	= {&gdmObj.au16TripTypeTrace[2], I2C_ADDRESS_INVALID, 0x003E ,
									0x0000,0xffff, 0x0000,
									DM_ATTRIBUTE_Read | DM_TYPE_UInt16,
									DM_UNIT_TRIP, NULL, NULL};
const DM_Cell g_dmTripTrace4	= {&gdmObj.au16TripTypeTrace[3], I2C_ADDRESS_INVALID, 0x003F ,
									0x0000,0xffff, 0x0000,
									DM_ATTRIBUTE_Read | DM_TYPE_UInt16,
									DM_UNIT_TRIP, NULL, NULL};
const DM_Cell g_dmTripTrace5	= {&gdmObj.au16TripTypeTrace[4], I2C_ADDRESS_INVALID, 0x0040 ,
									0x0000,0xffff, 0x0000,
									DM_ATTRIBUTE_Read | DM_TYPE_UInt16,
									DM_UNIT_TRIP, NULL, NULL};

const DM_FunCell g_dmFunCellU2_01 			= {&g_dmTripTrace1, 	1, "Fault #1", &g_ai8DescTripTrace[0][0]};
const DM_FunCell g_dmFunCellU2_02 			= {&g_dmTripTrace2, 	2, "Fault #2", &g_ai8DescTripTrace[0][0]};
const DM_FunCell g_dmFunCellU2_03 			= {&g_dmTripTrace3, 	3, "Fault #3", &g_ai8DescTripTrace[0][0]};
const DM_FunCell g_dmFunCellU2_04 			= {&g_dmTripTrace4, 	4, "Fault #4", &g_ai8DescTripTrace[0][0]};
const DM_FunCell g_dmFunCellU2_05 			= {&g_dmTripTrace5, 	5, "Fault #5", &g_ai8DescTripTrace[0][0]};



//const DM_FunCell g_dmFunCellU3_01 			= {&g_dmCellReserved, 	1, "Reserved", NULL};


const DM_FunCell* g_dmFunCellArrayU1[] = {&g_dmFunCellU1_01, &g_dmFunCellU1_02, &g_dmFunCellU1_03, &g_dmFunCellU1_04, &g_dmFunCellU1_05,
										  &g_dmFunCellU1_06, &g_dmFunCellU1_07, &g_dmFunCellU1_08, &g_dmFunCellU1_09, &g_dmFunCellU1_10,
										  &g_dmFunCellU1_11, &g_dmFunCellU1_12, &g_dmFunCellU1_13, &g_dmFunCellU1_14, &g_dmFunCellU1_15,
										  &g_dmFunCellU1_16, &g_dmFunCellU1_17, &g_dmFunCellU1_18, &g_dmFunCellU1_19, &g_dmFunCellU1_20,
										  &g_dmFunCellU1_21, &g_dmFunCellU1_22, &g_dmFunCellU1_23, &g_dmFunCellU1_24, &g_dmFunCellU1_25,
										  &g_dmFunCellU1_26, &g_dmFunCellU1_27, &g_dmFunCellU1_28, &g_dmFunCellU1_29};
const DM_FunCell* g_dmFunCellArrayU2[] = {&g_dmFunCellU2_01, &g_dmFunCellU2_02, &g_dmFunCellU2_03, &g_dmFunCellU2_04,&g_dmFunCellU2_05};
//const DM_FunCell* g_dmFunCellArrayU3[] = {&g_dmFunCellU3_01};

const DM_Function g_dmFunU1 ={&g_dmFunCellArrayU1[0], 29, "U1","Monitor"};	//21
const DM_Function g_dmFunU2 ={&g_dmFunCellArrayU2[0], 5, "U2","Fault Trace"};
//const DM_Function g_dmFunU3 ={&g_dmFunCellArrayU3[0], 1, "U3","Fault History"};

const DM_Function* g_dmFunUArray[] = {&g_dmFunU1, &g_dmFunU2};



//const DM_Cell g_dmCellLanguage  	= {&g_u8Language, I2C_ADDRESS_INVALID, 0x0100,
//										0x0000,0x0001, 0x0000,
//										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Point0,
//										DM_UNIT_None, NULL, NULL};
const DM_Cell g_dmCellAccessLvl  	= {&gdmObj.operatorAccess, 0x1A, 0x0101,
										0x0004,0x0004, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Point0,
										DM_UNIT_None, NULL, NULL};
const DM_Cell g_dmCellInitial  	= {&gdmObj.u8InitialLevel, I2C_ADDRESS_INVALID, 0x0103,
										0x0000,0x0004, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Point0,
										DM_UNIT_None, NULL, NULL};
//const DM_Cell g_dmCellPassword  	= {&g_u16Password, I2C_ADDRESS_INVALID, 0x0104,
//										0x0000,9999, 0x0000,
//										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Point0,
//										DM_UNIT_None, NULL, NULL};


//const DM_FunCell g_dmFunCellA1_01 	= {&g_dmCellLanguage,  1, "Select Language", &g_ai8DescLanguage[0][0]};
const DM_FunCell g_dmFunCellA1_02 	= {&g_dmCellAccessLvl,  1, "Access Level", &g_ai8DescAccessLevel[0][0]};
const DM_FunCell g_dmFunCellA1_03 	= {&g_dmCellCtlMethod,  2, "Control Method", &g_ai8DescCtrlMethod[0][0]};
const DM_FunCell g_dmFunCellA1_04 	= {&g_dmCellInitial,  3, "Initial Param", &g_ai8DescInitial[0][0]};
//const DM_FunCell g_dmFunCellA1_05 	= {&g_dmCellPassword,  4, "Password", NULL};





const DM_FunCell* g_dmFunCellArrayA1[] = { &g_dmFunCellA1_02, &g_dmFunCellA1_03,  &g_dmFunCellA1_04};
const DM_FunCell* g_dmFunCellArrayA2[] = {&g_dmFunCellReserved};
const DM_Function g_dmFunA1 ={&g_dmFunCellArrayA1[0], 3, "A1","Initial"};
const DM_Function g_dmFunA2 ={&g_dmFunCellArrayA2[0], 1, "A2","User Consants"};
const DM_Function* g_dmFunAArray[] = {&g_dmFunA1, &g_dmFunA2};


const DM_Cell g_dmCellReferenceSel  = {&gdmObj.refSelect, 0x20, 0x0180,
										0x0000,0x0003, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Point0,
										DM_UNIT_None, NULL, NULL};
const DM_Cell g_dmCellOperationSel  = {&gdmObj.runSelect, 0x22, 0x0181,
										0x0000,0x0003, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Point0,
										DM_UNIT_None, NULL, NULL};
const DM_Cell g_dmCellStopMethod  = {&gdmObj.stopSelect, 0x24, 0x0182,
										0x0000,0x0003, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Point0,
										DM_UNIT_None, NULL, NULL};
const DM_Cell g_dmCellReverseProhibit  = {&gdmObj.bReverseProhibit, 0x26, 0x0183,
										0x0000,0x0001, 0x0000,
										DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 | DM_ATTRIBUTE_Point0,
										DM_UNIT_None, NULL, NULL};
const DM_Cell g_dmCellAccTime    = {&gdmObj.u16AccTime, 0x28, 0x0184,
									20,60000, 0x0001,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point2,
									DM_UNIT_None, NULL, DM_setCallbackAccTime};
const DM_Cell g_dmCellBLDC2FOC    = {&gdmObj.u16BLDC2FOCSpeed, 0x2A, 0x0185,
									1200,60000, 10,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point0,
									DM_UNIT_RPM, NULL,DM_setCallbackBLDCtoFOC };
const DM_Cell g_dmCellFOC2BLDC    = {&gdmObj.u16FOC2BLDCSpeed, 0x2C, 0x0186,
									800,60000, 10,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point0,
									DM_UNIT_RPM, NULL,DM_setCallbackFOCtoBLDC };



const DM_FunCell g_dmFunCellb1_01 	= {&g_dmCellReferenceSel,  1, "Reference Sel",&g_ai8DescReferSelect[0][0]};
const DM_FunCell g_dmFunCellb1_02 	= {&g_dmCellOperationSel,  2, "Operation Sel",&g_ai8DescReferSelect[0][0]};
const DM_FunCell g_dmFunCellb1_03 	= {&g_dmCellStopMethod,  3, "Stopping Sel",&g_ai8DescStopMethod[0][0]};
const DM_FunCell g_dmFunCellb1_04 	= {&g_dmCellReverseProhibit,  4, "Reverse Oper",&g_ai8DescEnable[0][0]};
const DM_FunCell g_dmFunCellb1_05 	= {&g_dmCellAccTime,  5, "Acc. Time",NULL};
const DM_FunCell g_dmFunCellb1_06 	= {&g_dmCellBLDC2FOC,  6, "BLDCtoFOC",NULL};
const DM_FunCell g_dmFunCellb1_07 	= {&g_dmCellFOC2BLDC,  7, "FOCtoBLDC",NULL};




const DM_Cell g_dmCellDCBusMax  = {&gdmObj.u16DCBusMax, 0x30, 0x0200,
									60,600, 30,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point0,
									DM_UNIT_Volt, NULL, NULL};

const DM_Cell g_dmCellDCBusMin  = {&gdmObj.u16DCBusMin, 0x32, 0x0201,
									40,600, 10,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point0,
									DM_UNIT_Volt, NULL, NULL};

const DM_Cell g_dmCellCurrentMax  = {&gdmObj.u16CurrentMax, 0x34, 0x0202,
									20,600, 5,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point1,
									DM_UNIT_Amp, NULL, NULL};




const DM_FunCell g_dmFunCellb2_01 	= {&g_dmCellDCBusMax,  1, "DC Bus Max",NULL};
const DM_FunCell g_dmFunCellb2_02 	= {&g_dmCellDCBusMin,  2, "DC Bus Min",NULL};
const DM_FunCell g_dmFunCellb2_03 	= {&g_dmCellCurrentMax,  3, "Current Max",NULL};

const DM_FunCell* g_dmFunCellArrayb1[] = {&g_dmFunCellb1_01, &g_dmFunCellb1_02, &g_dmFunCellb1_03, &g_dmFunCellb1_04, &g_dmFunCellb1_05,
										  &g_dmFunCellb1_06, &g_dmFunCellb1_07};
const DM_FunCell* g_dmFunCellArrayb2[] = {&g_dmFunCellb2_01, &g_dmFunCellb2_02, &g_dmFunCellb2_03};


const DM_Function g_dmFunb1 ={&g_dmFunCellArrayb1[0], 7, "b1","Sequence"};
const DM_Function g_dmFunb2 ={&g_dmFunCellArrayb2[0], 3, "b2","Ref Limit"};
/*const DM_Function g_dmFunb3 ={&g_dmFunCellArrayb3[0], 1, "b3","Speed Search"};
const DM_Function g_dmFunb4 ={&g_dmFunCellArrayb4[0], 1, "b4","Delay Timers"};
const DM_Function g_dmFunb5 ={&g_dmFunCellArrayb5[0], 1, "b5","PID Control"};
const DM_Function g_dmFunb6 ={&g_dmFunCellArrayb6[0], 1, "b6","Reference Dwell"};
const DM_Function g_dmFunb7 ={&g_dmFunCellArrayb7[0], 1, "b7","Droop Control"};
const DM_Function g_dmFunb8 ={&g_dmFunCellArrayb8[0], 1, "b8","Energy Saving"};
const DM_Function g_dmFunb9 ={&g_dmFunCellArrayb9[0], 1, "b9","Zero Servo"};
const DM_Function* g_dmFunbArray[] = {&g_dmFunb1, &g_dmFunb2, &g_dmFunb3, &g_dmFunb4, &g_dmFunb5, &g_dmFunb6,
									  &g_dmFunb7, &g_dmFunb8, &g_dmFunb9  };*/

const DM_Function* g_dmFunbArray[] = {&g_dmFunb1, &g_dmFunb2 };

const DM_Cell g_dmCellResEstCurrent  = {&gdmObj.i16ResEstCurrent, 0x40, 0x0200,
									10,2000, 1,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point1,
									DM_UNIT_Amp, NULL, DM_setCallbackResEstCurrent};
const DM_Cell g_dmCellIndEstCurrent  = {&gdmObj.i16IndEstCurrent, 0x42, 0x0201,
									(uint16_t)(-10),(uint16_t)(-1), (uint16_t)(-2000),
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point1,
									DM_UNIT_Amp, NULL, DM_setCallbackIndEstCurrent};
const DM_Cell g_dmCellFluxEstHz  = {&gdmObj.i16FluxEstHz, 0x44, 0x0202,
									200, 2000, 1,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point1,
									DM_UNIT_Hz, NULL, DM_setCallbackFluxEstHz};

const DM_FunCell g_dmFunCellC1_01 	= {&g_dmCellResEstCurrent,  1, "Res Est Curr",NULL};
const DM_FunCell g_dmFunCellC1_02 	= {&g_dmCellIndEstCurrent,  2, "Ind Est Curr",NULL};
const DM_FunCell g_dmFunCellC1_03 	= {&g_dmCellFluxEstHz,  3, "Flux Est Hz",NULL};




const DM_FunCell* g_dmFunCellArrayC1[] = {&g_dmFunCellC1_01, &g_dmFunCellC1_02, &g_dmFunCellC1_03};

const DM_Function g_dmFunC1 ={&g_dmFunCellArrayC1[0], 3, "C1","PreTune"};

const DM_Cell g_dmCellVBias1  = {&gdmObj.u16VBias1, 0x50, 0x0203,
									5000,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, NULL};	//};
const DM_Cell g_dmCellVBias2  = {&gdmObj.u16VBias2, 0x52, 0x0204,
									5000,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, NULL};	//DM_setcallbackVBias2};
const DM_Cell g_dmCellVBias3  = {&gdmObj.u16VBias3, 0x54, 0x0205,
									5000,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, NULL};	//DM_setcallbackVBias3};
const DM_Cell g_dmCellIBias1  = {&gdmObj.u16IBias1, 0x56, 0x0206,
									5000,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, NULL};	//DM_setcallbackIBias1};
const DM_Cell g_dmCellIBias2  = {&gdmObj.u16IBias2, 0x58, 0x0207,
									5000,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, NULL};	//DM_setcallbackIBias2};
const DM_Cell g_dmCellIBias3  = {&gdmObj.u16IBias3, 0x5A, 0x0208,
									5000,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, NULL};	//DM_setcallbackIBias3};

const DM_FunCell g_dmFunCellC2_01 	= {&g_dmCellVBias1,  1, "Vbias #1",NULL};
const DM_FunCell g_dmFunCellC2_02 	= {&g_dmCellVBias2,  2, "Vbias #2",NULL};
const DM_FunCell g_dmFunCellC2_03 	= {&g_dmCellVBias3,  3, "Vbias #3",NULL};
const DM_FunCell g_dmFunCellC2_04 	= {&g_dmCellIBias1,  4, "Ibias #1",NULL};
const DM_FunCell g_dmFunCellC2_05 	= {&g_dmCellIBias2,  5, "Ibias #2",NULL};
const DM_FunCell g_dmFunCellC2_06 	= {&g_dmCellIBias3,  6, "Ibias #3",NULL};

const DM_FunCell* g_dmFunCellArrayC2[] = {&g_dmFunCellC2_01, &g_dmFunCellC2_02, &g_dmFunCellC2_03,
										  &g_dmFunCellC2_04, &g_dmFunCellC2_05, &g_dmFunCellC2_06};

const DM_Function g_dmFunC2 ={&g_dmFunCellArrayC2[0], 6, "C2","Bias"};


const DM_Function* g_dmFunCArray[] = {&g_dmFunC1, &g_dmFunC2 };

const char g_ai8DescMotorType[][MAX_DESCRIPTION_LENGTH]={"Induction","PM Motor"};

const DM_Cell g_dmCellMotorType  = {&gUserParams.motor_type, 0x60, 0x0250,
									MOTOR_Type_Pm,MOTOR_Type_Pm, MOTOR_Type_Induction,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 ,
									DM_UNIT_None, NULL, NULL};
const DM_Cell g_dmCellPolePair  = {&gUserParams.u16motor_numPolePairs, 0x62, 0x0251,
									4,50, 1,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 ,
									DM_UNIT_None, NULL, NULL};

const DM_Cell g_dmCellMaxCurrent  = {&gdmObj.i16MaxCurrent, 0x64, 0x0252,
									180,500, 1,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_Int16 | DM_ATTRIBUTE_Point1,
									DM_UNIT_Amp, NULL, DM_setCallbackMaxCurrent};

const DM_Cell g_dmCellResStator  = {&gdmObj.u16MotorRs, 0x66, 0x0253,
									2924,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, DM_setCallbackResStator};
const DM_Cell g_dmCellResRotor  = {&gdmObj.u16MotorRr, 0x68, 0x0254,
									0,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, DM_setCallbackResRotor};
const DM_Cell g_dmCellIndAxisD  = {&gdmObj.u16MotorLd, 0x6A, 0x0255,
									0,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point3,
									DM_UNIT_mHenry, NULL, DM_setCallbackIndAxisD};
const DM_Cell g_dmCellIndAxisQ  = {&gdmObj.u16MotorLq, 0x6C, 0x0256,
									0,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point3,
									DM_UNIT_mHenry, NULL, DM_setCallbackIndAxisQ};
const DM_Cell g_dmCellRatedFlux  = {&gdmObj.u16RatedFlux, 0x6E, 0x0257,
									0,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point3,
									DM_UNIT_None, NULL, DM_setCallbackRatedFlux};
const DM_Cell g_dmCellMagnetCur  = {&gdmObj.u16MagnetCur, 0x70, 0x0258,
									0,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, DM_setCallbackMagnetCur};




const DM_FunCell g_dmFunCellD1_01 	= {&g_dmCellMotorType,  1, "Motor Type",&g_ai8DescMotorType[0][0]};
const DM_FunCell g_dmFunCellD1_02 	= {&g_dmCellPolePair,  2, "Pole Pairs",NULL};
const DM_FunCell g_dmFunCellD1_03 	= {&g_dmCellMaxCurrent,  3, "Max Current",NULL};

const DM_FunCell g_dmFunCellD1_04 	= {&g_dmCellResStator,  4, "Stator Resist",NULL};
const DM_FunCell g_dmFunCellD1_05 	= {&g_dmCellResRotor,  5, "Rotor Resist",NULL};
const DM_FunCell g_dmFunCellD1_06 	= {&g_dmCellIndAxisD,  6, "Ind D-axis",NULL};
const DM_FunCell g_dmFunCellD1_07 	= {&g_dmCellIndAxisQ,  7, "Ind Q-axis",NULL};
const DM_FunCell g_dmFunCellD1_08 	= {&g_dmCellRatedFlux, 8, "Rated Flux",NULL};
const DM_FunCell g_dmFunCellD1_09 	= {&g_dmCellMagnetCur, 9, "Magnetic Cur",NULL};

const DM_FunCell* g_dmFunCellArrayD1[] = {&g_dmFunCellD1_01, &g_dmFunCellD1_02, &g_dmFunCellD1_03, &g_dmFunCellD1_04,
										  &g_dmFunCellD1_05, &g_dmFunCellD1_06, &g_dmFunCellD1_07, &g_dmFunCellD1_08,
										  &g_dmFunCellD1_09};


const DM_Function g_dmFunD1 ={&g_dmFunCellArrayD1[0], 9, "d1","Motor"};

const DM_Cell g_dmCellKp_Speed  = {&gdmObj.u16Kp_Speed, 0x80, 0x0300,
									6000,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point3,
									DM_UNIT_None, NULL, DM_setCallbackKpSpeed};
const DM_Cell g_dmCellKi_Speed  = {&gdmObj.u16Ki_Speed, 0x82, 0x0301,
									30,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, DM_setCallbackKiSpeed};
const DM_Cell g_dmCellKp_Id  = {&gdmObj.u16Kp_Id, 0x84, 0x0302,
									100,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point3,
									DM_UNIT_None, NULL, DM_setCallbackKp_Id};
const DM_Cell g_dmCellKi_Id  = {&gdmObj.u16Ki_Id, 0x86, 0x0303,
									12,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, DM_setCallbackKi_Id};
const DM_Cell g_dmCellKp_Iq  = {&gdmObj.u16Kp_Iq, 0x88, 0x0304,
									1,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point3,
									DM_UNIT_None, NULL, DM_setCallbackKp_Iq};
const DM_Cell g_dmCellKi_Iq  = {&gdmObj.u16Ki_Iq, 0x8a, 0x0305,
									12,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, DM_setCallbackKi_Iq};
const DM_Cell g_dmCellKp_BLDC  = {&gdmObj.u16Kp_BLDC, 0x8c, 0x0306,
									340,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point3,
									DM_UNIT_None, NULL, DM_setCallbackKp_BLDC};
const DM_Cell g_dmCellKi_BLDC  = {&gdmObj.u16Ki_BLDC, 0x8e, 0x0307,
									39,65535, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt16 | DM_ATTRIBUTE_Point4,
									DM_UNIT_None, NULL, DM_setCallbackKi_BLDC};

const DM_FunCell g_dmFunCellD2_01 	= {&g_dmCellKp_Speed,  1, "Kp (Speed) ",NULL};
const DM_FunCell g_dmFunCellD2_02 	= {&g_dmCellKi_Speed,  2, "Ki (Speed) ",NULL};
const DM_FunCell g_dmFunCellD2_03 	= {&g_dmCellKp_Id,  3, "Kp (Id) ",NULL};
const DM_FunCell g_dmFunCellD2_04 	= {&g_dmCellKi_Id,  4, "Ki (Id) ",NULL};
const DM_FunCell g_dmFunCellD2_05 	= {&g_dmCellKp_Iq,  5, "Kp (Iq) ",NULL};
const DM_FunCell g_dmFunCellD2_06 	= {&g_dmCellKi_Iq,  6, "Ki (Iq) ",NULL};
const DM_FunCell g_dmFunCellD2_07 	= {&g_dmCellKp_BLDC,  7, "Kp (BLDC) ",NULL};
const DM_FunCell g_dmFunCellD2_08 	= {&g_dmCellKi_BLDC,  8, "Ki (BLDC) ",NULL};

const DM_FunCell* g_dmFunCellArrayD2[] = {&g_dmFunCellD2_01, &g_dmFunCellD2_02, &g_dmFunCellD2_03, &g_dmFunCellD2_04,
										  &g_dmFunCellD2_05, &g_dmFunCellD2_06, &g_dmFunCellD2_07, &g_dmFunCellD2_08};

const DM_Function g_dmFunD2 ={&g_dmFunCellArrayD2[0], 8, "d2","PID"};


//const DM_Cell g_dmCellFactory  = {&gdmObj.u8FactorySetting, NULL, 0x0310,
//									0,1, 0,
//									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 ,
//									DM_UNIT_None, NULL, NULL};
const DM_Cell g_dmCellMotorModel  = {&gdmObj.u16ModelIndex, 0x90, 0x0311,
									0,5, 0,
									DM_ATTRIBUTE_Read | DM_ATTRIBUTE_Write | DM_ATTRIBUTE_UInt8 ,
									DM_UNIT_None, NULL, DM_setCallbackMotorModel};

//const char g_ai8DescFactory[][MAX_DESCRIPTION_LENGTH]={"No","Yes"};
const char g_ai8DescModel[][MAX_DESCRIPTION_LENGTH]={"SDCQ BL129S25"," Delta C30604E", "EMJ_04APB22", "QSMotor 3KW", "QSMotor 1.5KW", "ADLEE 1HP"};

//const DM_FunCell g_dmFunCellD3_01 	= {&g_dmCellFactory,  1, "Factory",&g_ai8DescFactory[0][0]};
const DM_FunCell g_dmFunCellD3_02 	= {&g_dmCellMotorModel, 1, "Model",&g_ai8DescModel[0][0]};

const DM_FunCell* g_dmFunCellArrayD3[] = { &g_dmFunCellD3_02};
const DM_Function g_dmFunD3 ={&g_dmFunCellArrayD3[0], 1, "d3","Restore Paras"};

const DM_Function* g_dmFunDArray[] = {&g_dmFunD1 , &g_dmFunD2, &g_dmFunD3};





/*
const DM_FunCell g_dmFunCellC1_01 	= {&g_dmCellDCBusMax,  1, "DC-Bus Max",NULL};
const DM_FunCell g_dmFunCellC1_02 	= {&g_dmCellDCBusMin,  2, "DC-Bus Min",NULL};
const DM_FunCell* g_dmFunCellArrayC1[] = {&g_dmFunCellC1_01, &g_dmFunCellC1_02};
const DM_FunCell* g_dmFunCellArrayC2[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayC3[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayC4[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayC5[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayC6[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayC7[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayC8[] = {&g_dmFunCellReserved};


const DM_Function g_dmFunC1 ={&g_dmFunCellArrayC1[0], 2, "C1","Accel/Decel"};
const DM_Function g_dmFunC2 ={&g_dmFunCellArrayC2[0], 1, "C2","S-Curve Acc/Dec"};
const DM_Function g_dmFunC3 ={&g_dmFunCellArrayC3[0], 1, "C3","Motor-Slip Comp"};
const DM_Function g_dmFunC4 ={&g_dmFunCellArrayC4[0], 1, "C4","Torque Comp"};
const DM_Function g_dmFunC5 ={&g_dmFunCellArrayC5[0], 1, "C5","ASR Tuning"};
const DM_Function g_dmFunC6 ={&g_dmFunCellArrayC6[0], 1, "C6","Carrier Freq"};
const DM_Function g_dmFunC7 ={&g_dmFunCellArrayC7[0], 1, "C7","Hunting Prev"};
const DM_Function g_dmFunC8 ={&g_dmFunCellArrayC8[0], 1, "C8","Factory Tuning"};
const DM_Function* g_dmFunCArray[] = {&g_dmFunC1, &g_dmFunC2, &g_dmFunC3, &g_dmFunC4, &g_dmFunC5, &g_dmFunC6,
									  &g_dmFunC7, &g_dmFunC8 };

const DM_FunCell* g_dmFunCellArrayd1[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayd2[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayd3[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayd4[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayd5[] = {&g_dmFunCellReserved};

const DM_Function g_dmFund1 ={&g_dmFunCellArrayd1[0], 1, "d1","Preset Reference"};
const DM_Function g_dmFund2 ={&g_dmFunCellArrayd2[0], 1, "d2","Reference Limits"};
const DM_Function g_dmFund3 ={&g_dmFunCellArrayd3[0], 1, "d3","Jump Frequencies"};
const DM_Function g_dmFund4 ={&g_dmFunCellArrayd4[0], 1, "d4","Sequence"};
const DM_Function g_dmFund5 ={&g_dmFunCellArrayd5[0], 1, "d5","Torque Control"};
const DM_Function* g_dmFundArray[] = {&g_dmFund1, &g_dmFund2, &g_dmFund3, &g_dmFund4, &g_dmFund5 };

const DM_FunCell* g_dmFunCellArrayE1[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayE2[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayE3[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayE4[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayE5[] = {&g_dmFunCellReserved};

const DM_Function g_dmFunE1 ={&g_dmFunCellArrayE1[0], 1, "E1","V/f Pattern"};
const DM_Function g_dmFunE2 ={&g_dmFunCellArrayE2[0], 1, "E2","Motor Setup"};
const DM_Function g_dmFunE3 ={&g_dmFunCellArrayE3[0], 1, "E3","Motor 2 Ctl Meth"};
const DM_Function g_dmFunE4 ={&g_dmFunCellArrayE4[0], 1, "E4","V/F pattern 2"};
const DM_Function g_dmFunE5 ={&g_dmFunCellArrayE5[0], 1, "E5","Motor 2 Setup"};
const DM_Function* g_dmFunEArray[] = {&g_dmFunE1, &g_dmFunE2, &g_dmFunE3, &g_dmFunE4, &g_dmFunE5 };

const DM_FunCell* g_dmFunCellArrayF1[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayF2[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayF3[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayF4[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayF5[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayF6[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayF7[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayF8[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayF9[] = {&g_dmFunCellReserved};

const DM_Function g_dmFunF1 ={&g_dmFunCellArrayF1[0], 1, "F1","PG Option Setup"};
const DM_Function g_dmFunF2 ={&g_dmFunCellArrayF2[0], 1, "F2","AI-14 Setup"};
const DM_Function g_dmFunF3 ={&g_dmFunCellArrayF3[0], 1, "F3","DI-08, 16 Setup"};
const DM_Function g_dmFunF4 ={&g_dmFunCellArrayF4[0], 1, "F4","AO-08, 12 Setup"};
const DM_Function g_dmFunF5 ={&g_dmFunCellArrayF5[0], 1, "F5","DO-02C"};
const DM_Function g_dmFunF6 ={&g_dmFunCellArrayF6[0], 1, "F6","DO-08"};
const DM_Function g_dmFunF7 ={&g_dmFunCellArrayF7[0], 1, "F7","PO-36F Setup"};
const DM_Function g_dmFunF8 ={&g_dmFunCellArrayF8[0], 1, "F8","SI-F/G"};
const DM_Function g_dmFunF9 ={&g_dmFunCellArrayF9[0], 1, "F9","DDS/SI-B"};
const DM_Function* g_dmFunFArray[] = {&g_dmFunF1, &g_dmFunF2, &g_dmFunF3, &g_dmFunF4, &g_dmFunF5,
									  &g_dmFunF6, &g_dmFunF7, &g_dmFunF8, &g_dmFunF9 };

const DM_FunCell* g_dmFunCellArrayH1[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayH2[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayH3[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayH4[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayH5[] = {&g_dmFunCellReserved};

const DM_Function g_dmFunH1 ={&g_dmFunCellArrayH1[0], 1, "H1","Digital Inputs"};
const DM_Function g_dmFunH2 ={&g_dmFunCellArrayH2[0], 1, "H2","Digital Outputs"};
const DM_Function g_dmFunH3 ={&g_dmFunCellArrayH3[0], 1, "H3","Analog Inputs"};
const DM_Function g_dmFunH4 ={&g_dmFunCellArrayH4[0], 1, "H4","Analog Outputs"};
const DM_Function g_dmFunH5 ={&g_dmFunCellArrayH5[0], 1, "H5","Serial Com Setup"};
const DM_Function* g_dmFunHArray[] = {&g_dmFunH1, &g_dmFunH2, &g_dmFunH3, &g_dmFunH4, &g_dmFunH5};

const DM_FunCell* g_dmFunCellArrayL1[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayL2[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayL3[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayL4[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayL5[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayL6[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayL7[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayL8[] = {&g_dmFunCellReserved};

const DM_Function g_dmFunL1 ={&g_dmFunCellArrayL1[0], 1, "L1","Motor Overload"};
const DM_Function g_dmFunL2 ={&g_dmFunCellArrayL2[0], 1, "L2","PwrLoss Ridethru"};
const DM_Function g_dmFunL3 ={&g_dmFunCellArrayL3[0], 1, "L3","Stall Prevention"};
const DM_Function g_dmFunL4 ={&g_dmFunCellArrayL4[0], 1, "L4","Ref Detection"};
const DM_Function g_dmFunL5 ={&g_dmFunCellArrayL5[0], 1, "L5","Fault Restart"};
const DM_Function g_dmFunL6 ={&g_dmFunCellArrayL6[0], 1, "L6","Torque Detection"};
const DM_Function g_dmFunL7 ={&g_dmFunCellArrayL7[0], 1, "L7","Torque Limit"};
const DM_Function g_dmFunL8 ={&g_dmFunCellArrayL8[0], 1, "L8","Hdwe Protection"};
const DM_Function* g_dmFunLArray[] = {&g_dmFunL1, &g_dmFunL2, &g_dmFunL3, &g_dmFunL4, &g_dmFunL5,
									  &g_dmFunL6, &g_dmFunL7, &g_dmFunL8};

const DM_FunCell* g_dmFunCellArrayo1[] = {&g_dmFunCellReserved};
const DM_FunCell* g_dmFunCellArrayo2[] = {&g_dmFunCellReserved};

const DM_Function g_dmFuno1 ={&g_dmFunCellArrayo1[0], 1, "o1","Monitor Select"};
const DM_Function g_dmFuno2 ={&g_dmFunCellArrayo2[0], 1, "o2","Key Selections"};
const DM_Function* g_dmFunoArray[] = {&g_dmFuno1, &g_dmFuno2};
*/

const DM_Group		g_dmGroupU 	={&g_dmFunUArray[0], 2, "U", "Monitor"};
const DM_Group		g_dmGroupA 	={&g_dmFunAArray[0], 2, "A", "Initialize"};
const DM_Group		g_dmGroupb 	={&g_dmFunbArray[0], 2, "b","Application"};
const DM_Group		g_dmGroupC 	={&g_dmFunCArray[0], 2, "C","Tuning"};
const DM_Group		g_dmGroupD 	={&g_dmFunDArray[0], 3, "d","Setting"};

/*const DM_Group		g_dmGroupC 	={&g_dmFunCArray[0], 8, "C","Tuning"};
const DM_Group		g_dmGroupd 	={&g_dmFundArray[0], 5, "d","Reference"};
const DM_Group		g_dmGroupE 	={&g_dmFunEArray[0], 5, "E","Motor"};
const DM_Group		g_dmGroupF 	={&g_dmFunFArray[0], 9, "F","Option"};
const DM_Group		g_dmGroupH 	={&g_dmFunHArray[0], 5, "H","Terminal"};
const DM_Group		g_dmGroupL 	={&g_dmFunLArray[0], 8, "L","Protection"};
const DM_Group		g_dmGroupo 	={&g_dmFunoArray[0], 2, "o","Operator"};
*/

const DM_Group* 	g_dmGroupOperArray[] = {&g_dmGroupU};
const DM_Group* 	g_dmGroupInitArray[] = {&g_dmGroupA};
const DM_Group* 	g_dmGroupProgArray[] = {&g_dmGroupb, &g_dmGroupC, &g_dmGroupD};
//const DM_Group* 	g_dmGroupProgArray[] = {&g_dmGroupb, &g_dmGroupC, &g_dmGroupd,
//									    	&g_dmGroupE, &g_dmGroupF, &g_dmGroupH, &g_dmGroupL,
//									    	&g_dmGroupo};

const DM_Mode		g_dmModeOperation ={&g_dmGroupOperArray[0], 1, "Operation"};
const DM_Mode 		g_dmModeInitialize = {&g_dmGroupInitArray[0],1,  "Initialize"};
const DM_Mode   	g_dmModeProgram = {&g_dmGroupProgArray[0], 3, "Programming"};
const DM_Mode   	g_dmModeAutotuning = {NULL,0, "Autotuning"};

const DM_Mode*  	g_dmModeArray[] = {&g_dmModeOperation, &g_dmModeInitialize, &g_dmModeProgram, &g_dmModeAutotuning};



//------------------------------------------------------------------------
//
//------------------------------------------------------------------------

uint16_t DATA_min(uint16_t u16Val1, uint16_t u16Val2)
{
	return (u16Val1 < u16Val2) ? u16Val1 : u16Val2;

}

void Data_SetDelayCnt(uint16_t u16Delay)
{
	g_u16DelayCnt =  DATA_ConvertToCnt(u16Delay);
}

uint16_t Data_GetDelayCnt()		// internal
{
	return g_u16DelayCnt;
}

uint16_t DATA_ConvertToCnt(uint16_t u16Delay)
{
	return u16Delay/((uint16_t) USER_TIMER_PERIOD_msec);
}

void DATA_Delay(uint16_t u16Delay)	//ms
{
	uint16_t u16Value = 0;
	Data_SetDelayCnt(u16Delay);
	for (;;)
	{
		u16Value = Data_GetDelayCnt();
		if (u16Value == 0) return;

	}

}

uint16_t DATA_MakeWord(uint_least8_t u8HiWord, uint_least8_t u8LoWord )
{
	return ((uint16_t)(u8HiWord << 8) ) | ((uint16_t) u8LoWord);
}


uint_least8_t DATA_HiByte(uint16_t u16Value)
{
	return (uint_least8_t) (u16Value >> 8);
}

uint_least8_t DATA_LoByte(uint16_t u16Value)
{
	return (uint_least8_t) (u16Value & 0x00ff);
}

uint16_t DATA_InRange(DM_TYPE_e dmType, uint16_t u16Value, uint16_t u16DefValue, uint16_t u16MaxValue, uint16_t u16MinValue)
{
	switch (dmType)
	{
		case DM_TYPE_Int16:
		{
			int16_t i16Value, i16MaxValue, i16MinValue;
			i16Value = (int16_t)u16Value;
			i16MaxValue = (int16_t)u16MaxValue;
			i16MinValue = (int16_t)u16MinValue;
			return  ((i16Value >= i16MinValue) && (i16Value <= i16MaxValue)) ?
					u16Value : u16DefValue;
		}

		case DM_TYPE_UInt16:
			return ((u16Value >= u16MinValue) && (u16Value <= u16MaxValue)) ?
					u16Value : u16DefValue;
		case DM_TYPE_Int8:
		{
			int_least8_t i8Value, i8MaxValue, i8MinValue;
			i8Value = (int_least8_t)u16Value;
			i8MaxValue = (int_least8_t)u16MaxValue;
			i8MinValue = (int_least8_t)u16MinValue;
			return  ((i8Value >= i8MinValue) && (i8Value <= i8MaxValue)) ?
					  u16Value : u16DefValue;
		}

		case DM_TYPE_UInt8:
		{
			uint_least8_t u8Value, u8MaxValue, u8MinValue;
			u8Value = (uint_least8_t)u16Value;
			u8MaxValue = (uint_least8_t)u16MaxValue;
			u8MinValue = (uint_least8_t)u16MinValue;
			return  ((u8Value >= u8MinValue) && (u8Value <= u8MaxValue)) ?
					  u16Value : u16DefValue;
		}

	}
	return u16Value;
}


uint16_t DATA_CalCRC16(uint_least8_t au8Buff[], uint_least8_t u8TotNo)
{
	uint16_t u16Crc16 = 0xffff;
	uint_least8_t	i, j;
    for (i = 0; i < u8TotNo; i++)
    {
		u16Crc16 ^=  (au8Buff[i] & 0x00ffu);
        for (j = 0; j < 8; j++)
        {
			if ((u16Crc16 & 0x0001u) != 0)
            {
                u16Crc16 >>= 1;
                u16Crc16 ^= 0xa001u;
            }
            else
				u16Crc16 >>= 1;
        }
    }
    return u16Crc16;
}
//------------------------------------------------------------------------
//


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
DM_Handle DM_init(const void *pMemory,const size_t numBytes)
{
    DM_Handle dmHandle;
    uint_least8_t i;
    if (numBytes < sizeof(DM_Obj))
        return((DM_Handle)NULL);

    // assign the handle
    dmHandle = (DM_Handle)pMemory;
    DM_Obj *pdmObj = (DM_Obj *) pMemory;



    pdmObj->pdmMode = &g_dmModeArray[0];
    pdmObj->u8ArraySize = 4;
    strcpy(pdmObj->ai8ObjName, "Rich");

    pdmObj->filterHandleDCBus = FILTER_init(&pdmObj->filterDCBus, sizeof(pdmObj->filterDCBus));
    pdmObj->filterHandleOutFreq = FILTER_init(&pdmObj->filterOutFreq, sizeof(pdmObj->filterOutFreq));
    pdmObj->filterHandleOutEncoder = FILTER_init(&pdmObj->filterOutEncoder, sizeof(pdmObj->filterOutEncoder));
    pdmObj->filterHandleOutCurrent = FILTER_init(&pdmObj->filterOutCurrent, sizeof(pdmObj->filterOutCurrent));


    DM_setLocalRemoteMode(dmHandle, false);	// initial at remote mode
    DM_setAutotuneMode(dmHandle, false);
    //DM_setHardwareFaultType(dmHandle,FaultType_NoFault );
    //DM_setInverterTripType(dmHandle, TripType_NoTrip);

    pdmObj->i16RefFreqHz = pdmObj->i16RefCurrent = 0;
    //pdmObj-> hardwareFaultType = FaultType_NoFault;
    for (i=0; i< MAX_TRIP_TRACE; i++ )
    	pdmObj->au16TripTypeTrace [i] = TripType_NoTrip;
    pdmObj->bFirstDisplayTripe = false;


    pdmObj->u8OperationSignal = Signal_Stop;
    pdmObj->refSelect = REF_DigialOperator;
    pdmObj->runSelect = REF_DigialOperator;



    FILTER_setupCoeffs(pdmObj->filterHandleDCBus,1000.0f, 10.0f);            //10 Hz
    FILTER_setupCoeffs(pdmObj->filterHandleOutFreq,1000.0f, 10.0f);          //10 Hz
    FILTER_setupCoeffs(pdmObj->filterHandleOutEncoder,1000.0f, 10.0f);      //10 Hz
    FILTER_setupCoeffs(pdmObj->filterHandleOutCurrent,1000.0f, 10.0f);       //10 Hz

    return(dmHandle);
}

void DM_run(DM_Handle dmHandle, HAL_AdcData_t *pAdcData, _iq iqOutFreq, _iq iqOutCur, _iq OutEncoder)

{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	if (g_u16DelayCnt)
		g_u16DelayCnt--;

	DM_runRefLimit(dmHandle);
	DM_runOperation(dmHandle);

	//DM_runOperStatus(dmHandle);
	//DM_runInputRef(dmHandle);


	FILTER_run(pdmObj->filterHandleDCBus, pAdcData->iqdcBus);
	FILTER_run(pdmObj->filterHandleOutFreq, iqOutFreq);
    FILTER_run(pdmObj->filterHandleOutCurrent, iqOutCur);
	FILTER_run(pdmObj->filterHandleOutEncoder, OutEncoder);

	pdmObj->iqOutVoltageU = pAdcData->V.aiqValue[0];
	pdmObj->iqOutVoltageV = pAdcData->V.aiqValue[1];
	pdmObj->iqOutVoltageW = pAdcData->V.aiqValue[2];

	pdmObj->iqOutCurrentU = pAdcData->I.aiqValue[0];
    pdmObj->iqOutCurrentV = pAdcData->I.aiqValue[1];
    pdmObj->iqOutCurrentW = pAdcData->I.aiqValue[2];


}


DM_TYPE_e getDMType(const DM_Cell *pDMCell);

bool DM_TerminalStatus(DM_Handle dmHandle, uint_least8_t u8Terminal)    // Active Low
{
    return ((~IOEXPAND_InputTermA(DM_getIoexpandHandle(dmHandle))) & u8Terminal) ;
}

/*bool DM_isRunStatus(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    bool bResult;
    if (DM_isLocalMode(dmHandle))
        bResult = (pdmObj->u8OperationSignal &  Signal_Run);
    else
        bResult = DM_TerminalStatus(DM_TERMINAL1);
    return bResult;
}*/


void DM_setup(DM_Handle dmHandle, EEPROM_Handle eepromHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    uint_least8_t i1;


    pdmObj->eepromHandle = eepromHandle;

    for (i1=0; i1<pdmObj->u8ArraySize; i1++)
    {
    	DM_Mode **pdmModeArray = (DM_Mode **) pdmObj->pdmMode;
    	//DM_Mode *pdmMode = pdmModeArray[u8ModeNo];
        DM_Mode *pdmMode= pdmModeArray[i1];
        DM_Group **pdmGroupArray = (DM_Group **) pdmMode->pdmGroup;
        uint_least8_t i2;
        for (i2=0; i2<pdmMode->u8ArraySize; i2++)
        {
            DM_Group *pdmGroup = pdmGroupArray[i2];
            DM_Function **pdmFunctionArray = (DM_Function **) pdmGroup->pdmFunction;
            uint_least8_t i3;
            if (pdmGroup == NULL) continue;
            for (i3=0; i3<pdmGroup->u8ArraySize; i3++)
            {
                DM_Function *pdmFunction = pdmFunctionArray[i3];
                DM_FunCell **pdmFunCellArray = (DM_FunCell **) pdmFunction->pdmFunCell;
                uint_least8_t i4;
                if (pdmFunction == NULL) continue;
                for (i4=0; i4<pdmFunction->u8ArraySize; i4++)
                {
                    DM_FunCell *pdmFunCell = pdmFunCellArray[i4];

                    //if (pdmFunCell == NULL) continue;
                    const DM_Cell *pdmCell = (DM_Cell *) pdmFunCell->pdmCell;
                    DM_TYPE_e  dmType = getDMType(pdmCell);
                    uint16_t u16Value;

                    if ( pdmCell->u16EEPromAddr == I2C_ADDRESS_INVALID)
                        u16Value = *((uint16_t *) pdmCell->pValue);
                    else
                        EEPROM_Read(eepromHandle,pdmCell->u16EEPromAddr, &u16Value);
                    *((uint16_t *)pdmCell->pValue) = DATA_InRange(dmType, u16Value, pdmCell->u16DefValue,
                            pdmCell->u16MaxValue, pdmCell->u16MinValue);
                    if (pdmCell->setCallbackFunction)
                    	pdmCell->setCallbackFunction(dmHandle, pdmCell);
                }
            }
        }

    }
}




DM_TYPE_e getDMType(const DM_Cell *pDMCell)
{
    uint16_t u16Attribute = pDMCell->u16Attribute & 0x0f00;
    switch (u16Attribute)
    {
        case DM_ATTRIBUTE_Int16:
            return DM_TYPE_Int16;
        case DM_ATTRIBUTE_UInt16:
            return DM_TYPE_UInt16;
        case DM_ATTRIBUTE_Int8:
            return DM_TYPE_Int8;
        default:
            return DM_TYPE_UInt8;
    }


}

const char* DM_getModeName(DM_Handle dmHandle, uint_least8_t u8ModeNo)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;

	DM_Mode **pdmModeArray = (DM_Mode **) pdmObj->pdmMode;
	DM_Mode *pdmMode = pdmModeArray[u8ModeNo];
	return pdmMode->ai8ModeName;

}

uint_least8_t DM_getModeSizeNo(DM_Handle dmHandle)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	if (pdmObj->operatorAccess == OP_ACCESS_MONITOR)
		return 2;	// For only operation & initial modes
	return pdmObj->u8ArraySize;
}

uint_least8_t DM_getOperModeSizeNo(DM_Handle dmHandle)
{
	return 24;
}

uint_least8_t DM_getAccessLevel(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	return (uint_least8_t)pdmObj->operatorAccess;
}


DM_Mode *DM_getMode(DM_Handle dmHandle, uint_least8_t u8ModeIndex)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	DM_Mode **pdmModeArray = (DM_Mode **) pdmObj->pdmMode;
	return  pdmModeArray[u8ModeIndex];
}

DM_Group *DM_getGroup(DM_Handle dmHandle,
						uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex)
{
	DM_Mode *pdmMode= DM_getMode(dmHandle, u8ModeIndex);
	DM_Group **pdmGroupArray = (DM_Group **) pdmMode->pdmGroup;
	return pdmGroupArray[u8GroupIndex];
}

DM_Function *DM_getFunction(DM_Handle dmHandle,
							  uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex, uint_least8_t u8FunctionIndex)
{
	DM_Group *pdmGroup =DM_getGroup(dmHandle, u8ModeIndex, u8GroupIndex);
	DM_Function **pdmFunctionArray = (DM_Function **) pdmGroup->pdmFunction;
	return pdmFunctionArray[u8FunctionIndex];
}

DM_FunCell *DM_getFunCell(DM_Handle dmHandle,
							uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex, uint_least8_t u8FunctionIndex, uint_least8_t u8CellIndex)
{

	DM_Function *pdmFunction = DM_getFunction(dmHandle, u8ModeIndex, u8GroupIndex, u8FunctionIndex);
	DM_FunCell*	*pdmFunCellArray= (DM_FunCell **) pdmFunction->pdmFunCell;
	return pdmFunCellArray[u8CellIndex];
}


uint_least8_t DM_getInitModeLevel(DM_Handle dmHandle)
{

	DM_Function *pdmFunction = DM_getFunction(dmHandle,OP_MODE_INIT,0,0);
	return pdmFunction->u8ArraySize;
}

uint_least8_t DM_getGroupSizeNo(DM_Handle dmHandle,uint_least8_t u8ModeIndex)
{
	DM_Mode *pdmMode = DM_getMode(dmHandle,u8ModeIndex);
	return pdmMode->u8ArraySize;

}

uint_least8_t DM_getFunctionSizeNo(DM_Handle dmHandle,uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex)
{

	DM_Group *pdmGroup = DM_getGroup(dmHandle,u8ModeIndex, u8GroupIndex);
	return pdmGroup->u8ArraySize;
}



uint_least8_t DM_getCellSizeNo(DM_Handle dmHandle,uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex, uint_least8_t u8FunctionIndex)
{

	DM_Function *pdmFunction = DM_getFunction(dmHandle,u8ModeIndex, u8GroupIndex, u8FunctionIndex);
	return pdmFunction->u8ArraySize;

}

uint16_t DM_getTotalFunctionSizeNo(DM_Handle dmHandle)
{
	DM_Mode *pdmMode = DM_getMode(dmHandle,OP_MODE_PROG);
	DM_Group **pdmGroupArray = (DM_Group **) pdmMode->pdmGroup;
	uint16_t u16TotNo = 0;
	uint_least8_t i2;
	for (i2=0; i2<pdmMode->u8ArraySize; i2++)
	{
		DM_Group *pdmGroup = pdmGroupArray[i2];
		u16TotNo += pdmGroup->u8ArraySize;
	}
	return u16TotNo;
}



uint16_t DM_getTotalCellSizeNo(DM_Handle dmHandle)
{
	DM_Mode *pdmMode = DM_getMode(dmHandle,OP_MODE_PROG);
	DM_Group **pdmGroupArray = (DM_Group **) pdmMode->pdmGroup;
	uint16_t u16TotNo = 0;
	uint_least8_t i2;
	for (i2=0; i2<pdmMode->u8ArraySize; i2++)
	{
		DM_Group *pdmGroup = pdmGroupArray[i2];
		DM_Function **pdmFunctionArray = (DM_Function **) pdmGroup->pdmFunction;
		uint_least8_t i3;
		//if (pdmGroup == NULL) continue;
		for (i3=0; i3<pdmGroup->u8ArraySize; i3++)
		{
			DM_Function *pdmFunction = pdmFunctionArray[i3];
			u16TotNo += pdmFunction->u8ArraySize;
		}
	}
	return u16TotNo;
}

void DM_findGroupFunctionIndex(DM_Handle dmHandle, uint16_t u16TotFunIndex,
							    uint_least8_t *pGroupIndex, uint_least8_t *pFunctionIndex)
{
	DM_Mode *pdmMode = DM_getMode(dmHandle,OP_MODE_PROG);
	DM_Group **pdmGroupArray = (DM_Group **) pdmMode->pdmGroup;

	uint_least8_t i2;
	*pFunctionIndex = 0;
	for (i2=0; i2<pdmMode->u8ArraySize; i2++)
	{
		DM_Group *pdmGroup = pdmGroupArray[i2];
		uint_least8_t u8ArraySize = pdmGroup->u8ArraySize;
		*pGroupIndex = i2;
		if (u16TotFunIndex < u8ArraySize)
		{
			*pFunctionIndex = (uint_least8_t)u16TotFunIndex;
			return;
		}
		u16TotFunIndex -= u8ArraySize;
	}
}

void DM_findGroupFunctionCellIndex(DM_Handle dmHandle, uint16_t u16TotCellIndex,
							       uint_least8_t *pGroupIndex, uint_least8_t *pFunctionIndex, uint_least8_t *pCellIndex
							       )
{
	DM_Mode *pdmMode = DM_getMode(dmHandle,OP_MODE_PROG);
	DM_Group **pdmGroupArray = (DM_Group **) pdmMode->pdmGroup;

	uint_least8_t i2;
	*pCellIndex = 0;
	for (i2=0; i2<pdmMode->u8ArraySize; i2++)
	{
		DM_Group *pdmGroup = pdmGroupArray[i2];
		DM_Function **pdmFunctionArray = (DM_Function **) pdmGroup->pdmFunction;
		uint_least8_t i3;
		*pGroupIndex = i2;
		for (i3=0; i3<pdmGroup->u8ArraySize; i3++)
		{
			DM_Function *pdmFunction = pdmFunctionArray[i3];
			uint_least8_t u8ArraySize = pdmFunction->u8ArraySize;
			*pFunctionIndex = i3;
			if (u16TotCellIndex < u8ArraySize)
			{
				*pCellIndex = (uint_least8_t)u16TotCellIndex;
				return;
			}
			u16TotCellIndex -= u8ArraySize;
		}

	}
}

char* ConvertToUnit(const DM_Cell *pdmCell)
{
	switch (pdmCell->u16Unit)
	{
		case DM_UNIT_Hz:
			return "Hz";
		case DM_UNIT_RPM:
			return " Rpm";
		case DM_UNIT_Amp:
			return "A";
		case DM_UNIT_Volt:
			return "V";
		case DM_UNIT_Power:
			return "kW";
		case DM_UNIT_Percent:
			return "%";
		case DM_UNIT_Sec:
			return "Sec";
		case DM_UNIT_Temp:
			return "C";
		case DM_UNIT_Ntm:
		    return "Nm";
		case DM_UNIT_mHenry:
			return "mH";
		default:
			return "";
	}
}

char* ConvertrToU8ValueString(uint_least8_t u8Value, uint16_t u16Attribute)
{
	uint16_t  u16Temp = u16Attribute & 0x00f0;
	static char ai8Buff[10];

	switch (u16Temp)
	{
		case DM_ATTRIBUTE_Point0:
			sprintf(ai8Buff,"%u", u8Value);
			break;
		case DM_ATTRIBUTE_Point1:
			sprintf(ai8Buff,"%u.%01u", u8Value/10, u8Value % 10);
			break;
		case DM_ATTRIBUTE_Point2:
			sprintf(ai8Buff,"%u.%02u", u8Value/100, u8Value % 100);
			break;
		case DM_ATTRIBUTE_Point3:
			sprintf(ai8Buff,"%u.%03u", u8Value/1000, u8Value % 1000);
			break;
		case DM_ATTRIBUTE_Point4:
			sprintf(ai8Buff,"%u.%04u", u8Value/10000, u8Value % 10000);
			break;
		case  DM_ATTRIBUTE_Hex2:
			sprintf(ai8Buff,"%02x", u8Value);
			break;
		case DM_ATTRIBUTE_Hex4:
			sprintf(ai8Buff,"%04x", u8Value);
			break;
		case DM_ATTRIBUTE_Sts:
		{
			uint_least8_t i;
			for (i=0;i<8; i++)
			{
				ai8Buff[i] = (u8Value & 0x80) ? '1' : '0';
				u8Value <<= 1;
			}
			ai8Buff[8]=0;
			break;
		}
		default:
			ai8Buff[0]=0;

	}
	return ai8Buff;
}

char* ConvertrToU8EditValueString(uint_least8_t u8Value, uint16_t u16Attribute)
{
	uint16_t  u16Temp = u16Attribute & 0x00f0;
	static char ai8Buff[10];

	switch (u16Temp)
	{

		case DM_ATTRIBUTE_Point0:
			sprintf(ai8Buff,"%03u", u8Value);
			break;
		case DM_ATTRIBUTE_Point1:
			sprintf(ai8Buff,"%02u.%01u", u8Value/10, u8Value % 10);
			break;
		case DM_ATTRIBUTE_Point2:
			sprintf(ai8Buff,"%01u.%02u", u8Value/100, u8Value % 100);
			break;
		case DM_ATTRIBUTE_Point3:
			sprintf(ai8Buff,"%01u.%03u", u8Value/1000, u8Value % 1000);
			break;
		case DM_ATTRIBUTE_Point4:
			sprintf(ai8Buff,"%01u.%04u", u8Value/10000, u8Value % 10000);
			break;
		case DM_ATTRIBUTE_Sts:
		case  DM_ATTRIBUTE_Hex2:
			sprintf(ai8Buff,"%02x", u8Value);
			break;
		case DM_ATTRIBUTE_Hex4:
			sprintf(ai8Buff,"%04x", u8Value);
			break;
		default:
			ai8Buff[0]=0;

	}
	return ai8Buff;
}

char* ConvertrToI8ValueString(int_least8_t i8Value, uint16_t u16Attribute)
{
	uint16_t  u16Temp = u16Attribute & 0x00f0;
	static char ai8Buff[10];

	switch (u16Temp)
	{
		case DM_ATTRIBUTE_Point0:
			sprintf(ai8Buff,"%d", i8Value);
			break;
		case DM_ATTRIBUTE_Point1:
			sprintf(ai8Buff,"%d.%01d", i8Value/10, i8Value % 10);
			break;
		case DM_ATTRIBUTE_Point2:
			sprintf(ai8Buff,"%d.%02d", i8Value/100, i8Value % 100);
			break;
		case DM_ATTRIBUTE_Point3:
			sprintf(ai8Buff,"%d.%03d", i8Value/1000, i8Value % 1000);
			break;
		case DM_ATTRIBUTE_Point4:
			sprintf(ai8Buff,"%d.%04d", i8Value/10000, i8Value % 10000);
			break;
		case  DM_ATTRIBUTE_Hex2:
			sprintf(ai8Buff,"%02x", i8Value);
			break;
		case DM_ATTRIBUTE_Hex4:
			sprintf(ai8Buff,"%04x", i8Value);
			break;
		default:
			ai8Buff[0]=0;

	}
	return ai8Buff;
}

char* ConvertrToI8EditValueString(int_least8_t i8Value, uint16_t u16Attribute)
{
	uint16_t  u16Temp = u16Attribute & 0x00f0;
	static char ai8Buff[10];

	switch (u16Temp)
	{
		case DM_ATTRIBUTE_Point0:
			sprintf(ai8Buff,"%03d", i8Value);
			break;
		case DM_ATTRIBUTE_Point1:
			sprintf(ai8Buff,"%02d.%01d", i8Value/10, i8Value % 10);
			break;
		case DM_ATTRIBUTE_Point2:
			sprintf(ai8Buff,"%01d.%02d", i8Value/100, i8Value % 100);
			break;
		case DM_ATTRIBUTE_Point3:
			sprintf(ai8Buff,"%0d.%03d", i8Value/1000, i8Value % 1000);
			break;
		case DM_ATTRIBUTE_Point4:
			sprintf(ai8Buff,"%0d.%04d", i8Value/10000, i8Value % 10000);
			break;
		case  DM_ATTRIBUTE_Hex2:
			sprintf(ai8Buff,"%02x", i8Value);
			break;
		case DM_ATTRIBUTE_Hex4:
			sprintf(ai8Buff,"%04x", i8Value);
			break;
		default:
			ai8Buff[0]=0;

	}
	return ai8Buff;
}

char* ConvertrToU16ValueString(uint16_t u16Value, uint16_t u16Attribute)
{
	uint16_t  u16Temp = u16Attribute & 0x00f0;
	static char ai8Buff[10];

	switch (u16Temp)
	{
		case DM_ATTRIBUTE_Point0:
			sprintf(ai8Buff,"%u", u16Value);
			break;
		case DM_ATTRIBUTE_Point1:
			sprintf(ai8Buff,"%u.%01u", u16Value/10, u16Value % 10);
			break;
		case DM_ATTRIBUTE_Point2:
			sprintf(ai8Buff,"%u.%02u", u16Value/100, u16Value % 100);
			break;
		case DM_ATTRIBUTE_Point3:
			sprintf(ai8Buff,"%u.%03u", u16Value/1000, u16Value % 1000);
			break;
		case DM_ATTRIBUTE_Point4:
			sprintf(ai8Buff,"%u.%04u", u16Value/10000, u16Value % 10000);
			break;
		case  DM_ATTRIBUTE_Hex2:
			sprintf(ai8Buff,"%02x", u16Value);
			break;
		case DM_ATTRIBUTE_Hex4:
			sprintf(ai8Buff,"%04x", u16Value);
			break;
		default:
			ai8Buff[0]=0;

	}
	return ai8Buff;
}

char* ConvertrToU16EditValueString(uint16_t u16Value, uint16_t u16Attribute)
{
	uint16_t  u16Temp = u16Attribute & 0x00f0;
	static char ai8Buff[10];

	switch (u16Temp)
	{
		case DM_ATTRIBUTE_Point0:
			sprintf(ai8Buff,"%05u", u16Value);
			break;
		case DM_ATTRIBUTE_Point1:
			sprintf(ai8Buff,"%04u.%01u", u16Value/10, u16Value % 10);
			break;
		case DM_ATTRIBUTE_Point2:
			sprintf(ai8Buff,"%03u.%02u", u16Value/100, u16Value % 100);
			break;
		case DM_ATTRIBUTE_Point3:
			sprintf(ai8Buff,"%02u.%03u", u16Value/1000, u16Value % 1000);
			break;
		case DM_ATTRIBUTE_Point4:
			sprintf(ai8Buff,"%01u.%04u", u16Value/10000, u16Value % 10000);
			break;
		case  DM_ATTRIBUTE_Hex2:
			sprintf(ai8Buff,"%02x", u16Value);
			break;
		case DM_ATTRIBUTE_Hex4:
			sprintf(ai8Buff,"%04x", u16Value);
			break;
		default:
			ai8Buff[0]=0;

	}
	return ai8Buff;
}



void ConvertAddingPoint(char *pString, uint_least8_t u8Position)
{
	uint_least8_t u8Index1 = 0, u8Index2;
	while (pString[++u8Index1] != 0);

	for (u8Index2 = 0; u8Index2 <= u8Position; u8Index2++)
	{
		pString[u8Index1+1] = pString[u8Index1];
		u8Index1--;
	}
	pString[u8Index1+1]='.';
}

char* ConvertrToI16ValueString(int16_t i16Value, uint16_t u16Attribute)
{
	uint16_t  u16Temp = u16Attribute & 0x00f0;
	static char ai8Buff[10];
	switch (u16Temp)
	{
		case DM_ATTRIBUTE_Point0:
			sprintf(ai8Buff,"%d", i16Value);
			break;
		case DM_ATTRIBUTE_Point1:
			sprintf(ai8Buff,"%02d", i16Value);
			ConvertAddingPoint(ai8Buff,1);
			break;
		case DM_ATTRIBUTE_Point2:
			sprintf(ai8Buff,"%03d", i16Value);
			ConvertAddingPoint(ai8Buff,2);
			break;
		case DM_ATTRIBUTE_Point3:
			sprintf(ai8Buff,"%04d", i16Value);
			ConvertAddingPoint(ai8Buff,3);
			break;
		case  DM_ATTRIBUTE_Hex2:
			sprintf(ai8Buff,"%02x", i16Value);
			break;
		case DM_ATTRIBUTE_Hex4:
			sprintf(ai8Buff,"%04x", i16Value);
			break;
		default:
			ai8Buff[0]=0;

	}
	return ai8Buff;
}

char* ConvertrToI16EditValueString(int16_t i16Value, uint16_t u16Attribute)
{
	uint16_t  u16Temp = u16Attribute & 0x00f0;
	static char ai8Buff[10];
	switch (u16Temp)
	{
		case DM_ATTRIBUTE_Point0:
			sprintf(ai8Buff,"%05d", i16Value);
			break;
		case DM_ATTRIBUTE_Point1:
			sprintf(ai8Buff,"%05d", i16Value);
			ConvertAddingPoint(ai8Buff,1);
			break;
		case DM_ATTRIBUTE_Point2:
			sprintf(ai8Buff,"%05d", i16Value);
			ConvertAddingPoint(ai8Buff,2);
			break;
		case DM_ATTRIBUTE_Point3:
			sprintf(ai8Buff,"%05d", i16Value);
			ConvertAddingPoint(ai8Buff,3);
			break;
		case  DM_ATTRIBUTE_Hex2:
			sprintf(ai8Buff,"%02x", i16Value);
			break;
		case DM_ATTRIBUTE_Hex4:
			sprintf(ai8Buff,"%04x", i16Value);
			break;
	}
	return ai8Buff;
	/*uint16_t  u16Temp = u16Attribute & 0x00f0;
	uint16_t  u16ModeValue;
	static char ai8Buff[10];

	switch (u16Temp)
	{
		case DM_ATTRIBUTE_Point0:
			sprintf(ai8Buff,"%05d", i16Value);
			break;
		case DM_ATTRIBUTE_Point1:
			u16ModeValue = abs(i16Value) % 10;
			sprintf(ai8Buff,"%04d.%01u", i16Value/10, u16ModeValue);
			break;
		case DM_ATTRIBUTE_Point2:
			u16ModeValue = abs(i16Value) % 100;
			sprintf(ai8Buff,"%03d.%02u", i16Value/100, u16ModeValue);
			break;
		case DM_ATTRIBUTE_Point3:
			u16ModeValue = abs(i16Value) % 1000;
			sprintf(ai8Buff,"%02d.%03u", i16Value/1000, u16ModeValue);
			break;
		case  DM_ATTRIBUTE_Hex2:
			sprintf(ai8Buff,"%02x", i16Value);
			break;
		case DM_ATTRIBUTE_Hex4:
			sprintf(ai8Buff,"%04x", i16Value);
			break;
		default:
			ai8Buff[0]=0;

	}
	return ai8Buff;*/
}

char* ConverterToValueString(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_TYPE_e dmType = getDMType(pdmCell);
	uint16_t  u16Attribute = pdmCell->u16Attribute;
	int32_t i32Value = DM_getCellValue(dmHandle, pdmCell);


	switch (dmType)
	{
		case DM_TYPE_Int16:
			return ConvertrToI16ValueString( i32Value ,u16Attribute);
			//return ConvertrToI16EditValueString( i32Value,u16Attribute);
		case DM_TYPE_UInt16:
			return ConvertrToU16ValueString( i32Value ,u16Attribute);
		case DM_TYPE_Int8:
			return ConvertrToI8ValueString( i32Value ,u16Attribute);
		case DM_TYPE_UInt8:
			return ConvertrToU8ValueString( i32Value ,u16Attribute);
		default:
			return "";
	}
}

char* ConverterToEditValueString(int32_t i32Value, const DM_Cell *pdmCell)
{
	DM_TYPE_e dmType = getDMType(pdmCell);
	uint16_t  u16Attribute = pdmCell->u16Attribute;
	switch (dmType)
	{
		case DM_TYPE_Int16:
			return ConvertrToI16EditValueString( (int16_t) i32Value ,u16Attribute);
		case DM_TYPE_UInt16:
			return ConvertrToU16EditValueString( (uint16_t) i32Value ,u16Attribute);
		case DM_TYPE_Int8:
			return ConvertrToI8EditValueString( (int_least8_t)i32Value ,u16Attribute);
		case DM_TYPE_UInt8:
			return ConvertrToU8EditValueString( (uint_least8_t)i32Value ,u16Attribute);
		default:
			return "";
	}
}

int32_t DM_getCellValue(DM_Handle handle,const DM_Cell  *pdmCell)
{
	if (pdmCell->pValue == NULL)
	{
		return pdmCell->getCallbackFunction(handle);
	}
	else
	{
		DM_TYPE_e dmType = getDMType(pdmCell);
		switch (dmType)
		{
			case DM_TYPE_Int16:
				return  *((int16_t *)pdmCell->pValue);
			case DM_TYPE_UInt16:
				return  *((uint16_t *)pdmCell->pValue);
			case DM_TYPE_Int8:
				return  *((int_least8_t *)pdmCell->pValue);
			case DM_TYPE_UInt8:
				return *((uint_least8_t *)pdmCell->pValue);
			default:
				return 0;
		}

	}
}

void DM_getToValueInt32(DM_Handle dmHandle,
		 	 	 	 	   uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
						   uint_least8_t u8FunIndex, uint_least8_t u8CellIndex,
						   int32_t *pi32Value, int32_t *pi32MaxValue, int32_t *pi32MinValue, int32_t *pi32IncValue )
{

	DM_FunCell *pdmFunCell = DM_getFunCell(dmHandle,u8ModeIndex, u8GroupIndex, u8FunIndex,  u8CellIndex);
	const DM_Cell *pdmCell = pdmFunCell->pdmCell;
	DM_TYPE_e dmType = getDMType(pdmCell);


	switch (dmType)
	{
		case DM_TYPE_Int16:
			*pi32Value = *((int16_t *)pdmCell->pValue);
			*pi32MaxValue = ((int16_t) pdmCell->u16MaxValue);
			*pi32MinValue = ((int16_t) pdmCell->u16MinValue);
			break;
		case DM_TYPE_UInt16:
			*pi32Value = *((uint16_t *)pdmCell->pValue);
			*pi32MaxValue = ((uint16_t) pdmCell->u16MaxValue);
			*pi32MinValue = ((uint16_t) pdmCell->u16MinValue);
			break;
		case DM_TYPE_Int8:
			*pi32Value = *((int_least8_t *)pdmCell->pValue);
			*pi32MaxValue = ((int_least8_t) pdmCell->u16MaxValue);
			*pi32MinValue = ((int_least8_t) pdmCell->u16MinValue);
			break;
		case DM_TYPE_UInt8:
			*pi32Value = *((uint_least8_t *)pdmCell->pValue);
			*pi32MaxValue = ((uint_least8_t) pdmCell->u16MaxValue);
			*pi32MinValue = ((uint_least8_t) pdmCell->u16MinValue);
			break;
		default:
			*pi32Value = *pi32MaxValue = *pi32MinValue = 0;

	}

	switch (pdmCell->u16Attribute & 0x00f0)
	{
		case DM_ATTRIBUTE_Point4:
			*pi32IncValue = 10000;
			break;
		case DM_ATTRIBUTE_Point3:
			*pi32IncValue = 1000;
			break;
		case DM_ATTRIBUTE_Point2:
			*pi32IncValue = 100;
			break;
		case DM_ATTRIBUTE_Point1:
			*pi32IncValue = 10;
			break;
		default:
			*pi32IncValue = 1;
			break;
	}
}

bool DM_saveToEEprom(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
    uint16_t u16Address = pdmCell->u16EEPromAddr;
    if (u16Address != I2C_ADDRESS_INVALID )
    {
        DM_Obj *pDmObj = (DM_Obj *)dmHandle;
        if ( EEPROM_WriteVerify(pDmObj->eepromHandle, u16Address, *((uint16_t *)pdmCell->pValue)) ==false)
        {
        	DM_setInverterTripType(dmHandle, TripType_EEProm);
        	DM_runEmergyStop(dmHandle);
        }
    }
    return true;
}

void DM_setValue(DM_Handle dmHandle,
	 	   	   	 uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
	 	   	   	 uint_least8_t u8FunIndex, uint_least8_t u8CellIndex,
	 	   	   	 int32_t i32Value)
{
	DM_FunCell *pdmFunCell = DM_getFunCell(dmHandle,u8ModeIndex, u8GroupIndex, u8FunIndex,  u8CellIndex);
	const DM_Cell *pdmCell = pdmFunCell->pdmCell;

	DM_TYPE_e dmType = getDMType(pdmCell);
	switch (dmType)
	{
		case DM_TYPE_Int16:
			*((int16_t *)pdmCell->pValue) = (int16_t) i32Value;
			break;
		case DM_TYPE_UInt16:
			*((uint16_t *)pdmCell->pValue)= (uint16_t) i32Value;
			break;
		case DM_TYPE_Int8:
			*((int_least8_t *)pdmCell->pValue) =  (int_least8_t)i32Value;
			break;
		case DM_TYPE_UInt8:
			*((uint_least8_t *)pdmCell->pValue) = (uint_least8_t)i32Value;

	}

	DM_saveToEEprom(dmHandle, pdmCell);
	if (pdmCell->setCallbackFunction != NULL)
		pdmCell->setCallbackFunction(dmHandle, pdmCell);


}

bool DM_isWrite(DM_Handle dmHandle,
			    uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
			    uint_least8_t u8FunIndex, uint_least8_t u8CellIndex)
{
	DM_FunCell *pdmFunCell = DM_getFunCell(dmHandle,u8ModeIndex, u8GroupIndex, u8FunIndex,  u8CellIndex);
	const DM_Cell *pdmCell = pdmFunCell->pdmCell;
	return  ((pdmCell->u16Attribute & DM_ATTRIBUTE_Write) != 0);
}

int32_t DM_convTripCodeToDescIndex(int32_t i32Code)
{
	if (i32Code == TripType_NoTrip) return 0;
	else if ( i32Code & TripType_EEProm  ) return 1;
	else if ( i32Code & TripType_IOExpander ) return 2;
	else if ( i32Code & TripType_UV) return 3;
	else if ( i32Code & TripType_OV) return 4;
	else if ( i32Code & TripType_OC) return 5;
	else if ( i32Code & TripType_OH) return 6;
	else if ( i32Code & TripType_Extern) return 7;
	return 8;
}


bool DM_outEditCell(DM_Handle dmHandle,
				   uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
				   uint_least8_t u8FunIndex, uint_least8_t u8CellIndex,
				   int32_t i32EditValue,
				   char *pai8Line1, char *pai8Line2)
{
	DM_Function *pdmFunction = DM_getFunction(dmHandle, u8ModeIndex, u8GroupIndex, u8FunIndex);
	DM_FunCell **pdmFunCellArray = (DM_FunCell **) pdmFunction->pdmFunCell;
	DM_FunCell *pdmFunCell = pdmFunCellArray[u8CellIndex];

	if (pdmFunCell->pai8DescArray != NULL)
	{
		uint_least8_t u8Value=  *((uint_least8_t *)pdmFunCell->pdmCell->pValue);
		const char *pi8Desc = pdmFunCell->pai8DescArray;
		if (pdmFunCell->pdmCell->u16Unit == DM_UNIT_TRIP)
		{
			sprintf(pai8Line1,"%s-%02u= %xh", pdmFunction->ai8FunSym, pdmFunCell->u8Index,(uint_least8_t) i32EditValue);
			i32EditValue = DM_convTripCodeToDescIndex(i32EditValue);
		}
		else
		{
			sprintf(pai8Line1,"%s-%02u= %u", pdmFunction->ai8FunSym, pdmFunCell->u8Index,(uint_least8_t) i32EditValue);
			if (u8Value == i32EditValue) strcat(pai8Line1, "**");

		}
		strcpy(pai8Line2, pi8Desc+i32EditValue*MAX_DESCRIPTION_LENGTH);
		return false;
	}
	else
	{

		strcpy(pai8Line1,pdmFunCell->ai8CellName);
		sprintf(pai8Line2,"%s %s", ConverterToEditValueString(i32EditValue, pdmFunCell->pdmCell), ConvertToUnit(pdmFunCell->pdmCell));
		return true;

	}

}

void DM_outFunCell(DM_Handle dmHandle,
				   uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
				   uint_least8_t u8FunIndex, uint_least8_t u8CellIndex,
				   char *pai8Line1, char *pai8Line2)
{

	DM_Function *pdmFunction = DM_getFunction(dmHandle, u8ModeIndex, u8GroupIndex, u8FunIndex);
	DM_FunCell **pdmFunCellArray = (DM_FunCell **) pdmFunction->pdmFunCell;
	DM_FunCell *pdmFunCell = pdmFunCellArray[u8CellIndex];

	//const DM_FunCell g_dmFunCellA1_01 	= {&g_dmCellLanguage,  0, "Select Language", &g_ai8DescLanguage[0][0]};
	strcpy(pai8Line1,pdmFunCell->ai8CellName);
	if (pdmFunCell->pai8DescArray != NULL)
	{
		const char *pi8Desc = pdmFunCell->pai8DescArray;
		uint_least8_t u8Value=  *((uint_least8_t *)pdmFunCell->pdmCell->pValue);
		if (pdmFunCell->pdmCell->u16Unit == DM_UNIT_TRIP)
		{
			//sprintf(pai8Line1,"%s-%02u= %xh", pdmFunction->ai8FunSym, pdmFunCell->u8Index,(uint_least8_t) i32EditValue);
			u8Value = DM_convTripCodeToDescIndex(u8Value);
		}



		strcpy(pai8Line2, pi8Desc+u8Value*MAX_DESCRIPTION_LENGTH);
		//const char g_ai8DescCtrlMethod[][MAX_DESCRIPTION_LENGTH] = {"V/f Control", "V/f with Pg", "Open Loop Vector","Flux Vector"};

	}
	else
	{
		sprintf(pai8Line2,"%s-%02u= %s%s",pdmFunction->ai8FunSym, pdmFunCell->u8Index,
			     ConverterToValueString(dmHandle, pdmFunCell->pdmCell), ConvertToUnit(pdmFunCell->pdmCell));
	}
}

void DM_outGroup(DM_Handle dmHandle,
				   uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex,
				   char *pai8Line1, char *pai8Line2)
{
	DM_Group *pdmGroup = DM_getGroup(dmHandle, u8ModeIndex, u8GroupIndex);
	sprintf(pai8Line1, "Group %s", pdmGroup->ai8GroupSym);
	strcpy(pai8Line2, pdmGroup->ai8GroupName);

}

void DM_outFunction(DM_Handle dmHandle,
				   uint_least8_t u8ModeIndex, uint_least8_t u8GroupIndex, uint_least8_t u8FunIndex,
				   char *pai8Line1, char *pai8Line2)
{

	DM_Function *pdmFunction = DM_getFunction(dmHandle, u8ModeIndex, u8GroupIndex, u8FunIndex);
	sprintf(pai8Line1, "Function %s", pdmFunction->ai8FunSym);
	strcpy(pai8Line2, pdmFunction->ai8FunName);
}
//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
_iq DM_getLocalFreqRef(DM_Handle dmHandle) // called by DM_runFreqRef()
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;

    //_iq iqHz_to_krpm_sf = _IQ(60.0/USER_MOTOR_NUM_POLE_PAIRS/1000/100); // g_i16FreqRef/100 (Point2)
    _iq iqHz_to_krpm_sf = _IQ(60.0/gUserParams.u16motor_numPolePairs/1000/100); // g_i16FreqRef/100 (Point2)

     return _IQmpyI32( iqHz_to_krpm_sf, (long) pdmObj->i16RefFreqHz);
}
//------------------------------------------------------------------------
_iq DM_getRemoteFreqRef(DM_Handle dmHandle)
{
	 DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	 _iq iqValue = _IQ(0.0f);
	 switch (pdmObj->refSelect)
	 {
	  	 case REF_DigialOperator:
	           iqValue =  DM_getLocalFreqRef(dmHandle);
	           break;
	     case REF_Terminal:
	           {
	        	   _iq Speed_pu = Throttle_getFilterResult(DM_getThrottleHandle(dmHandle));

	                if (!DM_TerminalStatus(dmHandle, DM_FOR_TERM1 ))	//Forward
	                    Speed_pu *= -1;
	                _iq Speed_pu_to_krpm_sf = EST_get_pu_to_krpm_sf(CTRL_getEstHandle(DM_getCtlHandle(dmHandle)));
	                iqValue  = _IQmpy(Speed_pu,Speed_pu_to_krpm_sf);
	                 break;
	           }
	      case REF_SerinalCommum:
	      case REF_OptionPCB:
	              break;

	}
	 return iqValue;

}
//------------------------------------------------------------------------
_iq DM_getLocalCurrentRef(DM_Handle dmHandle)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	_iq iqAmp_sf = _IQ(1/100.); // g_i16FreqRef/100 (Point2)
	return _IQmpyI32( iqAmp_sf, (long) pdmObj->i16RefCurrent);	//Amp
}
//------------------------------------------------------------------------
_iq DM_getRemoteCurrentRef(DM_Handle dmHandle)
{
	 DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	 _iq iqValue = _IQ(0.0f);
	 switch (pdmObj->refSelect)
	 {
	  	 case REF_DigialOperator:
	           iqValue =  DM_getLocalCurrentRef(dmHandle);
	           break;
	     case REF_Terminal:
	           {
	        	   _iq Current_pu = Throttle_getFilterResult(DM_getThrottleHandle(dmHandle));
	        	   	if (! DM_TerminalStatus(dmHandle,DM_FOR_TERM1 ))
	        	   		 Current_pu *= -1;
	        	   	_iq Current_pu_to_amp_sf =  _IQ(USER_IQ_FULL_SCALE_CURRENT_A);	//  EST_get_pu_to_krpm_sf(gpCtlObj->estHandle);
	        	   	iqValue  = _IQmpy(Current_pu,Current_pu_to_amp_sf);
	        	   	break;
	           }
	      case REF_SerinalCommum:
	      case REF_OptionPCB:
	              break;

	}
	 return iqValue;

}
//------------------------------------------------------------------------
void DM_runEmergyStop(DM_Handle dmHandle)
{
	DM_setStopStatus(dmHandle);
	DM_FreeRun(dmHandle);
}

//------------------------------------------------------------------------
void DM_runRefLimit(DM_Handle dmHandle)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	static uint_least8_t u8DCBusCheckCnt = 0;
	static uint_least8_t u8IOTerminalCnt = 0;

	if (++u8DCBusCheckCnt > 5)
	{
		uint16_t u16DCBus;
		u8DCBusCheckCnt =0;
		//u16DCBus = (uint16_t)  _IQmpyI32int(gAdcData.iqdcBus, USER_IQ_FULL_SCALE_VOLTAGE_V );
		u16DCBus = (uint16_t)  _IQmpyI32int(gAdcData.iqdcBus, gUserParams.fFullScaleVoltage_V);


		if (u16DCBus > pdmObj->u16DCBusMax)
		{
			DM_setInverterTripType(dmHandle, TripType_OV);
			DM_runEmergyStop(dmHandle);
		}
		else
			DM_clrInverterTripType(dmHandle, TripType_OV);


		if (u16DCBus < pdmObj->u16DCBusMin)
		{
			DM_setInverterTripType(dmHandle, TripType_UV);
			DM_runEmergyStop(dmHandle);
		}
		else
			DM_clrInverterTripType(dmHandle, TripType_UV);
	}


	/*if (u16DCBus < pdmObj->u16CurrentMax)
	{
		DM_setInverterTripType(dmHandle, TripType_OC);
		DM_runEmergyStop(dmHandle);
	}
	else
		DM_clrInverterTripType(dmHandle, TripType_OC);*/

	if (++u8IOTerminalCnt > 5)
	{
		u8IOTerminalCnt = 0;

		if (IOEXPAND_getTripStatus(DM_getIoexpandHandle(dmHandle)))
			DM_setInverterTripType(gdmHandle,TripType_EEProm );
		else
			DM_clrInverterTripType(gdmHandle,TripType_EEProm );

		if (DM_TerminalStatus(dmHandle, DM_TRIP_TERM3 ))		//External Fault
		{
			DM_setInverterTripType(gdmHandle,TripType_Extern );
			DM_runEmergyStop(dmHandle);
		}
		else
			DM_clrInverterTripType(gdmHandle,TripType_Extern );

	}




}
//------------------------------------------------------------------------
void DM_RampStop(DM_Handle dmHandle)
{
	gMotorVars.iqSpeedRef_krpm = 0;
	gMotorVars.iqIqRef_A = 0;
	if (DM_isFreqOutLowSpeed(dmHandle))
	{
		gMotorVars.bFlag_Run_Identify = false;
		gPwmData.Tabc.aiqValue[0]=gPwmData.Tabc.aiqValue[1]=gPwmData.Tabc.aiqValue[2] = 0;

	}
}
//------------------------------------------------------------------------
void DM_FreeRun(DM_Handle dmHandle)
{
	gMotorVars.iqSpeedRef_krpm = 0;
	gMotorVars.iqIqRef_A = 0;
	HAL_setGpioHigh(DM_getHalHandle(dmHandle), (GPIO_Number_e)HAL_Gpio_DriveEnable);
	//gMotorVars.bFlag_enableSys = gMotorVars.bFlag_Run_Identify = false;		//?? Process correct ?
	//if (DM_isFreqOutLowSpeed(dmHandle))
	{
		//HAL_disablePwm(DM_getHalHandle(dmHandle));
		gMotorVars.bFlag_enableSys = false;
		gPwmData.Tabc.aiqValue[0]=gPwmData.Tabc.aiqValue[1]=gPwmData.Tabc.aiqValue[2] = 0;


		//CTRL_setSpd_int_ref_pu(gCtrlHandle, _IQ(0.0));
	}
}
//------------------------------------------------------------------------
void DM_setFreqRef(DM_Handle dmHandle)  //, Throttle_Handle throttleHandle)
{
   // DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    _iq iqValue;
     if (DM_isLocalMode(dmHandle))
         iqValue =  DM_getLocalFreqRef(dmHandle);
     else
    	 iqValue = DM_getRemoteFreqRef(dmHandle);

     gMotorVars.iqSpeedRef_krpm = iqValue;
     gMotorVars.iqIqRef_A = _IQ(0.0);


}
//------------------------------------------------------------------------
void DM_setTorqueRef(DM_Handle dmHandle)
{
	 //DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	 _iq iqValue;
	 if (DM_isLocalMode(dmHandle))
	 	iqValue =   DM_getLocalCurrentRef(dmHandle);
	 else
		iqValue =   DM_getRemoteCurrentRef(dmHandle);
	 gMotorVars.iqSpeedRef_krpm = _IQ(0.0);
	 gMotorVars.iqIqRef_A = iqValue;

}
//------------------------------------------------------------------------
void DM_setInputRef(DM_Handle dmHandle)
{
	if(CTRL_getFlag_enableSpeedCtrl(DM_getCtlHandle(dmHandle)))
	{	//Speed control
		DM_setFreqRef(dmHandle);
	}
	else
	{	//Torque control
		DM_setTorqueRef(dmHandle);
	}


}
//------------------------------------------------------------------------
void DM_runOperation(DM_Handle dmHandle)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	if (DM_isAutotuneMode(dmHandle)) return;
	//if (DM_isInverterTrip(dmHandle)) return;

	if (!DM_isLocalMode(dmHandle))	// Check Run/Stop
	{

		switch(pdmObj->runSelect)
		{
			case REF_DigialOperator:
			          break;
			case REF_Terminal:
			{
			   if (DM_TerminalStatus(dmHandle, DM_FOR_TERM1 | DM_REV_TERM2) )
			       DM_setRunStatus(dmHandle);
			   else
			       DM_setStopStatus(dmHandle);
			   break;
			}
			case REF_SerinalCommum:
			case REF_OptionPCB:
			     break;
		}
	}

	if (DM_isRunStatus(dmHandle))
	{
		//static uint_least8_t u8DelayCnt;
	    HAL_setGpioLow(DM_getHalHandle(dmHandle), (GPIO_Number_e)HAL_Gpio_DriveEnable);
	    gMotorVars.bFlag_MotorIdentified = true;
    	gMotorVars.bFlag_enableUserParams = true;
	    if (gMotorVars.bFlag_enableSys)
	    {
	    	gMotorVars.bFlag_Run_Identify =true;	//Get Frequency
	    	DM_setInputRef(dmHandle);
	    }
	    else
	    {
	    	CTRL_setParams(DM_getCtlHandle(dmHandle),&gUserParams);
	    	gMotorVars.bFlag_enableSys = true;	// Start from Freerun
	    }
	}
	else
	{
	    //Stop status
	    switch (pdmObj->stopSelect)
	    {
	     	 case STOP_Ramp:
	     		DM_RampStop(dmHandle);
	     		break;
	         case STOP_Coast://Free Run
	        	 DM_FreeRun(dmHandle);
	             break;
	         case STOP_DCInjection:
	         case STOP_Timer:
	             break;
	    }
	}

}


//------------------------------------------------------------------------
void DM_toggleLocalRemoteMode(DM_Handle dmHandle)
{
    if (DM_isRunStatus(dmHandle) == false)  // Stop condition
    {
        DM_Obj *pdmObj = (DM_Obj *) dmHandle;
        pdmObj->bLocalMode ^= 1;
/*          if (pdmObj->bRemoteFlag)
          {

          }
          else
          {   // change from Remote to Local
              pdmObj->i16RefFreqHz = pdmObj->i16RefTorque = 0;


          }
  */
    }
}

//------------------------------------------------------------------------
/*void DM_outHardwareFaultName(DM_Handle dmHandle, char *pi8String)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    if (pdmObj->hardwareFaultType == FaultType_NoFault)
        strcpy(pi8String, "No Fault");
    else if (pdmObj->hardwareFaultType & FaultType_EEProm24LC32)
        strcpy(pi8String, "EEProm 24LC32");
    else if (pdmObj->hardwareFaultType & FaultType_IoExpander)
        strcpy(pi8String, "I/O MCP23017");
    else
        strcpy(pi8String, "Unknown");

}*/

//------------------------------------------------------------------------
void DM_outInverterTripName2(uint16_t u16TripType, char *pi8String)
{
    //DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    if (u16TripType == TripType_NoTrip )
        strcpy(pi8String, "No Trip");
    else if (u16TripType & TripType_EEProm )
    	strcpy(pi8String, "EEProm 24LC32");
    else if (u16TripType & TripType_IOExpander)
    	strcpy(pi8String, "I/O MCP23017");
    else if (u16TripType & TripType_UV )
        strcpy(pi8String, "Under Voltage");
    else if (u16TripType & TripType_OV )
        strcpy(pi8String, "Over Voltage");
    else if (u16TripType & TripType_OC )
        strcpy(pi8String, "Over Current");
    else if (u16TripType & TripType_OH )
         strcpy(pi8String, "Over Heat");
    else if (u16TripType & TripType_Extern )
            strcpy(pi8String, "Terminal #3");
    else
          strcpy(pi8String, "Unknown");

}

//------------------------------------------------------------------------
void DM_outInverterTripName1(uint16_t u16TripType, char *pi8String)
{
   // DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    if (u16TripType == TripType_NoTrip )
        strcpy(pi8String, "None");
    else if (u16TripType & TripType_EEProm )
      	strcpy(pi8String, "Hardware Fault");
    else if (u16TripType & TripType_IOExpander )
      	strcpy(pi8String, "Hardware Fault");
    else if (u16TripType & TripType_UV )
        strcpy(pi8String, "UV");
    else if (u16TripType & TripType_OV )
        strcpy(pi8String, "OV");
    else if (u16TripType & TripType_OC )
        strcpy(pi8String, "OC");
    else if (u16TripType & TripType_OH )
        strcpy(pi8String, "OH");
    else if (u16TripType & TripType_Extern )
         strcpy(pi8String, "Extern");
    else
        strcpy(pi8String, "Unknown");

}
//------------------------------------------------------------------------

EST_State_e DM_getAutotuneState(DM_Handle dmHandle)
{
	EST_State_e estState;
	estState = EST_getState(CTRL_getEstHandle(DM_getCtlHandle(dmHandle)));	//gpCtlObj->estHandle);
	return estState;

}
//------------------------------------------------------------------------

void DM_autotunePrepStart(DM_Handle dmHandle)
{
	//DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	//pdmObj->bAutotuneMode = true;

	gMotorVars.bFlag_enableOffsetcalc = true;
	gMotorVars.bFlag_enableUserParams = false;
	gMotorVars.bFlag_MotorIdentified = false;
	gMotorVars.bFlag_enableSys = false;
	gMotorVars.bFlag_Run_Identify = false;


}

void DM_autotuneStart(DM_Handle dmHandle)
{
	//HAL_setGpioLow(gHalHandle, (GPIO_Number_e)HAL_Gpio_DriveEnable);
	HAL_setGpioLow(DM_getHalHandle(dmHandle), (GPIO_Number_e)HAL_Gpio_DriveEnable);

	gMotorVars.bFlag_enableSys = true;
	gMotorVars.bFlag_Run_Identify = true;
}

void DM_autotuneEnd(DM_Handle dmHandle)
{
	//DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	gMotorVars.bFlag_enableOffsetcalc = false;
	gMotorVars.bFlag_enableSys = false;
	//gMotorVars.bFlag_Run_Identify = false;

	//pdmObj->bAutotuneMode = false;
}

bool DM_getRunIdentifySignal(DM_Handle dmHandle)
{
	return gMotorVars.bFlag_Run_Identify;
}

/*void DM_changeCtlLoop(DM_Handle dmHandle)
{
	DM_setStopStatus(dmHandle);
	gMotorVars.bFlag_enableSys = false;
}*/

void DM_setCallbackCtlLoop(DM_Handle dmHandle, const DM_Cell *pdmCell)		//XX.XX Hz
{
	//DM_changeCtlLoop(dmHandle);
	DM_runEmergyStop(dmHandle);
}

//------------------------------------------------------------------------
/*void DM_setCallbackFreqRef(DM_Handle dmHandle, const DM_Cell *pdmCell)		//XX.XX Hz
{
   // DM_runFreqRef(dmHandle);

}*/

//------------------------------------------------------------------------
/*void DM_setCallbackCurrentRef(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	//DM_runInputRef(dmHandle);
    DM_Obj *pdmObj = (DM_Obj *) handle;
    _iq iqHz_to_krpm_sf = _IQ(60.0/USER_MOTOR_NUM_POLE_PAIRS/1000/100); // g_i16FreqRef/100 (Point2)
    int16_t i16Value= *((int16_t *)pdmCell->pValue);

    gMotorVars.iqSpeedRef_krpm  =   _IQmpyI32( iqHz_to_krpm_sf, (long) i16Value);

}*/



/*void DM_setCallbackCtlMethod(DM_Handle handle, const DM_Cell *pdmCell)
{
    DM_setCtlMethod(handle, (CTL_METHOD_e) *((CTL_METHOD_e *)pdmCell->pValue));
}*/

void DM_setCallbackReferenceSel(DM_Handle handle, const DM_Cell *pdmCell)
{

}
//------------------------------------------------------------------------
//int32_t DM_getCallbackFreqRef(DM_Handle handle)
//{
//    return (int32_t) DM_getRefFreq(handle);
//}



/*int32_t DM_getCallbackReferenceSel(DM_Handle handle)
{
    return (int32_t) DM_getSelectionRef(handle);
}*/

/*int32_t DM_getCallbackCtlMethod(DM_Handle handle)
{
    //DM_Obj *pdmObj = (DM_Obj *) handle;
    return (int32_t) DM_getCtlMethod(handle);
}*/
//extern HAL_AdcData_t g_AdcData;
//------------------------------------------------------------------------
int32_t DM_getCallbackFreqOutHz(DM_Handle handle)	//XX.XX Hz Electric
{
	if (gMotorVars.bFlag_Run_Identify)
	{
	    DM_Obj *pdmObj = (DM_Obj *) handle;
	    _iq iqValue = FILTER_getValue(pdmObj->filterHandleOutFreq);
		//return (int32_t) _IQmpyI32int(iqValue, (long)(USER_IQ_FULL_SCALE_FREQ_Hz*100));
		return (int32_t) _IQmpyI32int(iqValue, gUserParams.fFullScaleFreq_Hz*100);
	}
	else
		return 0;

}

int32_t DM_ConvtoKrpm(DM_Handle dmHandle, _iq iqValuePu)
{
	_iq iqFreqOutKrpm = _IQmpy(iqValuePu, EST_get_pu_to_krpm_sf(CTRL_getEstHandle(DM_getCtlHandle(dmHandle))));	//gpCtlObj->estHandle));
	return (int32_t) _IQmpyI32int(iqFreqOutKrpm, 1000);
}

int32_t DM_getCallbackFreqOutRpm(DM_Handle handle)	//XXXX RPM
{
	if (gMotorVars.bFlag_Run_Identify)
	{
	    DM_Obj *pdmObj = (DM_Obj *) handle;
	    _iq iqValuePu = FILTER_getValue(pdmObj->filterHandleOutFreq);
	    return DM_ConvtoKrpm(handle, iqValuePu);
	}
	else
		return 0;

}

int32_t DM_getCallbackEnc1OutRpm(DM_Handle dmHandle)	//XXXX RPM
{
	if (gMotorVars.bFlag_Run_Identify)
	{
	    _iq iqValueKRPM = ENC_getSpeedKRPM(DM_getEncHandle(dmHandle));
	    return _IQmpyI32int(iqValueKRPM, 1000);	//DM_ConvtoKrpm(dmHandle, iqValuePu);
	}
	else
		return 0;

}

int32_t DM_getCallbackEnc2OutRpm(DM_Handle dmHandle)	//XXXX RPM
{
	return DM_getCallbackEnc1OutRpm(dmHandle);

}

int32_t DM_getCallbackHallOutRpm(DM_Handle dmHandle)	//XXXX RPM
{
	if (gMotorVars.bFlag_Run_Identify)
	{
	    _iq iqValuePu = HallBLDC_getSpeedPu(DM_getHallBLDCHandle(dmHandle));
	    return DM_ConvtoKrpm(dmHandle, iqValuePu);
	}
	else
		return 0;

}


int32_t DM_getCallbackCurrentOut(DM_Handle dmHandle)	//Point2
{
	if (gMotorVars.bFlag_Run_Identify)
	{
	    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	    _iq iqValue = FILTER_getValue(pdmObj->filterHandleOutCurrent);
		//return (int32_t) _IQmpyI32int(iqValue, (long) (100*USER_IQ_FULL_SCALE_CURRENT_A));
	    return (int32_t) _IQmpyI32int(iqValue, gUserParams.fFullScaleCurrent_A*100);


	}
	else
		return 0;
}

int32_t DM_getCallbackTorqueOut(DM_Handle dmHandle)
{
    if (gMotorVars.bFlag_Run_Identify)
    {
        return (int32_t) _IQmpyI32int(gMotorVars.iqTorque_Nm, (long) 100);
        /*DM_Obj *pdmObj = (DM_Obj *) handle;
        _iq iqValue = FILTER_getValue(pdmObj->filterHandleOutCurrent);
        return (int32_t) _IQmpyI32int(iqValue, (long) (100*USER_IQ_FULL_SCALE_CURRENT_A));*/

    }
    else
        return 0;
}

int32_t DM_getCallbackPowerOut(DM_Handle handle)
{
    if (gMotorVars.bFlag_Run_Identify)
    {
        _iq iqValue = _IQmpy(gMotorVars.iqTorque_Nm, gMotorVars.iqSpeedRef_krpm);
        return (int32_t) _IQmpyI32int(iqValue, (long) (2*3.14159*1000./60.));


    }
    else
        return 0;
}


int32_t DM_getCallbackDCBus(DM_Handle handle)
{
	//if (gMotorVars.bFlag_Run_Identify)
	//	return (int32_t) _IQmpyI32int(EST_getDcBus_pu(gpCtlObj->estHandle), USER_IQ_FULL_SCALE_VOLTAGE_V);
	//else

    DM_Obj *pdmObj = (DM_Obj *) handle;
    _iq iqValue = FILTER_getValue(pdmObj->filterHandleDCBus);
    return (int32_t) _IQmpyI32int(iqValue, gUserParams.fFullScaleVoltage_V*10);

}

int32_t DM_getCallbackVoltageU(DM_Handle handle)
{
    DM_Obj *pdmObj = (DM_Obj *) handle;
   //return (int32_t) _IQmpyI32int(pdmObj->iqOutVoltageU, USER_IQ_FULL_SCALE_VOLTAGE_V*10);
    return (int32_t) _IQmpyI32int(pdmObj->iqOutVoltageU, gUserParams.fFullScaleVoltage_V*10);
}

int32_t  DM_getCallbackVoltageV(DM_Handle handle)
{

    DM_Obj *pdmObj = (DM_Obj *) handle;
    return (int32_t) _IQmpyI32int(pdmObj->iqOutVoltageV,gUserParams.fFullScaleVoltage_V*10);// USER_IQ_FULL_SCALE_VOLTAGE_V*10);
}

int32_t  DM_getCallbackVoltageW(DM_Handle handle)
{
    DM_Obj *pdmObj = (DM_Obj *) handle;
    return (int32_t) _IQmpyI32int(pdmObj->iqOutVoltageW, gUserParams.fFullScaleVoltage_V*10);	//USER_IQ_FULL_SCALE_VOLTAGE_V*10);

}

int32_t DM_getCallbackCurrentU(DM_Handle handle)
{
    DM_Obj *pdmObj = (DM_Obj *) handle;
    return (int32_t) _IQmpyI32int(pdmObj->iqOutCurrentU, gUserParams.fFullScaleVoltage_V*10);	//USER_IQ_FULL_SCALE_CURRENT_A*100);
}

int32_t DM_getCallbackCurrentV(DM_Handle handle)
{
    DM_Obj *pdmObj = (DM_Obj *) handle;
    return (int32_t) _IQmpyI32int(pdmObj->iqOutCurrentV, gUserParams.fFullScaleVoltage_V*10);	//USER_IQ_FULL_SCALE_CURRENT_A*100);
}

int32_t DM_getCallbackCurrentW(DM_Handle dmHandle)
{
    DM_Obj *pdmObj = (DM_Obj *) dmHandle;
    return (int32_t) _IQmpyI32int(pdmObj->iqOutCurrentW, gUserParams.fFullScaleVoltage_V*10);	//USER_IQ_FULL_SCALE_CURRENT_A*100);
}

int32_t DM_getCallbackInputTerm(DM_Handle dmHandle)
{
	return (int32_t) IOEXPAND_InputTermA(DM_getIoexpandHandle(dmHandle));
}

/*int32_t DM_getCallbackOutputTerm(DM_Handle handle)
{
    //return (int32_t) IOEXPAND_OutputTerminals(gIoexpandHandle);
    //g_u8OutputTerm;
    //g_u8OutputTerm &= 0x000f;
    //IOEXPAND_setHigh(gIoexpandHandle,(g_u8OutputTerm << 4), I2C_ASYNC);

}*/

int32_t DM_getCallbackExtTemp(DM_Handle dmHandle)
{
	return (int32_t) _IQmpyI32int(gAdcData.iqExtTemp, (long)(USER_IQ_FULL_SCALE_EXT_TEMP_C*10));
}


int32_t DM_getCallbackExtAd1(DM_Handle dmHandle)
{
	return (int32_t) _IQmpyI32int(gAdcData.iqExtAdc[0], (long)(USER_IQ_FULL_SCALE_EXT_ADC_C*100));

}

int32_t DM_getCallbackExtAd2(DM_Handle dmHandle)
{
	return (int32_t) _IQmpyI32int(gAdcData.iqExtAdc[1], (long)(USER_IQ_FULL_SCALE_EXT_ADC_C*100));

}

int32_t DM_getCallbackExtAd3(DM_Handle handle)
{
	return (int32_t) _IQmpyI32int(gAdcData.iqExtAdc[2], (long)(USER_IQ_FULL_SCALE_EXT_ADC_C*100));

}

int32_t DM_getCallbackExtAd4(DM_Handle handle)
{
	return (int32_t) _IQmpyI32int(gAdcData.iqExtAdc[3], (long)(USER_IQ_FULL_SCALE_EXT_ADC_C*100));

}

int32_t DM_getCallbackExtAd5(DM_Handle handle)
{
	return (int32_t) _IQmpyI32int(gAdcData.iqExtAdc[4], (long)(USER_IQ_FULL_SCALE_EXT_ADC_C*100));

}

int32_t DM_getCallbackExtAd6(DM_Handle handle)
{
	return (int32_t) _IQmpyI32int(gAdcData.iqExtAdc[5], (long)(USER_IQ_FULL_SCALE_EXT_ADC_C*100));

}

int32_t DM_getCallbackVersion(DM_Handle handle)
{
	return (int32_t) VERSION;

}

/*int32_t DM_getCallbackAccTime(DM_Handle handle)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	// _iq iqValue = _IQ(pdmObj->u16AccTime/100.);
	return (int32_t) pdmObj->u16AccTime;
}*/

void DM_setCallbackAccTime(DM_Handle handle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	gMotorVars.iqMaxAccel_krpmps = _IQ(pdmObj->u16AccTime/100.);
}


void DM_setCallbackResEstCurrent(DM_Handle handle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	_iq iqValue = _IQ(pdmObj->i16ResEstCurrent/10.);
	gUserParams.fmaxCurrent_resEst = _IQtoF(iqValue);
}

void DM_setCallbackIndEstCurrent(DM_Handle handle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	_iq iqValue = _IQ(((int16_t)pdmObj->i16IndEstCurrent)/10.);
	gUserParams.fmaxCurrent_indEst = _IQtoF(iqValue);
}

void DM_setCallbackFluxEstHz(DM_Handle handle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	_iq iqValue = _IQ(pdmObj->i16FluxEstHz/10.);
	gUserParams.ffluxEstFreq_Hz = _IQtoF(iqValue);
}

void DM_setCallbackMaxCurrent(DM_Handle handle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	_iq iqValue = _IQ(pdmObj->i16MaxCurrent/10.);
	gUserParams.fmaxCurrent = _IQtoF(iqValue);
}

void DM_setCallbackResStator(DM_Handle handle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	_iq iqValue = _IQ(pdmObj->u16MotorRs/10000.);
	gUserParams.fmotor_Rs = _IQtoF(iqValue);
}

void DM_setCallbackResRotor(DM_Handle handle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	_iq iqValue = _IQ(pdmObj->u16MotorRr/10000.);
	gUserParams.fmotor_Rr = _IQtoF(iqValue);
}

void DM_setCallbackIndAxisD(DM_Handle handle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	_iq iqValue = _IQ(pdmObj->u16MotorLd/1000000.);
	gUserParams.fmotor_Ls_d = _IQtoF(iqValue);
}

void DM_setCallbackIndAxisQ(DM_Handle handle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	_iq iqValue = _IQ(pdmObj->u16MotorLq/1000000.);
	gUserParams.fmotor_Ls_q = _IQtoF(iqValue);
}


void DM_setCallbackRatedFlux(DM_Handle handle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	_iq iqValue = _IQ(pdmObj->u16RatedFlux/1000.);
	gUserParams.fmotor_ratedFlux = _IQtoF(iqValue);

}

void DM_setCallbackMagnetCur(DM_Handle handle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) handle;
	_iq iqValue = _IQ(pdmObj->u16MotorLq/10000.);
	gUserParams.fIdRated = _IQtoF(iqValue);
}


void DM_setCallbackKpSpeed(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	CTRL_Handle ctlHandle = DM_getCtlHandle(dmHandle);
	_iq iqValue = _IQ(pdmObj->u16Kp_Speed/1000.);

	gMotorVars.iqKp_spd = iqValue;
	//CTRL_setKp(ctlHandle,CTRL_Type_PID_spd,iqValue);
	//PID_setKp(gpCtlObj->pidHandle_spd ,iqValue);
}

void DM_setCallbackKiSpeed(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	CTRL_Handle ctlHandle = DM_getCtlHandle(dmHandle);
	_iq iqValue = _IQ(pdmObj->u16Ki_Speed/10000.);

	gMotorVars.iqKi_spd = iqValue;
	//CTRL_setKi(ctlHandle,CTRL_Type_PID_spd,iqValue);
	//PID_setKi(gpCtlObj->pidHandle_spd ,iqValue);
}

void DM_setCallbackKp_Id(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	CTRL_Handle ctlHandle = DM_getCtlHandle(dmHandle);
	_iq iqValue = _IQ(pdmObj->u16Kp_Id/1000.);

	gMotorVars.iqKp_Id = iqValue;
	//CTRL_setKp(ctlHandle,CTRL_Type_PID_Id,iqValue);
	//PID_setKp(gpCtlObj->pidHandle_Id ,iqValue);
}

void DM_setCallbackKi_Id(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	CTRL_Handle ctlHandle = DM_getCtlHandle(dmHandle);
	_iq iqValue = _IQ(pdmObj->u16Ki_Id/10000.);

	gMotorVars.iqKi_Id = iqValue;
	//CTRL_setKi(ctlHandle,CTRL_Type_PID_Id,iqValue);

	//PID_setKi(gpCtlObj->pidHandle_Id ,iqValue);
}

void DM_setCallbackKp_Iq(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	CTRL_Handle ctlHandle = DM_getCtlHandle(dmHandle);
	_iq iqValue = _IQ(pdmObj->u16Kp_Iq/1000.);

	gMotorVars.iqKp_Iq = iqValue;
	//CTRL_setKp(ctlHandle,CTRL_Type_PID_Iq,iqValue);
	//PID_setKp(gpCtlObj->pidHandle_Iq ,iqValue);
}

void DM_setCallbackKi_Iq(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	CTRL_Handle ctlHandle = DM_getCtlHandle(dmHandle);
	_iq iqValue = _IQ(pdmObj->u16Ki_Iq/10000.);

	gMotorVars.iqKi_Iq = iqValue;
	//CTRL_setKi(ctlHandle,CTRL_Type_PID_Iq,iqValue);
	//PID_setKi(gpCtlObj->pidHandle_Iq ,iqValue);
}

void DM_setCallbackKp_BLDC(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	HALLBLDC_Handle hallBLDCHandle = DM_getHallBLDCHandle(dmHandle);

	_iq iqValue = _IQ(pdmObj->u16Kp_BLDC/1000.);

	PID_setKp(HallBLDC_getPIDHandle(hallBLDCHandle),iqValue);
}

void DM_setCallbackKi_BLDC(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	HALLBLDC_Handle hallBLDCHandle = DM_getHallBLDCHandle(dmHandle);

	_iq iqValue = _IQ(pdmObj->u16Ki_BLDC/10000.);

	PID_setKi(HallBLDC_getPIDHandle( hallBLDCHandle) ,iqValue);
}

/*
void DM_setcallbackVBias1(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	HALLBLDC_Handle hallBLDCHandle = DM_getHallBLDCHandle(dmHandle);

	_iq iqValue = _IQ(pdmObj->u16VBias1/10000.);
	gMotorVars.V_bias.aiqValue[0] = iqValue;
}

void DM_setcallbackVBias2(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	HALLBLDC_Handle hallBLDCHandle = DM_getHallBLDCHandle(dmHandle);

	_iq iqValue = _IQ(pdmObj->u16VBias2/10000.);
	gMotorVars.V_bias.aiqValue[1] = iqValue;
}

void DM_setcallbackVBias3(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	HALLBLDC_Handle hallBLDCHandle = DM_getHallBLDCHandle(dmHandle);

	_iq iqValue = _IQ(pdmObj->u16VBias3/10000.);
	gMotorVars.V_bias.aiqValue[2] = iqValue;
}

void DM_setcallbackIBias1(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	HALLBLDC_Handle hallBLDCHandle = DM_getHallBLDCHandle(dmHandle);

	_iq iqValue = _IQ(pdmObj->u16IBias1/10000.);
	gMotorVars.I_bias.aiqValue[0] = iqValue;
}

void DM_setcallbackIBias2(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	HALLBLDC_Handle hallBLDCHandle = DM_getHallBLDCHandle(dmHandle);

	_iq iqValue = _IQ(pdmObj->u16IBias2/10000.);
	gMotorVars.I_bias.aiqValue[1] = iqValue;
}

void DM_setcallbackIBias3(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	HALLBLDC_Handle hallBLDCHandle = DM_getHallBLDCHandle(dmHandle);

	_iq iqValue = _IQ(pdmObj->u16IBias3/10000.);
	gMotorVars.I_bias.aiqValue[2] = iqValue;
}
*/

_iq DM_transSpeedPu(uint16_t u16SpeedRpm)
{
	//DM_Obj *pdmObj = (DM_Obj *) handle;
	float fValue =	u16SpeedRpm* gUserParams.u16motor_numPolePairs/(gUserParams.fFullScaleFreq_Hz*60.0);
	return _IQ(fValue);
}

void DM_setCallbackBLDCtoFOC(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;

    HallBLDC_setSpeedBldcToFastHighPu(DM_getHallBLDCHandle(dmHandle),  DM_transSpeedPu(pdmObj->u16BLDC2FOCSpeed));    //1200 rpm
}

void DM_setCallbackFOCtoBLDC(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	HallBLDC_setSpeedFasttoBldcLowPu(DM_getHallBLDCHandle(dmHandle), DM_transSpeedPu(pdmObj->u16FOC2BLDCSpeed));    //1200 rpm
}

void DM_calcPIgains(DM_Handle dmHandle)
{
	//DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	CTRL_Handle ctlHandle = DM_getCtlHandle(dmHandle);
	CTRL_Obj *pctlObj = (CTRL_Obj *) ctlHandle;

	float_t ffullScaleCurrent =  gUserParams.fFullScaleCurrent_A;	//    USER_IQ_FULL_SCALE_CURRENT_A;
	float_t ffullScaleVoltage = gUserParams.fFullScaleVoltage_V;	//  USER_IQ_FULL_SCALE_VOLTAGE_V;
	float_t fctrlPeriod_sec = CTRL_getCtrlPeriod_sec(ctlHandle);
	float_t fLs_d;
	float_t fLs_q;
	float_t fRs;
	float_t fRoverLs_d;
	float_t fRoverLs_q;

	_iq iqKp_Id;
	_iq iqKi_Id;
	_iq iqKp_Iq;
	_iq iqKi_Iq;
	_iq iqKp_Speed;
	_iq iqKi_Speed;


#ifdef __TMS320C28XX_FPU32__
	int32_t i32tmp;

	// when calling EST_ functions that return a float, and fpu32 is enabled, an integer is needed as a return
	// so that the compiler reads the returned value from the accumulator instead of fpu32 registers
	i32tmp = EST_getLs_d_H(pctlObj->estHandle);
	fLs_d = *((float_t *)&i32tmp);

	i32tmp = EST_getLs_q_H(pctlObj->estHandle);
	fLs_q = *((float_t *)&i32tmp);

	i32tmp = EST_getRs_Ohm(pctlObj->estHandle);
	fRs = *((float_t *)&i32tmp);
#else
	fLs_d = EST_getLs_d_H(pctlObj->estHandle);

	fLs_q = EST_getLs_q_H(pctlObj->estHandle);

	fRs = EST_getRs_Ohm(pctlObj->estHandle);
#endif

	fRoverLs_d = fRs/fLs_d;
	iqKp_Id = _IQ((0.25*fLs_d*ffullScaleCurrent)/(fctrlPeriod_sec*ffullScaleVoltage));
	iqKi_Id = _IQ(fRoverLs_d*fctrlPeriod_sec);

	fRoverLs_q = fRs/fLs_q;
	iqKp_Iq = _IQ((0.25*fLs_q*ffullScaleCurrent)/(fctrlPeriod_sec*ffullScaleVoltage));
	iqKi_Iq = _IQ(fRoverLs_q*fctrlPeriod_sec);


	iqKp_Speed = _IQ(0.02*gUserParams.fmaxCurrent*gUserParams.fFullScaleFreq_Hz/ffullScaleCurrent);
	iqKi_Speed = _IQ(2.0*gUserParams.fmaxCurrent*gUserParams.fFullScaleFreq_Hz*gUserParams.fctrlPeriod_sec/ffullScaleCurrent);



	gMotorVars.iqKp_Id = iqKp_Id;
	gMotorVars.iqKi_Id = iqKi_Id;
	gMotorVars.iqKp_Iq = iqKp_Iq;
	gMotorVars.iqKi_Iq = iqKi_Iq;

	gMotorVars.iqKp_spd = iqKp_Speed;
	gMotorVars.iqKi_spd = iqKi_Speed;

	DM_SavePIgains(dmHandle);



} // end of calcPIgains() function

void DM_SavePIgains(DM_Handle dmHandle)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	pdmObj->u16Kp_Id = (uint16_t) _IQmpyI32int(gMotorVars.iqKp_Id, 1000);
	pdmObj->u16Ki_Id = (uint16_t) _IQmpyI32int(gMotorVars.iqKi_Id, 10000);
	pdmObj->u16Kp_Iq = (uint16_t) _IQmpyI32int(gMotorVars.iqKp_Iq, 1000);
	pdmObj->u16Ki_Iq = (uint16_t) _IQmpyI32int(gMotorVars.iqKi_Iq, 10000);
	pdmObj->u16Kp_Speed = (uint16_t) _IQmpyI32int(gMotorVars.iqKp_spd, 1000);
	pdmObj->u16Ki_Speed = (uint16_t) _IQmpyI32int(gMotorVars.iqKi_spd, 10000);

		//_iq iqValue = _IQ(pdmObj->u16Ki_BLDC/1000.);

		//PID_setKi(HallBLDC_getPIDHandle( hallBLDCHandle) ,iqValue);


	DM_saveToEEprom(dmHandle, &g_dmCellKp_Speed);
	DM_saveToEEprom(dmHandle, &g_dmCellKi_Speed);
	DM_saveToEEprom(dmHandle, &g_dmCellKp_Id);
	DM_saveToEEprom(dmHandle, &g_dmCellKi_Id);
	DM_saveToEEprom(dmHandle, &g_dmCellKp_Iq);
	DM_saveToEEprom(dmHandle, &g_dmCellKi_Iq);
	DM_saveToEEprom(dmHandle, &g_dmCellKp_BLDC);
	DM_saveToEEprom(dmHandle, &g_dmCellKi_BLDC);
}

void DM_setCallbackMotorModel(DM_Handle dmHandle, const DM_Cell *pdmCell)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	static bool bFirstTimeFlag = true;
	if (bFirstTimeFlag)
	{
		bFirstTimeFlag = false;
		return;
	}

	DM_runEmergyStop(dmHandle);

	switch (pdmObj->u16ModelIndex)
	{
		case 0:	//BL129S25", , "Hub3000"}
			gUserParams.u16motor_numPolePairs = 4;
			gUserParams.motor_type = MOTOR_Type_Pm;

			pdmObj->i16MaxCurrent = 180;	//18A
			pdmObj->i16ResEstCurrent = 8;	//0.8A
			pdmObj->i16IndEstCurrent = -8;	//0.8A
			pdmObj->i16FluxEstHz = 200;

			pdmObj->u16AccTime = 20;	//0.2

			gMotorVars.fRs_Ohm = 0.29247;
			gMotorVars.fRr_Ohm = 0;
			gMotorVars.fLsd_H = 0.0003781;
		    gMotorVars.fLsq_H = 0.0003781;
		    gMotorVars.fFlux_VpHz = 0.11662;
		    gMotorVars.fMagnCurr_A = 0;
		    break;
		case 1: //" Delta C30604E"
			gUserParams.u16motor_numPolePairs = 4;
			gUserParams.motor_type = MOTOR_Type_Pm;

			pdmObj->i16MaxCurrent = 10;	//1A
			pdmObj->i16ResEstCurrent = 5;	//0.5A
			pdmObj->i16IndEstCurrent = -5;	//0.5A
			pdmObj->i16FluxEstHz = 200;		//20.0
			pdmObj->u16AccTime = 20;	//0.2

			gMotorVars.fRs_Ohm = 1.5843;
			gMotorVars.fRr_Ohm = 0;
			gMotorVars.fLsd_H = 0.006434;
			gMotorVars.fLsq_H = 0.006434;
			gMotorVars.fFlux_VpHz = 0.3268;
			gMotorVars.fMagnCurr_A = 0;
			break;
		case 2: //EMJ_04APB22
			gUserParams.u16motor_numPolePairs = 2;
			gUserParams.motor_type = MOTOR_Type_Pm;

			pdmObj->i16MaxCurrent = 20;	//2A
			pdmObj->i16ResEstCurrent = 8;	//0.8A
			pdmObj->i16IndEstCurrent = -8;	//0.8A
			pdmObj->i16FluxEstHz = 200;
			pdmObj->u16AccTime = 20;	//0.2

			gMotorVars.fRs_Ohm = 2.2002;
			gMotorVars.fRr_Ohm = 0;
			gMotorVars.fLsd_H = 0.008721;
			gMotorVars.fLsq_H = 0.008721;
			gMotorVars.fFlux_VpHz = 0.384698;
			gMotorVars.fMagnCurr_A = 0;
			break;

		case 3: // QSmotor¡Ä¢²¢Ù¢å
			gUserParams.u16motor_numPolePairs = 16;
			gUserParams.motor_type = MOTOR_Type_Pm;

			pdmObj->i16MaxCurrent = 80;	//2A
			pdmObj->i16ResEstCurrent = 25;	//0.8A
			pdmObj->i16IndEstCurrent = -25;	//0.8A
			pdmObj->i16FluxEstHz = 100;
			pdmObj->u16AccTime = 2;	//0.02

			gMotorVars.fRs_Ohm = 0.0193886;
			gMotorVars.fRr_Ohm = 0;
			gMotorVars.fLsd_H = 4.8015e-5;
			gMotorVars.fLsq_H = 4.8015e-5;
			gMotorVars.fFlux_VpHz = 0.20511;
			gMotorVars.fMagnCurr_A = 0;
			break;
		case 4: // QSmotor¡Ä1.5¢Ù¢å
			gUserParams.u16motor_numPolePairs = 16;
			gUserParams.motor_type = MOTOR_Type_Pm;

			pdmObj->i16MaxCurrent = 80;	//2A
			pdmObj->i16ResEstCurrent = 25;	//0.8A
			pdmObj->i16IndEstCurrent = -25;	//0.8A
			pdmObj->i16FluxEstHz = 100;
			pdmObj->u16AccTime = 2;	//0.02

			gMotorVars.fRs_Ohm = 0.0267187;
			gMotorVars.fRr_Ohm = 0;
			gMotorVars.fLsd_H = 8.64771e-6;
			gMotorVars.fLsq_H = 8.64771e-6;
			gMotorVars.fFlux_VpHz = 0.113901;
			gMotorVars.fMagnCurr_A = 0;
			break;
		case 5: //ADLEDD 760W
			gUserParams.u16motor_numPolePairs = 4;
			gUserParams.motor_type = MOTOR_Type_Pm;

			pdmObj->i16MaxCurrent = 40;	//4A
			pdmObj->i16ResEstCurrent = 8;	//0.8A
			pdmObj->i16IndEstCurrent = -8;	//0.8A
			pdmObj->i16FluxEstHz = 200;
			pdmObj->u16AccTime = 20;	//0.02

			gMotorVars.fRs_Ohm = 0.70242;
			gMotorVars.fRr_Ohm = 0;
			gMotorVars.fLsd_H = 0.003947;
			gMotorVars.fLsq_H = 0.003947;
			gMotorVars.fFlux_VpHz = 0.57421;
			gMotorVars.fMagnCurr_A = 0;
			break;

	}


	DM_saveToEEprom(dmHandle, &g_dmCellPolePair);
	DM_saveToEEprom(dmHandle, &g_dmCellMotorType);
	DM_saveToEEprom(dmHandle, &g_dmCellAccTime);

	DM_saveToEEprom(dmHandle, &g_dmCellMaxCurrent);

	DM_saveToEEprom(dmHandle, &g_dmCellResEstCurrent);
	DM_saveToEEprom(dmHandle, &g_dmCellIndEstCurrent);
	DM_saveToEEprom(dmHandle, &g_dmCellFluxEstHz);

	DM_setCallbackAccTime(dmHandle, &g_dmCellAccTime);
	DM_setCallbackMaxCurrent(dmHandle,&g_dmCellMaxCurrent);
	DM_setCallbackResEstCurrent(dmHandle,&g_dmCellResEstCurrent);
	DM_setCallbackIndEstCurrent(dmHandle,&g_dmCellIndEstCurrent);
	DM_setCallbackFluxEstHz(dmHandle, &g_dmCellFluxEstHz);



	DM_saveMotorParameters(dmHandle);
	DM_calcPIgains(dmHandle);

}





void DM_saveBiasParameters(DM_Handle dmHandle)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	pdmObj->u16VBias1 = (uint16_t) _IQmpyI32int(gMotorVars.V_bias.aiqValue[0], 10000);
	pdmObj->u16VBias2 = (uint16_t) _IQmpyI32int(gMotorVars.V_bias.aiqValue[1], 10000);
	pdmObj->u16VBias3 = (uint16_t) _IQmpyI32int(gMotorVars.V_bias.aiqValue[2], 10000);

	pdmObj->u16IBias1 = (uint16_t) _IQmpyI32int(gMotorVars.I_bias.aiqValue[0], 10000);
	pdmObj->u16IBias2 = (uint16_t) _IQmpyI32int(gMotorVars.I_bias.aiqValue[1], 10000);
	pdmObj->u16IBias3 = (uint16_t) _IQmpyI32int(gMotorVars.I_bias.aiqValue[2], 10000);



	DM_saveToEEprom(dmHandle, &g_dmCellVBias1  );
	DM_saveToEEprom(dmHandle, &g_dmCellVBias2  );
	DM_saveToEEprom(dmHandle, &g_dmCellVBias3  );
	DM_saveToEEprom(dmHandle, &g_dmCellIBias1  );
	DM_saveToEEprom(dmHandle, &g_dmCellIBias2  );
	DM_saveToEEprom(dmHandle, &g_dmCellIBias3  );



}

void DM_saveMotorParameters(DM_Handle dmHandle)
{
	DM_Obj *pdmObj = (DM_Obj *) dmHandle;
	pdmObj->u16MotorRs = gMotorVars.fRs_Ohm  *10000;
	pdmObj->u16MotorRr = gMotorVars.fRr_Ohm *10000;
	pdmObj->u16MotorLd = gMotorVars.fLsd_H *1000000;
	pdmObj->u16MotorLq = gMotorVars.fLsq_H *1000000;
	pdmObj->u16RatedFlux = gMotorVars.fFlux_VpHz *1000;
	pdmObj->u16MagnetCur = gMotorVars.fMagnCurr_A *10000;

	gUserParams.fmotor_Rs = gMotorVars.fRs_Ohm;
	gUserParams.fmotor_Rr = gMotorVars.fRr_Ohm;
	gUserParams.fmotor_Ls_d = gMotorVars.fLsd_H;
	gUserParams.fmotor_Ls_q = gMotorVars.fLsq_H;
	gUserParams.fmotor_ratedFlux = gMotorVars.fFlux_VpHz;
	gUserParams.fIdRated =  gMotorVars.fMagnCurr_A;

	DM_saveToEEprom(dmHandle, &g_dmCellResStator);
	DM_saveToEEprom(dmHandle, &g_dmCellResRotor);
	DM_saveToEEprom(dmHandle, &g_dmCellIndAxisD);
	DM_saveToEEprom(dmHandle, &g_dmCellIndAxisQ);
	DM_saveToEEprom(dmHandle, &g_dmCellRatedFlux);
	DM_saveToEEprom(dmHandle, &g_dmCellMagnetCur);



	DM_saveBiasParameters(dmHandle);

	gMotorVars.bFlag_enableSys = false;

}

