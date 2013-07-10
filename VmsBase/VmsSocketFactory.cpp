/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsSocketFactory.cpp RRRR   WWWWWWWW  TT  HHHHHH  */
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

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsSocketFactory.h"

#include "VmsSendSocket.h"
#include "VmsReceiveSocket.h"
#include "VmsSendRequestSocket.h"
#include "VmsAnswerRequestSocket.h"

#include "VmsSocketCoreFactory.h"

#include <VistaBase/VistaStreamUtils.h>

#include <cassert>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsSocketFactory::VmsSocketFactory()
{

}

VmsSocketFactory::~VmsSocketFactory()
{
}

void VmsSocketFactory::RegisterProtocol(const std::string &strProtocolPrefix, 
										VmsSocketCoreFactory *pFactory )
{
	assert(pFactory != NULL);

	TProtocolMap::iterator itProto = m_mapPrefixToFactory.find(strProtocolPrefix);
	if(itProto != m_mapPrefixToFactory.end())
	{
		//protocol exists ==> replace
		VmsSocketCoreFactory *pOld = itProto->second;
		itProto->second = pFactory;
		delete pOld;
	}
	else
	{
		m_mapPrefixToFactory.insert(std::make_pair(strProtocolPrefix, pFactory));
	}

	assert(this->HasProtocol(strProtocolPrefix));
}

bool VmsSocketFactory::HasProtocol( const std::string &strProtocolPrefix )
{
	return m_mapPrefixToFactory.find(strProtocolPrefix) != m_mapPrefixToFactory.end();
}

VmsSendSocket * VmsSocketFactory::CreateSendSocket(const std::string &strAddress, 
												   VmsVocabulary *pVocabulary )
{
	VmsSocketCoreFactory *pFactory = this->GetCoreFactory(strAddress);
	if(pFactory == NULL)
		return NULL;

	VmsSocketCore *pCore = pFactory->CreateSendCore(strAddress, pVocabulary);
	if(pCore == NULL)
		return NULL;
	
	return new VmsSendSocket(pCore);
}

VmsReceiveSocket * VmsSocketFactory::CreateReceiveSocket(const std::string &strAddress, 
														 VmsVocabulary *pVocabulary )
{
	VmsSocketCoreFactory *pFactory = this->GetCoreFactory(strAddress);
	if(pFactory == NULL)
		return NULL;

	VmsSocketCore *pCore = pFactory->CreateReceiveCore(strAddress, pVocabulary);
	if(pCore == NULL)
		return NULL;

	return new VmsReceiveSocket(pCore);
}

VmsSendRequestSocket * VmsSocketFactory::CreateSendRequestSocket(const std::string &strAddress, 
																 VmsVocabulary *pVocabulary )
{
	VmsSocketCoreFactory *pFactory = this->GetCoreFactory(strAddress);
	if(pFactory == NULL)
		return NULL;

	VmsSocketCore *pCore = pFactory->CreateSendRequestCore(strAddress, pVocabulary);
	if(pCore == NULL)
		return NULL;

	return new VmsSendRequestSocket(pCore);
}

VmsAnswerRequestSocket * VmsSocketFactory::CreateAnswerRequestSocket(const std::string &strAddress, 
																	 VmsVocabulary *pVocabulary )
{
	VmsSocketCoreFactory *pFactory = this->GetCoreFactory(strAddress);
	if(pFactory == NULL)
		return NULL;

	VmsSocketCore *pCore = pFactory->CreateAnswerRequestCore(strAddress, pVocabulary);
	if(pCore == NULL)
		return NULL;

	return new VmsAnswerRequestSocket(pCore);
}


std::string VmsSocketFactory::ParseProtocol( const std::string &strAddress )
{
	size_t iPos = strAddress.find(":");
	return strAddress.substr(0, iPos);
}


VmsSocketCoreFactory* VmsSocketFactory::GetCoreFactory( const std::string &strAddress )
{
	std::string strProtocol = this->ParseProtocol(strAddress);
	if(strProtocol.empty())
	{
		vstr::errp() << "[VMSBase] Unable to parse protocol prefix from <" << strAddress << ">" << std::endl;
		return NULL;
	}

	TProtocolMap::iterator itProto = m_mapPrefixToFactory.find(strProtocol);
	if(itProto == m_mapPrefixToFactory.end())
	{
		vstr::errp() << "[VMSBase] Unregistered protocol <" << strProtocol << ">" << std::endl;
#ifdef DEBUG
		TProtocolMap::const_iterator itCurrent = m_mapPrefixToFactory.begin();
		TProtocolMap::const_iterator itEnd = m_mapPrefixToFactory.end();
		vstr::outi() << "[VMSBase] Valid protocols are:" << std::endl;
		vstr::IndentObject oIndent;
		for(;itCurrent != itEnd; ++itCurrent)
			vstr::outi() << itCurrent->first << std::endl;
#endif
		return NULL;
	}
	else
		return itProto->second;
}




/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/