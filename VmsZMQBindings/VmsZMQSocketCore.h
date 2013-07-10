/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsZMQSocketCore.h			 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSZMQSOCKETCORE_H
#define VMSZMQSOCKETCORE_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsVocabulary;
class IVistaSerializable;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsZMQConfig.h"
#include <VmsBase/VmsSocketCore.h>
#include <VistaBase/VistaBaseTypes.h>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
class VMSZMQAPI VmsZMQSocketCore : public VmsSocketCore
{
public:
	/**
	 *	Create a new core. 
	 *
	 *	It is assumed that the socket is preconfigured prior to creating this
	 *	core, i.e. it is fully bound/connected, already.
	 *	The core takes ownership of its underlying socket i.e. it closes 
	 *	it upon its own deletion.
	 */
	VmsZMQSocketCore(void *pZMQSocket, VmsVocabulary *pVocabulary);
	virtual ~VmsZMQSocketCore();

	virtual void Send( IVistaSerializable *pMsg );

	virtual IVistaSerializable * Receive();

	virtual IVistaSerializable *ReceiveNonBlocking(int iWaitTime);

	/**
	 * Access the actual ZMQ socket.
	 * USE AT YOUR OWN PERIL!
	 */
	void *GetZMQSocket();

protected:
	IVistaSerializable *DecodeMessage(VistaType::byte *pBuffer, int iSize);

private:
	void *m_pSocket;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/