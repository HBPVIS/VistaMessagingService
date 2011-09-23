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
#include "VmsEndpointFactory.h"
#include <sstream>

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


int VmsMsgReceptor::ProcessIncomingMsg()
{

	VmsMsg * pMsg=m_pMsgReceiver->ReceiveMsg();
	int id=pMsg->GetType();
	
	if (id < 0)
	{
		printf("*** ERROR *** [VmsMsgReceptor::ProcessIncomingMsg] Unknown MsgType: 1\n");
		return -1;
	}
	if((size_t)id >= m_vecHandlers.size())
	{
		printf("*** ERROR *** [VmsMsgReceptor::ProcessIncomingMsg] MsgType not registered: 1\n");
		return -1;
	}
	VmsMsgHandler *pHnd = m_vecHandlers[static_cast<size_t>(id)];
	if(pHnd == NULL)
	{
		printf("*** ERROR *** [VmsMsgReceptor::ProcessIncomingMsg] MsgType not registered: 2\n");
		return -1;
	}
	pHnd->HandleMessage(pMsg);

	//free the message because we won't use it any more
	delete pMsg;

	return 0;
}
	
int VmsMsgReceptor::RegisterHandler(VmsMsgHandler *pHandler, int id)
{
	size_t len = m_vecHandlers.size();
	
	if(id >= len)
	{
		m_vecHandlers.resize(id+1);
		for(size_t i=len; i<id; ++i)
		{
			m_vecHandlers[i]=NULL;
		}
	}
	m_vecHandlers[id]=pHandler;
	
	return 0;
}