/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsSerialMsgCodec.h			 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSSERIALMSGCODEC_H
#define VMSSERIALMSGCODEC_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsMsg;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsConfig.h"
#include "VmsMsgCodec.h"
#include <VistaBase/VistaBaseTypes.h>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 * The SerialEncoder actually "flat-encodes" the given message
 * i.e. it will really serialize it for sending it to another machine.
 * NOTE: In order to use this codec, make sure that all messages that
 *       ought to be encoded/decoded are registered with the VmsMsgFactory.
 */
class VMSAPI VmsSerialMsgCodec : public IVmsMsgCodec
{
public:
	VmsSerialMsgCodec();
	virtual ~VmsSerialMsgCodec();

	/**
	 * message encode: vista msg ==> raw byte buffer
	 */
	virtual bool Encode(VmsMsg *pInMsg,
						ubyte8 *&pBuffer, size_t &iSize) const;
	/**
	 * In case of inter-process communication as intended by this class, 
	 * this call will actually delete the input message.
	 */
	virtual void GiveUpOwnership(VmsMsg *pInMsg) const;
	
	/**
	 * message decode: raw byte buffer ==> vista msg
	 *
	 * NOTE: This will acutally create a new message object of the type
	 *       encoded in pBuffer. Therefore, calling code should treat
	 *		 this as a pOutMsg = new <SomeConcreteMessageType> and therefore
	 *		 take care of proper disposal of the resulting message.
	 */
	virtual bool Decode(ubyte8 *pBuffer, const size_t iSize,
						VmsMsg *&pOutMsg) const;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
