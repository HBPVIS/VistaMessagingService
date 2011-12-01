/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsgReceiver.h			 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSMSGRECEIVER_H
#define VMSMSGRECEIVER_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class IVmsMsgCodec;
class VmsMsg;

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
 * The MsgReceiver is a Vms communication endpoint that receives incoming 
 * messages from a sending endpoint.
 */
class VMSAPI VmsMsgReceiver
{
public:
	/**
	 * create a new receiver object given a zmq subscriber (already connected)
	 * and a message codec which encapsulates intra/inter process de-/coding
 	 * of messages. 
	 *
	 * NOTE: The object takes responsibility for both, the given subscriber
	 *       and the codec instances, i.e. the calling code MUST NOT delete
	 *       them!
	 */
	VmsMsgReceiver(zmq::socket_t *pSubscriber, IVmsMsgCodec *pCodec);
	virtual ~VmsMsgReceiver();

	/**
	 * receive and decode a single message 
	 *
	 * NOTE: The semantics of receive require the client of this class to
	 *       take responsibility correctly deleting received messages, i.e.
	 *		 read this as a "new VistaMsg". Therefore it is the client's
	 *		 responsibility to correctly delete any incoming messages.
	 */
	VmsMsg *ReceiveMsg();

	/**
	 * Receive a block of raw data.
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

	 * Unlike the ReceiveMsg interface, no memory is allocated internally,
	 * i.e. pRawData has to point to a suitable block of pre-allocated
	 * memory of iTargetReadSize many bytes length. The read operation
	 * will try and read EXACTLY iTargetReadSize many bytes from the next
	 * incoming message. It will fail if the number of bytes in the 
	 * message does not fit.
	 */
	bool ReceiveRaw(VistaType::byte *pRawData, VistaType::sint32 iTargetReadSize);
protected:
	
private:
	zmq::socket_t *m_pSubscriber;
	IVmsMsgCodec *m_pCodec;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
