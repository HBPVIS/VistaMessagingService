/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsZMQSocketDemux.cpp		 RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsZMQSocketDemux.h"

#include "VmsZMQSocketCore.h"

#include <VmsBase/VmsReceiveSocket.h>

#include <zmq.h>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsZMQSocketDemux::VmsZMQSocketDemux()
{ }

VmsZMQSocketDemux::~VmsZMQSocketDemux()
{ }

int VmsZMQSocketDemux::DemultiplexSockets(std::vector<VmsReceiveSocket*> &vecSockets, size_t iTimeOutMs )
{
	const size_t iNumSockets = vecSockets.size();
	zmq_pollitem_t *pEntries = new zmq_pollitem_t[iNumSockets];

	for(size_t s=0; s<iNumSockets; ++s)
	{
		zmq_pollitem_t &rEntry = pEntries[s];
		VmsZMQSocketCore *pCore = dynamic_cast<VmsZMQSocketCore*>(vecSockets[s]->GetCore());
		//this demux is only able to handle sockets of zmq type ==> if there is 
		//a missing socket or a failed cast ==> bail out
		if(pCore == NULL)
		{
			delete[] pEntries;
			return DM_ERROR;
		}
		rEntry.socket = pCore->GetZMQSocket();
		rEntry.events = ZMQ_POLLIN;
	}

	int iZMQTimeout = (iTimeOutMs==0 ? -1 : static_cast<int>(iTimeOutMs));
	int iPollResult = zmq_poll(pEntries, static_cast<int>(iNumSockets), iZMQTimeout);

	int iReturnVal = 0;
	if(iPollResult > 0)
	{
		//find first entry which has data available
		for(size_t e=0; e<iNumSockets; ++e)
		{
			if(pEntries[e].revents & ZMQ_POLLIN)
			{
				iReturnVal = static_cast<int>(e);
				break;
			}
		}
	}
	else if(iPollResult == 0)
	{
		iReturnVal = DM_TIMEOUT;
	}
	else 
	{
		iReturnVal = DM_ERROR;
	}

	//mind to clean up properly
	delete[] pEntries;
	return iReturnVal;
}




/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/