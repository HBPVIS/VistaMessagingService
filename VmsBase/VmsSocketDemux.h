/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsSocketDemux.h			 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSSOCKETDEMUX_H
#define VMSSOCKETDEMUX_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsReceiveSocket;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsBaseConfig.h"

#include <vector>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
class VMSBASEAPI VmsSocketDemux
{
public:
	enum{
		DM_TIMEOUT = -1,
		DM_ERROR   = -2
	};
	VmsSocketDemux();
	virtual ~VmsSocketDemux();

	/**
	 *	Check the given sockets and determine which one has at least one
	 *	message available for reception. This operation will block for iTimeOutMs
	 *	milliseconds. If the timeout is set to 0, the call will block 
	 *	without timeout i.e. until there is - eventually - an incoming message.
	 *
	 *	NOTE that it is implicitly assumed that all given sockets share the 
	 *	same underlying socket core type, i.e. the same protocol implementation
	 *	is used.
	 *
	 *	\return id of the socket which is ready to receive 
	 *			TIMEOUT in case of time out
	 *			ERROR in case of another, internal error
	 */
	virtual int DemultiplexSockets(std::vector<VmsReceiveSocket*> &vecSockets, 
								   size_t iTimeOutMs) = 0;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/