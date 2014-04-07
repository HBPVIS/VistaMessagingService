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

#include "VmsTypedReceiveSocket.h"
#include "VmsTypedSendSocket.h"

#include "VmsSocketCoreFactory.h"
#include "VmsTypedSendSocket.h"
#include "VmsTypedReceiveSocket.h"
#include "VmsSerializingCodec.h"

#include <string>
#include <map>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 *	The SocketFactory is used to create ready-to-use sockets without the hassle
 *	of manually going through the configuration process.
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
	 *	Create a typed send socket. Semantically, this is essentially identical
	 *	to VmsSendSocket. It does not rely on a vocabulary, though. Instead, a
	 *	typed socket only communicates via a single message type that has to 
	 *	be fixed at compile time. It is assumed that this socket is tied to a 
	 *	VmsTypedReceiveSocket of the same message type.
	 *
	 */
	template<typename TMsgType>
	VmsTypedSendSocket<TMsgType> *CreateTypedSendSocket(const std::string &strAddress);

	/**
	*	Create a typed receive socket. Semantically, this is essentially identical
	*	to VmsReceiveSocket. It does not rely on a vocabulary, though. Instead, a
	*	typed socket only communicates via a single message type that has to 
	*	be fixed at compile time. It is assumed that this socket is tied to a 
	*	VmsTypedSendSocket of the same message type.
	*	The created socket will take ownership of the creator that is passed to 
	*	this call, so DO NOT DELETE it and DO NOT SHARE it with other sockets!
	*	It is assumed that TCreator is of type TMsgType::TCreator.
	*/
	template<typename TMsgType, typename TCreator>
	VmsTypedReceiveSocket<TMsgType> *CreateTypedReceiveSocket(const std::string &strAddress, TCreator *pCreator);

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





/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
template<typename TMsgType>
inline VmsTypedSendSocket<TMsgType>* VmsSocketFactory::CreateTypedSendSocket( const std::string &strAddress)
{
	VmsSocketCoreFactory *pFactory = this->GetCoreFactory(strAddress);
	if(pFactory == NULL)
		return NULL;

	//create a serializing codec as default for this socket type for now.
	//since the msg type is implicit, we can settle for this one in favor of
	//the marshalling version. We pass in NULL here because this socket
	//will only be sending!
	VmsSerializingCodec<TMsgType> *pCodec = new VmsSerializingCodec<TMsgType>(NULL);

	VmsSocketCore *pCore = pFactory->CreateSendCore(strAddress, pCodec);
	if(pCore == NULL)
		return NULL;

	return new VmsTypedSendSocket<TMsgType>(pCore);
}

template<typename TMsgType, typename TCreator>
inline VmsTypedReceiveSocket<TMsgType>* VmsSocketFactory::CreateTypedReceiveSocket( const std::string &strAddress, TCreator *pCreator)
{
	VmsSocketCoreFactory *pFactory = this->GetCoreFactory(strAddress);
	if(pFactory == NULL)
		return NULL;

	//create a serializing codec as default for this socket type for now.
	//since the msg type is implicit, we can settle for this one in favor of
	//the marshalling version.
	VmsSerializingCodec<TMsgType> *pCodec = new VmsSerializingCodec<TMsgType>(pCreator);

	VmsSocketCore *pCore = pFactory->CreateReceiveCore(strAddress, pCodec);
	if(pCore == NULL)
		return NULL;

	return new VmsTypedReceiveSocket<TMsgType>(pCore);
}

#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/