/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsZMQSocketDemux.h			 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSZMQSOCKETDEMUX_H
#define VMSZMQSOCKETDEMUX_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/


/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsZMQConfig.h"

#include <VmsBase/VmsSocketDemux.h>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
class VMSZMQAPI VmsZMQSocketDemux : public VmsSocketDemux
{
public:
	VmsZMQSocketDemux();
	virtual ~VmsZMQSocketDemux();

	/**
	 *  Implement demultiplexing on sockets via zmq's polling mechanism
	 */
	virtual int DemultiplexSockets(std::vector<VmsReceiveSocket*> &vecSockets, 
								   size_t iTimeOutMs );


};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/