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

 
 
//! \file   solutions/instaspin_motion/src/ctrl.c
//! \brief  Contains the various functions related to the controller (CTRL) object
//!
//! (C) Copyright 2011, Texas Instruments, Inc.


// **************************************************************************
// the includes

#include <math.h>
// drivers


// modules
//#include "sw/modules/dlog/src/32b/dlog4ch.h"
#include "sw/modules/math/src/32b/math.h"


// platforms
#include "ctrlQEP.h"
#include "sw/modules/hal/boards/hvkit_rev1p1/f28x/f2806x/src/hal.h"
//#include "hal.h"
#include "sw/modules/user/src/32b/user.h"


#ifdef FLASH
#pragma CODE_SECTION(CTRL_run,"ramfuncs");
#pragma CODE_SECTION(CTRL_setup,"ramfuncs");
#endif


// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the function prototypes

void CTRL_getGains(CTRL_Handle handle,const CTRL_Type_e ctrlType,
                   _iq *piqKp,_iq *piqKi,_iq *piqKd)
{

	*piqKp = CTRL_getKp(handle,ctrlType);
	*piqKi = CTRL_getKi(handle,ctrlType);
	*piqKd = CTRL_getKd(handle,ctrlType);


} // end of CTRL_getGains() function


void CTRL_getIab_filt_pu(CTRL_Handle handle,MATH_vec2 *pIab_filt_pu)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

	pIab_filt_pu->aiqValue[0] = pCtlObj->Iab_filt.aiqValue[0];
	pIab_filt_pu->aiqValue[1] = pCtlObj->Iab_filt.aiqValue[1];


} // end of CTRL_getIab_filt_pu() function


void CTRL_getIab_in_pu(CTRL_Handle handle,MATH_vec2 *pIab_in_pu)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

	pIab_in_pu->aiqValue[0] = pCtlObj->Iab_in.aiqValue[0];
	pIab_in_pu->aiqValue[1] = pCtlObj->Iab_in.aiqValue[1];


} // end of CTRL_getIab_in_pu() function


void CTRL_getIdq_in_pu(CTRL_Handle handle,MATH_vec2 *pIdq_in_pu)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

	pIdq_in_pu->aiqValue[0] = pCtlObj->Idq_in.aiqValue[0];
	pIdq_in_pu->aiqValue[1] = pCtlObj->Idq_in.aiqValue[1];


} // end of CTRL_getIdq_in_pu() function


void CTRL_getIdq_ref_pu(CTRL_Handle handle,MATH_vec2 *pIdq_ref_pu)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

	pIdq_ref_pu->aiqValue[0] = pCtlObj->Idq_ref.aiqValue[0];
	pIdq_ref_pu->aiqValue[1] = pCtlObj->Idq_ref.aiqValue[1];

} // end of CTRL_getIdq_ref_pu() function


_iq CTRL_getMagCurrent_pu(CTRL_Handle handle)
{

	return(CTRL_getIdRated_pu(handle));
} // end of CTRL_getMagCurrent_pu() function


_iq CTRL_getMaximumSpeed_pu(CTRL_Handle handle)
{

	return(CTRL_getSpd_max_pu(handle));
} // end of CTRL_getMaximumSpeed_pu() function


void CTRL_getVab_in_pu(CTRL_Handle handle,MATH_vec2 *pVab_in_pu)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

	pVab_in_pu->aiqValue[0] = pCtlObj->Vab_in.aiqValue[0];
	pVab_in_pu->aiqValue[1] = pCtlObj->Vab_in.aiqValue[1];


} // end of CTRL_getVab_in_pu() function


void CTRL_getVab_out_pu(CTRL_Handle handle,MATH_vec2 *pVab_out_pu)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

	pVab_out_pu->aiqValue[0] = pCtlObj->Vab_out.aiqValue[0];
	pVab_out_pu->aiqValue[1] = pCtlObj->Vab_out.aiqValue[1];


} // end of CTRL_getVab_out_pu() function


void CTRL_getVdq_out_pu(CTRL_Handle handle,MATH_vec2 *pVdq_out_pu)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

	pVdq_out_pu->aiqValue[0] = pCtlObj->Vdq_out.aiqValue[0];
	pVdq_out_pu->aiqValue[1] = pCtlObj->Vdq_out.aiqValue[1];


} // end of CTRL_getVdq_out_pu() function


