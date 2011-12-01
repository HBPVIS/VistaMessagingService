/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsEndpointFactory.cpp		 RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsEndpointFactory.h"

#include "VmsMsgCodec.h"
#include "VmsMsgSender.h"
#include "VmsMsgReceiver.h"
#include "VmsStringMsg.h"
#include "VmsInPlaceMsgCodec.h"
#include "VmsSerialMsgCodec.h"

#include <VistaBase/VistaBaseTypes.h>

#include <stdio.h>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsEndpointFactory::VmsEndpointFactory(zmq::context_t *pContext)
:	m_pContext(pContext)
{

}

VmsEndpointFactory::~VmsEndpointFactory()
{

}


VmsMsgSender *VmsEndpointFactory::CreateSender(	const std::string &strReceiverURL,
											   const std::string &strPublisherURL)
{
	//first create a fitting codec depending on the protocol
	//issued in the publisher's URL
	IVmsMsgCodec *pCodec = this->CreateCodec(strReceiverURL);
	if(pCodec == NULL)
		return NULL;

	zmq::socket_t *pPublisher = NULL;
	//we are working with ZMQ which throws exceptions for
	//basically everything... so catch 'em alltogether
	try
	{
		//create the publisher socket
		pPublisher = new zmq::socket_t(*m_pContext, ZMQ_PUB);
		pPublisher->bind(strPublisherURL.c_str());

		//create request socket for sending "connect to publisher" 
		//request
		//the receiver should have bound its socket beforehand,
		//so connect to it here.
		zmq::socket_t oReqSocket(*m_pContext, ZMQ_REQ);
		oReqSocket.connect(strReceiverURL.c_str());

		//insert this particular socket into polling structure used below
		//in order to intercept the incoming ack package
		zmq::pollitem_t oPollSock;
		oPollSock.socket = (void*)oReqSocket;
		oPollSock.events = ZMQ_POLLIN;

		//send request to open a subscriber socket at target host
		printf("Trying to connect publisher socket @ <%s>\n\t", strPublisherURL.c_str());
		VmsStringMsg *pVistaConnectMsg = new VmsStringMsg(strPublisherURL);
		VistaType::byte *pBuffer = NULL;
		size_t iSize = 0;
		pCodec->Encode(pVistaConnectMsg, pBuffer, iSize);
		zmq::message_t oZMQConnectMsg(pBuffer, iSize, VmsMsgSender::free_buffer);
		oReqSocket.send(oZMQConnectMsg);
		pCodec->GiveUpOwnership(pVistaConnectMsg);

		//while subscription has not been acknowledged
		//	==> publish another dummy message
		//	==> poll again for ack
		//@TODO add a maximum number of retries here in order to 
		//		enable a graceful exit in case the receiving end
		//		is not listening!
		while(zmq::poll(&oPollSock, 1 , 20) == 0)
		{
			//create the dummy inside the loop because we must
			//not use a zmq message multiple times!
			zmq::message_t dummy;
			pPublisher->send(dummy);
			printf(".");
		}

		//breaking the loop means that we most likely have
		//received something on the other side's subscriber socket
		//read the ack
		if(oPollSock.revents == ZMQ_POLLIN)
		{
			zmq::message_t oAck;
			oReqSocket.recv(&oAck);
			printf("\nDONE!\n\n");
		}
		else
		{
			printf("\nFAILED!\n*** ERROR *** Polling for incoming ack failed\n");
			delete pPublisher;
			delete pCodec;
			return NULL;
		}
		//o.k. now the other side has to clear the channel of
		//and excess dummies ==> we use an empty message here!
		zmq::message_t oClearMsg(1);
		oReqSocket.send(oClearMsg);

		//wait for the corresponding ack
		zmq::message_t oFinalAck;
		oReqSocket.recv(&oFinalAck);
	}
	catch(zmq::error_t &oZMQExcept)
	{
		printf("***ERROR*** Encountered ZMQ exception\n<%s>\n\n",oZMQExcept.what());
		//if something is thrown --> clean up!
		delete pPublisher;
		delete pCodec;
		return NULL;
	}
	return new VmsMsgSender(pPublisher, pCodec);
}

