/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsgReceptor.cpp			 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#include "VmsMsgReceptor.h"

#include "VmsMsgReceiver.h"
#include "VmsMsgHandler.h"
#include "VmsEndpointFactory.h"

#include <VistaAspects/VistaMarshalledObjectFactory.h>

#include <cstdio>

using namespace std;

VmsMsgReceptor::VmsMsgReceptor(zmq::context_t *pContext, const std::string &strURL)
{
	VmsEndpointFactory oFactory = VmsEndpointFactory(pContext);
	m_pMsgReceiver = oFactory.CreateReceiver(strURL);
}

VmsMsgReceptor::~VmsMsgReceptor(void)
{
	delete m_pMsgReceiver;
	const size_t iSize=m_vecHandlers.size();
	for(size_t i=0; i<iSize; ++i)
	{
		delete m_vecHandlers[i];
	}
}

int VmsMsgReceptor::RegisterHandler(VmsMsg *pMsgType, VmsMsgHandler *pHandler)
{
	VistaMarshalledObjectFactory *pFactory = VistaMarshalledObjectFactory::GetSingleton();
	
	//determine global id
	int id=pFactory->GetGlobalTypeId(*pMsgType);
	if(id < 0)
	{
		printf("*** ERROR *** [VmsMsgReceptor::RegisterHandler] Msg type not registered with factory!");
		return -1;
	}
	
	//resize handler vector if necessary
	size_t iTgtSlot = static_cast<size_t>(id);
	size_t len = m_vecHandlers.size();
	if(static_cast<size_t>(id) >= len)
	{
		m_vecHandlers.resize(id+1);
		for(size_t i=len; i<iTgtSlot; ++i)
		{
			m_vecHandlers[i]=NULL;
		}
	}
	
	//assign handler
	m_vecHandlers[iTgtSlot]=pHandler;

	return static_cast<int>(iTgtSlot);
}

int VmsMsgReceptor::ProcessIncomingMsg()
{
	//receive the message 
	VmsMsg * pMsg=m_pMsgReceiver->ReceiveMsg();
	
	//retrieve globally unique type id
	VistaMarshalledObjectFactory *pFactory = VistaMarshalledObjectFactory::GetSingleton();
	VistaType::sint32 id = pFactory->GetGlobalTypeId(*pMsg);
	
	//error checking
	if(id < 0 || static_cast<size_t>(id) >= m_vecHandlers.size())
	{
		printf("*** ERROR *** [VmsMsgReceptor::ProcessIncomingMsg] Invalid message type!\n");
		return -1;
	}
	VmsMsgHandler *pHnd = m_vecHandlers[static_cast<size_t>(id)];
	if(pHnd == NULL)
	{
		printf("*** ERROR *** [VmsMsgReceptor::ProcessIncomingMsg] No handler for message type id <%d>!\n", id);
		return -1;
	}
	
	//Pass messag to the given handler for that message type
	int iret = pHnd->HandleMessage(pMsg);

	//free the message because we won't use it any more
	delete pMsg;

	return iret;
}

