/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsgReceiver.cpp RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsMsgReceiver.h"

#include "VmsMsg.h"
#include "VmsMsgFactory.h"
#include "VmsMsgCodec.h"

#include <cassert>
#include <stdio.h>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsMsgReceiver::VmsMsgReceiver(zmq::socket_t *pSubscriber, 
							   IVmsMsgCodec *pCodec)
:	m_pSubscriber(pSubscriber),
	m_pCodec(pCodec)
{}


VmsMsgReceiver::~VmsMsgReceiver()
{
	delete m_pSubscriber;
	delete m_pCodec;
}


VmsMsg *VmsMsgReceiver::ReceiveMsg()
{
	zmq::message_t oZMQMsg;
	try
	{
		m_pSubscriber->recv(&oZMQMsg);
	}
	catch(zmq::error_t &oError)
	{
		printf("*** ERROR *** zmq send failed\n\t<%s>\n\n", oError.what());
		return NULL;
	}
	//decode the message content
	VmsMsg *pMsg = NULL;
	if(!m_pCodec->Decode(static_cast<VistaType::ubyte8*>(oZMQMsg.data()), oZMQMsg.size(), pMsg))
	{
		delete pMsg;
		return NULL;
	}
	return pMsg;
}




bool VmsMsgReceiver::ReceiveRaw(VistaType::ubyte8 *pRawData, VistaType::sint32 iTargetReadSize)
{
	zmq::message_t oZMQMsg;
	try
	{
		m_pSubscriber->recv(&oZMQMsg);
	}
	catch(zmq::error_t &oError)
	{
		printf("*** ERROR *** zmq send failed\n\t<%s>\n\n", oError.what());
		return false;
	}
	VistaType::sint32 iRawId = VmsMsgFactory::GetRawMsgTypeId();
	size_t iMsgSize = oZMQMsg.size();
	VistaType::ubyte8 *pBuffer = static_cast<VistaType::ubyte8*>(oZMQMsg.data());
	//make sure we have correct message length and type flag here!
	//since we are dealing with raw data here, we are pretty serious about this!
	assert(iMsgSize == iTargetReadSize+sizeof(iRawId));
	//copy message data to output buffer
	memcpy(pRawData, &(pBuffer[sizeof(iRawId)]), iTargetReadSize);
	return true;
}
/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