VmsMsgReceiver *VmsEndpointFactory::CreateReceiver(const std::string &strReceiverURL)
{
	zmq::socket_t *pSubscriber = NULL;
	VmsMsg *pVistaMsg = NULL;
	//@todo currently we assume that both the receiverURL and the 
	//      new publisher URL are using the same protocol.
	//		Check if this is still the case when we create sockets
	//		for both, intra- and inter-process comm.
	IVmsMsgCodec *pCodec = this->CreateCodec(strReceiverURL);
	if(pCodec == NULL)
	{
		printf("*** ERROR *** Unable to create codec for given connection protocol!\n");
		return NULL;
	}

	try
	{
		printf("Listening for incoming \"connect\" requests @ %s\n", strReceiverURL.c_str());
		//open service REPLY socket in order to intercept 
		//incoming "SUBSCRIBE TO" request
		zmq::socket_t oReplier(*m_pContext, ZMQ_REP);
		oReplier.bind(strReceiverURL.c_str());

		//blocking wait for incoming request
		zmq::message_t oMsg;
		oReplier.recv(&oMsg);

		VistaType::byte *pBuffer = static_cast<VistaType::byte*>(oMsg.data());
		size_t iSize = oMsg.size();
		if(!pCodec->Decode(pBuffer, iSize, pVistaMsg))
		{
			printf("*** ERROR *** Failed to decode potential <subscribe to> msg!\n");
			return NULL;
		}

		VmsStringMsg *pStrMsg = dynamic_cast<VmsStringMsg*>(pVistaMsg);
		std::string strPublisherURL = pStrMsg->GetString();
		if(strPublisherURL.empty())
		{
			printf("*** ERROR *** Empty publisher URL\n");
			delete pVistaMsg;
			return NULL;
		}
		printf("Got new connect request for <%s>\n", strPublisherURL.c_str());

		pSubscriber = new zmq::socket_t(*m_pContext, ZMQ_SUB);
		//connect to the given publisher
		pSubscriber->connect(strPublisherURL.c_str());
		//subscribe to all messages
		//THIS IS IMPORTANT, i.e. w/o subscriptions you will not get ANY messages!
		pSubscriber->setsockopt(ZMQ_SUBSCRIBE, NULL, 0);

		//listen on subscriber until there is an incoming message
		//this should block until we get one...
		zmq::message_t oDummy;
		pSubscriber->recv(&oDummy);

		//send out our reply that we are good to go
		zmq::message_t oAck(1);
		oReplier.send(oAck);

		//now wait for the sender to ack that it has stopped sending
		//dummies
		zmq::message_t oBarrier;
		oReplier.recv(&oBarrier);

		//now take out any excess dummy messages
		zmq::pollitem_t oPollSock;
		oPollSock.socket = (void*)(*pSubscriber);
		oPollSock.events = ZMQ_POLLIN;

		//while we get something on the subscriber within 100ms
		//==> read it and throw it away
		while(zmq::poll(&oPollSock, 1 , 100) > 0)
		{
			zmq::message_t oDummy2;
			pSubscriber->recv(&oDummy2);
		}
		//now the subscriber should be clear of spurious messages...
		//ack to the sender side that we are good to go
		zmq::message_t goodToGo(1);
		oReplier.send(goodToGo);
	}
	catch (zmq::error_t &oError)
	{
		printf("***ERROR*** Encountered ZMQ exception\n<%s>\n\n",oError.what());
	}
	delete pVistaMsg;

	return new VmsMsgReceiver(pSubscriber, pCodec);
}





IVmsMsgCodec *VmsEndpointFactory::CreateCodec(const std::string &strURL)
{
	//return new VistaZMQSerialCodec;
	if(strURL.find("inproc",0,6)==0)
	{
		//we have an inproc (intra-process) socket
		//==> use an in-place codec
		return new VmsInPlaceMsgCodec;
	}
	else if(strURL.find("tcp",0,3)==0)
	{
		//we have a tcp inter-process socket
		//==> use a "real" serializing codec
		return new VmsSerialMsgCodec;
	}
	else
	{
		printf("*** ERROR *** Socket type currently not supported!\n");
		return NULL;
	}
}


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
