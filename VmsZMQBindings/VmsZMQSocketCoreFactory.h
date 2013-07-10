/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsZMQSocketCoreFactory.h	 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSZMQCOREFACTORY_H
#define VMSZMQCOREFACTORY_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsSocketFactory;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsZMQConfig.h"
#include <VmsBase/VmsSocketCoreFactory.h>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 *	The VmsZMQCoreFactory implements the interface of VmsCoreFactory in order 
 *	to transparently create & configure zmq sockets for each socket type 
 *	defined in VmsMessagingBase.
 * 
 *	As part of this task, it manages the central zmq context. NOTE that
 *	each instance of VmsZMQCoreFactory will create & destroy its own zmq
 *	context, so if in doubt, just use exactly ONE factory of this type 
 *	throughout your application!
 *	Also note that as a direct consequence you HAVE TO KEEP THE FACTORY
 *	ALIVE throughout your application, i.e. it is a VERY BAD IDEA to use
 *	a temporary socket core factory just for creating a couple of cores
 *	and then dump it. Essentially, the behavior in that case is undefined
 *	but as of now you'd probably end up with a deadlocked application.
 */
class VMSZMQAPI VmsZMQSocketCoreFactory : public VmsSocketCoreFactory
{
public:
	/**
	 *	Create a socket core factory
	 * 
	 *  --	this implicitly creates a ZMQ context.
	 */
	VmsZMQSocketCoreFactory();

	/**
	 *	Delete this socket core factory
	 *
	 *	--	this implicitly deletes the ZMQ context.
	 *		If you see this call block, this is most likely due to the fact
	 *		that not all (zmq) sockets attached to this context have been 
	 *		closed properly. Hence, check whether you have properly disposed
	 *		all your vms sockets!
	 */
	virtual ~VmsZMQSocketCoreFactory();

	/**
	 * create & configure zmq specific socket core for VmsSendSocket
	 */
	virtual VmsSocketCore * CreateSendCore(const std::string& strAddress, 
										   VmsVocabulary *pVocabulary);

	/**
	 * create & configure zmq specific socket core for VmsReceiveSocket
	 */
	virtual VmsSocketCore * CreateReceiveCore(const std::string& strAddress,
										      VmsVocabulary *pVocabulary);

	/**
	 * create & configure zmq specific socket core for VmsSendRequestSocket
	 */
	virtual VmsSocketCore * CreateSendRequestCore(const std::string& strAddress, 
												  VmsVocabulary *pVocabulary);

	/**
	 * create & configure zmq specific socket core for VmsAnswerRequestSocket
	 */
	virtual VmsSocketCore * CreateAnswerRequestCore(const std::string& strAddress, 
													VmsVocabulary *pVocabulary);

	/**
	 *	Convenience method registering the prefixes tcp, udp, and inproc 
	 *	with the given socket factory.
	 */
	void RegisterDefaultZMQProtocols(VmsSocketFactory *pFactory);

	/**
	 *	Helper to make the zmq context accessible from the outside
	 *	This is something you usually should not have to use, so
	 *	USE AT YOUR OWN RISK!
	 */
	void *GetZMQContext();

protected:
private:
	void *m_pContext;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/