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
#ifndef _DATALOG_H_
#define _DATALOG_H_

//! \file   modules/datalog/src/32b/datalog.h
//! \brief  Contains the public interface to the 
//!         data logging (DATALOG) module routines
//!
//! (C) Copyright 2011, Texas Instruments, Inc.


// **************************************************************************
// the includes

#include "sw/modules/math/src/32b/math.h"
#include "sw/modules/types/src/types.h"



//!
//!
//! \defgroup DATALOG DATALOG
//!
//@{
// Include the algorithm overview defined in modules/<module>/docs/doxygen/doxygen.h
//! \defgroup DATALOG_OVERVIEW


#ifdef __cplusplus
extern "C" {
#endif


// **************************************************************************
// the defines

//! \brief Defines the maximum buffer arrays number
//!
#define DATA_LOG_BUFF_NUM		3

//! \brief Defines the maximum buffer memory length
//!
#define DATA_LOG_BUFF_SIZE		400


//! \brief Defines the default initialization for the DATALOG object
//!
                     
#define DATALOG_DEFAULTS {        \
                            {NULL,  \
							 NULL,  \
                             NULL }, \
							true, \
							false, \
                            0, \
                            0xC0 \
	}



// **************************************************************************
// the typedefs
 
//! \brief Defines the data logging (DATALOG) object
//!
typedef struct _DATALOG_OBJ_
{    
  int32_t  *api32Ptr[DATA_LOG_BUFF_NUM];        //!< Input: First input pointer

  bool  bFlag_EnableLogData;					//!< Parameter: Maximum data buffer
  bool  bFlag_EnableLogOneShot;				//!< Parameter: Maximum data buffer
  int16_t  i16Cntr;          					//!< Variable:  Data log counter
  int16_t  i16Size;          					//!< Parameter: Maximum data buffer
} DATALOG_Obj;
                                                         

//! \brief Defines the DATALOG handle
//!
typedef struct _DATALOG_Obj_   *DATALOG_Handle;
                                                         
// **************************************************************************
// the globals
extern int32_t ai32DatalogBuff1[DATA_LOG_BUFF_SIZE];
extern int32_t ai32DatalogBuff2[DATA_LOG_BUFF_SIZE];
extern int32_t ai32DatalogBuff3[DATA_LOG_BUFF_SIZE];

//! \brief Defines the DATALOG object
//!
extern DATALOG_Obj gDatalog;
extern DATALOG_Handle gDatalogHandle;

// **************************************************************************
// the globals


// **************************************************************************
// the function prototypes


//! \brief     Initializes the data logger
//! \param[in] ptr  The pointer to memory
extern DATALOG_Handle DATALOG_init(void *pMemory,const size_t numBytes);


//! \brief     Updates the data logger
//! \param[in] ptr  The pointer to memory
static inline DATALOG_update(DATALOG_Handle handle)
{
	DATALOG_Obj *pObj = (DATALOG_Obj *)handle;

	if(pObj->bFlag_EnableLogData == true)
	{
	    int16_t i16Cnt = pObj->i16Cntr;
	    ai32DatalogBuff1[i16Cnt] = (*pObj->api32Ptr[0]);
		ai32DatalogBuff2[i16Cnt] = (*pObj->api32Ptr[1]);
		ai32DatalogBuff3[i16Cnt] = (*pObj->api32Ptr[2]);

		if(++i16Cnt >= pObj->i16Size)
		{
		    i16Cnt = 0;

			if(pObj->bFlag_EnableLogOneShot == true)
			{
			    pObj->bFlag_EnableLogData = false;
			    pObj->bFlag_EnableLogOneShot = false;
			}
		}
		pObj->i16Cntr = i16Cnt++;
	}
	else if(pObj->bFlag_EnableLogOneShot == true)
	{
	    pObj->bFlag_EnableLogData = true;
	    pObj->i16Cntr = 0;
	}
}
                                                    
#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _DATALOG_H_ definition

