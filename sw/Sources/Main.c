/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, LineStream Technologies Incorporated
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
* *  Neither the names of Texas Instruments Incorporated, LineStream
 *    Technologies Incorporated, nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
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
//! \file   solutions/instaspin_motion/src/proj_lab12b.c
//! \brief  SpinTAC Velocity Controller using a quadrature encoder for feedback
//!
//! (C) Copyright 2012, LineStream Technologies, Inc.
//! (C) Copyright 2011, Texas Instruments, Inc.

//! \defgroup PROJ_LAB12b PROJ_LAB12b
//@{

//! \defgroup PROJ_LAB12b_OVERVIEW Project Overview
//!
//! SpinTAC Velocity Controller using a quadrature encoder for feedback
//!

// **************************************************************************
// the includes

// system includes
#include <math.h>
#include <stdio.h>



#include "include/main.h"
#include "include/user_Data.h"

#include "sw/modules/datalog/src/32b/datalog.h"
#include "sw/modules/hallbldc/src/32b/hallbldc.h"
#include "sw/modules/throttle/src/32b/throttle.h"

#ifdef FLASH
   #pragma CODE_SECTION(adcISR,"ramfuncs");
#endif

// Include header files used in the main function


// **************************************************************************
// the defines

//#define LED_BLINK_FREQ_Hz   5
//#define PWM_TEST

//#define HIGH_INERTIA
#define ENCODER_MODULE
#define HALL_BLDC


#define HAL_Gpio_Relay 			GPIO_Number_58
#define HAL_Gpio_DriveEnable 	GPIO_Number_24

// **************************************************************************
// the globals

uint_least16_t gu16Counter_updateGlobals = 0;

bool bFlag_Latch_softwareUpdate = true;



USER_Params gUserParams;

HAL_PwmData_t gPwmData = {_IQ(0.0), _IQ(0.0), _IQ(0.0)};
HAL_AdcData_t gAdcData;

_iq giqMaxCurrentSlope = _IQ(0.0);

#ifdef FAST_ROM_V1p6
CTRL_Obj *gpCtlObj;
#else
CTRL_Obj ctrl;				//v1p7 format
#endif

CTRL_Handle 		gCtrlHandle;
HAL_Handle 			gHalHandle;


SCIMessage_Handle   gSciMessageHandle[2];
SCI_Message			gSciMessage[2];

MODBUS_Handle       gModbusHandle;
MODBUS_Obj          gModbusObj;


I2CMessage_Handle   gI2cMessageHandle;
I2C_Message         gI2cMessage;

EEPROM_Handle       gEepromHandle;
EEPROM24LC32_Obj    gEepromObj;


IOEXPAND23017_Obj 	gIoexpandObj;
IOEXPAND_Handle     gIoexpandHandle;

//LCD_Handle            gLcdHandle;
//LCD1602_Obj			gLcdObj;

OPERATOR_Handle     gOperHandle;
OPERATOR_Obj		gOperObj;

ENC_Obj             gEncObj;
ENC_Handle          gEncHandle;

HALLBLDC_Obj        gHallBLDCObj;
HALLBLDC_Handle     gHallBLDCHandle;


Throttle_Obj        gThrottleObj;
Throttle_Handle     gThrottleHandle;


//SLIP_Handle slipHandle;
//SLIP_Obj slip;

//ST_Obj st_obj;
//ST_Handle stHandle;

//uint16_t gLEDcnt = 0;

volatile MOTOR_Vars_t gMotorVars = MOTOR_Vars_INIT;

#ifdef FLASH
// Used for running BackGround in flash, and ISR in RAM
extern uint16_t *RamfuncsLoadStart, *RamfuncsLoadEnd, *RamfuncsRunStart;
#endif

#ifndef F2802xF
HAL_DacData_t gDacData;
#endif

#ifdef DRV8301_SPI
// Watch window interface to the 8301 SPI
DRV_SPI_8301_Vars_t gDrvSpi8301Vars;
#endif

_iq giqFlux_pu_to_Wb_sf;
_iq giqFlux_pu_to_VpHz_sf;
_iq giqTorque_Ls_Id_Iq_pu_to_Nm_sf;
_iq giqTorque_Flux_Iq_pu_to_Nm_sf;

// **************************************************************************
// the functions

