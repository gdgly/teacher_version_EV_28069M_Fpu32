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
//! \file   modules/hallbldc/src/32b/hallbldc.c
//! \brief  Portable C fixed point code.  These functions define the
//!         Clarke transform module routines
//!
//! (C) Copyright 2011, Texas Instruments, Inc.


// **************************************************************************
// the includes

#include "sw/modules/hallbldc/src/32b/hallbldc.h"
#include "sw/modules/pid/src/32b/pid.h"

//#include "sw/modules/hal/boards/hvkit_rev1p1/f28x/f2806x/src/hal.h"
#include "sw/modules/ctrl/src/32b/ctrl_obj.h"
#ifndef QEP
#include "sw/modules/ctrl/src/32b/ctrl.h"
#else
#include "sw/modules/ctrl/src/32b/ctrlQEP.h"
#endif



// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions

HALLBLDC_Handle HALLBLDC_init(void *pMemory,const size_t numBytes)      //using timer#2
{
	HALLBLDC_Handle handle;

	if(numBytes < sizeof(HALLBLDC_Obj))
	  return((HALLBLDC_Handle)NULL);

	// assign the handle
	handle = (HALLBLDC_Handle)pMemory;

	HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)handle;

	pHallBldcObj->pidHandle_Bldc = PID_init(&(pHallBldcObj->pid_Bldc),sizeof(pHallBldcObj->pid_Bldc));

	pHallBldcObj->u32Hall_timer = 0;
	pHallBldcObj->u32Hall_time_delta = 0;
	pHallBldcObj->u8Hall_State = 0;
	//pHallBldcObj->i8Hall_dir = 1;
	pHallBldcObj->u16Hall_PwmCnt = 0;
	pHallBldcObj->u8Hall_Bldc_Cnt = 0;

	return handle;
} // end of HALLBLDC_init() function

/*void HALLBLDC_setHallHandle(HALLBLDC_Handle hallBLDChandle, HAL_Handle hallHandle)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
    pHallBldcObj->halHandle = hallHandle;
}*/

void HallBLDC_setSpeedFasttoBldcLowPu(HALLBLDC_Handle hallBLDChandle, _iq iqSpeedPu)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
    pHallBldcObj->iqHall_speed_FastToBldc_low_pu = iqSpeedPu;
}

void HallBLDC_setSpeedBldcToFastHighPu(HALLBLDC_Handle hallBLDChandle, _iq iqSpeedPu)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
    pHallBldcObj->iqHall_speed_BldcToFast_high_pu = iqSpeedPu;
}

void HallBLDC_setHallSpeedScale(HALLBLDC_Handle hallBLDChandle, uint32_t u32SpeedScale)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
    pHallBldcObj->u32Hall_speed_scale = u32SpeedScale;
}

void HallBLDC_setSpeed0p01hzToPuSf(HALLBLDC_Handle hallBLDChandle, _iq iqPuSf)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
    pHallBldcObj->iqHall_Speed_0p01hz_to_pu_sf = iqPuSf;
}

void HallBLDC_setPwmCntMax(HALLBLDC_Handle hallBLDChandle, uint16_t u16CntMax)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
    pHallBldcObj->u16Hall_PwmCntMax = u16CntMax;
}

void HallBLDC_setEnableStartup(HALLBLDC_Handle hallBLDChandle, bool bHall_Flag_EnableStartup)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
    pHallBldcObj->bHall_Flag_EnableStartup = bHall_Flag_EnableStartup;
}

void HallBLDC_setEnableBldc(HALLBLDC_Handle hallBLDChandle, bool bHall_Flag_EnableBldc)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
    pHallBldcObj->bHall_Flag_EnableBldc = bHall_Flag_EnableBldc;
}




int_least8_t gai8Hall_PwmIndex[8] = {4, 1, 5, 6, 3, 2, 4, 1};
_iq   giqHall_PwmDuty;