void CTRL_getWaitTimes(CTRL_Handle handle,uint_least32_t *pu32WaitTimes)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;
	uint_least16_t u16stateCnt;

	for(u16stateCnt=0;u16stateCnt<CTRL_numStates;u16stateCnt++)
    {
		pu32WaitTimes[u16stateCnt] = pCtlObj->au32WaitTimes[u16stateCnt];
    }

} // end of CTRL_getWaitTimes() function


void CTRL_run(CTRL_Handle ctlHandle,HAL_Handle halHandle,HALLBLDC_Handle hallBLDCHandle,
              const HAL_AdcData_t *pAdcData,
              HAL_PwmData_t *pPwmData,
              _iq  iqElectricalAnglePu)
{
	uint_least16_t u16count_isr = CTRL_getCount_isr(ctlHandle);
	uint_least16_t u16numIsrTicksPerCtrlTick = CTRL_getNumIsrTicksPerCtrlTick(ctlHandle);


  // if needed, run the controller
	if(u16count_isr >= u16numIsrTicksPerCtrlTick)
    {
		CTRL_State_e ctrlState = CTRL_getState(ctlHandle);

		// reset the isr count
		CTRL_resetCounter_isr(ctlHandle);

		// increment the state counter
		CTRL_incrCounter_state(ctlHandle);

		// increment the trajectory count
		CTRL_incrCounter_traj(ctlHandle);

		// run the appropriate controller
		if(ctrlState == CTRL_State_OnLine)
        {
			CTRL_Obj *pCtlObj = (CTRL_Obj *)ctlHandle;

			// increment the current count
			CTRL_incrCounter_current(ctlHandle);

			// increment the speed count
			CTRL_incrCounter_speed(ctlHandle);

			if(EST_getState(pCtlObj->estHandle) >= EST_State_MotorIdentified)
            {
				// run the online controller
				CTRL_runOnLine_User(ctlHandle,halHandle,hallBLDCHandle, pAdcData,pPwmData, iqElectricalAnglePu);
            }
			else
            {
				// run the online controller
				CTRL_runOnLine(ctlHandle,pAdcData,pPwmData);
            }
        }
		else if(ctrlState == CTRL_State_OffLine)
        {
			// run the offline controller
			CTRL_runOffLine(ctlHandle,halHandle,pAdcData,pPwmData);
        }
    }
	else
    {
		// increment the isr count
		CTRL_incrCounter_isr(ctlHandle);
    }


} // end of CTRL_run() function


void CTRL_setGains(CTRL_Handle handle,const CTRL_Type_e ctrlType,
                   const _iq iqKp,const _iq iqKi,const _iq iqKd)
{

	CTRL_setKp(handle,ctrlType,iqKp);
	CTRL_setKi(handle,ctrlType,iqKi);
	CTRL_setKd(handle,ctrlType,iqKd);

} // end of CTRL_setGains() function


void CTRL_setMagCurrent_pu(CTRL_Handle handle,const _iq iqmagCurrent_pu)
{

	CTRL_setIdRated_pu(handle,iqmagCurrent_pu);


} // end of CTRL_setMagCurrent_pu() function


void CTRL_setMaximumSpeed_pu(CTRL_Handle handle,const _iq iqmaxSpeed_pu)
{

	CTRL_setSpd_max_pu(handle,iqmaxSpeed_pu);

} // end of CTRL_setMaximumSpeed_pu() function