void main(void)
{
	//uint_least8_t u8estNumber = 0;

#ifdef FAST_ROM_V1p6
	//uint_least8_t u8ctrlNumber = 0;
#endif

	// Only used if running from FLASH
	// Note that the variable FLASH is defined by the project
#ifdef FLASH
	// Copy time critical code and Flash setup code to RAM
	// The RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
	// symbols are created by the linker. Refer to the linker files.
	memCopy((uint16_t *)&RamfuncsLoadStart,(uint16_t *)&RamfuncsLoadEnd,(uint16_t *)&RamfuncsRunStart);
#endif

	gHalHandle = HAL_init(&ghal,sizeof(ghal));      // initialize the hardware abstraction layer

	gEncHandle = ENC_init(&gEncObj, sizeof(gEncObj));   // initialize the ENC module

	gI2cMessageHandle= I2CMessage_init(&gI2cMessage, sizeof(gI2cMessage));
	gSciMessageHandle[0] = SCIMessage_init(&gSciMessage[0], sizeof(gSciMessage[0]));
	gSciMessageHandle[1] = SCIMessage_init(&gSciMessage[1], sizeof(gSciMessage[1]));
	gOperHandle = OPERATOR_init(&gOperObj, sizeof(gOperObj));
	gModbusHandle = MODBUS_init(&gModbusObj, sizeof(gModbusObj));
 	gEepromHandle = EEPROM_init(&gEepromObj, sizeof(gEepromObj));
  	gIoexpandHandle = IOEXPAND_init(&gIoexpandObj, sizeof(gIoexpandObj));
  	//gLcdHandle =LCD_init(&gLcdObj, sizeof(gLcdObj));
	gdmHandle = DM_init(&gdmObj,sizeof(gdmObj));

	USER_checkForErrors(&gUserParams);	// check for errors in user parameters

	gMotorVars.UserErrorCode = USER_getErrorCode(&gUserParams); // store user parameter error in global variable

	// do not allow code execution if there is a user parameter error
	if(gMotorVars.UserErrorCode != USER_ErrorCode_NoError)
    {
		for(;;)
        {
			gMotorVars.bFlag_enableSys = false;
        }
    }

	// initialize the user parameters
	USER_setParams(&gUserParams);

	// set the hardware abstraction layer parameters
	HAL_setParams(gHalHandle,&gUserParams);

	// initialize the controller
#ifdef FAST_ROM_V1p6
	//gCtrlHandle = CTRL_initCtrl(u8ctrlNumber, u8estNumber);  		//v1p6 format (06xF and 06xM devices) Org
	gCtrlHandle = CTRL_initCtrl(0, 0);
	gpCtlObj = (CTRL_Obj *)gCtrlHandle;
#else
	ctrlHandle = CTRL_initCtrl(estNumber,&ctrl,sizeof(ctrl));	//v1p7 format default
#endif

	{
		CTRL_Version version;
		// get the version number
		CTRL_getVersion(gCtrlHandle,&version);
		gMotorVars.CtrlVersion = version;

		//CTRL_getVersion(gCtrlHandle,&gMotorVars.CtrlVersion);
	}

	// set the default controller parameters
	//CTRL_setParams(gCtrlHandle,&gUserParams);

	// setup the ENC module
	ENC_setup(gEncHandle, 100, USER_MOTOR_NUM_POLE_PAIRS, USER_MOTOR_ENCODER_LINES, 0, USER_IQ_FULL_SCALE_FREQ_Hz, USER_ISR_FREQ_Hz, 8000.0); //Org

	I2CMessage_setup(gI2cMessageHandle,((HAL_Obj *)gHalHandle)->i2cHandle);
  	SCIMessage_setup(gSciMessageHandle[0], ((HAL_Obj *)gHalHandle)->sciHandle[0]);
  	SCIMessage_setup(gSciMessageHandle[1], ((HAL_Obj *)gHalHandle)->sciHandle[1]);
	OPERATOR_setup(gOperHandle, gdmHandle);
  	MODBUS_setup(gModbusHandle);
	EEPROM_setup(gEepromHandle,  ((HAL_Obj *)gHalHandle)->gpioHandle, gI2cMessageHandle);


	gHallBLDCHandle = HALLBLDC_init(&gHallBLDCObj,sizeof(gHallBLDCObj));   // Initialize BLDC Hall
	{
	   // _iq maxCurrent_pu = _IQ(USER_MOTOR_MAX_CURRENT / USER_IQ_FULL_SCALE_CURRENT_A);
	   // _iq maxVoltage_pu = _IQ(USER_MAX_VS_MAG_PU * USER_VD_SF);
	    float_t fullScaleCurrent = USER_IQ_FULL_SCALE_CURRENT_A;
	    float_t fullScaleVoltage = USER_IQ_FULL_SCALE_VOLTAGE_V;
	    float_t IsrPeriod_sec = 1.0 / USER_ISR_FREQ_Hz;
	    //float_t Ls_d = USER_MOTOR_Ls_d;
	    float_t Ls_q = USER_MOTOR_Ls_q;
	    float_t Rs = USER_MOTOR_Rs;
	    //float_t RoverLs_d = Rs/Ls_d;
	    float_t RoverLs_q = Rs/Ls_q;
	    _iq iqKp_Iq = _IQ((0.25*Ls_q*fullScaleCurrent)/(IsrPeriod_sec*fullScaleVoltage));
	    _iq iqKi_Iq = _IQ(RoverLs_q*IsrPeriod_sec);


	    HallBLDC_setSpeedFasttoBldcLowPu(gHallBLDCHandle,  _IQ(0.800*USER_MOTOR_NUM_POLE_PAIRS*1000.0/(USER_IQ_FULL_SCALE_FREQ_Hz * 60.0))); //800 rpm
	    HallBLDC_setSpeedBldcToFastHighPu(gHallBLDCHandle,  _IQ(1.20*USER_MOTOR_NUM_POLE_PAIRS*1000.0/(USER_IQ_FULL_SCALE_FREQ_Hz * 60.0)));    //1200 rpm
	    HallBLDC_setHallSpeedScale(gHallBLDCHandle, USER_SYSTEM_FREQ_MHz*1000000*100/6);
	    HallBLDC_setSpeed0p01hzToPuSf(gHallBLDCHandle, _IQ((float_t)0.01/(USER_IQ_FULL_SCALE_FREQ_Hz)));
	    HallBLDC_setPwmCntMax(gHallBLDCHandle, 5000);

	    PID_Handle pidHandle = HallBLDC_getPIDHandle(gHallBLDCHandle);
	    PID_setGains(pidHandle,iqKp_Iq,iqKi_Iq,_IQ(0.0));
	    PID_setMinMax(pidHandle,_IQ(-1.0),_IQ(1.0));
	    PID_setUi(pidHandle,_IQ(0.0));

	    HallBLDC_setEnableStartup(gHallBLDCHandle, false);
	    HallBLDC_setEnableBldc(gHallBLDCHandle, true);
	}

	gThrottleHandle = Throttle_init(&gThrottleObj,sizeof(gThrottleObj));
	{
	    Throttle_setParams(gThrottleHandle, false,_IQ(USER_EXT_ADC_SF), _IQ(0.1), _IQ(1.0), _IQ(0.0));
	}

#ifndef F2802xF
  // set DAC parameters

//  gDacData.ptrData[0] = &gPwmData.Tabc.value[0];
//  gDacData.ptrData[1] = &gPwmData.Tabc.value[1];
//  gDacData.ptrData[2] = &gPwmData.Tabc.value[2];
//  gDacData.ptrData[3] = &gAdcData.V.value[0];

//	gDacData.api32Data[0] = &gPwmData.Tabc.aiqValue[1];
//	gDacData.api32Data[1] = &gAdcData.I.aiqValue[0];
//	gDacData.api32Data[2] = &gPwmData.Tabc.aiqValue[0];
//	gDacData.api32Data[3] = &gAdcData.V.aiqValue[0];

	gDacData.api32Data[0] = &gPwmData.Tabc.aiqValue[0];
	gDacData.api32Data[1] = &gPwmData.Tabc.aiqValue[1];
	gDacData.api32Data[2] = &gPwmData.Tabc.aiqValue[2];
	gDacData.api32Data[3] = &gPwmData.Tabc.aiqValue[0];


    HAL_setDacParameters(gHalHandle, &gDacData);
#endif

#ifndef F2802xF
  // Initialize Datalog
    gDatalogHandle = DATALOG_init(&gDatalog,sizeof(gDatalog));

  // Connect inputs of the datalog module
    gDatalog.api32Ptr[0] = &gPwmData.Tabc.aiqValue[0];      // datalogBuff[0]
    gDatalog.api32Ptr[1] = &gPwmData.Tabc.aiqValue[1];      // datalogBuff[1]
    gDatalog.api32Ptr[2] = &gPwmData.Tabc.aiqValue[2];      // datalogBuff[2]

   // gDatalog.api32Ptr[0] = &gAdcData.I.aiqValue[0];        // datalogBuff[0]
   // gDatalog.api32Ptr[1] = &gAdcData.I.aiqValue[0];       // datalogBuff[1]
   // gDatalog.api32Ptr[2] = &gAdcData.V.aiqValue[0];       // datalogBuff[2]

    gDatalog.bFlag_EnableLogData = true;
    gDatalog.bFlag_EnableLogOneShot = false;
#endif


	HAL_setupFaults(gHalHandle);
	HAL_initIntVectorTable(gHalHandle);

	HAL_enableAdcInts(gHalHandle);
	HAL_enableTimer0Int(gHalHandle);
	HAL_enableTimer1Int(gHalHandle);
	HAL_enableSciAInt(gHalHandle);
  	HAL_enableSciBInt(gHalHandle);
  	HAL_enableI2cInt(gHalHandle);

	HAL_enableGlobalInts(gHalHandle);
	HAL_enableDebugInt(gHalHandle);


	HAL_disablePwm(gHalHandle);

//	LCD_setup(gLcdHandle, gI2cMessageHandle);		//I2C interrupt must turn on
	{
		DM_setCtlHandle(gdmHandle, gCtrlHandle);
		DM_setHalHandle(gdmHandle, gHalHandle);
		DM_setThrottleHandle(gdmHandle, gThrottleHandle);
		DM_setEndHandle(gdmHandle, gEncHandle);
		DM_setIoexpandHandle(gdmHandle, gIoexpandHandle);
		DM_setHallBLDCHandle(gdmHandle, gHallBLDCHandle);

		DM_setup(gdmHandle,gEepromHandle );
	}
    IOEXPAND_setup(gIoexpandHandle,gI2cMessageHandle);

    // set the default controller parameters
    CTRL_setParams(gCtrlHandle,&gUserParams);	//Reload Parameters (called from EEProm)

/* (Org)
  // initialize the SLIP module
  slipHandle = SLIP_init(&slip, sizeof(slip));


  // setup the SLIP module
  SLIP_setup(slipHandle, _IQ(gUserParams.ctrlPeriod_sec));


  // initialize the SpinTAC Components
  stHandle = ST_init(&st_obj, sizeof(st_obj));
  
  
  // setup the SpinTAC Components
  ST_setupVelCtl(stHandle);
  ST_setupPosConv(stHandle);
*/

	HAL_setGpioHigh(gHalHandle, (GPIO_Number_e)HAL_Gpio_Relay);	//  Adding Turn on standby relay
	HAL_setGpioLow(gHalHandle, (GPIO_Number_e)HAL_Gpio_DriveEnable);


#ifdef DRV8301_SPI
	// turn on the DRV8301 if present
	HAL_enableDrv(gHalHandle);
	// initialize the DRV8301 interface
 	HAL_setupDrvSpi(gHalHandle,&gDrvSpi8301Vars);
#endif


#ifdef HIGH_INERTIA
 	gMotorVars.iqMaxAccel_krpmps /= 10;

#endif


 	//HAL_setupDrvSpi
 	// enable DC bus compensation
	CTRL_setFlag_enableDcBusComp(gCtrlHandle, true);

 	// compute scaling factors for flux and torque calculations
 	giqFlux_pu_to_Wb_sf = USER_computeFlux_pu_to_Wb_sf();
 	giqFlux_pu_to_VpHz_sf = USER_computeFlux_pu_to_VpHz_sf();
 	giqTorque_Ls_Id_Iq_pu_to_Nm_sf = USER_computeTorque_Ls_Id_Iq_pu_to_Nm_sf();

 	giqTorque_Flux_Iq_pu_to_Nm_sf = USER_computeTorque_Flux_Iq_pu_to_Nm_sf();



#ifdef PWM_TEST
 	HAL_enablePwm(gHalHandle);
 	for (; ;)
 	{

 	}

#endif

 	for(;;)
 	{
	  while(!(gMotorVars.bFlag_enableSys));
	  HAL_enablePwm(gHalHandle);

	  {
		  CTL_LOOP_e cltLoop = DM_getCtlLoop(gdmHandle);
		  bool bResult = !(cltLoop == CTL_Current_Loop);
		  CTRL_setFlag_enableSpeedCtrl(gCtrlHandle, bResult);
	  }

	  {
		  CTL_METHOD_e ctlMethod = DM_getCtlMethod(gdmHandle);
		  HallBLDC_setEnableStartup(gHallBLDCHandle, (ctlMethod ==CTL_MIX_BLDC));
		 /* if (ctlMethod ==CTL_MIX_BLDC)

		  else
			  HallBLDC_setEnableStartup(gHallBLDCHandle, false);*/
	  }

	  // loop while the enable system flag is true
	  while(gMotorVars.bFlag_enableSys)
      {
		  CTRL_Obj *pobj = (CTRL_Obj *)gCtrlHandle;
		  //ST_Obj *stObj = (ST_Obj *)stHandle;

		  gu16Counter_updateGlobals++;
		  CTRL_setFlag_enableUserMotorParams(gCtrlHandle,gMotorVars.bFlag_enableUserParams);
		  EST_setFlag_enableRsRecalc(pobj->estHandle,gMotorVars.bFlag_enableRsRecalc);
		  CTRL_setFlag_enableOffset(gCtrlHandle,gMotorVars.bFlag_enableOffsetcalc);

		  if(CTRL_isError(gCtrlHandle))
          {
			  CTRL_setFlag_enableCtrl(gCtrlHandle,false);
			  gMotorVars.bFlag_enableSys = false;
			  HAL_disablePwm(gHalHandle);
          }
		  else
          {
			  // update the controller state
			  bool bflag_ctrlStateChanged = CTRL_updateState(gCtrlHandle);
			  CTRL_setFlag_enableCtrl(gCtrlHandle, gMotorVars.bFlag_Run_Identify);

			  if (bflag_ctrlStateChanged)
              {
				  CTRL_State_e ctrlState = CTRL_getState(gCtrlHandle);

				  if(ctrlState == CTRL_State_OffLine)
                  {
					  HAL_enablePwm(gHalHandle);
                  }
				  else if(ctrlState == CTRL_State_OnLine)
                  {
					  if(gMotorVars.bFlag_enableOffsetcalc == true)
					  {
						  HAL_updateAdcBias(gHalHandle);
					  }
					  else
					  {
						  // set the current bias
						 /* HAL_setBias(gHalHandle,HAL_SensorType_Current,0,_IQ(I_A_offset));
						  HAL_setBias(gHalHandle,HAL_SensorType_Current,1,_IQ(I_B_offset));
						  HAL_setBias(gHalHandle,HAL_SensorType_Current,2,_IQ(I_C_offset));*/

						  // set the voltage bias
						 /* HAL_setBias(gHalHandle,HAL_SensorType_Voltage,0,_IQ(V_A_offset));
						  HAL_setBias(gHalHandle,HAL_SensorType_Voltage,1,_IQ(V_B_offset));
						  HAL_setBias(gHalHandle,HAL_SensorType_Voltage,2,_IQ(V_C_offset));*/

						  HAL_setBias(gHalHandle,HAL_SensorType_Voltage,0,_IQ(gdmObj.u16VBias1/1000.));		//Point3
						  HAL_setBias(gHalHandle,HAL_SensorType_Voltage,1,_IQ(gdmObj.u16VBias2/1000.));
						  HAL_setBias(gHalHandle,HAL_SensorType_Voltage,2,_IQ(gdmObj.u16VBias3/1000.));

						  HAL_setBias(gHalHandle,HAL_SensorType_Current,0,_IQ(gdmObj.u16IBias1/1000.));
						  HAL_setBias(gHalHandle,HAL_SensorType_Current,1,_IQ(gdmObj.u16IBias2/1000.));
						  HAL_setBias(gHalHandle,HAL_SensorType_Current,2,_IQ(gdmObj.u16IBias3/1000.));

					  }

					  // Return the bias value for currents
					  gMotorVars.I_bias.aiqValue[0] = HAL_getBias(gHalHandle,HAL_SensorType_Current,0);
					  gMotorVars.I_bias.aiqValue[1] = HAL_getBias(gHalHandle,HAL_SensorType_Current,1);
					  gMotorVars.I_bias.aiqValue[2] = HAL_getBias(gHalHandle,HAL_SensorType_Current,2);

					  // Return the bias value for voltages
					  gMotorVars.V_bias.aiqValue[0] = HAL_getBias(gHalHandle,HAL_SensorType_Voltage,0);
					  gMotorVars.V_bias.aiqValue[1] = HAL_getBias(gHalHandle,HAL_SensorType_Voltage,1);
					  gMotorVars.V_bias.aiqValue[2] = HAL_getBias(gHalHandle,HAL_SensorType_Voltage,2);

					  //gMotorVars.bFlag_enableOffsetcalc = false;		// Disable Offset Calc
					  HAL_enablePwm(gHalHandle);
                  }
				  else if(ctrlState == CTRL_State_Idle)
                  {
					  HAL_disablePwm(gHalHandle);
					  //gPwmData.Tabc.aiqValue[0]=gPwmData.Tabc.aiqValue[1]=gPwmData.Tabc.aiqValue[2] = 0;
					  gMotorVars.bFlag_Run_Identify = false;
                  }

				  if((CTRL_getFlag_enableUserMotorParams(gCtrlHandle) == true) &&
					 (ctrlState > CTRL_State_Idle) &&
                     (gMotorVars.CtrlVersion.minor == 6))
                  {
					  // call this function to fix 1p6
					  USER_softwareUpdate1p6(gCtrlHandle);
                  }

              }
          }


		  if(EST_isMotorIdentified(pobj->estHandle))
          {
			  EST_setMaxCurrentSlope_pu(pobj->estHandle,giqMaxCurrentSlope);
			  gMotorVars.bFlag_MotorIdentified = true;

			  // set the speed reference
			  CTRL_setSpd_ref_krpm(gCtrlHandle,gMotorVars.iqSpeedRef_krpm);

			  CTRL_setMaxAccel_pu(gCtrlHandle,_IQmpy(MAX_ACCEL_KRPMPS_SF,gMotorVars.iqMaxAccel_krpmps));


			  /* Org
            	// enable the SpinTAC Speed Controller
            	STVELCTL_setEnable(stObj->velCtlHandle, true);

            	if(EST_getState(obj->estHandle) != EST_State_OnLine)
            	{
            		// if the estimator is not running, place SpinTAC into reset
            		STVELCTL_setEnable(stObj->velCtlHandle, false);
            	}*/

			  if(bFlag_Latch_softwareUpdate)
			  {
				  bFlag_Latch_softwareUpdate = false;

				  USER_calcPIgains(gCtrlHandle);


				  // initialize the watch window kp and ki current values with pre-calculated values
				  gMotorVars.iqKp_Id = CTRL_getKp(gCtrlHandle,CTRL_Type_PID_Id);
				  gMotorVars.iqKi_Id = CTRL_getKi(gCtrlHandle,CTRL_Type_PID_Id);

				  gMotorVars.iqKp_Iq = CTRL_getKp(gCtrlHandle,CTRL_Type_PID_Iq);
				  gMotorVars.iqKi_Iq = CTRL_getKi(gCtrlHandle,CTRL_Type_PID_Iq);

				  gMotorVars.iqKp_spd = CTRL_getKp(gCtrlHandle,CTRL_Type_PID_spd);
				  gMotorVars.iqKi_spd = CTRL_getKi(gCtrlHandle,CTRL_Type_PID_spd);


				  /* Org
			  	  // initialize the watch window Bw value with the default value
              	  gMotorVars.SpinTAC.VelCtlBw_radps = STVELCTL_getBandwidth_radps(stObj->velCtlHandle);

              	  // initialize the watch window with maximum and minimum Iq reference
              	  gMotorVars.SpinTAC.VelCtlOutputMax_A = _IQmpy(STVELCTL_getOutputMaximum(stObj->velCtlHandle), _IQ(USER_IQ_FULL_SCALE_CURRENT_A));
              	  gMotorVars.SpinTAC.VelCtlOutputMin_A = _IQmpy(STVELCTL_getOutputMinimum(stObj->velCtlHandle), _IQ(USER_IQ_FULL_SCALE_CURRENT_A));
            	*/
			  }

          }
		  else
          {
			  bFlag_Latch_softwareUpdate = true;
			  giqMaxCurrentSlope = EST_getMaxCurrentSlope_pu(pobj->estHandle);
          }


		  // when appropriate, update the global variables
		  if(gu16Counter_updateGlobals >= NUM_MAIN_TICKS_FOR_GLOBAL_VARIABLE_UPDATE)
          {
			  gu16Counter_updateGlobals = 0;
			  updateGlobalVariables_motor(gCtrlHandle);	//, stHandle);
         }

		  if(CTRL_getFlag_enableSpeedCtrl(gCtrlHandle))
		  {
			  // update Kp and Ki gains
			  updateKpKiGains(gCtrlHandle);
		  }
		  else
		  {

			  // update Iq reference
			  updateIqRef(gCtrlHandle);

		  }

		  /* Org
        	// set the SpinTAC (ST) bandwidth scale
        	STVELCTL_setBandwidth_radps(stObj->velCtlHandle, gMotorVars.SpinTAC.VelCtlBw_radps);

        	// set the maximum and minimum values for Iq reference
        	STVELCTL_setOutputMaximums(stObj->velCtlHandle, _IQmpy(gMotorVars.SpinTAC.VelCtlOutputMax_A, _IQ(1.0/USER_IQ_FULL_SCALE_CURRENT_A)), _IQmpy(gMotorVars.SpinTAC.VelCtlOutputMin_A, _IQ(1.0/USER_IQ_FULL_SCALE_CURRENT_A)));
		   */

		  // enable/disable the forced angle
		  EST_setFlag_enableForceAngle(pobj->estHandle,gMotorVars.bFlag_enableForceAngle);

		  // enable or disable power warp
		  CTRL_setFlag_enablePowerWarp(gCtrlHandle,gMotorVars.bFlag_enablePowerWarp);

#ifdef DRV8301_SPI
		  HAL_writeDrvData(gHalHandle,&gDrvSpi8301Vars);

		  HAL_readDrvData(gHalHandle,&gDrvSpi8301Vars);
#endif

      } // end of while(gFlag_enableSys) loop


	  HAL_disablePwm(gHalHandle);
	  CTRL_setParams(gCtrlHandle,&gUserParams);
	  gMotorVars.bFlag_Run_Identify = false;
	
    /* Org
    // setup the SpinTAC Components
    ST_setupVelCtl(stHandle);
    ST_setupPosConv(stHandle); */

 	} // end of for(;;) loop

} // end of main() function


