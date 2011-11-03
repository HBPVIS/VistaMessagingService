/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsgCodec.h RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSMSGCODEC_H
#define VMSMSGCODEC_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsMsg;
/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsConfig.h"
#include <VistaBase/VistaBaseTypes.h>

#include <stddef.h>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 * IVmsMsgCodec implements the encoding and decoding of
 * VistaMsg objects to/from raw byte buffers.
 */
class IVmsMsgCodec
{
public:
	virtual ~IVmsMsgCodec();
	/**
	 * message encode: vms msg ==> raw byte buffer
	 *
	 * return value indicates success
	 */
	virtual bool Encode(VmsMsg *pInMsg,
						VistaType::byte *&pBuffer, size_t &iSize) const = 0;

	/**
	 * Indicate that the given message is no longer needed
	 * by the calling process. This is assumed to be used
	 * in conjunction with a prior Encode call in order 
	 * to free the memory occupied by the given message
	 * (if needed). This, however, depends on the type of 
	 * connection (i.e. intra-process or inter-process).
	 * For correct memory management, make sure you call this 
	 * after Encode() whenever you actually send a message!
	 * NOTE: Just to make sure: the given pointer might be invalidated
	 *       so <b>do not</b> use it after you call this!
	 */
	virtual void GiveUpOwnership(VmsMsg *pInMsg) const = 0;

	/**
	 * message decode: raw byte buffer ==> vms msg
	 * NOTE: This internally creates a NEW message object
	 * and it is the calling client's responsibility to properly delete
	 * the created message after use!
	 * 
	 * return value indicates success
	 */
	virtual bool Decode(VistaType::byte *pBuffer, const size_t iSize,
						VmsMsg *&pOutMsg) const = 0;
protected:
	IVmsMsgCodec();
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