void CTRL_setParams(CTRL_Handle handle,USER_Params *pUserParams)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

	_iq iqKp,iqKi,iqKd;
	_iq iqoutMin,iqoutMax;
	_iq iqmaxModulation;

	MATH_vec2 Iab_out_pu = {_IQ(0.0),_IQ(0.0)};
	MATH_vec2 Idq_out_pu = {_IQ(0.0),_IQ(0.0)};
	MATH_vec2 Idq_ref_pu = {_IQ(0.0),_IQ(0.0)};
	MATH_vec2 Vab_in_pu = {_IQ(0.0),_IQ(0.0)};
	MATH_vec2 Vab_out_pu = {_IQ(0.0),_IQ(0.0)};
	MATH_vec2 Vdq_out_pu = {_IQ(0.0),_IQ(0.0)};


	// assign the motor type
	CTRL_setMotorParams(handle,pUserParams->motor_type,
                      pUserParams->u16motor_numPolePairs,
                      pUserParams->fmotor_ratedFlux,
                      pUserParams->fmotor_Ls_d,
                      pUserParams->fmotor_Ls_q,
                      pUserParams->fmotor_Rr,
                      pUserParams->fmotor_Rs);


	// assign other controller parameters
	CTRL_setNumIsrTicksPerCtrlTick(handle,pUserParams->u16numIsrTicksPerCtrlTick);
	CTRL_setNumCtrlTicksPerCurrentTick(handle,pUserParams->u16numCtrlTicksPerCurrentTick);
	CTRL_setNumCtrlTicksPerSpeedTick(handle,pUserParams->u16numCtrlTicksPerSpeedTick);
	CTRL_setNumCtrlTicksPerTrajTick(handle,pUserParams->u16numCtrlTicksPerTrajTick);

	CTRL_setCtrlFreq_Hz(handle,pUserParams->u32ctrlFreq_Hz);
	CTRL_setTrajFreq_Hz(handle,pUserParams->u32trajFreq_Hz);
	CTRL_setTrajPeriod_sec(handle,_IQ(1.0/pUserParams->u32trajFreq_Hz));

	CTRL_setCtrlPeriod_sec(handle,pUserParams->fctrlPeriod_sec);

	CTRL_setMaxVsMag_pu(handle,_IQ(pUserParams->fmaxVsMag_pu));

	CTRL_setIab_in_pu(handle,&Iab_out_pu);
	CTRL_setIdq_in_pu(handle,&Idq_out_pu);
	CTRL_setIdq_ref_pu(handle,&Idq_ref_pu);

	CTRL_setIdRated_pu(handle,_IQ(pUserParams->fIdRated/pUserParams->fFullScaleCurrent_A));

	CTRL_setVab_in_pu(handle,&Vab_in_pu);
	CTRL_setVab_out_pu(handle,&Vab_out_pu);
	CTRL_setVdq_out_pu(handle,&Vdq_out_pu);

	CTRL_setSpd_out_pu(handle,_IQ(0.0));

	CTRL_setRhf(handle,0.0);
	CTRL_setLhf(handle,0.0);
	CTRL_setRoverL(handle,0.0);


	// reset the counters
	CTRL_resetCounter_current(handle);
	CTRL_resetCounter_isr(handle);
	CTRL_resetCounter_speed(handle);
	CTRL_resetCounter_state(handle);
	CTRL_resetCounter_traj(handle);


	// set the wait times for each state
	CTRL_setWaitTimes(handle,&pUserParams->au32ctrlWaitTime[0]);


	// set flags
	CTRL_setFlag_enablePowerWarp(handle,false);
	CTRL_setFlag_enableCtrl(handle,false);
	CTRL_setFlag_enableOffset(handle,true);
	CTRL_setFlag_enableSpeedCtrl(handle,true);
	CTRL_setFlag_enableUserMotorParams(handle,false);
	CTRL_setFlag_enableDcBusComp(handle,true);


	// initialize the controller error code
	CTRL_setErrorCode(handle,CTRL_ErrorCode_NoError);


	// set the default controller state
	CTRL_setState(handle,CTRL_State_Idle);


	// set the number of current sensors
	CTRL_setupClarke_I(handle,pUserParams->u8numCurrentSensors);


	// set the number of voltage sensors
	CTRL_setupClarke_V(handle,pUserParams->u8numVoltageSensors);


	// set the default Id PID controller parameters
	iqKp = _IQ(0.1);
	iqKi = _IQ(pUserParams->fctrlPeriod_sec/0.004);
	iqKd = _IQ(0.0);
	iqoutMin = _IQ(-0.95);
	iqoutMax = _IQ(0.95);

	PID_setGains(pCtlObj->pidHandle_Id,iqKp,iqKi,iqKd);
	PID_setUi(pCtlObj->pidHandle_Id,_IQ(0.0));
	PID_setMinMax(pCtlObj->pidHandle_Id,iqoutMin,iqoutMax);
	CTRL_setGains(handle,CTRL_Type_PID_Id,iqKp,iqKi,iqKd);


	// set the default the Iq PID controller parameters
	iqKp = _IQ(0.1);
	iqKi = _IQ(pUserParams->fctrlPeriod_sec/0.004);
	iqKd = _IQ(0.0);
	iqoutMin = _IQ(-0.95);
	iqoutMax = _IQ(0.95);

	PID_setGains(pCtlObj->pidHandle_Iq,iqKp,iqKi,iqKd);
	PID_setUi(pCtlObj->pidHandle_Iq,_IQ(0.0));
	PID_setMinMax(pCtlObj->pidHandle_Iq,iqoutMin,iqoutMax);
	CTRL_setGains(handle,CTRL_Type_PID_Iq,iqKp,iqKi,iqKd);


	// set the default speed PID controller parameters
	iqKp = _IQ(0.02*pUserParams->fmaxCurrent*pUserParams->fFullScaleFreq_Hz/pUserParams->fFullScaleCurrent_A);
	iqKi = _IQ(2.0*pUserParams->fmaxCurrent*pUserParams->fFullScaleFreq_Hz*pUserParams->fctrlPeriod_sec/pUserParams->fFullScaleCurrent_A);
	iqKd = _IQ(0.0);
	iqoutMin = _IQ(-1.0);
	iqoutMax = _IQ(1.0);

	PID_setGains(pCtlObj->pidHandle_spd,iqKp,iqKi,iqKd);
 	PID_setUi(pCtlObj->pidHandle_spd,_IQ(0.0));
 	PID_setMinMax(pCtlObj->pidHandle_spd,iqoutMin,iqoutMax);
 	CTRL_setGains(handle,CTRL_Type_PID_spd,iqKp,iqKi,iqKd);


 	// set the speed reference
 	CTRL_setSpd_ref_pu(handle,_IQ(0.0));


 	// set the default Id current trajectory module parameters
 	TRAJ_setIntValue(pCtlObj->trajHandle_Id,_IQ(0.0));
 	TRAJ_setTargetValue(pCtlObj->trajHandle_Id,_IQ(0.0));
 	TRAJ_setMinValue(pCtlObj->trajHandle_Id,_IQ(0.0));
  	TRAJ_setMaxValue(pCtlObj->trajHandle_Id,_IQ(0.0));
  	TRAJ_setMaxDelta(pCtlObj->trajHandle_Id,_IQ(0.0));


  	// set the default the speed trajectory module parameters
  	TRAJ_setIntValue(pCtlObj->trajHandle_spd,_IQ(0.0));
  	TRAJ_setTargetValue(pCtlObj->trajHandle_spd,_IQ(0.0));
  	TRAJ_setMinValue(pCtlObj->trajHandle_spd,_IQ(0.0));
  	TRAJ_setMaxValue(pCtlObj->trajHandle_spd,_IQ(0.0));
  	TRAJ_setMaxDelta(pCtlObj->trajHandle_spd,_IQ(0.0));


  	// set the default maximum speed trajectory module parameters
  	TRAJ_setIntValue(pCtlObj->trajHandle_spdMax,_IQ(0.0));
  	TRAJ_setTargetValue(pCtlObj->trajHandle_spdMax,_IQ(0.0));
  	TRAJ_setMinValue(pCtlObj->trajHandle_spdMax,_IQ(0.0)); // not used
  	TRAJ_setMaxValue(pCtlObj->trajHandle_spdMax,_IQ(0.0)); // not used
  	TRAJ_setMaxDelta(pCtlObj->trajHandle_spdMax,_IQ(0.0)); // not used

  
  	// set the default estimator parameters
  	CTRL_setEstParams(pCtlObj->estHandle,pUserParams);


  	// set the maximum modulation for the SVGEN module
  	iqmaxModulation = SVGEN_4_OVER_3;
  	SVGEN_setMaxModulation(pCtlObj->svgenHandle,iqmaxModulation);


} // end of CTRL_setParams() function