interrupt void adcInt1ISR(void)	// MainISR
{

	// static uint16_t stCnt = 0;
	CTRL_Obj *pobj = (CTRL_Obj *)gCtrlHandle;

#ifdef PWM_TEST
	HAL_readAdcData(gHalHandle,&gAdcData);
	HAL_writePwmData(gHalHandle,&gPwmData);	// write the PWM compare values
	HAL_acqAdcInt(gHalHandle,ADC_IntNumber_1);
	return
#endif

	// compute the electrical angle
	//ENC_calcElecAngle(gEncHandle, HAL_getQepPosnCounts(gHalHandle));	//Org
	ENC_run(gEncHandle, HAL_getQepPosnCounts(gHalHandle));
	HAL_readAdcData(gHalHandle,&gAdcData);


	HALLBLDC_State_Check(gHalHandle, gHallBLDCHandle);

	/* Org
	// Run the SpinTAC Components
	if(stCnt++ >= ISR_TICKS_PER_SPINTAC_TICK)
	{
		ST_runPosConv(stHandle, encHandle, ctrlHandle);
		ST_runVelCtl(stHandle, ctrlHandle);
		stCnt = 1;
	 }
	*/
	if(USER_MOTOR_TYPE == MOTOR_Type_Induction)
	{
		 /*  Org
		// update the electrical angle for the SLIP module
		SLIP_setElectricalAngle(slipHandle, ENC_getElecAngle(encHandle));
		// compute the amount of slip
		SLIP_run(slipHandle);


		// run the controller
		CTRL_run(ctrlHandle,halHandle,&gAdcData,&gPwmData,SLIP_getMagneticAngle(slipHandle));
		*/
	}
	else
	{
		// run the controller
		//CTRL_run(gCtrlHandle,gHalHandle,&gAdcData,&gPwmData,ENC_getElecAnglePu(gEncHandle));	//Org
		CTL_METHOD_e ctlMethod = DM_getCtlMethod(gdmHandle);
		_iq iqAnglePu;
		if (ctlMethod == CTL_FOC_w_Encoder1)
			iqAnglePu = ENC_getElecAnglePu(gEncHandle);
		else
			iqAnglePu = EST_getAngle_pu(gCtrlHandle->estHandle);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CTRL_run(gCtrlHandle,gHalHandle,gHallBLDCHandle, &gAdcData,&gPwmData,iqAnglePu);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}


	HAL_writePwmData(gHalHandle,&gPwmData);	// write the PWM compare values
	CTRL_setup(gCtrlHandle);		// setup the controller

	// if we are forcing alignment, using the Rs Recalculation, align the eQEP angle with the rotor angle
	if((EST_getState(pobj->estHandle) == EST_State_Rs) && (USER_MOTOR_TYPE == MOTOR_Type_Pm))
	{
		ENC_setZeroOffset(gEncHandle, (uint32_t)(HAL_getQepPosnMaximum(gHalHandle) - HAL_getQepPosnCounts(gHalHandle)));
	}


	HAL_acqAdcInt(gHalHandle,ADC_IntNumber_1);

#ifndef F2802xF
    DATALOG_update(gDatalogHandle);
#endif

#ifndef F2802xF
  // connect inputs of the PWMDAC module.
    {
        uint_least8_t u8Cnt;
        for (u8Cnt =0 ; u8Cnt< 4;u8Cnt++)
        {
            gDacData.aiqValue[u8Cnt] = (*gDacData.api32Data[u8Cnt]);
        }

        HAL_writeDacData(gHalHandle,&gDacData);
    }
#endif



} // end of mainISR() function

