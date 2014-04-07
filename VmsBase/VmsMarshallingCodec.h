/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMarshallingCodec.h RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSMARSHALLINGCODEC_H
#define VMSMARSHALLINGCODEC_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsVocabulary;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsBaseConfig.h"
#include "VmsMsgCodec.h"

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 *	A marshalling codec implements the message codec interface by storing
 *	both, state and object type information in the resulting serial 
 *	representation. As such, it is the default codec for all communication
 *	channels where message type information is not implicitely given.
 *	Essentially, this just just serves as a proxy for the given vocabulary.
 */
class VMSBASEAPI VmsMarshallingCodec : public VmsMsgCodec
{
public:
	/**
	 *	c'tor for a new codec based on a pre-configured vocabulary. 
	 *
	 *	The given vocabulary will just be referenced, but ownership remains 
	 *	with the caller, i.e. it remains the caller's responsibility to clean 
	 *	this up when done.
	 */
	VmsMarshallingCodec(VmsVocabulary *pVoc);
	virtual ~VmsMarshallingCodec();

	/**
	 *	Marshall the given message object to the serializer.
	 */
	 virtual int EncodeMsg( IVistaSerializable *pMsg, IVistaSerializer &rSerializer ) const;

	/**
	 *	Create a new object by unmarshalling it from the given deserializer.
	 *
	 *	Clients should treat this as a new, i.e. they assume responsibility for
	 *	the returned object.
	 */
	virtual IVistaSerializable * DecodeMsg( IVistaDeSerializer &rDeSer ) const;

protected:

private:
	VmsVocabulary *m_pVocabulary;

};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