void CTRL_setSpd_ref_pu(CTRL_Handle handle,const _iq iqSpd_ref_pu)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

	pCtlObj->iqSpd_ref = iqSpd_ref_pu;


} // end of CTRL_setSpd_ref_pu() function


void CTRL_setSpd_ref_krpm(CTRL_Handle handle,const _iq iqspd_ref_krpm)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

	_iq iqkrpm_to_pu_sf = EST_get_krpm_to_pu_sf(pCtlObj->estHandle);

	_iq iqspd_ref_pu = _IQmpy(iqspd_ref_krpm,iqkrpm_to_pu_sf);

	pCtlObj->iqSpd_ref = iqspd_ref_pu;


} // end of CTRL_setSpd_ref_krpm() function


void CTRL_setup(CTRL_Handle handle)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

	uint_least16_t u16count_traj = CTRL_getCount_traj(handle);
	uint_least16_t u16numCtrlTicksPerTrajTick = CTRL_getNumCtrlTicksPerTrajTick(handle);


	// as needed, update the trajectory
	if(u16count_traj >= u16numCtrlTicksPerTrajTick)
    {
		_iq iqintValue_Id = TRAJ_getIntValue(pCtlObj->trajHandle_Id);

		// reset the trajectory count
		CTRL_resetCounter_traj(handle);

		// run the trajectories
		CTRL_runTraj(handle);
    } // end of if(gFlag_traj) block


} // end of CTRL_setup() function