interrupt void adcInt2ISR(void)
{
	HAL_readAdcDataApx(gHalHandle,&gAdcData);	//External ADC
	HAL_acqAdcInt(gHalHandle,ADC_IntNumber_2);

} // end of mainISR() function

interrupt void timer0ISR(void)
{
	// acknowledge the Timer 0 interrupt	// 1ms interrupt
	HAL_acqTimer0Int(gHalHandle);


	//HAL_toggleLed(halHandle,HAL_GPIO_LED3);
} // end of timer0ISR() function


interrupt void timer1ISR(void)		//Low priority
{
	// acknowledge the Timer 1 interrupt	// 1ms interrupt

    static uint_least8_t u8Index = 0;
    _iq iqSpeedPu;
    //bool bResult;

    HAL_enableGlobalInts(gHalHandle);       //for multiple interrupts


    iqSpeedPu = (HallBLDC_getDoBLDC(gHallBLDCHandle))?
                 HallBLDC_getSpeedPu(gHallBLDCHandle) :  EST_getFm_pu(gpCtlObj->estHandle);

    DM_run(gdmHandle,&gAdcData, iqSpeedPu, gpCtlObj->pidHandle_Iq->iqfbackValue, ENC_getSpeedKRPM(gEncHandle));


    Throttle_run(gThrottleHandle,gAdcData.iqExtAdc[0]);

    u8Index = (u8Index+1) & 0x0003;
    switch (u8Index)
    {
        case 0:
            SCIMessageSlave_run(gSciMessageHandle[0]);
            break;
        case 1:
            OPERATOR_run(gOperHandle);
            break;
        case 2:
            SCIMessageMaster_run(gSciMessageHandle[1]);
            break;
        case 3:
            IOEXPAND_run(gIoexpandHandle);

            break;
    }


    HAL_acqTimer1Int(gHalHandle);
    HAL_disableGlobalInts(gHalHandle);

} // end of timer1ISR() function


