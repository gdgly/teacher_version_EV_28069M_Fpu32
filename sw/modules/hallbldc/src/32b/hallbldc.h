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
#ifndef _HALLBLDC_H_
#define _HALLBLDC_H_

//! \file   modules/hallbldc/src/32b/hallbldc.h
//! \brief  Contains the public interface to the 
//!         Clarke transform (CLARKE) module routines
//!
//! (C) Copyright 2011, Texas Instruments, Inc.


// **************************************************************************
// the includes

// modules
#include "sw/modules/iqmath/src/32b/IQmathLib.h"
#include "sw/modules/math/src/32b/math.h"
#include "sw/modules/types/src/types.h"

#include "sw/modules/hal/boards/hvkit_rev1p1/f28x/f2806x/src/hal.h"

//#include "hal.h"
//#include "hal_obj.h"
//#include "sw/modules/hal/boards/hvkit_rev1p1/f28x/f2806x/src/hal.h"
//!
//!
//! \defgroup HALLBLDC HALLBLDC
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif


// **************************************************************************
// the defines



// **************************************************************************
// the typedefs

typedef enum
{
  BLDC_CW = 0,          //!< ClockWise
  BLDC_CCW  = 1         //!< CounterClockWise
} BLDC_DIR_e;


//! \brief Defines the CLARKE object
//!
typedef struct _HALLBLDC_Obj_
{
    uint_least8_t     u8PrevState_bin;     //!< the previous hall state for determining direction
    //_iq               iqAngle_pu;          //!< the pu angle of the current hall state

    uint_least8_t     u8SensorType;         //!< 120 deg or 60 deg
    uint_least8_t     u8AngleSelect;

    bool              bHall_Flag_EnableStartup;
    bool              bHall_Flag_EnableBldc;
    bool              bHall_Flag_State_Change;


    uint_least8_t     u8Hall_State;
    uint_least8_t     u8Hall_Bldc_Cnt, u8Hall_Fast_Cnt;

    uint_least8_t     u8Hall_BLDC_Flag_Is_fdb;
    int_least8_t      i8Hall_dir;

    uint32_t          u32Hall_timer, u32Hall_time_delta;


    PID_Handle        pidHandle_Bldc;                //!< the handle for the speed PID controller
    PID_Obj           pid_Bldc;                      //!< the speed PID controller object


    uint32_t          u32Hall_speed_scale;        // = USER_SYSTEM_FREQ_MHz*1000000*100/6;
    //uint32_t          u32Hall_speed_fdb_0p01Hz;   // = 0;
    _iq               iqHall_Speed_0p01hz_to_pu_sf;  // = _IQ((float_t)0.01/(USER_IQ_FULL_SCALE_FREQ_Hz));
    _iq               iqHall_speed_fdb_pu;         // = _IQ(0.0);
    _iq               iqgHall_BLDC_Is_ref_pu;       // = _IQ(0.0);
  //  _iq               iqHall_PwmDuty;               // = _IQ(0.0);

    // Coefficient for pid regulator switch from Hall to Fast
    _iq                iqHall2Fast_Spd_Coef;    // = _IQ(1.0);
    _iq                iqHall2Fast_Iq_coef;     // = _IQ(0.50);
    _iq                iqHall2Fast_Ui_coef;     // = _IQ(0.50);

    // Coefficient for pid regulator switch from Fast to Hall
    _iq               iqFast2Hall_Spd_coef; // = _IQ(1.0);
    _iq               iqFast2Hall_Iq_coef;  // = _IQ(0.0);
    _iq               iqFast2Hall_Ui_coef;  // = _IQ(2.0);

    //_iq               iqFast2Hall_Ui_coef;          // = _IQ(2.0);
   // _iq               iqFast2Hall_Spd_coef;         // = _IQ(1.0);
   // _iq               iqFast2Hall_Iq_coef;          // = _IQ(0.0);
    _iq               iqHall_speed_FastToBldc_low_pu;     //  = _IQ(0.400*USER_MOTOR_NUM_POLE_PAIRS*1000.0/(USER_IQ_FULL_SCALE_FREQ_Hz * 60.0));       // 400rpm
    _iq               iqHall_speed_BldcToFast_high_pu;    // = _IQ(0.750*USER_MOTOR_NUM_POLE_PAIRS*1000.0/(USER_IQ_FULL_SCALE_FREQ_Hz * 60.0));       // 750rpm

    _iq               iqAngle_pu, iqSpeed_pu;

    uint16_t           u16Hall_PwmCnt;      // = 0;
    uint16_t           u16Hall_PwmCntMax;   // = 5000;


   // GPIO_Number_e     HAL_HallGpioA, HAL_HallGpioB, HAL_HallGpioC;
    //HAL_Handle        halHandle;

} HALLBLDC_Obj;


//! \brief Defines the CLARKE handle
//!
typedef struct _HALLBLDC_Obj_ *HALLBLDC_Handle;


// **************************************************************************
// the globals


// **************************************************************************
// the function prototypes

//! \brief     Gets the angle of the hall feedback
//! \param[in] handle  The Hall Bldc handle
//! \return    The hall rotor angle
static inline _iq HALLBLDC_getAngle_pu(HALLBLDC_Handle handle)
{
	HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)handle;

	return(pHallBldcObj->iqAngle_pu);
} // end of HALLBLDC_getAngle_pu() function