void CTRL_setupClarke_I(CTRL_Handle handle,uint_least8_t u8numCurrentSensors)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;
	_iq iqalpha_sf,iqbeta_sf;
  

	// initialize the Clarke transform module for current
	if(u8numCurrentSensors == 3)
    {
		iqalpha_sf = _IQ(MATH_ONE_OVER_THREE);
		iqbeta_sf = _IQ(MATH_ONE_OVER_SQRT_THREE);
    }
	else if(u8numCurrentSensors == 2)
    {
		iqalpha_sf = _IQ(1.0);
		iqbeta_sf = _IQ(MATH_ONE_OVER_SQRT_THREE);
    }
	else
    {
		iqalpha_sf = _IQ(0.0);
		iqbeta_sf = _IQ(0.0);
    }

	// set the parameters
	CLARKE_setScaleFactors(pCtlObj->clarkeHandle_I,iqalpha_sf,iqbeta_sf);
	CLARKE_setNumSensors(pCtlObj->clarkeHandle_I,u8numCurrentSensors);

} // end of CTRL_setupClarke_I() function


void CTRL_setupClarke_V(CTRL_Handle handle,uint_least8_t u8numVoltageSensors)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;
	_iq iqalpha_sf,iqbeta_sf;
  

	// initialize the Clarke transform module for current
	if(u8numVoltageSensors == 3)
    {
		iqalpha_sf = _IQ(MATH_ONE_OVER_THREE);
		iqbeta_sf = _IQ(MATH_ONE_OVER_SQRT_THREE);
    }
	else
    {
		iqalpha_sf = _IQ(0.0);
		iqbeta_sf = _IQ(0.0);
    }

	// set the parameters
	CLARKE_setScaleFactors(pCtlObj->clarkeHandle_V,iqalpha_sf,iqbeta_sf);
	CLARKE_setNumSensors(pCtlObj->clarkeHandle_V,u8numVoltageSensors);

} // end of CTRL_setupClarke_V() function


void CTRL_setWaitTimes(CTRL_Handle handle,const uint_least32_t *pu32WaitTimes)
{
	CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;
	uint_least16_t u16stateCnt;

	for(u16stateCnt=0;u16stateCnt<CTRL_numStates;u16stateCnt++)
    {
		pCtlObj->au32WaitTimes[u16stateCnt] = pu32WaitTimes[u16stateCnt];
    }


} // end of CTRL_setWaitTimes() function


