/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsStringMsg.h				 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VmsMsgReceptor_H
#define VmsMsgReceptor_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsMsgHandler;
class VmsMsgReceiver;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsConfig.h"
#include <VistaBase/VistaBaseTypes.h>
#include <string>
#include <vector>
#include <zmq.hpp>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/

class VmsMsgReceptor
{
public:
	VmsMsgReceptor(zmq::context_t *pContext, const std::string strIp, const unsigned int strPort);


	~VmsMsgReceptor(void);
	
	/**
	 * Receive an incoming Message from frontend and uses the correct Message Handler 
	 * for processing
	 */
	int ProcessIncomingMsg();
	
	/**
	 * Register a new Handler. The ownership of the handler is passed to the receptor. All handlers will
	 * be destructed on cleanup.
	 */
	int RegisterHandler(VmsMsgHandler *pHandler, int id);
	

private:
	//List of all registered handlers
	std::vector<VmsMsgHandler*> m_oHandlers;
	//Incomming connection from frontend
	VmsMsgReceiver *m_pMsgReceiver;
};


#endif // Include guard.
/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
