/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsSerialMsgCodec.cpp        RRRR   WWWWWWWW  TT  HHHHHH  */
/*                                               RR RR   WWW WWW  TT  HH  HH  */
/*      Module   :  			                 RR  R    WW  WW  TT  HH  HH  */
/*                                                                            */
/*      Project  :  			                   Rheinisch-Westfaelische    */
/*                                               Technische Hochschule Aachen */
/*      Purpose  :                                                            */
/*                                                                            */
/*                                                 Copyright (c)  1998-2001   */
/*                                                 by  RWTH-Aachen, Germany   */
/*                                                 All rights reserved.       */
/*                                             .                              */
/*============================================================================*/
/*                                                                            */
/*    THIS SOFTWARE IS PROVIDED 'AS IS'. ANY WARRANTIES ARE DISCLAIMED. IN    */
/*    NO CASE SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DAMAGES.    */
/*    REDISTRIBUTION AND USE OF THE NON MODIFIED TOOLKIT IS PERMITTED. OWN    */
/*    DEVELOPMENTS BASED ON THIS TOOLKIT MUST BE CLEARLY DECLARED AS SUCH.    */
/*                                                                            */
/*============================================================================*/
// $Id: $

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsSerialMsgCodec.h"

#include "VmsMsg.h"
#include "VmsMsgFactory.h"

#include <VistaInterProcComm/Connections/VistaByteBufferSerializer.h>
#include <VistaInterProcComm/Connections/VistaByteBufferDeSerializer.h>

#include <stdio.h>
#include <memory.h>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsSerialMsgCodec::VmsSerialMsgCodec()
{ }


VmsSerialMsgCodec::~VmsSerialMsgCodec()
{ }


bool VmsSerialMsgCodec::Encode(VmsMsg *pInMsg,
							   VistaType::ubyte8 *&pBuffer, size_t &iSize) const 
{
	VistaByteBufferSerializer oSerializer;	
	int iret = pInMsg->Serialize(oSerializer);

	//error checking
	if(iret<0)
	{
		printf("*** ERROR *** Failed to serialize message content!\n");
		return false;
	}
	VistaType::sint32 iType = pInMsg->GetType();
	//allocate buffer for sending
	iSize = static_cast<size_t>(oSerializer.GetBufferSize())+sizeof(iType);
	pBuffer = new VistaType::ubyte8[iSize+sizeof(iType)];
	//write message type into first 4 bytes for later message creation
	//on the other side
	memcpy(pBuffer, &iType, sizeof(iType));
	//finally copy message content from serializer
	//
	//NOTE: For safety reasons, we do a copy here. Strictly speaking, 
	//we could get around this copy if we just thought hard enough.
	//The main problem is, that on the one hand the byte-buffer 
	//serializer will kill its buffer upon deletion and on the other 
	//hand setting an own buffer to get around this will jeopardize 
	//the serializer's internal memory management.
	memcpy(&(pBuffer[sizeof(iType)]), oSerializer.GetBuffer(), iSize);
	return true;
}


void VmsSerialMsgCodec::GiveUpOwnership(VmsMsg *pInMsg) const
{
	// In case of a serial encode, we actually have to delete the
	// message because it will not be used anymore on the sender side.
	// the receiving side will create an own object upon reception.
	delete pInMsg;
}


bool VmsSerialMsgCodec::Decode(VistaType::ubyte8 *pBuffer, const size_t iSize,
							   VmsMsg *&pOutMsg) const 
{
	pOutMsg = NULL;
	//we have written the type id as VistaType::sint32 -> copy it out as VistaType::sint32
	VistaType::sint32 iTypeId = 0;
	memcpy(&iTypeId, pBuffer, sizeof(iTypeId));

	//create output message
	pOutMsg = VmsMsgFactory::CreateInstanceOfType(iTypeId);
	if(pOutMsg == NULL)
	{
		printf("*** ERROR *** Unknown message type!\n");
		return false;
	}
	//finally, deserialize the message
	VistaByteBufferDeSerializer oDeser;
	//NOTE that the first four bytes of the message buffer have been used
	//for the type id so give the deserializer the offset buffer here!
	//void *pMsgContent = static_cast<void*>(&(pBuffer[sizeof(iTypeId)])); 
	char *pMsgContent = reinterpret_cast<char*>(&(pBuffer[sizeof(iTypeId)])); 
	int iContentSize = static_cast<int>(iSize-sizeof(iTypeId));
	oDeser.SetBuffer(pMsgContent, iContentSize, false);
	int iret = pOutMsg->DeSerialize(oDeser);
	if(iret < 0)
	{
		printf("*** ERROR *** Deserializing message failed!\n");
		delete pOutMsg;
		pOutMsg = NULL;
		return false;
	}
	return true;
}


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
