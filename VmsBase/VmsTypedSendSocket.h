/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsTypedSendSocket.h		 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSTYPEDSENDSOCKET_H
#define VMSTYPEDSENDSOCKET_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsSocketFactory;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsSocketCore.h"

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 *	The typed send socket behaves just like its cousin the "standard" send
 *	socket. The only difference is that it communicates by means of exactly 
 *	one message type. This eases its use in scenarios where no multi-
 *	message vocabulary is required. Note, though, that it is assumed that
 *	this socket !always" talks to a typed receive socket on the other end.
 *	Any different setup will result in undefined behavior.
 */
template<class TMsgType>
class VmsTypedSendSocket
{
public:
	friend class VmsSocketFactory;

	~VmsTypedSendSocket();

	/**
	 * just send out a message of given type "fire-and-forget" style
	 */
	void Send(TMsgType *pMsg);

	/**
	 * Access the underlying implementation core.
	 * USE AT YOUR OWN PERIL!
	 */
	VmsSocketCore *GetCore();

protected:
	VmsTypedSendSocket(VmsSocketCore *pCore);

private:
	VmsSocketCore *m_pCore;
};


/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
template<class TMsgType>
inline VmsTypedSendSocket<TMsgType>::VmsTypedSendSocket( VmsSocketCore *pCore )
	: m_pCore(pCore)
{

}

template<class TMsgType>
inline VmsTypedSendSocket<TMsgType>::~VmsTypedSendSocket()
{
	delete m_pCore;
}

template<class TMsgType>
inline void VmsTypedSendSocket<TMsgType>::Send( TMsgType *pMsg )
{
	m_pCore->Send(pMsg);
}


template<class TMsgType>
inline VmsSocketCore * VmsTypedSendSocket<TMsgType>::GetCore()
{
	return m_pCore;
}


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
