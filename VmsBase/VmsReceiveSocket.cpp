/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsReceiveSocket.cpp RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsReceiveSocket.h"
#include "VmsSocketCore.h"

#include <cassert>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsReceiveSocket::VmsReceiveSocket( VmsSocketCore *pCore )
	:	m_pCore(pCore)
{
	assert(m_pCore != NULL);
}



VmsReceiveSocket::~VmsReceiveSocket()
{
	delete m_pCore;
}

IVistaSerializable * VmsReceiveSocket::Receive()
{
	return m_pCore->Receive();
}

IVistaSerializable * VmsReceiveSocket::TryReceive( const int iWaitMilliSecs )
{
	return m_pCore->ReceiveNonBlocking(iWaitMilliSecs);
}

VmsSocketCore * VmsReceiveSocket::GetCore()
{
	return m_pCore;
}

/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/