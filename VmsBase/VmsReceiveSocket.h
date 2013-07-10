/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsReceiveSocket.h RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSRECEIVESOCKET_H
#define VMSRECEIVESOCKET_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsVocabulary;
class VmsSocketCore;
class IVistaSerializable;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsBaseConfig.h"

#include <string>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 *	This socket receives messages sent by a VmsSendSocket.
 *
 *	No ack mechanism is provided, making this end point suitable for fast,
 *	high-frequency communication.
 *
 *	NOTE that currently this socket may only be paired with a VmsSendSocket.
 *	Connecting it to something else will result in undefined behavior.
 *
 *	Creation is handled exclusively by VmsSocketFactory.
 */
class VMSBASEAPI VmsReceiveSocket
{
public:
	friend class VmsSocketFactory;

	virtual ~VmsReceiveSocket();

	/**
	 *	Receive an incoming message.
	 *
	 *	This call will block until a message is actually read.
	 *
	 *	Calling code will assume ownership of the message created by this 
	 *	call, i.e. it is the caller's responsibility to free the message
	 *	after use.
	 */
	IVistaSerializable *Receive();
	/**
	 *	Try to receive an incoming message for iWaitMilliSecs ms.
	 *	If there is no message after this time, NULL will be returned.
	 *
	 *	Calling code will assume ownership of the message created by this 
	 *	call, i.e. it is the caller's responsibility to free the message
	 *	after use.
	 */
	IVistaSerializable *TryReceive(const int iWaitMilliSecs);

	/**
	 * Access the underlying implementation core.
	 * USE AT YOUR OWN PERIL!
	 */
	VmsSocketCore *GetCore();

protected:
	/**
	 *	Create a new socket. 
	 *
	 *	The socket takes ownership of the core i.e. it deletes it upon its
	 *	own deletion.
	 */
	VmsReceiveSocket(VmsSocketCore *pCore);

private:
	VmsSocketCore *m_pCore;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/