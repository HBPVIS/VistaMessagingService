/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsSendSocket.h				 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSSENDSOCKET_H
#define VMSSENDSOCKET_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class IVistaSerializable;
class VmsVocabulary;
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
 *	A "fire-and-forget" style socket that is used to just send out messages.
 *
 *	No ack of any kind is provided. Hence, the sender will neither know if
 *	nor when a specific message has made it to the receiving end. This is 
 *	suitable for high-frequency scenarios that are potentially loss tolerant.
 *	That being said, client code can usually assume that everything works out
 *	just fine. If you, however, need strict send-ack semantics, use 
 *	VmsSendRequestSocket instead.
 *
 *	NOTE that this is currently only compatible with VmsSendSocket i.e. 
 *	you cannot pair this socket with any other socket type! Doing so will
 *	result in undefined behavior!
 *
 *	Creation is handled exclusively by VmsSocketFactory.
 */
class VMSBASEAPI VmsSendSocket
{
public:
	friend class VmsSocketFactory;

	virtual ~VmsSendSocket();

	/**
	 *	Just put out a message
	 */
	void Send(IVistaSerializable *pMsg);

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
	VmsSendSocket(VmsSocketCore *pCore);

private:
	VmsSocketCore *m_pCore;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/