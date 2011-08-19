/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsInPlaceMsgCodec.h		 RRRR   WWWWWWWW  TT  HHHHHH  */
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
// $Id: $

#ifndef VMSINPLACEMSGCODEC_H
#define VMSINPLACEMSGCODEC_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/


/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsConfig.h"
#include "VmsMsgCodec.h"

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 * The VmsInPlaceCodec encodes a given method into a byte buffer so that it
 * can be passed across thread boundaries.
 * Effectively, this just encodes the address of the given message which may 
 * then be passed to another thread. The message itself is not touched at all.
 * Hence, this encoder is efficient for intra-thread communication but not
 * usable for intra-process communication. For the latter case use 
 * VmsSerialMsgEncoder instead.
 */
class VMSAPI VmsInPlaceMsgCodec : public IVmsMsgCodec
{
public:
	/**
	 * message encode: vista msg ==> serial raw buffer
	 */
	virtual bool Encode(VmsMsg *pInMsg,
						ubyte8 *&pBuffer, size_t &iSize) const;
	/**
	 * 
	 */
	virtual void GiveUpOwnership(VmsMsg *pInMsg) const;
	/**
	 * message decode: serial raw buffer ==> vista msg
	 */
	virtual bool Decode(ubyte8 *pBuffer, const size_t iSize,
						VmsMsg *&pOutMsg) const;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
