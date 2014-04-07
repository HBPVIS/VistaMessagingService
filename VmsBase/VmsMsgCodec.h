/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsgCodec.h				 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSMSGCODEC_H
#define VMSMSGCODEC_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class IVistaSerializable;
class IVistaSerializer;
class IVistaDeSerializer;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsBaseConfig.h"

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 *	A message codec defines a map from a concrete object to a serializer
 *	and vice versa. This may include more steps than just "flattening" the
 *	object into a byte buffer representation, e.g. marshalling it (i.e.
 *	including both object type and state data) or just passing a pointer.
 *	See specific implementations for details.
 */
class VMSBASEAPI VmsMsgCodec
{
public:
	/**
	 * Store a message to the given serializer
	 */
	virtual int EncodeMsg(IVistaSerializable *pMsg, IVistaSerializer &rSerializer) const = 0;

	/**
	 * Retrieve a message from the given deserializer.
	 *
	 *	Clients should treat this as a new, i.e. they assume responsibility for
	 *	the returned object.
	 */
	virtual IVistaSerializable *DecodeMsg(IVistaDeSerializer &rDeSer) const = 0;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
