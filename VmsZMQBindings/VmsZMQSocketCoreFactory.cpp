/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsZMQSocketCoreFactory.cpp  RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsZMQSocketCoreFactory.h"

#include "VmsZMQSocketCore.h"

#include <VmsBase/VmsSocketFactory.h>

#include <VistaBase/VistaStreamUtils.h>

#include <zmq.h>

#include <cassert>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsZMQSocketCoreFactory::VmsZMQSocketCoreFactory()
	: m_pContext(zmq_ctx_new())
{
	assert(m_pContext != NULL);
}

VmsZMQSocketCoreFactory::~VmsZMQSocketCoreFactory()
{
	int iret = zmq_ctx_destroy(m_pContext);
	assert(iret == 0);
}

VmsSocketCore * VmsZMQSocketCoreFactory::CreateSendCore( const std::string& strAddress, VmsMsgCodec *pCodec )
{
	void *pZMQSocket = zmq_socket(m_pContext, ZMQ_PUB);

	if(pZMQSocket == NULL)
	{
		vstr::errp() << "[VmsZMQ] Unable to create PUB socket!" << std::endl;
		return NULL;
	}

	int iret = zmq_bind(pZMQSocket, strAddress.c_str());
	if(iret != 0)
	{
		vstr::errp() << "[VmsZMQ] Unable to bind PUB socket!" << std::endl;
		zmq_close(pZMQSocket);
		return NULL;
	}

	return new VmsZMQSocketCore(pZMQSocket, pCodec);
}

VmsSocketCore * VmsZMQSocketCoreFactory::CreateReceiveCore( const std::string& strAddress, VmsMsgCodec *pCodec )
{
	void *pZMQSocket = zmq_socket(m_pContext, ZMQ_SUB);

	if(pZMQSocket == NULL)
	{
		vstr::errp() << "[VmsZMQ] Unable to create SUB socket!" << std::endl;
		return NULL;
	}

	int iret = zmq_connect(pZMQSocket, strAddress.c_str());
	if(iret != 0)
	{
		vstr::errp() << "[VmsZMQ] Unable to connect PUB socket!" << std::endl;
		zmq_close(pZMQSocket);
		return NULL;
	}

	iret = zmq_setsockopt(pZMQSocket, ZMQ_SUBSCRIBE, NULL, 0);
	if(iret != 0)
	{
		vstr::errp() << "[VmsZMQ] Unable to properly subscribe SUB socket!" << std::endl;
		zmq_close(pZMQSocket);
		return NULL;
	}

	return new VmsZMQSocketCore(pZMQSocket, pCodec);
}


VmsSocketCore * VmsZMQSocketCoreFactory::CreateSendRequestCore( const std::string& strAddress, VmsMsgCodec *pCodec )
{
	void *pZMQSocket = zmq_socket(m_pContext, ZMQ_REQ);

	if(pZMQSocket == NULL)
	{
		vstr::errp() << "[VmsZMQ] Unable to create REQ socket!" << std::endl;
		return NULL;
	}

	int iret = zmq_connect(pZMQSocket, strAddress.c_str());
	if(iret != 0)
	{
		vstr::errp() << "[VmsZMQ] Unable to connect REQ socket!" << std::endl;
		zmq_close(pZMQSocket);
		return NULL;
	}

	return new VmsZMQSocketCore(pZMQSocket, pCodec);
}

VmsSocketCore * VmsZMQSocketCoreFactory::CreateAnswerRequestCore( const std::string& strAddress, VmsMsgCodec *pCodec )
{
	void *pZMQSocket = zmq_socket(m_pContext, ZMQ_REP);

	if(pZMQSocket == NULL)
	 {
		 vstr::errp() << "[VmsZMQ] Unable to create REP socket!" << std::endl;
		 return NULL;
	}

	int iret = zmq_bind(pZMQSocket, strAddress.c_str());
	if(iret != 0)
	{
		vstr::errp() << "[VmsZMQ] Unable to bind REP socket!" << std::endl;
		zmq_close(pZMQSocket);
		return NULL;
	}

	return new VmsZMQSocketCore(pZMQSocket, pCodec);
}

void VmsZMQSocketCoreFactory::RegisterDefaultZMQProtocols( VmsSocketFactory *pFactory )
{
	pFactory->RegisterProtocol("tcp", this);
	pFactory->RegisterProtocol("udp", this);
	pFactory->RegisterProtocol("inproc", this);
}

void * VmsZMQSocketCoreFactory::GetZMQContext()
{
	return m_pContext;
}

/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
