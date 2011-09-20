/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsgFactory.cpp RRRR   WWWWWWWW  TT  HHHHHH  */
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

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsMsgFactory.h"

#include "VmsMsg.h"

std::map<std::string, VmsMsg*> VmsMsgFactory::m_mapStringToPrototype;
std::map<int, VmsMsg*> VmsMsgFactory::m_mapIntToPrototype;
int VmsMsgFactory::m_iTypeIdCounter = 1;
const VistaType::sint32 VmsMsgFactory::m_iRAWMessagetTypeId = 0;
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
int VmsMsgFactory::RegisterPrototype(VmsMsg *pMsg)
{
	m_mapStringToPrototype.insert(
		std::make_pair(pMsg->GetSignature(), pMsg));
	int iCurrentId = m_iTypeIdCounter;
	m_mapIntToPrototype.insert(
		std::make_pair(iCurrentId, pMsg));
	++m_iTypeIdCounter;
	return iCurrentId;
}

VmsMsg *VmsMsgFactory::CreateInstanceOfType(const std::string &strTypeDesignator)
{
	std::map<std::string,VmsMsg*>::iterator itFind = 
		m_mapStringToPrototype.find(strTypeDesignator);
	if(itFind == m_mapStringToPrototype.end())
		return NULL;

	return (itFind->second)->CreateInstance();
}

VmsMsg *VmsMsgFactory::CreateInstanceOfType(const int iTypeId)
{
	std::map<int,VmsMsg*>::iterator itFind = 
		m_mapIntToPrototype.find(iTypeId);
	if(itFind == m_mapIntToPrototype.end())
		return NULL;

	return (itFind->second)->CreateInstance();
}

VistaType::sint32 VmsMsgFactory::GetRawMsgTypeId()
{
	return m_iRAWMessagetTypeId;
}
/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
