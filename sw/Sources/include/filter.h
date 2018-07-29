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
#ifndef _FILTER__H_
#define _FILTER__H_

//! \file   modules/filter/src/32b/filter_fo.h
//! \brief  Contains the public interface to the 
//!         first-order filter (FILTER_FO) module routines
//!
//! (C) Copyright 2012, Texas Instruments, Inc.


// **************************************************************************
// the includes

#include "sw/modules/types/src/types.h"
#include "sw/modules/filter/src/32b/filter_fo.h"

#include "sw/modules/iqmath/src/32b/IQmathLib.h"


//!
//!
//! \defgroup FILTER_FO FILTER_FO
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif


// **************************************************************************
// the defines


// **************************************************************************
// the typedefs
 
//! \brief Defines the first-order filter (FILTER_FO) object
//!
typedef struct _FILTER_
{
    FILTER_FO_Obj   filerFoObj;
    FILTER_FO_Handle   filerFoHandle;
    //_iq             *piqInput;
    _iq             iqValue;
} FILTER_Obj;


//! \brief Defines the first-order filter (FILTER_FO) handle
//!
typedef struct _FILTER_Obj_  *FILTER_Handle;
                                                         

//! \brief     Initializes the first-order filter
//! \param[in] pMemory   A pointer to the memory for the first-order filter object
//! \param[in] numBytes  The number of bytes allocated for the first-order filter object, bytes
//! \return The filter (FILTER) object handle
extern FILTER_Handle FILTER_init(void *pMemory,const size_t numBytes);
extern void FILTER_setupCoeffs(FILTER_Handle filterHandle, float fFilterFreqHz,float fPole_rps);
inline _iq FILTER_getValue(FILTER_Handle handle)
{
    FILTER_Obj *pObj = (FILTER_Obj *)handle;
    return(pObj->iqValue);
} // end of FILTER_FO_get_a1() function



inline void FILTER_run(FILTER_Handle handle, _iq iqInputValue)
{
    FILTER_Obj *pObj = (FILTER_Obj *)handle;
    //FILTER_FO_Handle filerFoHandle = (FILTER_FO_Handle) &(pObj->filerFoObj);
    pObj->iqValue = FILTER_FO_run(pObj->filerFoHandle,iqInputValue );

    //pObj->iqValue  = iqInputValue;
} // end of FILTER_FO_run() function


#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _FILTER_FO_H_ definition


