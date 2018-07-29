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


// platforms
#include "sw/modules/ctrl/src/32b/ctrlQEP.h"
#include "sw/modules/hal/boards/hvkit_rev1p1/f28x/f2806x/src/hal.h"
//#include "hal.h"
#include "sw/modules/user/src/32b/user.h"
#include "include/i2c_mcp23017.h"



// **************************************************************************
// the defines
// **************************************************************************
// the globals

#define IOEXPAND_DEVICE_ADDRESS 0x20

#define IOEXPAND_IODIRA		0x00
#define IOEXPAND_IODIRB		0x01
#define IOEXPAND_PULLUPA	0x0c
#define IOEXPAND_PULLUPB	0x0d
#define IOEXPAND_GPIOA		0x12
#define IOEXPAND_GPIOB		0x13
#define IOEXPAND_OLATA		0x14
#define IOEXPAND_OLATB		0x15

//uint_least8_t 	gu16Capacity,gu16OutputTerms;
//uint16_t	    gu16InputTerms;

//bool bSetupFlag = false;

IOEXPAND_Handle IOEXPAND_init(void *pMemory,const size_t numBytes)
{
	IOEXPAND_Handle ioexpandHandle;
	IOEXPAND23017_Obj *pIOexpand;

	if (numBytes < sizeof(IOEXPAND23017_Obj))
		return((IOEXPAND_Handle)NULL);

	// assign the handle
	ioexpandHandle = (IOEXPAND_Handle)pMemory;
	pIOexpand = (IOEXPAND23017_Obj *)pMemory;
	pIOexpand->bSetupComplete = false;
	return(ioexpandHandle);
}

void IOEXPAND_setup(IOEXPAND_Handle ioexpandHandle, I2CMessage_Handle i2cMessageHandle)
{
	IOEXPAND23017_Obj *pIOexpand;
	pIOexpand = (IOEXPAND23017_Obj *) ioexpandHandle;
	pIOexpand->i2cMessageHandle = i2cMessageHandle;
	//pIOexpand->u8InputTermA = pIOexpand->u8InputTermB = 0;

	IOEXPAND_clrTrip(ioexpandHandle);
	IOEXPAND_WriteRegister8(ioexpandHandle, IOEXPAND_IODIRA,
	                       IOEXAND_INPUT_Bit7 | IOEXAND_INPUT_Bit6 | IOEXAND_INPUT_Bit5 | IOEXAND_INPUT_Bit4 |
	                       IOEXAND_INPUT_Bit3  | IOEXAND_INPUT_Bit2  | IOEXAND_INPUT_Bit1  | IOEXAND_INPUT_Bit0);   //default input
	IOEXPAND_WriteRegister8(ioexpandHandle, IOEXPAND_IODIRB,
	                        IOEXAND_OUTPUT_Bit7 | IOEXAND_OUTPUT_Bit6 | IOEXAND_OUTPUT_Bit5 | IOEXAND_OUTPUT_Bit4 |
	                        IOEXAND_INPUT_Bit3  | IOEXAND_INPUT_Bit2  | IOEXAND_INPUT_Bit1  | IOEXAND_INPUT_Bit0);

	IOEXPAND_WriteRegister8(ioexpandHandle,IOEXPAND_PULLUPA,
	                        IOEXAND_PullUp_Bit7 | IOEXAND_PullUp_Bit6 | IOEXAND_PullUp_Bit5 | IOEXAND_PullUp_Bit4 |
	                        IOEXAND_PullUp_Bit3 | IOEXAND_PullUp_Bit2 | IOEXAND_PullUp_Bit1 | IOEXAND_PullUp_Bit0 );

	IOEXPAND_WriteRegister8(ioexpandHandle,IOEXPAND_PULLUPB,
	                        IOEXAND_PullUp_Bit7 | IOEXAND_PullUp_Bit6 | IOEXAND_PullUp_Bit5 | IOEXAND_PullUp_Bit4 |
	                        IOEXAND_PullUp_Bit3 | IOEXAND_PullUp_Bit2 | IOEXAND_PullUp_Bit1 | IOEXAND_PullUp_Bit0 );

	//IOEXPAND_WriteRegister8(ioexpandHandle,IOEXPAND_PULLUPA,
	//                       IOEXAND_PullDisable_Bit0 | IOEXAND_PullDisable_Bit1 | IOEXAND_PullDisable_Bit2 | IOEXAND_PullDisable_Bit3 |
	 //                       IOEXAND_PullDisable_Bit4 | IOEXAND_PullDisable_Bit5 | IOEXAND_PullDisable_Bit6 | IOEXAND_PullDisable_Bit7 );

	//IOEXPAND_WriteRegister8(ioexpandHandle,IOEXPAND_PULLUPB,
	//                        IOEXAND_PullDisable_Bit0 | IOEXAND_PullDisable_Bit1 | IOEXAND_PullDisable_Bit2 | IOEXAND_PullDisable_Bit3 |
	//                        IOEXAND_PullDisable_Bit4 | IOEXAND_PullDisable_Bit5 | IOEXAND_PullDisable_Bit6 | IOEXAND_PullDisable_Bit7 );

	IOEXPAND_setLow(ioexpandHandle,IOEXPAND_Term4 |IOEXPAND_Term5 | IOEXPAND_Term6 |IOEXPAND_Term7);
	IOEXPAND_getInputs(ioexpandHandle);

	pIOexpand->bSetupComplete = true;
	//bSetupFlag = true;
}



