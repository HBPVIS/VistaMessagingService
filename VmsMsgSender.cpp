/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsgSender.cpp RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsMsgSender.h"

#include "VmsMsg.h"
#include "VmsMsgFactory.h"
#include "VmsMsgCodec.h"

#include <stdio.h>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsMsgSender::VmsMsgSender(zmq::socket_t *pPublisher, IVmsMsgCodec *pCodec)
:	m_pPublisher(pPublisher),
m_pCodec(pCodec)
{}

VmsMsgSender::~VmsMsgSender()
{
	delete m_pPublisher;
	delete m_pCodec;
}


int VmsMsgSender::SendMsg(VmsMsg *pMsg)
{
	//encode message into suitable byte stream
	VistaType::ubyte8 *pBuffer = NULL;
	size_t iSize = 0;
	m_pCodec->Encode(pMsg, pBuffer, iSize);

	//after encoding we do not really need the VistaMsg anymore
	m_pCodec->GiveUpOwnership(pMsg);

	//transfer buffer to zmq message
	zmq::message_t oZMQMsg(pBuffer, iSize, VmsMsgSender::free_buffer);

	try
	{
		m_pPublisher->send(oZMQMsg);
	}
	catch(zmq::error_t &oError)
	{
		printf("*** ERROR *** zmq send failed\n\t<%s>\n\n", oError.what());
		return 0;
	}
	return 1;
}

int VmsMsgSender::SendRaw(VistaType::ubyte8 *pData, VistaType::sint32 iSize)
{
	VistaType::sint32 iRawId = VmsMsgFactory::GetRawMsgTypeId();

	zmq::message_t oZMQMsg(iSize+sizeof(iRawId));
	//set message type id to signal a RAW message
	VistaType::ubyte8 *pBuffer = static_cast<VistaType::ubyte8*>(oZMQMsg.data());
	//copy raw type id to buffer
	memcpy(&pBuffer, &iRawId, sizeof(iRawId));
	//copy raw buffer to the message body
	memcpy(&(pBuffer[sizeof(iRawId)]), pData, iSize);

	try
	{
		m_pPublisher->send(oZMQMsg);
	}
	catch(zmq::error_t &oError)
	{
		printf("*** ERROR *** zmq send failed\n\t<%s>\n\n", oError.what());
		return 0;
	}
	return 1;
}

void VmsMsgSender::free_buffer(void *pBuffer, void *hint)
{
	VistaType::ubyte8 *pByteBuffer = static_cast<VistaType::ubyte8*>(pBuffer);
	delete[] pByteBuffer;
}




/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
