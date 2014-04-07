/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsSocketCore.h				 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSSOCKETCORE_H
#define VMSSOCKETCORE_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsMsgCodec;
class IVistaSerializable;
/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsBaseConfig.h"

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 *	Abstract interface for protocol-specific core implementations. 
 */
class VMSBASEAPI VmsSocketCore
{
public:
	/**
	 * Create a new socket core using the given codec for encoding/decoding
	 * messages into/from byte streams.
	 *
	 * The core will take ownership of the codec, i.e. it will delete the 
	 * codec upon its own destruction.
	 */
	VmsSocketCore(VmsMsgCodec *pCodec);
	virtual ~VmsSocketCore();

	/**
	 *	Send out a message via the underlying protocol.
	 *
	 *	This includes marshalling the message, i.e. serializing its type and
	 *	state information, wrapping it up for transport and sending it out.
	 *	The message itself will not be deleted in the process.
	 */
	virtual void Send(IVistaSerializable *pMsg) = 0;

	/**
	 *	Receive a message via the underlying protocol.
	 * 
	 *	This includes receiving the message in its raw serial form, 
	 *	unmarshalling it and returning it to the caller. 
	 *	Callers assume ownership of the message i.e. it is the caller's 
	 *	responsibility to delete the resulting message object after use.
	 */
	virtual IVistaSerializable *Receive() = 0;

	/**
	 *	Receive a message via the underlying protocol.
	 *
	 *	Semantics are the same as above apart from the fact that this call
	 *	will only block for iWaitTime milliseconds.
	 */
	virtual IVistaSerializable *ReceiveNonBlocking(int iWaitTime) = 0;

protected:
	const VmsMsgCodec *GetCodec() const;

private:
	VmsMsgCodec *m_pCodec;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/