bool IOEXPAND_WriteRegister16(IOEXPAND_Handle ioexpandHandle, uint_least8_t u8RegAdd, uint16_t u16Value)
{
    //if (!IOEXPAND_getTripStatus(ioexpandHandle))
    {
        IOEXPAND23017_Obj *pIOexpand = (IOEXPAND23017_Obj *) ioexpandHandle;
        I2C_Message *pI2cMessage = (I2C_Message *) pIOexpand->i2cMessageHandle;

        pI2cMessage->u16SlaveAddress = IOEXPAND_DEVICE_ADDRESS;
        pI2cMessage->au8TxBuffer[0] =  u8RegAdd & 0x00ff;
        pI2cMessage->au8TxBuffer[1] =  u16Value >> 8;
        pI2cMessage->au8TxBuffer[2] =  u16Value & 0x00ff;
        pI2cMessage->u8TxNumOfBytes = 3;
        pI2cMessage->u8RxNumOfBytes = 0;

        if (I2CMessage_WriteMessage(pI2cMessage))
        	IOEXPAND_clrTrip(ioexpandHandle);
        else
            IOEXPAND_setTrip(ioexpandHandle);
    }
    return IOEXPAND_getTripStatus(ioexpandHandle);
}

bool IOEXPAND_WriteRegister8(IOEXPAND_Handle ioexpandHandle, uint_least8_t u8RegAdd, uint_least8_t u8Value)
{
    //if (!IOEXPAND_getTripStatus(ioexpandHandle))
    {
        IOEXPAND23017_Obj *pIOexpand = (IOEXPAND23017_Obj *) ioexpandHandle;
        I2C_Message *pI2cMessage = (I2C_Message *) pIOexpand->i2cMessageHandle;

        pI2cMessage->u16SlaveAddress = IOEXPAND_DEVICE_ADDRESS;
        pI2cMessage->au8TxBuffer[0] =  u8RegAdd & 0x00ff;
        pI2cMessage->au8TxBuffer[1] =  u8Value & 0x00ff;
        pI2cMessage->u8TxNumOfBytes = 2;
        pI2cMessage->u8RxNumOfBytes = 0;

        if (I2CMessage_WriteMessage(pI2cMessage))
           IOEXPAND_clrTrip(ioexpandHandle);
        else
           IOEXPAND_setTrip(ioexpandHandle);
    }
    return IOEXPAND_getTripStatus(ioexpandHandle);

}


bool IOEXPAND_ReadRegister16(IOEXPAND_Handle ioexpandHandle,uint_least8_t u8RegAdd, uint16_t *pu16Value)
{
    //if (!IOEXPAND_getTripStatus(ioexpandHandle))
    {
        IOEXPAND23017_Obj *pIOexpand = (IOEXPAND23017_Obj *) ioexpandHandle;
        I2C_Message *pI2cMessage = (I2C_Message *) pIOexpand->i2cMessageHandle;

        //pI2cMessage->pau16DataBuffer = pu16Value;
        pI2cMessage->u16SlaveAddress = IOEXPAND_DEVICE_ADDRESS;
        pI2cMessage->au8TxBuffer[0] =  u8RegAdd & 0x00ff;

        pI2cMessage->u8TxNumOfBytes = 1;
        pI2cMessage->u8RxNumOfBytes = 2;

        if (I2CMessage_ReadMessage(pI2cMessage))
            IOEXPAND_clrTrip(ioexpandHandle);
        else
            IOEXPAND_setTrip(ioexpandHandle);
        *pu16Value = ( pI2cMessage->au8RxBuffer[0] << 8) | (pI2cMessage->au8RxBuffer[1]) ;
    }
    return IOEXPAND_getTripStatus(ioexpandHandle);
}

