/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsEndpointFactory.h		 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSENDPOINTFACTORY_H
#define VMSENDPOINTFACTORY_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsMsgSender;
class VmsMsgReceiver;
class IVmsMsgCodec;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsConfig.h"

#include <zmq.hpp>

#include <string>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 * Factory class for correctly implementing the handshake between
 * connection end points.
 * This process is delegated to a dedicated class because the handshake
 * protocol is somewhat more complicated and would thus clog the 
 * implementation of the endpoints themselves.
 *
 * \todo Refactor the service REQ-REP socket pair into member variables
 *       in order to reuse them. This also entails a refactoring of the 
 *		 current mode of operation (in-process or inter-process) and
 *		 a corresponding codec creation process.
 *
 * \todo Mixed mode endpoints i.e. those allowing intra- and inter-process
 *		 communication at the same time will entail a major redesign here,
 *	     because then an endpoint would probably need multiple encoders 
 *		 at the same time.
 *
 * \todo cleanup error/info messages
 */
class VMSAPI VmsEndpointFactory
{
public:
	VmsEndpointFactory(zmq::context_t *pContext);
	
	virtual ~VmsEndpointFactory();

	/**
	 * Create a new "sender" type end point of a message channel
	 *
	 * This construction method encapsulates not only the creation
	 * itself but also a secure handshake between the new sender
	 * and its receiving counterpart.
	 *
	 * <b>NOTE</b>: Make sure the receiving client is up and running
	 *				and a corresponding CreateReceiver call has been issued
	 *				on the other side. Otherwise, this method might block
	 *				indefinitely!
	 *
	 * \todo: add time-out here!
	 *
	 * <b>NOTE</b>: Make sure you call this method from within the thread you
	 *				will be using with the created sender. Do not share
	 *				senders among threads as this might break the underlying 
	 *				zmq structures!
	 *
	 * <b>NOTE</b>: Due to the current implementation the receiver ALWAYS 
	 *				has to be setup before the sender when using intra-process 
	 *				communication. Otherwise any connection tries by the 
	 *				will result in a failed call to CreateSender().
	 *
	 * In zmq terms the handshake works as follows:
	 * First, a PUBLISHER socket is created for the new sender.
	 * Second, a REQUEST socket is opened and bound to strReceiverURL.
	 * Third, the REQUEST socket is used to ask the receiving app
	 * to connect to the PUBLISHER socket. The receiving end creates 
	 * a SUBSCRIBER socket and binds it to the given PUBLISHER address.
	 * In step 4, the publisher socket will send consecutive dummy messages
	 * until - in step 5 - the receiving end signals that it has successfully 
	 * intercepted a message from the publisher. 
	 * Now, the publisher is definitely connected to the subscriber
	 * and we can be sure that no subsequent messages will be lost 
	 * in the channel. 
	 * Steps 6 through 8 ensure that any excess dummy messages are 
	 * cleared out of the channel. This is done by an additional 
	 * reqest-reply pair that works as a barrier.
	 * The following "image" sketches the process
	 *
	 *   PUB	REQ				REP		SUB
	 * 1) *
	 * 2)        *
	 *  			 connect!
	 * 3)        *------------->*
	 *                                   *
	 *             1st dummy
	 * 4) *--------------------------->(lost)
	 *             2nd dummy
	 *    *--------------------------->(lost)
	 *             nth dummy
	 *    *--------------------------->(success)
	 *
	 *               connected!
	 * 5)        *<-------------*
	 *
	 *              clean up!
	 * 6)        *------------->*
	 *
	 *
	 * 7) *--------------------------->(read excess dummies)
	 *
	 *                done!
	 * 8)       *<--------------*
	 *
	 * This rather complex protocol is used in order to 
	 * ensure that the publisher-subscriber pair is connected
	 * correctly. Note, that in PUB/SUB mode, ZMQ does not buffer
	 * messages for oncoming subscribers, i.e. a subscriber gets a
	 * specific "first" messages some time after its subscription
	 * but will not get any messages before that first one. Therefore,
	 * some of the dummies in the above handshake procedure might be
	 * lost. However, ZMQ will gurarantee that the subscriber will get
	 * everything AFTER that first messsage.
	 */
	VmsMsgSender *CreateSender(const std::string &strReceiverURL,
							   const std::string &strPublisherURL);

	/**
	 * Create a generic message receiver using the analogues protocol outlined
	 * for sender creation above.
	 *
	 * <b>NOTE</b>: The CreateReceiver call should be issued first, i.e. it is
	 *				the receiving end's responsibility to create a listening socket
	 *				to which the sending end can connect.
	 *
	 * <b>NOTE</b>: Make sure you call this method from within the thread you
	 *				will be using with the created receiver. Do not share
	 *				receivers among threads as this might break the underlying 
	 *				zmq structures!
	 *
	 * <b>NOTE</b>: Due to the current implementation the receiver ALWAYS 
	 *				has to be setup before the sender when using intra-process 
	 *				communication. Otherwise any connection tries by the 
	 *				will result in a failed call to CreateSender().
	 *
	 */
	VmsMsgReceiver *CreateReceiver(const std::string &strReceiverURL);

protected:
	/**
	 * depending on the connection protocol, which is evident from
	 * the first few letters of the target URL, create a codec that
	 * suits this protocol's needs, i.e. that implements either local
	 * or remote data encoding.
	 */
	IVmsMsgCodec *CreateCodec(const std::string &strURL);

private:
	zmq::context_t *m_pContext;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
