/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsSocketFactory.h			 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSSOCKETFACTORY_H
#define VMSSOCKETFACTORY_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsSendSocket;
class VmsReceiveSocket;
class VmsAnswerRequestSocket;
class VmsSendRequestSocket;
class VmsVocabulary;
class VmsSocketCoreFactory;
/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsBaseConfig.h"

#include <string>
#include <map>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 *	The SocketFactory is used to create ready-to-use sockets without the hassle
 *	of going through the configuration process manually.
 * 
 *	It encapsulates the handling of different transport protocols which are
 *	chosen based on the input strAddress argument.
 *	Addresses are of the format <protocol>://<address>
 *
 *	For example:
 *	tcp://127.0.0.1:5555 will try to bind/connect to the local loopback
 *	device and tcp will ultimately be used as the underlying transport.
 *
 */
class VMSBASEAPI VmsSocketFactory
{
public:
	VmsSocketFactory();
	virtual ~VmsSocketFactory();

	/**
	 *	Register a protocol identifier.
	 *
	 *	Given the protocol prefix strProtocol, this factory will delegate
	 *	the creation of socket cores to the given core factory whenever
	 *	a new socket is created whose target address features that prefix.
	 *
	 *	In this way, a single VmsSocketFactory can be used to create and
	 *	configure sockets with different underlying protocol implementations.
	 *
	 *	Double registration with the same protocol string will overwrite
	 *	the existing value.
	 *
	 *	The caller remains the owner of the given core factory, i.e.
	 *	it is the caller's responsibility to eventually delete that factory.
	 *
	 *	NULL is an invalid input value and will result in program failure,
	 *	so make sure you don't use it to "unregister" a protocol.
	 */
	void RegisterProtocol(const std::string &strProtocolPrefix, 
						  VmsSocketCoreFactory *pFactory);

	/**
	 *	Check whether or not the given protocol prefix is registered
	 *	with this factory.
	 */
	bool HasProtocol(const std::string &strProtocolPrefix);

	/**
	 *	Create a VmsSendSocket and bind it to the given address.
	 */
	VmsSendSocket *CreateSendSocket(const std::string &strAddress, 
									VmsVocabulary *pVocabulary);

	/**
	 *	Create a VmsReceiveSocket and connect it to a sender 
	 *	which has been bound to the given address.
	 */
	VmsReceiveSocket *CreateReceiveSocket(const std::string &strAddress, 
										  VmsVocabulary *pVocabulary);

	/**
	 *	Create a VmsSendRequestSocket and connect it to a 
	 *	VmsAnswerRequestSocket bound to the given address.
	 */
	VmsSendRequestSocket *CreateSendRequestSocket(const std::string &strAddress, 
												  VmsVocabulary *pVocabulary);

	/**
	 *	Create a VmsAnswerRequestSocket and bind it to the given 
	 *	address.
	 */
	VmsAnswerRequestSocket *CreateAnswerRequestSocket(const std::string &strAddress, 
													  VmsVocabulary *pVocabulary);

protected:
	/**
	 *	Extract the protocol prefix from an address string
	 *	i.e. the part before the first ":"
	 */
	std::string ParseProtocol(const std::string &strAddress);

	/**
	 *	Given an address, return the corresponding socket 
	 *	factory (if any)
	 */
	VmsSocketCoreFactory *GetCoreFactory(const std::string &strAddress);

private:
	typedef std::map<std::string, VmsSocketCoreFactory*> TProtocolMap;
	/**
	 *	Map registered protocol factories to prefixes.
	 */
	TProtocolMap m_mapPrefixToFactory;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/