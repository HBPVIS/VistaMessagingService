/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsSendRequestSocket.cpp	 RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsSendRequestSocket.h"
#include "VmsSocketCore.h"

#include <cassert>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsSendRequestSocket::VmsSendRequestSocket(VmsSocketCore *pCore)
	:	m_pCore(pCore)
{
	assert(pCore != NULL);
}

VmsSendRequestSocket::~VmsSendRequestSocket()
{
	delete m_pCore;
}

IVistaSerializable * VmsSendRequestSocket::SendRequestWithAck( IVistaSerializable *pMsg )
{
	this->SendRequest(pMsg);
	return this->ReceiveAck();
}

void VmsSendRequestSocket::SendRequest( IVistaSerializable *pMsg )
{
	m_pCore->Send(pMsg);
}

IVistaSerializable* VmsSendRequestSocket::ReceiveAck()
{
	return m_pCore->Receive();
}

VmsSocketCore * VmsSendRequestSocket::GetCore()
{
	return m_pCore;
}





/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/