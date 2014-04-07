/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsTypedReceiveSocket.h		 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSTYPEDRECEIVESOCKET_H
#define VMSTYPEDRECEIVESOCKET_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsSocketFactory;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsSocketCore.h"
#include <cassert>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
  *	A typed receive socket behaves just like its "standard" counterpart the
  *	VmsReceiveSocket, except that it only communicates a single message type.
  *	Hence, it does not require an underlying vocabulary, which eases its use
  * in scenarios where this flexibility is not needed. Note that it is assumed
  * that a typed receiver will !always! talk to a typed sender. Any other
  *	setup will result in undefined behavior.
  */
template<typename TMsgType>
class VmsTypedReceiveSocket
{
public:
	friend class VmsSocketFactory;

	virtual ~VmsTypedReceiveSocket();

	/**
	 *	Just put out a message -- which in this implementation has to be
	 *	of the predefined type.
	 */
	TMsgType* Receive();

	/**
	 * Access the underlying implementation core.
	 * USE AT YOUR OWN PERIL!
	 */
	VmsSocketCore *GetCore();

protected:
	VmsTypedReceiveSocket(VmsSocketCore *pCore);

private:
	VmsSocketCore *m_pCore;
};

/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/

template<typename TMsgType>
inline VmsTypedReceiveSocket<TMsgType>::VmsTypedReceiveSocket( VmsSocketCore *pCore )
	: m_pCore(pCore)
{
	assert(m_pCore != NULL);
}

template<typename TMsgType>
inline VmsTypedReceiveSocket<TMsgType>::~VmsTypedReceiveSocket()
{
	delete m_pCore;
}

template<typename TMsgType>
inline TMsgType* VmsTypedReceiveSocket<TMsgType>::Receive()
{
	return dynamic_cast<TMsgType*>(m_pCore->Receive());
}

template<typename TMsgType>
inline VmsSocketCore * VmsTypedReceiveSocket<TMsgType>::GetCore()
{
	return m_pCore;
}




#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