//! \brief using timer#2
void HALLBLDC_State_Check( HAL_Handle halHandle, HALLBLDC_Handle handleBldc)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)handleBldc;

    // Hall_A, Hall_B, Hall_C
    uint_least8_t u8Hall_GpioData;
    u8Hall_GpioData  = (~HAL_readGpio(halHandle, (GPIO_Number_e)HAL_HallGpio_C) & 0x1) << 2;   //CAP1->J10/J4_1->green
    u8Hall_GpioData += (~HAL_readGpio(halHandle, (GPIO_Number_e)HAL_HallGpio_B) & 0x1) << 1;   //CAP2->J10/J4_2->green&white
    u8Hall_GpioData += (~HAL_readGpio(halHandle, (GPIO_Number_e)HAL_HallGpio_A) & 0x1);        //CAP3->J10/J4_3->gray&white

    //pHallBldcObj->u8Hall_State = u8Hall_GpioData;

    if (u8Hall_GpioData != pHallBldcObj-> u8Hall_State)
    {
        uint32_t u32Hall_timer_now, u32Hall_time_delta_now, u32Hall_time_delta;

        u32Hall_timer_now = HAL_readTimerCnt(halHandle,2);
        u32Hall_time_delta_now = pHallBldcObj->u32Hall_timer- u32Hall_timer_now;
        pHallBldcObj->u32Hall_timer = u32Hall_timer_now;

        u32Hall_time_delta = (u32Hall_time_delta_now + pHallBldcObj->u32Hall_time_delta)>>1;
        pHallBldcObj->u32Hall_time_delta = u32Hall_time_delta;

        uint32_t    u32Hall_speed_fdb_0p01Hz;   // = 0;
        u32Hall_speed_fdb_0p01Hz = pHallBldcObj->u32Hall_speed_scale/u32Hall_time_delta;
        pHallBldcObj->iqHall_speed_fdb_pu = u32Hall_speed_fdb_0p01Hz * pHallBldcObj->iqHall_Speed_0p01hz_to_pu_sf;

//      if(TRAJ_getIntValue(trajHandle_spd) < _IQ(0.0))
//      {
//          gHall_speed_fdb_pu = -gHall_speed_fdb_pu;
//      }

        //direction check
        int_least8_t i8Hall_State_delta = gai8Hall_PwmIndex[u8Hall_GpioData] - gai8Hall_PwmIndex[pHallBldcObj->u8Hall_State];
        int_least8_t i8Hall_dir;
        if ((i8Hall_State_delta == -1) || (i8Hall_State_delta == 5))
        {
            i8Hall_dir = 1;      // positive direction
        }
        else if((i8Hall_State_delta == 1) || (i8Hall_State_delta == -5))
        {
            i8Hall_dir = 2;      // negative direction
            pHallBldcObj->iqHall_speed_fdb_pu = -pHallBldcObj->iqHall_speed_fdb_pu;
        }
        else
        {
            i8Hall_dir = 0;      // direction change ??
        }

        //check if dirction is chagned.
        //if direction is changed, speed feedback is reset.
        if(i8Hall_dir != pHallBldcObj->i8Hall_dir)
        {
            pHallBldcObj->iqHall_speed_fdb_pu = _IQ(0.0);
            //pHallBldcObj->bHall_dir_change = 1;  //??
        }

        //gHall_LastState = gHall_PrevState;      //??
        //gHall_PrevState = gHall_State;
        pHallBldcObj-> u8Hall_State = u8Hall_GpioData;  //
        pHallBldcObj-> i8Hall_dir = i8Hall_dir;

        pHallBldcObj->bHall_Flag_State_Change = true;
        pHallBldcObj->u16Hall_PwmCnt = 0;
    }
    else
    {
        if(++pHallBldcObj->u16Hall_PwmCnt > pHallBldcObj->u16Hall_PwmCntMax)
        {
            pHallBldcObj->u16Hall_PwmCnt = 0;
            pHallBldcObj->iqHall_speed_fdb_pu = _IQ(0.0);
        }
    }

}


