/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsgReceptor.h			 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSMSGRECEPTOR_H
#define VMSMSGRECEPTOR_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsMsgHandler;
class VmsMsgReceiver;
class VmsMsg;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsConfig.h"

#include <string>
#include <vector>

#include <zmq.hpp>

#include <VistaBase/VistaBaseTypes.h>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 * 
 */
class VMSAPI VmsMsgReceptor
{
public:
	VmsMsgReceptor(zmq::context_t *pContext, const std::string &strReceiverURL);


	virtual ~VmsMsgReceptor();
	
	/**
	* Register a new handler for the given message type. 
	* The ownership of the handler is passed to the receptor. All handlers will 
	* be destructed on cleanup.
	* \return slot id if successful, -1 in case of error
	*/
	int RegisterHandler(VmsMsg *pMsgType, VmsMsgHandler *pHandler);
	
	/**
	 * Receive an incoming message on the internal receiver and hand it over 
	 * to the corresponding handler (if any). After handling, delete the 
	 * message.
	 * \return return code of HandleMsg if a message is passed to the handler,
	 *         -1 in case of error.
	 */
	int ProcessIncomingMsg();

private:
	/** List of all registered handlers */
	std::vector<VmsMsgHandler*> m_vecHandlers;
	/** receiver for incoming messages */
	VmsMsgReceiver *m_pMsgReceiver;
};


#endif // Include guard.
/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
