/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsSocketCoreFactory.h		 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSCOREFACTORY_H
#define VMSCOREFACTORY_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsSocketCore;
class VmsMsgCodec;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsBaseConfig.h"
#include <string>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 *	Interface for creating protocol-specific socket cores.
 */
class VMSBASEAPI VmsSocketCoreFactory
{
public:
	virtual ~VmsSocketCoreFactory();

	/**
	 *	Create and configure a core for a VmsSendSocket.
	 *
	 *	In addition to mere creation, this will bind the sender 
	 *	to the given address.
	 */
	virtual VmsSocketCore *CreateSendCore(const std::string& strAddress, 
										  VmsMsgCodec *pCodec) = 0;
	/**
	 *	Create and configure a core for a VmsReceiveSocket.
	 *
	 *	In addition to mere creation, this will connect the receiver
	 *	to a sender at the given address.
	 */
	virtual VmsSocketCore *CreateReceiveCore(const std::string& strAddress, 
											 VmsMsgCodec *pCodec) = 0;
	/**
	 *	Create and configure a core for a VmsSendRequestSocket.
	 *
	 *	In addition to mere creation, this will connect the sender 
	 *	to a server at the given address.
	 */
	virtual VmsSocketCore *CreateSendRequestCore(const std::string& strAddress, 
												 VmsMsgCodec *pCodec) = 0;
	/**
	 *	Create and configure a core for a VmsAnswerRequestSocket.
	 *
	 *	In addition to mere creation, this will bind the server
	 *	to the given address.
	 */
	virtual VmsSocketCore *CreateAnswerRequestCore(const std::string& strAddress, 
												   VmsMsgCodec *pCodec) = 0;

protected:
	VmsSocketCoreFactory();
	
private:

};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/