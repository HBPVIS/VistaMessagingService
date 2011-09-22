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

VmsMsgReceptor::VmsMsgReceptor(zmq::context_t *pContext, const std::string strIp, const unsigned int iPort)
{
	VmsEndpointFactory oFactory = VmsEndpointFactory(pContext);
	std::stringstream ss;
	ss << "tcp://" << strIp << ":" << iPort;
	m_pMsgReceiver = oFactory.CreateReceiver(ss.str());
}

VmsMsgReceptor::~VmsMsgReceptor(void)
{
	delete m_pMsgReceiver;
	for(vector<VmsMsgHandler*>::iterator it=m_oHandlers.begin();it<m_oHandlers.end();it++ )
	{
		delete *it;
	}
}

int VmsMsgReceptor::ProcessIncomingMsg()
{

	VmsMsg * pMsg=m_pMsgReceiver->ReceiveMsg();
	int id=pMsg->GetType();
	
	if (id < 0)
	{
		printf("[VmsMsgReceptor::ProcessIncomingMsg] Unknown MsgType: 1");
		return -1;
	}
	if((unsigned int)id > m_oHandlers.size())
	{
		printf("[VmsMsgReceptor::ProcessIncomingMsg] MsgType not registered: 1");
		return -1;
	}
	VmsMsgHandler *h = m_oHandlers[(unsigned int)id];
	if(!h)
	{
		printf("[VmsMsgReceptor::ProcessIncomingMsg] MsgType not registered: 2");
		return -1;
	}
	h->HandleMessage(pMsg);
	return 0;
}
	
int VmsMsgReceptor::RegisterHandler(VmsMsgHandler *pHandler, int id)
{
	size_t len = m_oHandlers.size();
	if(id>=len)
	{
		m_oHandlers.resize(id+1);
		for(size_t i=len;i<id;i++)
		{
			m_oHandlers[i]=NULL;
		}
	}
	m_oHandlers[id]=pHandler;
	return 0;
}