bool CTRL_updateState(CTRL_Handle handle)
{
  CTRL_State_e ctrlState = CTRL_getState(handle);
  bool bflag_enableCtrl = CTRL_getFlag_enableCtrl(handle);
  bool bstateChanged = false;


  if(bflag_enableCtrl)
    {
      uint_least32_t u32waitTime = CTRL_getWaitTime(handle,ctrlState);
      uint_least32_t u32counter_ctrlState = CTRL_getCount_state(handle);


      // check for errors
      CTRL_checkForErrors(handle);


      if(u32counter_ctrlState >= u32waitTime)
      {
    	  // reset the counter
          CTRL_resetCounter_state(handle);

          if(ctrlState == CTRL_State_OnLine)
          {
              CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;
              _iq iqId_target = TRAJ_getTargetValue(pCtlObj->trajHandle_Id);

              // update the estimator state
              bool bflag_estStateChanged = EST_updateState(pCtlObj->estHandle,iqId_target);

              if(bflag_estStateChanged)
              {
                  // setup the controller
                  CTRL_setupCtrl(handle);

                  // setup the trajectory
                  CTRL_setupTraj(handle);
              }

              if(EST_isOnLine(pCtlObj->estHandle))
              {
                  // setup the estimator for online state
                  CTRL_setupEstOnLineState(handle);
              }

              if(EST_isLockRotor(pCtlObj->estHandle) ||
                 (EST_isIdle(pCtlObj->estHandle) && EST_isMotorIdentified(pCtlObj->estHandle)))
              {
                  // set the enable controller flag to false
                  CTRL_setFlag_enableCtrl(handle,false);

                  // set the next controller state
                  CTRL_setState(handle,CTRL_State_Idle);
              }
          }
          else if(ctrlState == CTRL_State_OffLine)
          {
              // set the next controller state
              CTRL_setState(handle,CTRL_State_OnLine);
          }
          else if(ctrlState == CTRL_State_Idle)
          {
              CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;
              bool  bflag_enableUserMotorParams = CTRL_getFlag_enableUserMotorParams(handle);

              if(bflag_enableUserMotorParams)
              {
                  // initialize the motor parameters using values from the user.h file
                  CTRL_setUserMotorParams(handle);
              }

              if(EST_isIdle(pCtlObj->estHandle))
              {
                  // setup the estimator for idle state
                  CTRL_setupEstIdleState(handle);

                  if(EST_isMotorIdentified(pCtlObj->estHandle))
                  {
                      if(CTRL_getFlag_enableOffset(handle))
                      {
                          // set the next controller state
                          CTRL_setState(handle,CTRL_State_OffLine);
                      }
                      else
                      {
                          // set the next controller state
                          CTRL_setState(handle,CTRL_State_OnLine);
                      }
                  }
                  else
                  {
                      // set the next controller state
                      CTRL_setState(handle,CTRL_State_OffLine);
                  }
              }
              else if(EST_isLockRotor(pCtlObj->estHandle))
              {
                  // set the next controller state
                  CTRL_setState(handle,CTRL_State_OnLine);
              }
          }
        }  // if(counter_ctrlState >= waitTime) loop
    } 
  else
    {
      CTRL_Obj *pCtlObj = (CTRL_Obj *)handle;

      // set the next controller state
      CTRL_setState(handle,CTRL_State_Idle);

      // set the estimator to idle
      if(!EST_isLockRotor(pCtlObj->estHandle))
        {
          if(EST_isMotorIdentified(pCtlObj->estHandle))
            {
              EST_setIdle(pCtlObj->estHandle);
            }
          else
            {
              EST_setIdle_all(pCtlObj->estHandle);

              EST_setRs_pu(pCtlObj->estHandle,_IQ30(0.0));
            }
        }
    }


  // check to see if the state changed
  if(ctrlState != CTRL_getState(handle))
  {
      bstateChanged = true;
  }

  return(bstateChanged);
} // end of CTRL_updateState() function