//! \brief     Initializes the Hall BLDC transform module
//! \param[in] pMemory   A pointer to the memory for the Clarke object
//! \param[in] numBytes  The number of bytes allocated for the Clarke object, bytes
//! \return The Hall Bldc (HALLBLDC) object handle
extern HALLBLDC_Handle HALLBLDC_init(void *pMemory,const size_t numBytes);
extern void HallBLDC_setSpeedFasttoBldcLowPu(HALLBLDC_Handle hallBLDChandle, _iq iqSpeedPu);
extern void HallBLDC_setSpeedBldcToFastHighPu(HALLBLDC_Handle hallBLDChandle, _iq iqSpeedPu);
extern void HallBLDC_setHallSpeedScale(HALLBLDC_Handle hallBLDChandle, uint32_t u32SpeedScale);
extern void HallBLDC_setSpeed0p01hzToPuSf(HALLBLDC_Handle hallBLDChandle, _iq iqPuSf);
extern void HallBLDC_setPwmCntMax(HALLBLDC_Handle hallBLDChandle, uint16_t u16CntMax);
extern void HallBLDC_setEnableStartup(HALLBLDC_Handle hallBLDChandle, bool bHall_Flag_EnableStartup);
extern void HallBLDC_setEnableBldc(HALLBLDC_Handle hallBLDChandle, bool bHall_Flag_EnableBldc);


extern void HALLBLDC_setHallHandle(HALLBLDC_Handle hallBLDChandle, HAL_Handle hallHandle);

extern void HALLBLDC_Ctrl_Run(CTRL_Handle ctlHandle, HAL_Handle halHandle,HALLBLDC_Handle handleBldc);

//extern _iq HallBLDC_getSpeedPu(HALLBLDC_Handle hallBLDChandle);
//extern bool HallBLDC_getDoBLDC(HALLBLDC_Handle hallBLDChandle);
//extern uint_least8_t HallBLDC_getCurrentFdbIndex(HALLBLDC_Handle hallBLDChandle);

inline PID_Handle HallBLDC_getPIDHandle(HALLBLDC_Handle hallBLDChandle)
{
	HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
	return pHallBldcObj->pidHandle_Bldc;
}

inline _iq HallBLDC_getSpeedPu(HALLBLDC_Handle hallBLDChandle)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
    return pHallBldcObj->iqHall_speed_fdb_pu;
}

inline bool HallBLDC_getDoBLDC(HALLBLDC_Handle hallBLDChandle)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
    return pHallBldcObj->bHall_Flag_EnableStartup && pHallBldcObj->bHall_Flag_EnableBldc;
}

inline uint_least8_t HallBLDC_getCurrentFdbIndex(HALLBLDC_Handle hallBLDChandle)
{
    HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)hallBLDChandle;
    return pHallBldcObj->u8Hall_BLDC_Flag_Is_fdb;
}


//! \brief     Runs the hall bldc module for hall inputs
//! \param[in] handle  The hall bldc handle
//! \param[in] hallState     The hall state 1 thru 6, A phase hall is MSB, C phase hall is LSB
//! \param[in] pCurrentVec        The pointer to the current vector
static inline void HALLBLDC_run(HALLBLDC_Handle handle,const uint_least8_t u8hallState)  //,MATH_vec3 *pCurrentVec)
{
	HALLBLDC_Obj *pHallBldcObj = (HALLBLDC_Obj *)handle;


	switch(u8hallState)
	{
    	case 5:
    	{
    		if(pHallBldcObj->u8AngleSelect == 0)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(180.0/360.0);
    		}
    		else if(pHallBldcObj->u8AngleSelect == 1)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(240.0/360.0);
    		}

    		break;
    	}
    	case 4:
    	{
    		if(pHallBldcObj->u8AngleSelect == 0)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(240.0/360.0);
    		}
    		else if(pHallBldcObj->u8AngleSelect == 1)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(300.0/360.0);
    		}

    		break;
    	}
    	case 6:
    	{
    		if(pHallBldcObj->u8AngleSelect == 0)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(300.0/360.0);
    		}
    		else if(pHallBldcObj->u8AngleSelect == 1)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(0.0/360.0);
    		}

    		break;
    	}
    	case 2:
    	{
    		if(pHallBldcObj->u8AngleSelect == 0)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(0.0/360.0);
    		}
    		else if(pHallBldcObj->u8AngleSelect == 1)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(60.0/360.0);
    		}

    		break;
    	}
    	case 3:
    	{
    		if(pHallBldcObj->u8AngleSelect == 0)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(60.0/360.0);
    		}
    		else if(pHallBldcObj->u8AngleSelect == 1)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(120.0/360.0);
    		}

    		break;
    	}
    	case 1:
    	{
    		if(pHallBldcObj->u8AngleSelect == 0)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(120.0/360.0);
    		}
    		else if(pHallBldcObj->u8AngleSelect == 1)
    		{
    		    pHallBldcObj->iqAngle_pu = _IQ(180.0/360.0);
    		}

    		break;
    	}
	}

	pHallBldcObj->u8PrevState_bin = u8hallState;

  
  return;
} // end of HALLBLDC_run() function


extern void HALLBLDC_State_Check(HAL_Handle halHandle, HALLBLDC_Handle handleBldc);
extern void HALLBLDC_Ctrl_Stop(CTRL_Handle ctlHandle, HALLBLDC_Handle handleBldc);
extern void HALLBLDC_Ctrl_PwmSet(  HAL_Handle halHandle, HALLBLDC_Handle handleBldc,
                                  _iq iqPwmDuty,HAL_PwmData_t *pPwmData);

#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _HALLBLDC_H_ definition

