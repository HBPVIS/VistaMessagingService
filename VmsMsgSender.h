/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsgSender.h				 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSMSGSENDER_H
#define VMSMSGSENDER_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsMsg;
class IVmsMsgCodec;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsConfig.h"
#include <zmq.hpp>
#include <VistaBase/VistaBaseTypes.h>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 * The MsgSender is a sending Vms endpoint that transmits messages to receiving
 * endpoints.
 */
class VMSAPI VmsMsgSender
{
public:
	/**
	 * create a new sender object given a zmq publisher (already connected)
	 * and a message codec which encapsulates intra/inter process de-/coding
	 * of messages.
	 *
	 * NOTE: The object takes responsibility for both, the given publisher
	 *       and the codec instances, i.e. the calling code MUST NOT delete
	 *       them!
	 */
	VmsMsgSender(zmq::socket_t *pPublisher, IVmsMsgCodec *pCodec);
	
	/**
	 * free the resources we got upon construction
	 */
	virtual ~VmsMsgSender();

	/**
	 * Send a message.
	 *
	 * NOTE: Message sending is equivalent to a transfer of ownership. Therefore
     *		 the calling class <b>must not</b> access the input message again.
	 *		 The given pointer might even be invalidated by this call! Because of
	 *		 this make sure you pass in <b>pointers to objects allocated with new</b>
	 *		 and not pointers obtained via the &-operator!
	 */
	int SendMsg(VmsMsg *pMsg);

	/**
	 * Send raw data via the channel.
	 *
	 * This interface is intended for minimum-overhead, low latency, high
	 * frequency communication of small chunks of data, i.e. data where the 
	 * additional overhead of encoding it into a VmsMessage would be relatively
	 * large. Typical examples include but are not limited to the repeated
	 * sending of 3 floats for a tracker position, or the frequent update
	 * of a simulation time stamp.
	 *
	 * When mixing raw communication with messages on the same pair of
	 * endpoints, make sure you issue matching Send/Receive calls.
	 * Error detection in raw communication mode is minimal!
	 *
	 * In contrast to the SendMessage call, pData will not be modified or
	 * deleted and may thus be used for multiple successive calls.
	 *
	 */
	int SendRaw(VistaType::byte *pData, VistaType::sint32 iSize);

	/**
	 * custom free function to be provided with zmq::message_t::rebuild
	 */
	static void free_buffer(void *pBuffer, void *hint);

protected:

private:
	zmq::socket_t *m_pPublisher;
	IVmsMsgCodec *m_pCodec;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
