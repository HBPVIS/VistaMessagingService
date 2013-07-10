/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsAnswerRequestSocket.h	 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSANSWERREQUESTSOCKET_H
#define VMSANSWERREQUESTSOCKET_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsVocabulary;
class IVistaSerializable;
class VmsSocketCore;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsBaseConfig.h"

#include <string>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 *	This socket provides the server side of request handling. It is paired with
 *	VmsSendRequestSocket. A request is a two-sided transaction where the client 
 *	side sends a request that is answered by the server side with an ack. The 
 *	server always has to follow a receive-ack pattern of communication, i.e. it 
 *	receives a message defining a request and answers this message with an ack
 *	message of its own. It must not issue two consecutive acks or receives.
 *
 *	Creation is handled exclusively by VmsSocketFactory.
 */
class VMSBASEAPI VmsAnswerRequestSocket
{
public:
	friend class VmsSocketFactory;

	virtual ~VmsAnswerRequestSocket();

	/**
	 *	Blocking wait for an incoming request.
	 *
	 *	Note that this is essentially equivalent to a message allocation, 
	 *	i.e. the caller assumes ownership of the request message. Therefore, 
	 *	it's the caller's responsibility to free said message.
	 */
	IVistaSerializable *ReceiveRequest();
	
	/**
	 * Send out an ack message.
	 *
	 *	This call will NOT affect the message passed to it. Internally, 
	 *	message content is copied and therefore pMsg may be disposed right after 
	 *	this call.
	 */
	void SendAck(IVistaSerializable *pAck);

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
	VmsAnswerRequestSocket(VmsSocketCore* pCore);

private:
	VmsSocketCore *m_pCore;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/