interrupt void i2cInt1AISR(void)
{
	HAL_Obj *pHalObj = (HAL_Obj *)gHalHandle;
	I2C_Handle i2cHandle = pHalObj->i2cHandle;
	I2C_InterruptCode_e i2cInterruptCode;
	uint_least8_t i, u8Cnt;

	i2cInterruptCode = I2C_getIntFlagStatus(i2cHandle);	// Read interrupt source
	switch (i2cInterruptCode)
	{
		case I2C_StopDection:
			 // If completed message was writing data, reset msg to inactive state
			if (gI2cMessage.msgStatusCode == I2C_MSGSTAT_WRITE_BUSY)
				gI2cMessage.msgStatusCode = (I2C_MessageCode_e) I2C_MSGSTAT_INACTIVE;
			else if (gI2cMessage.msgStatusCode == I2C_MSGSTAT_READ_BUSY)
			{
				gI2cMessage.msgStatusCode = I2C_MSGSTAT_INACTIVE;
				u8Cnt = I2C_getReceiveFifoNo(i2cHandle);
				for (i=0;i < u8Cnt; i++)
					gI2cMessage.au8RxBuffer[gI2cMessage.u8RxIndex++] =  I2C_read(i2cHandle);

			}
			else //if (gI2cMessage.msgStatusCode == I2C_MSGSTAT_NOACK)
			{
				gI2cMessage.msgStatusCode = I2C_MSGSTAT_ERR;
				I2C_resetTxFifo(i2cHandle);
				I2C_enableTxFifo(i2cHandle);
			}
			break;
		case I2C_NackDection:
			/*if ((gI2cMessage.msgStatusCode == I2C_MSGSTAT_WRITE_POLL) ||
				(gI2cMessage.msgStatusCode == I2C_MSGSTAT_READ_POLL) )
				//(pI2cMessage->msgStatusCode == I2C_MSGSTAT_READ_CURRENT_POLL))
			{
				if (++gI2cMessage.u8RetryNo < 4)
				{
					I2C_resetTxFifo(i2cHandle);
					usDelay(200);
					I2C_enableTxFifo(i2cHandle);
					I2C_StartCond(i2cHandle);	//Retry
					I2C_clearTxFifoIntFlag(i2cHandle);
				}
				else
				{
					gI2cMessage.msgStatusCode = I2C_MSGSTAT_NOACK;
					I2C_StopCond(i2cHandle);	//I2caRegs.I2CMDR.bit.STP = 1;

				}
			}
			else
			{
				gI2cMessage.msgStatusCode = I2C_MSGSTAT_NOACK;
				I2C_StopCond(i2cHandle);	//I2caRegs.I2CMDR.bit.STP = 1;
			}*/

			gI2cMessage.msgStatusCode = I2C_MSGSTAT_NOACK;
			I2C_StopCond(i2cHandle);
			I2C_clearNackStatus(i2cHandle);	//I2C_MSGSTAT_POLL_ACK & I2C_MSGSTAT_WRITE_BUSY
			break;
		case I2C_RegisterReady:
			/*if (gI2cMessage.msgStatusCode == I2C_MSGSTAT_WRITE_POLL)
			{
				gI2cMessage.msgStatusCode = I2C_MSGSTAT_WRITE_BUSY;
				gI2cMessage.u8TxIndex = 0;
				I2C_setDataCount(i2cHandle,gI2cMessage.u8TxNumOfBytes);
				//I2C_setMode(i2cHandle, I2C_Mode_Master);
				//I2C_setTxRxMode(i2cHandle, I2C_Mode_Transmit);
				I2C_StartCond(i2cHandle);
				I2C_clearTxFifoIntFlag(i2cHandle);
			}
			else if (gI2cMessage.msgStatusCode == I2C_MSGSTAT_READ_POLL)
			{
				gI2cMessage.msgStatusCode = I2C_MSGSTAT_SEND_NOSTOP_BUSY;
				gI2cMessage.u8TxIndex = 0;
				I2C_setDataCount(i2cHandle,gI2cMessage.u8TxNumOfBytes);
				//I2C_setMode(i2cHandle, I2C_Mode_Master);
				//I2C_setTxRxMode(i2cHandle, I2C_Mode_Transmit);
				I2C_StartCond(i2cHandle);
				I2C_clearTxFifoIntFlag(i2cHandle);
			} */
		    if (gI2cMessage.msgStatusCode == I2C_MSGSTAT_SEND_NOSTOP_BUSY) //||
		    	//	 (pI2cMessage->msgStatusCode == I2C_MSGSTAT_READ_CURRENT_POLL))
			{
		    	gI2cMessage.msgStatusCode = I2C_MSGSTAT_READ_BUSY;
		    	gI2cMessage.u8RxIndex = 0;
				I2C_setDataCount(i2cHandle,gI2cMessage.u8RxNumOfBytes);  	//I2caRegs.I2CCNT = msg->NumOfBytes+2;
				I2C_setTxRxMode(i2cHandle, I2C_Mode_Receive);
				I2C_StartCond(i2cHandle);

				//if (gI2cMessage.u8RxNumOfBytes <= I2C_MAX_LEVEL )
				//	I2C_StopCond(i2cHandle);
				//else
				I2C_clearRxFifoIntFlag(i2cHandle);

			}
		    else if (gI2cMessage.msgStatusCode == I2C_MSGSTAT_READ_BUSY)
		    {
		    	I2C_StopCond(i2cHandle);
		    }
			break;

		default:
			// Generate some error due to invalid interrupt source
			__asm("   ESTOP0");
	}

	HAL_acqI2cInt(gHalHandle);
}