void HALLBLDC_Ctrl_Run(CTRL_Handle ctlHandle, HAL_Handle halHandle,HALLBLDC_Handle handleBldc)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)handleBldc;
    HAL_Obj *pHallObj = ( HAL_Obj  *) halHandle;
    CTRL_Obj *pCtlObj = (CTRL_Obj *) ctlHandle;

    if (pHallBldcObj->bHall_Flag_EnableStartup)
    {
        if (pHallBldcObj->bHall_Flag_EnableBldc)
        {
            //Hall to Fast
            if(_IQabs(pHallBldcObj->iqHall_speed_fdb_pu) > pHallBldcObj->iqHall_speed_BldcToFast_high_pu)            // Hall to FAST
            {
                if(++pHallBldcObj->u8Hall_Fast_Cnt > 20)
                {
                    pHallBldcObj->u8Hall_Fast_Cnt = 0;
                    pHallBldcObj->bHall_Flag_EnableBldc = false;
                    if (CTRL_getFlag_enableSpeedCtrl(ctlHandle) == false)    //Torque Control Mode
                    {
                        // The following instructions load the parameters for the speed PI
                        // controller.
                        //PID_setGains(pCtlObj->pidHandle_spd,_IQ(2.0),_IQ(0.02),_IQ(0.0));
                        // Set the initial condition value for the integrator output to 0, Id
                        //PID_setUi(pCtlObj->pidHandle_Id ,_IQ(0.0));
                        // Set the initial condition value for the integrator output to 0, Iq
                        //PID_setUi(pCtlObj->pidHandle_Iq,_IQmpy(PID_getUi(pHallBldcObj->pidHandle_Bldc), _IQ(0.25)));
                     }
                     else              // speed control
                     {
                        // The following instructions load the parameters for the speed PI
                        // controller.
                        //PID_setGains(pCtlObj->pidHandle_spd,_IQ(2.0),_IQ(0.02),_IQ(0.0));
                        // Set the initial condition value for the integrator output to 0, speed
                        //PID_setUi(pCtlObj->pidHandle_spd, _IQmpy(giqHall_PwmDuty, pHallBldcObj->iqHall2Fast_Spd_Coef));

                        // Set the initial condition value for the integrator output to 0, Iq
                        //PID_setUi(pCtlObj->pidHandle_Iq, _IQmpy(giqHall_PwmDuty, pHallBldcObj->iqHall2Fast_Iq_coef));

                        // Set the initial condition value for the integrator output to 0, Id
                        //PID_setUi(pCtlObj->pidHandle_Id,_IQmpy(giqHall_PwmDuty, _IQ(0.0)));
                      }
                      PWM_setSocAPulseSrc(pHallObj->pwmHandle[PWM_Number_1],PWM_SocPulseSrc_CounterEqualZero);
                      HAL_enablePwm(halHandle);
                }

            }
            else
                pHallBldcObj->u8Hall_Fast_Cnt = 0;

        }
        else //pHallBldcObj->bHall_Flag_EnableBldc == false
        {
            // Fast to Hall
            _iq iqSpeed_est_pu = EST_getFm_pu(pCtlObj->estHandle);
            if(_IQabs(iqSpeed_est_pu)  < pHallBldcObj->iqHall_speed_FastToBldc_low_pu)   // FAST to Hall
            {
                if(++pHallBldcObj->u8Hall_Bldc_Cnt > 20)
                {
                    pHallBldcObj->u8Hall_Bldc_Cnt = 0;
                    pHallBldcObj->bHall_Flag_EnableBldc = true;
                    if ( CTRL_getFlag_enableSpeedCtrl(ctlHandle) == false)    //Torque Control Mode
                    {
                        // The following instructions load the parameters for the speed PI
                        // controller.
                        //PID_setGains(pCtlObj->pidHandle_spd,_IQ(0.1),_IQ(0.005),_IQ(0.0));

                        // Set the initial condition value for the integrator output to 0
                        PID_setUi(pHallBldcObj->pidHandle_Bldc, _IQmpy(PID_getUi(pCtlObj->pidHandle_Iq), pHallBldcObj->iqFast2Hall_Ui_coef));
                    }
                    else        // Speed Control Mode
                    {
                        // The following instructions load the parameters for the speed PI
                        // controller.
                        //PID_setGains(pCtlObj->pidHandle_spd,_IQ(0.1),_IQ(0.005),_IQ(0.0));

                        //giqHall_PwmDuty = PID_getUi(pCtlObj->pidHandle_Iq); //   pid[2].Ui;

                        // Set the initial condition value for the integrator output to 0
                        //PID_setUi(pCtlObj->pidHandle_spd, _IQmpy(PID_getUi(pCtlObj->pidHandle_Iq), pHallBldcObj->iqFast2Hall_Spd_coef));
                    }

                }

            }
            else
                pHallBldcObj->u8Hall_Bldc_Cnt =0;
        }

    }
    else    //pHallBldcObj->bHall_Flag_EnableStartup
    {

    }

}

