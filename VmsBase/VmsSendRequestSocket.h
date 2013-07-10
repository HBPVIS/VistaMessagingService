/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsSendRequestSocket.h		 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSSENDREQUESTSOCKET_H
#define VMSSENDREQUESTSOCKET_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsSocketCore;
class VmsVocabulary;
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
 *	This socket provides the client side of request handling. A request is a 
 *	two-sided transaction where the client side sends a request that is 
 *	answered by the server side with an ack. A client always has to follow
 *	a send-ack pattern of communication, i.e. it has to process the ack for
 *	request_i before sending out request_i+1.
 *
 *	Creation is handled exclusively by VmsSocketFactory.
 */
class VMSBASEAPI VmsSendRequestSocket
{
public:
	friend class VmsSocketFactory;

	virtual ~VmsSendRequestSocket();

	/**
	 *	Send a request in a synchronous fashion, i.e. send out the request and
	 *	immediately wait for the incoming ack. Essentially, this is just a 
	 *	convenience method for SendRequestAsync(); ReceiveAck();
	 *
	 *	This call will NOT affect the message passed to it. Internally, 
	 *	message content is copied and therefore pMsg may be disposed right 
	 *	after this call.
	 *
	 *	\return The ack message is returned. Note that calling code assumes
	 *			ownership of this ack i.e. it is the calling code's 
	 *			responsibility to delete it.
	 */
	IVistaSerializable* SendRequestWithAck(IVistaSerializable *pMsg);

	/**
	 *	Send out request in an asynchronous fashion, i.e. just send out the
	 *	request itself and do not wait for the ack. The resulting ack has to
	 *  be read by a subsequent call to ReceiveAck(). Use this whenever the 
	 *  ack may take some time that can be conveniently spent on doing 
	 *	something useful.
	 *
	 *	This call will NOT affect the message passed to it. Internally, 
	 *	message content is copied and therefore pMsg may be disposed right 
	 *	after this call.
	 */
	void SendRequest(IVistaSerializable *pMsg);
	
	/**
	 *	Manually receive and ack resulting from and asynchronously sent request.
	 *	Remember: there has to be <bf>exactly</bf> one ack read for each request 
	 *	sent, i.e. you must not read two acks for one SendRequestAsync() and 
	 *	you neither should miss an ack!
	 *	Note also, that this is essentially equivalent to a message allocation, 
	 *	i.e. the caller assumes ownership of the ack message. Therefore, it's 
	 *	the caller's responsibility to free said message.
	 */
	IVistaSerializable* ReceiveAck();

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
	VmsSendRequestSocket(VmsSocketCore *pCore);
	
private:
	VmsSocketCore *m_pCore;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/