interrupt void i2cInt2AISR(void) //FIFO Interrupt
{
	HAL_Obj *pHalObj = (HAL_Obj *)gHalHandle;
	I2C_Handle i2cHandle = pHalObj->i2cHandle;
	uint_least8_t i, u8Cnt;


	//if (I2C_getFifoRxInt(i2cHandle))

	if ((gI2cMessage.msgStatusCode == I2C_MSGSTAT_WRITE_BUSY) ||
		(gI2cMessage.msgStatusCode == I2C_MSGSTAT_SEND_NOSTOP_BUSY))
	{
		u8Cnt = gI2cMessage.u8TxNumOfBytes-gI2cMessage.u8TxIndex;
		if (u8Cnt != 0)
		{
			u8Cnt = DATA_min(u8Cnt, I2C_MAX_LEVEL);
			for (i=0; i< u8Cnt; i++)
				I2C_write(i2cHandle,gI2cMessage.au8TxBuffer[gI2cMessage.u8TxIndex++]);
			I2C_clearTxFifoIntFlag(i2cHandle);
		}
		else
		{
			if (gI2cMessage.msgStatusCode == I2C_MSGSTAT_WRITE_BUSY)
				I2C_StopCond(i2cHandle);
		}

	}
	else if  (gI2cMessage.msgStatusCode ==I2C_MSGSTAT_READ_BUSY )
	{
		u8Cnt = I2C_getReceiveFifoNo(i2cHandle);
		for (i=0;i < u8Cnt; i++)
			gI2cMessage.au8RxBuffer[gI2cMessage.u8RxIndex++] =  I2C_read(i2cHandle);
		I2C_clearRxFifoIntFlag(i2cHandle);

	}
	HAL_acqI2cInt(gHalHandle);
}