bool IOEXPAND_ReadRegister8(IOEXPAND_Handle ioexpandHandle,uint_least8_t u8RegAdd, uint_least8_t *pu8Value)
{
    //if (!IOEXPAND_getTripStatus(ioexpandHandle))
    {
        IOEXPAND23017_Obj *pIOexpand = (IOEXPAND23017_Obj *) ioexpandHandle;
        I2C_Message *pI2cMessage = (I2C_Message *) pIOexpand->i2cMessageHandle;

        //pI2cMessage->pau16DataBuffer = pu16Value;
        pI2cMessage->u16SlaveAddress = IOEXPAND_DEVICE_ADDRESS;
        pI2cMessage->au8TxBuffer[0] =  u8RegAdd & 0x00ff;

        pI2cMessage->u8TxNumOfBytes = 1;
        pI2cMessage->u8RxNumOfBytes = 1;

        if (I2CMessage_ReadMessage(pI2cMessage))
            IOEXPAND_clrTrip(ioexpandHandle);
        else
            IOEXPAND_setTrip(ioexpandHandle);
        *pu8Value =  pI2cMessage->au8RxBuffer[0] & 0x00ff ;

    }
    return IOEXPAND_getTripStatus(ioexpandHandle);

}


bool IOEXPAND_setHigh(IOEXPAND_Handle ioexpandHandle,const uint_least8_t ioexpanderTerm)
{
	IOEXPAND23017_Obj *pIOexpand;
	pIOexpand = (IOEXPAND23017_Obj *) ioexpandHandle;

	pIOexpand->u8OutputTermB &= ~(0xf0);
	pIOexpand->u8OutputTermB |= ioexpanderTerm;
	return IOEXPAND_WriteRegister8(ioexpandHandle,IOEXPAND_OLATB, pIOexpand->u8OutputTermB);
}

bool IOEXPAND_setLow(IOEXPAND_Handle ioexpandHandle,const uint_least8_t ioexpanderTerm)
{
	IOEXPAND23017_Obj *pIOexpand;
	pIOexpand = (IOEXPAND23017_Obj *) ioexpandHandle;
	pIOexpand->u8OutputTermB |= 0x0f;
	pIOexpand->u8OutputTermB &= (~ioexpanderTerm);
	return IOEXPAND_WriteRegister8(ioexpandHandle,IOEXPAND_OLATB, pIOexpand->u8OutputTermB);
}


//IOEXPAND_setLow(ioexpandHandle,IOEXPAND_Term25 |IOEXPAND_Term26 | IOEXPAND_Term10 |IOEXPAND_Term18, I2C_SYNC);
bool IOEXPAND_getInputs(IOEXPAND_Handle ioexpandHandle)
{
	IOEXPAND23017_Obj *pIOexpand = (IOEXPAND23017_Obj *) ioexpandHandle;
	//uint_least8_t u8Term;
     return IOEXPAND_ReadRegister8(ioexpandHandle,IOEXPAND_GPIOA, &(pIOexpand->u8InputTermA));
    //pIOexpand->u8InputTermA = u8Term ;
    //pIOexpand->u8InputTermB = (u16Term & 0x00ff);
    //return bResult;
}


void IOEXPAND_run(IOEXPAND_Handle ioexpandHandle)
{
	IOEXPAND23017_Obj *pIOexpand = (IOEXPAND23017_Obj *) ioexpandHandle;
	if (pIOexpand->bSetupComplete)
		IOEXPAND_getInputs(ioexpandHandle);
}


uint_least8_t IOEXPAND_InputTermA(IOEXPAND_Handle ioexpandHandle)
{
	IOEXPAND23017_Obj *pIOexpand;
    pIOexpand = (IOEXPAND23017_Obj *) ioexpandHandle;
    return pIOexpand->u8InputTermA;

}

uint_least8_t IOEXPAND_Capacity(IOEXPAND_Handle ioexpandHandle)
{
	IOEXPAND23017_Obj *pIOexpand;
    pIOexpand = (IOEXPAND23017_Obj *) ioexpandHandle;
    return pIOexpand->u8InputTermB;

}

// end of file