void CTRL_runOnLine_User(CTRL_Handle ctlHandle,HAL_Handle halHandle, HALLBLDC_Handle hallBLDCHandle,
                           const HAL_AdcData_t *pAdcData,HAL_PwmData_t *pPwmData,
                           _iq iqElectricalAnglePu)
{
    CTRL_Obj *pctlObj = (CTRL_Obj *)ctlHandle;
    _iq iqangle_pu;
    MATH_vec2 phasor;

    // run Clarke transform on current
    CLARKE_run(pctlObj->clarkeHandle_I,&pAdcData->I,CTRL_getIab_in_addr(ctlHandle));
    // run Clarke transform on voltage
    CLARKE_run(pctlObj->clarkeHandle_V,&pAdcData->V,CTRL_getVab_in_addr(ctlHandle));
    // run the estimator
    EST_run(pctlObj->estHandle,CTRL_getIab_in_addr(ctlHandle),CTRL_getVab_in_addr(ctlHandle),
            pAdcData->iqdcBus,TRAJ_getIntValue(pctlObj->trajHandle_spd));


    HALLBLDC_Ctrl_Run(ctlHandle, halHandle, hallBLDCHandle);

    // generate the motor electrical angle
    //iqangle_pu = EST_getAngle_pu(pCtlObj->estHandle);

    // Update electrical angle from the sensor
    iqangle_pu = iqElectricalAnglePu; // org

    // compute the sin/cos phasor
    CTRL_computePhasor(iqangle_pu,&phasor);

    // set the phasor in the Park transform
    PARK_setPhasor(pctlObj->parkHandle,&phasor);

    // run the Park transform
    PARK_run(pctlObj->parkHandle,CTRL_getIab_in_addr(ctlHandle),CTRL_getIdq_in_addr(ctlHandle));

    // when appropriate, run the PID speed controller
    if(CTRL_doSpeedCtrl(ctlHandle))
    {
        _iq iqrefValue = TRAJ_getIntValue(pctlObj->trajHandle_spd);
        _iq iqoutMax = TRAJ_getIntValue(pctlObj->trajHandle_spdMax);
        _iq iqoutMin = -iqoutMax;
        _iq iqfbackValue;

        // reset the speed count
        CTRL_resetCounter_speed(ctlHandle);

        iqfbackValue = (HallBLDC_getDoBLDC(hallBLDCHandle))?
                        HallBLDC_getSpeedPu(hallBLDCHandle) :  EST_getFm_pu(pctlObj->estHandle);
        PID_setMinMax(pctlObj->pidHandle_spd,iqoutMin,iqoutMax);
        PID_run_spd(pctlObj->pidHandle_spd,iqrefValue,iqfbackValue,CTRL_getSpd_out_addr(ctlHandle));

   /*     if (HallBLDC_getDoBLDC(hallBLDCHandle))
        {

            HALLBLDC_Ctrl_PwmSet(halHandle,hallBLDCHandle,CTRL_getSpd_out_pu(ctlHandle)/10,pPwmData);
            return;
        }*/
     }


    // when appropriate, run the PID Id and Iq controllers
    if(CTRL_doCurrentCtrl(ctlHandle) )
    {
        _iq iqKp_Id = CTRL_getKp(ctlHandle,CTRL_Type_PID_Id);
        _iq iqKp_Iq = CTRL_getKp(ctlHandle,CTRL_Type_PID_Iq);
        _iq iqrefValue;
        _iq iqfbackValue;
        _iq iqoutMin,iqoutMax;

        _iq iqmaxVsMag = CTRL_getMaxVsMag_pu(ctlHandle); // read max voltage vector to set proper limits to current controllers
        CTRL_resetCounter_current(ctlHandle);   // reset the current count

        if (HallBLDC_getDoBLDC(hallBLDCHandle)) /* && (CTRL_getFlag_enableSpeedCtrl(ctlHandle) == false) )*/
        {
            _iq iqHall_BLDC_Is_fdb_pu, iqHall_BLDC_Is_ref_pu, iqBLDCPwmDuty;
            PID_Handle pidHandle;

            _iq iqSpeedKrpm = _IQmpy(HallBLDC_getSpeedPu(hallBLDCHandle),EST_get_pu_to_krpm_sf(pctlObj->estHandle));

            if(_IQabs(iqSpeedKrpm) < _IQ(0.15))      // 150 rpm
            {
            	if (CTRL_getFlag_enableSpeedCtrl(ctlHandle))
            		iqBLDCPwmDuty = CTRL_getSpd_out_pu(ctlHandle)/10;
            	else
            	{
            		if (CTRL_getIq_ref_pu(ctlHandle) >= 0)
            		   iqBLDCPwmDuty = _IQ(0.1);
            		else
            			iqBLDCPwmDuty = -_IQ(0.1);
            	}

            }
            else
            {

                iqHall_BLDC_Is_fdb_pu = pAdcData->I.aiqValue[HallBLDC_getCurrentFdbIndex(hallBLDCHandle)];
                //iqHall_BLDC_Is_ref_pu = pCtlObj->iqSpd_out;
                iqHall_BLDC_Is_ref_pu =(CTRL_getFlag_enableSpeedCtrl(ctlHandle)) ?
                                     CTRL_getSpd_out_pu(ctlHandle)/10: CTRL_getIq_ref_pu(ctlHandle);
                pidHandle = HallBLDC_getPIDHandle(hallBLDCHandle);
                PID_setMinMax(pidHandle,-iqmaxVsMag,iqmaxVsMag);    // set the minimum and maximum values
                //PID_setMinMax(pidHandle,_IQ(-0.2),_IQ(0.2));    // set
                PID_run(pidHandle ,iqHall_BLDC_Is_ref_pu,iqHall_BLDC_Is_fdb_pu,&iqBLDCPwmDuty);
            }
            HALLBLDC_Ctrl_PwmSet(halHandle,hallBLDCHandle,iqBLDCPwmDuty,pPwmData);
            return;
        }
        else
        {

            // ***********************************
            // configure and run the Id controller

            // compute the Kp gain
            // Scale Kp instead of output to prevent saturation issues
            if(CTRL_getFlag_enableDcBusComp(ctlHandle))
                iqKp_Id = _IQmpy(iqKp_Id,EST_getOneOverDcBus_pu(pctlObj->estHandle));

            PID_setKp(pctlObj->pidHandle_Id,iqKp_Id);

            // compute the reference value
            iqrefValue = TRAJ_getIntValue(pctlObj->trajHandle_Id) + CTRL_getId_ref_pu(ctlHandle);

            // update the Id reference value
            EST_updateId_ref_pu(pctlObj->estHandle,&iqrefValue);

            // get the feedback value
            iqfbackValue = CTRL_getId_in_pu(ctlHandle);

            // set minimum and maximum for Id controller output
            iqoutMax = iqmaxVsMag;
            iqoutMin = -iqoutMax;

            // set the minimum and maximum values
            PID_setMinMax(pctlObj->pidHandle_Id,iqoutMin,iqoutMax);

            // run the Id PID controller
            PID_run(pctlObj->pidHandle_Id,iqrefValue,iqfbackValue,CTRL_getVd_out_addr(ctlHandle));

            // ***********************************
            // configure and run the Iq controller

            // compute the Kp gain
            // Scale Kp instead of output to prevent saturation issues
            if(CTRL_getFlag_enableDcBusComp(ctlHandle))
                iqKp_Iq = _IQmpy(iqKp_Iq,EST_getOneOverDcBus_pu(pctlObj->estHandle));

            PID_setKp(pctlObj->pidHandle_Iq,iqKp_Iq);

            // get the reference value
            if(CTRL_getFlag_enableSpeedCtrl(ctlHandle))
            {
                iqrefValue = CTRL_getSpd_out_pu(ctlHandle);
            }
            else
            {
                // get the Iq reference value
                iqrefValue = CTRL_getIq_ref_pu(ctlHandle);
            }

            // get the feedback value
            iqfbackValue = CTRL_getIq_in_pu(ctlHandle);

            // set minimum and maximum for Id controller output
            iqoutMax = _IQsqrt(_IQmpy(iqmaxVsMag,iqmaxVsMag) - _IQmpy(CTRL_getVd_out_pu(ctlHandle),CTRL_getVd_out_pu(ctlHandle)));
            iqoutMin = -iqoutMax;

            // set the minimum and maximum values
            PID_setMinMax(pctlObj->pidHandle_Iq,iqoutMin,iqoutMax);

            // run the Iq PID controller
            PID_run(pctlObj->pidHandle_Iq,iqrefValue,iqfbackValue,CTRL_getVq_out_addr(ctlHandle));
        }
    }

    {
        _iq iqangleComp_pu;

        // compensate angle delay
        iqangleComp_pu = CTRL_angleDelayComp(ctlHandle, iqangle_pu);

        // compute the sin/cos phasor
        CTRL_computePhasor(iqangleComp_pu,&phasor);
    }


   // set the phasor in the inverse Park transform
   IPARK_setPhasor(pctlObj->iparkHandle,&phasor);

   // run the inverse Park module
   IPARK_run(pctlObj->iparkHandle,CTRL_getVdq_out_addr(ctlHandle),CTRL_getVab_out_addr(ctlHandle));

   // run the space Vector Generator (SVGEN) module
   SVGEN_run(pctlObj->svgenHandle,CTRL_getVab_out_addr(ctlHandle),&(pPwmData->Tabc));


} // end of CTRL_runOnLine_User() function
// end of file