void sciTxFifoISR(SCIMessage_Handle  sciMsgHandle)
{
	SCI_Message *pSCIMessage = (SCI_Message *)  sciMsgHandle;
	SCI_Handle sciHandle = pSCIMessage->sciHandle;
	uint_least8_t i, u8Cnt;

	u8Cnt = pSCIMessage->u8TxNumOfBytes - pSCIMessage->u8TxIndex;
	if (u8Cnt)
	{
		u8Cnt = DATA_min(u8Cnt, SCI_MAX_LEVEL);
		for (i=0; i< u8Cnt; i++)
			SCI_write(sciHandle, pSCIMessage->au8TxBuffer[pSCIMessage->u8TxIndex++]);
	}
	else
		SCI_disableTxFifoInt(sciHandle);
}

interrupt void sciATxFifoISR(void)
{
	sciTxFifoISR(gSciMessageHandle[0]);
	HAL_acqSciATxInt(gHalHandle);

}

interrupt void sciBTxFifoISR(void)
{
	sciTxFifoISR(gSciMessageHandle[1]);
	HAL_acqSciBTxInt(gHalHandle);
}

void sciRxFifoISR(SCIMessage_Handle  sciMsgHandle)
{
	SCI_Message *pSCIMessage = (SCI_Message *)  sciMsgHandle;
	SCI_Handle sciHandle = pSCIMessage->sciHandle;

	if (SCI_rxError(sciHandle))
	{
		SCI_disable(sciHandle);
		pSCIMessage->u8RxIndex = 0;
		SCI_enable(sciHandle);
	}
	else
	{
		if (SCI_rxFiFoOver(sciHandle))
		{
	    	SCI_resetRxFifo(sciHandle);
	    	SCI_clearRxFifoOvf(sciHandle);
	    	pSCIMessage->u8RxIndex=0;

		}
		else
		{
			uint_least8_t i, u8Cnt, u8Index;

			u8Cnt = SCI_getRxFifoStatus(sciHandle) >> 8;
	    	u8Index = pSCIMessage->u8RxIndex;
	    	for (i=0; i< u8Cnt; i++)
	    	{
	    		pSCIMessage->au8RxBuffer[u8Index] = (uint_least8_t) SCI_read(sciHandle);
	    		if (u8Index < SCI_RX_MAX_BUFFER_SZIE)
	    			pSCIMessage->u8RxIndex = ++u8Index;
	    	}
	    	pSCIMessage->u16TimeoutCnt = MAX_OPERTIMEOUT1;
		}
	 }

}

interrupt void sciARxFifoISR(void)
{
	sciRxFifoISR( gSciMessageHandle[0]);
	HAL_acqSciARxInt(gHalHandle);
}


interrupt void sciBRxFifoISR(void)
{
	sciRxFifoISR(gSciMessageHandle[1]);
	HAL_acqSciBRxInt(gHalHandle);
}



void updateGlobalVariables_motor(CTRL_Handle handle)	//, ST_Handle sthandle)
{
	CTRL_Obj *pobj = (CTRL_Obj *)handle;

#ifdef __TMS320C28XX_FPU32__
	int32_t i32tmp;
#endif

	// ST_Obj *stObj = (ST_Obj *)sthandle;
	 //int32_t i32tmp;
	 //float fLs_q;


	// get the speed estimate

	//_iq iqkrpm_to_pu_sf = EST_get_krpm_to_pu_sf(pobj->estHandle);
	gMotorVars.iqSpeed_krpm = EST_getSpeed_krpm(pobj->estHandle);

#ifdef HALL_BLDC
	gMotorVars.iqSpeedQEP_krpm =_IQmpy(HallBLDC_getSpeedPu(gHallBLDCHandle),EST_get_pu_to_krpm_sf(pobj->estHandle));

	//gMotorVars.iqSpeedQEP_krpm = ENC_getSpeedKRPM(gEncHandle);
#else
	gMotorVars.iqSpeedQEP_krpm = ENC_getSpeedKRPM(gEncHandle);
#endif

  // get the speed from eQEP
  //gMotorVars.SpeedQEP_krpm = _IQmpy(STPOSCONV_getVelocityFiltered(stObj->posConvHandle), _IQ(ST_SPEED_KRPM_PER_PU));	//Org

  //gMotorVars.Speed_krpm = ENC_getFilteredSpeed(encHandle);	//EST_getAngle_pu(obj->estHandle);
//	gMotorVars.SpeedQEP_krpm =_IQmpy(ENC_getFilteredSpeed(encHandle), _IQ(ST_SPEED_KRPM_PER_PU)) ;
  // gMotorVars.iqSpeedQEP_krpm = _IQ(ENC_getSpeedRPM(gEncHandle)/1000.0);

  //ENC_getFilteredSpeedPu

  //gMotorVars.SpeedQEP_krpm = ENC_getElecAngle(encHandle)-EST_getAngle_pu(obj->estHandle);

  // get the real time speed reference coming out of the speed trajectory generator
	gMotorVars.iqSpeedTraj_krpm = _IQmpy(CTRL_getSpd_int_ref_pu(handle),EST_get_pu_to_krpm_sf(pobj->estHandle));	//Org
  //gMotorVars.SpeedTraj_krpm = _IQmpy(ENC_getFilteredSpeed(encHandle),EST_get_pu_to_krpm_sf(obj->estHandle));

  // get the torque estimate
	gMotorVars.iqTorque_Nm = USER_computeTorque_Nm(handle, giqTorque_Flux_Iq_pu_to_Nm_sf, giqTorque_Ls_Id_Iq_pu_to_Nm_sf);

	 // get the magnetizing current
#ifdef __TMS320C28XX_FPU32__
	i32tmp = EST_getIdRated(pobj->estHandle);
	gMotorVars.fMagnCurr_A = *((float_t *)&i32tmp);
#else
    gMotorVars.fMagnCurr_A = EST_getIdRated(pobj->estHandle);
#endif


      // get the rotor resistance
#ifdef __TMS320C28XX_FPU32__
    i32tmp = EST_getRr_Ohm(pobj->estHandle);
    gMotorVars.fRr_Ohm = *((float_t *)&i32tmp);
#else
    gMotorVars.fRr_Ohm = EST_getRr_Ohm(pobj->estHandle);
#endif

    // get the stator resistance
#ifdef __TMS320C28XX_FPU32__
    i32tmp = EST_getRs_Ohm(pobj->estHandle);
    gMotorVars.fRs_Ohm = *((float_t *)&i32tmp);
#else
    gMotorVars.fRs_Ohm = EST_getRs_Ohm(pobj->estHandle);
#endif

    // get the stator inductance in the direct coordinate direction
#ifdef __TMS320C28XX_FPU32__
    i32tmp = EST_getLs_d_H(pobj->estHandle);
    gMotorVars.fLsd_H = *((float_t *)&i32tmp);
#else
    gMotorVars.fLsd_H = EST_getLs_d_H(pobj->estHandle);
#endif

    // get the stator inductance in the quadrature coordinate direction
#ifdef __TMS320C28XX_FPU32__
    i32tmp = EST_getLs_q_H(pobj->estHandle);
    gMotorVars.fLsq_H = *((float_t *)&i32tmp);
#else
    gMotorVars.fLsq_H = EST_getLs_q_H(pobj->estHandle);
#endif


  // get the flux in V/Hz in floating point
#ifdef __TMS320C28XX_FPU32__
    i32tmp = EST_getFlux_VpHz(pobj->estHandle);
    gMotorVars.fFlux_VpHz = *((float_t *)&i32tmp);

#else
    gMotorVars.fFlux_VpHz = EST_getFlux_VpHz(pobj->estHandle);
#endif




  // get the flux in Wb in fixed point
	gMotorVars.iqFlux_Wb = USER_computeFlux(handle, giqFlux_pu_to_Wb_sf);

  // get the controller state
	gMotorVars.CtrlState = CTRL_getState(handle);

  // get the estimator state
	gMotorVars.EstState = EST_getState(pobj->estHandle);

  // Get the DC buss voltage
	gMotorVars.iqVdcBus_kV = _IQmpy(gAdcData.iqdcBus,_IQ(USER_IQ_FULL_SCALE_VOLTAGE_V/1000.0));

  // get the Iq reference from the speed controller
 /* gMotorVars.IqRef_A = _IQmpy(STVELCTL_getTorqueReference(stObj->velCtlHandle), _IQ(USER_IQ_FULL_SCALE_CURRENT_A));

  // gets the Velocity Controller status
  gMotorVars.SpinTAC.VelCtlStatus = STVELCTL_getStatus(stObj->velCtlHandle);

  // get the inertia setting
  gMotorVars.SpinTAC.InertiaEstimate_Aperkrpm = _IQmpy(STVELCTL_getInertia(stObj->velCtlHandle), _IQ(ST_SPEED_PU_PER_KRPM * USER_IQ_FULL_SCALE_CURRENT_A));

  // get the friction setting
  gMotorVars.SpinTAC.FrictionEstimate_Aperkrpm = _IQmpy(STVELCTL_getFriction(stObj->velCtlHandle), _IQ(ST_SPEED_PU_PER_KRPM * USER_IQ_FULL_SCALE_CURRENT_A));

  // get the Velocity Controller error
  gMotorVars.SpinTAC.VelCtlErrorID = STVELCTL_getErrorID(stObj->velCtlHandle);

  // get the Position Converter error
  gMotorVars.SpinTAC.PosConvErrorID = STPOSCONV_getErrorID(stObj->posConvHandle);
*/

} // end of updateGlobalVariables_motor() function