//! \brief
void HALLBLDC_Ctrl_Stop(CTRL_Handle ctlHandle,HALLBLDC_Handle handleBldc)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)handleBldc;
    CTRL_Obj *pCtlObj = (CTRL_Obj *)ctlHandle;

    pHallBldcObj->bHall_Flag_EnableBldc = true;
    pHallBldcObj->bHall_Flag_State_Change = false;
    pHallBldcObj->iqHall_speed_fdb_pu = _IQ(0.0);

    // Set the initial condition value for the integrator output to 0
    PID_setUi(pCtlObj->pidHandle_spd,_IQ(0.0));
    PID_setUi(pCtlObj->pidHandle_Id,_IQ(0.0));
    PID_setUi(pCtlObj->pidHandle_Iq,_IQ(0.0));
    PID_setUi(pHallBldcObj->pidHandle_Bldc,_IQ(0.0));
}

//! \brief
void HALLBLDC_Ctrl_PwmSet(  HAL_Handle halHandle,HALLBLDC_Handle handleBldc,
                          _iq iqPwmDuty,HAL_PwmData_t *pPwmData)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)handleBldc;
    HAL_Obj *pHalObj = ( HAL_Obj  *) halHandle;

    int_least8_t i8Hall_PwmState = gai8Hall_PwmIndex[pHallBldcObj->u8Hall_State];
    //iqPwmDuty = _IQ(0.1);

    switch (i8Hall_PwmState) //   i8PwmState)
    {
        case 0:     // V+/W-
        {
            PWM_setOneShotTrip(pHalObj->pwmHandle[PWM_Number_1]);
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_2]);
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_3]);

            pPwmData->Tabc.aiqValue[0] = _IQ(0.0);
            pPwmData->Tabc.aiqValue[1] = iqPwmDuty;
            pPwmData->Tabc.aiqValue[2] = -iqPwmDuty;

/*
            pPwmData->Tabc.aiqValue[0] = _IQ(0.0);
            pPwmData->Tabc.aiqValue[1] = -iqPwmDuty;
            pPwmData->Tabc.aiqValue[2] = iqPwmDuty;
*/

            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 1;
            break;
        }
        case 1:     // U+/W-
        {
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_1]);
            PWM_setOneShotTrip(pHalObj->pwmHandle[PWM_Number_2]);
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_3]);

            pPwmData->Tabc.aiqValue[0] = iqPwmDuty;
            pPwmData->Tabc.aiqValue[1] = _IQ(0.0);
            pPwmData->Tabc.aiqValue[2] = -iqPwmDuty;

