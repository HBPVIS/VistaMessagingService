/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsZMQSocketCore.cpp		 RRRR   WWWWWWWW  TT  HHHHHH  */
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
/* $Id$ */

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsZMQSocketCore.h"

#include <VmsBase/VmsVocabulary.h>

#include <VistaInterProcComm/Connections/VistaByteBufferSerializer.h>
#include <VistaInterProcComm/Connections/VistaByteBufferDeSerializer.h>

#include <VistaBase/VistaTimer.h>
#include <VistaBase/VistaTimeUtils.h>

#include <zmq.h>

#include <cassert>
#include <cstring>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsZMQSocketCore::VmsZMQSocketCore(void *pZMQSocket, VmsVocabulary *pVocabulary)
	:	m_pSocket(pZMQSocket),
		VmsSocketCore(pVocabulary)	
{

}

VmsZMQSocketCore::~VmsZMQSocketCore()
{
	int iret = zmq_close(m_pSocket);
	assert(iret == 0);
}

void VmsZMQSocketCore::Send( IVistaSerializable *pMsg )
{
	const VmsVocabulary *pVoc = this->GetVocabulary();

	VistaByteBufferSerializer oSer;
	pVoc->MarshalMessage(pMsg, oSer);

	zmq_msg_t oRawMsg;
	int iret = zmq_msg_init_size(&oRawMsg, oSer.GetBufferSize());
	assert(iret == 0);

	memcpy(zmq_msg_data(&oRawMsg), oSer.GetBuffer(), oSer.GetBufferSize());

	iret = zmq_msg_send(&oRawMsg, m_pSocket, 0);
	//if the following assertion fails you probably try to do two
	//consecutive sends on a ZMQ_REQ or ZMQ_REP socket!
	assert(iret > 0 && "Failed to send message! Illegal double send?");

	iret = zmq_msg_close(&oRawMsg);
	assert(iret == 0);
}

IVistaSerializable * VmsZMQSocketCore::Receive()
{
	zmq_msg_t oRawMsg;
	int iret = zmq_msg_init(&oRawMsg);
	assert(iret == 0);

	//receive message
	iret = zmq_msg_recv(&oRawMsg, m_pSocket, 0);
	//if the following assertion fails you probably try to do two
	//consecutive receives on a ZMQ_REQ or ZMQ_REP socket!
	assert(iret > 0 && "Failed to receive message! Illegal double receive?");

	//manually decode
	IVistaSerializable *pMsg = this->DecodeMessage(static_cast<VistaType::byte*>(zmq_msg_data(&oRawMsg)), static_cast<int>(zmq_msg_size(&oRawMsg)));
	
	iret = zmq_msg_close(&oRawMsg);
	assert(iret == 0);

	return pMsg;
}

IVistaSerializable * VmsZMQSocketCore::ReceiveNonBlocking( int iWaitTime )
{
	zmq_msg_t oRawMsg;
	int iret = zmq_msg_init(&oRawMsg);
	assert(iret == 0);

	const double dWait = iWaitTime / 1000.0;
	VistaTimer oTimer;
	iret = -1;
	
	int iNumPolls = 0;
	while(iret < 0 && oTimer.GetLifeTime()<dWait)
	{
		//receive message
		iret = zmq_msg_recv(&oRawMsg, m_pSocket, ZMQ_DONTWAIT);
		//if the following assertion fails you probably try to do two
		//consecutive receives on a ZMQ_REQ or ZMQ_REP socket!
		assert(((iret <= 0 && zmq_errno() == EAGAIN) || (iret > 0)) && "Failed to receive message! Illegal double receive?");
		++iNumPolls;
	}

	//if we still got no message ==> return NULL to the outside
	if(iret <= 0 && zmq_errno() == EAGAIN)
		return NULL;
	
	//still here ==> manually decode what we got
	IVistaSerializable *pMsg = this->DecodeMessage(static_cast<VistaType::byte*>(zmq_msg_data(&oRawMsg)), static_cast<int>(zmq_msg_size(&oRawMsg)));

	iret = zmq_msg_close(&oRawMsg);
	assert(iret == 0);

	return pMsg;
}

IVistaSerializable * VmsZMQSocketCore::DecodeMessage( VistaType::byte *pBuffer, int iSize )
{
	const VmsVocabulary *pVoc = this->GetVocabulary();
	VistaByteBufferDeSerializer oDeSer;
	oDeSer.SetBuffer(pBuffer, iSize);
	return pVoc->UnMarshalMessage(oDeSer);
}

void * VmsZMQSocketCore::GetZMQSocket()
{
	return m_pSocket;
}

/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/


