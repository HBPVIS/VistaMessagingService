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

#include <VistaAspects/VistaMarshalledObjectFactory.h>

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
							   VistaType::byte *&pBuffer, size_t &iSize) const 
{
	//use the ViSTA MarshalledObjectFactory to encode not only the
	//object's state but also its type information.
	VistaByteBufferSerializer oSerializer;

	VistaMarshalledObjectFactory *pFactory = VistaMarshalledObjectFactory::GetSingleton();
	int iret = pFactory->MarshallObject(*pInMsg, oSerializer);

	if(iret<0)
	{
		fprintf(stderr, "*** ERROR *** Failed to marshall message!\n");
		return false;
	}
	//allocate buffer for sending
	iSize = static_cast<size_t>(oSerializer.GetBufferSize());
	pBuffer = new VistaType::byte[iSize];
	//copy marshalled message buffer to target buffer
	//
	//NOTE: For safety reasons, we do a copy here. Strictly speaking, 
	//we could get around this copy if we just thought hard enough.
	//The main problem is, that on the one hand the byte-buffer 
	//serializer will kill its buffer upon deletion and on the other 
	//hand setting an own buffer to get around this will jeopardize 
	//the serializer's internal memory management.
	memcpy(pBuffer, oSerializer.GetBuffer(), iSize);
	return true;
}


void VmsSerialMsgCodec::GiveUpOwnership(VmsMsg *pInMsg) const
{
	// In case of a serial encode, we actually have to delete the
	// message because it will not be used anymore on the sender side.
	// the receiving side will create an own object upon reception.
	delete pInMsg;
}


bool VmsSerialMsgCodec::Decode(VistaType::byte *pBuffer, const size_t iSize,
							   VmsMsg *&pOutMsg) const 
{
	//create deserializer and provide it the source buffer
	VistaByteBufferDeSerializer oDeser;
	oDeser.SetBuffer(pBuffer, static_cast<int>(iSize), false);
	//use ViSTA marshaling to decode type and state information
	VistaMarshalledObjectFactory *pFactory = VistaMarshalledObjectFactory::GetSingleton();
	pOutMsg = dynamic_cast<VmsMsg*>(pFactory->UnmarshallObject(oDeser));

	if(pOutMsg == NULL)
	{
		fprintf(stderr, "*** ERROR *** Unmarshalling of message failed!\n");
		return false;
	}
	return true;
}


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