/*
            pPwmData->Tabc.aiqValue[0] = -iqPwmDuty;
            pPwmData->Tabc.aiqValue[1] = _IQ(0.0);
            pPwmData->Tabc.aiqValue[2] = iqPwmDuty;
*/

            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 0;
//            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 2;
            break;
        }
        case 2:     //U+/V-
        {
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_1]);
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_2]);
            PWM_setOneShotTrip(pHalObj->pwmHandle[PWM_Number_3]);

            pPwmData->Tabc.aiqValue[0] = iqPwmDuty;
            pPwmData->Tabc.aiqValue[1] = -iqPwmDuty;
            pPwmData->Tabc.aiqValue[2] = _IQ(0.0);

/*
            pPwmData->Tabc.aiqValue[0] = -iqPwmDuty;
            pPwmData->Tabc.aiqValue[1] = iqPwmDuty;
            pPwmData->Tabc.aiqValue[2] = _IQ(0.0);
*/

            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 0;
//            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 1;
            break;
        }
        case 3:     //W+/V-
        {
            PWM_setOneShotTrip(pHalObj->pwmHandle[PWM_Number_1]);
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_2]);
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_3]);

            pPwmData->Tabc.aiqValue[0] = _IQ(0.0);
            pPwmData->Tabc.aiqValue[1] = -iqPwmDuty;
            pPwmData->Tabc.aiqValue[2] = iqPwmDuty;

/*
            pPwmData->Tabc.aiqValue[0] = _IQ(0.0);
            pPwmData->Tabc.aiqValue[1] = iqPwmDuty;
            pPwmData->Tabc.aiqValue[2] = -iqPwmDuty;
*/


            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 2;
//            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 1;
            break;
        }
        case 4:     //W+/U-
        {
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_1]);
            PWM_setOneShotTrip(pHalObj->pwmHandle[PWM_Number_2]);
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_3]);

            pPwmData->Tabc.aiqValue[0] = -iqPwmDuty;
            pPwmData->Tabc.aiqValue[1] = _IQ(0.0);
            pPwmData->Tabc.aiqValue[2] = iqPwmDuty;

/*
            pPwmData->Tabc.aiqValue[0] = iqPwmDuty;
            pPwmData->Tabc.aiqValue[1] = _IQ(0.0);
            pPwmData->Tabc.aiqValue[2] = -iqPwmDuty;
*/

            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 2;
//            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 0;
            break;
        }
        case 5:     // V+/U-
        {
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_1]);
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_2]);
            PWM_setOneShotTrip(pHalObj->pwmHandle[PWM_Number_3]);

            pPwmData->Tabc.aiqValue[0] = -iqPwmDuty;
            pPwmData->Tabc.aiqValue[1] = iqPwmDuty;
            pPwmData->Tabc.aiqValue[2] = _IQ(0.0);

/*
            pPwmData->Tabc.aiqValue[0] = iqPwmDuty;
            pPwmData->Tabc.aiqValue[1] = -iqPwmDuty;
            pPwmData->Tabc.aiqValue[2] = _IQ(0.0);
*/

            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 1;
//            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 0;
            break;
        }
        case 6:     // V+/W-
        {
            PWM_setOneShotTrip(pHalObj->pwmHandle[PWM_Number_1]);
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_2]);
            PWM_clearOneShotTrip(pHalObj->pwmHandle[PWM_Number_3]);

            pPwmData->Tabc.aiqValue[0] = _IQ(0.0);

            pPwmData->Tabc.aiqValue[1] = iqPwmDuty;
            pPwmData->Tabc.aiqValue[2] = -iqPwmDuty;

/*
            pPwmData->Tabc.aiqValue[0] = _IQ(0.0);
            pPwmData->Tabc.aiqValue[1] = -iqPwmDuty;
            pPwmData->Tabc.aiqValue[2] = iqPwmDuty;
*/

            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 1;
//            pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb = 2;
            break;
        }
        default:    // N/A
            break;
    }
}