void updateKpKiGains(CTRL_Handle handle)
{
	if((gMotorVars.CtrlState == CTRL_State_OnLine) && (gMotorVars.bFlag_MotorIdentified == true) && (bFlag_Latch_softwareUpdate == false))
    {
		// set the kp and ki speed values from the watch window
		CTRL_setKp(handle,CTRL_Type_PID_spd,gMotorVars.iqKp_spd);
		CTRL_setKi(handle,CTRL_Type_PID_spd,gMotorVars.iqKi_spd);

		// set the kp and ki current values for Id and Iq from the watch window
		CTRL_setKp(handle,CTRL_Type_PID_Id,gMotorVars.iqKp_Id);
		CTRL_setKi(handle,CTRL_Type_PID_Id,gMotorVars.iqKi_Id);
		CTRL_setKp(handle,CTRL_Type_PID_Iq,gMotorVars.iqKp_Iq);
		CTRL_setKi(handle,CTRL_Type_PID_Iq,gMotorVars.iqKi_Iq);

	}

} // end of updateKpKiGains() function

void updateIqRef(CTRL_Handle handle)
{
	_iq iq_ref = _IQmpy(gMotorVars.iqIqRef_A,_IQ(1.0/USER_IQ_FULL_SCALE_CURRENT_A));

  // set the speed reference so that the forced angle rotates in the correct direction for startup
  if(_IQabs(gMotorVars.iqSpeed_krpm) < _IQ(0.01))
    {
      if(iq_ref < _IQ(0.0))
        {
          CTRL_setSpd_ref_krpm(handle,_IQ(-0.01));
        }
      else if(iq_ref > _IQ(0.0))
        {
          CTRL_setSpd_ref_krpm(handle,_IQ(0.01));
        }
    }

  // Set the Iq reference that use to come out of the PI speed control
  CTRL_setIq_ref_pu(handle, iq_ref);

  return;
} // end of updateIqRef() function


/*void ST_runPosConv(ST_Handle handle, ENC_Handle encHandle, CTRL_Handle ctrlHandle)
{
	ST_Obj *stObj = (ST_Obj *)handle;

	// get the electrical angle from the ENC module
    STPOSCONV_setElecAngle_erev(stObj->posConvHandle, ENC_getElecAngle(encHandle));

    if(USER_MOTOR_TYPE ==  MOTOR_Type_Induction) {
      // The CurrentVector feedback is only needed for ACIM
      // get the vector of the direct/quadrature current input vector values from CTRL
      STPOSCONV_setCurrentVector(stObj->posConvHandle, CTRL_getIdq_in_addr(ctrlHandle));
    }

	// run the SpinTAC Position Converter
	STPOSCONV_run(stObj->posConvHandle);

	if(USER_MOTOR_TYPE ==  MOTOR_Type_Induction) {
	  // The Slip Velocity is only needed for ACIM
	  // update the slip velocity in electrical angle per second, Q24
	  SLIP_setSlipVelocity(slipHandle, STPOSCONV_getSlipVelocity(stObj->posConvHandle));
	}
}
*/
/*
void ST_runVelCtl(ST_Handle handle, CTRL_Handle ctrlHandle)
{
    _iq speedFeedback, iqReference;
    ST_Obj *stObj = (ST_Obj *)handle;
    CTRL_Obj *ctrlObj = (CTRL_Obj *)ctrlHandle;

    // Get the mechanical speed in pu
    speedFeedback = STPOSCONV_getVelocityFiltered(stObj->posConvHandle);

	// Run the SpinTAC Controller
	// Note that the library internal ramp generator is used to set the speed reference
    STVELCTL_setVelocityReference(stObj->velCtlHandle, TRAJ_getIntValue(ctrlObj->trajHandle_spd));
	STVELCTL_setAccelerationReference(stObj->velCtlHandle, _IQ(0.0));	// Internal ramp generator does not provide Acceleration Reference
	STVELCTL_setVelocityFeedback(stObj->velCtlHandle, speedFeedback);
	STVELCTL_run(stObj->velCtlHandle);

	// select SpinTAC Velocity Controller
	iqReference = STVELCTL_getTorqueReference(stObj->velCtlHandle);

	// Set the Iq reference that came out of SpinTAC Velocity Control
	CTRL_setIq_ref_pu(ctrlHandle, iqReference);
}
*/

//@} //defgroup
// end of file
