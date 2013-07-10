/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsVocabulary.cpp RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsVocabulary.h"

#include <VistaAspects/VistaMarshalledObjectFactory.h>

/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsVocabulary::VmsVocabulary()
	:	m_pFactory(new VistaMarshalledObjectFactory())
{
}

VmsVocabulary::~VmsVocabulary()
{
	delete m_pFactory;
}

VistaType::sint32 VmsVocabulary::RegisterMessage(IVistaSerializable *pType, 
												 IVistaCreator<IVistaSerializable> *pCreator )
{
	return m_pFactory->RegisterType(pType, pCreator);
}

bool VmsVocabulary::HasMessage( IVistaSerializable *pType ) const
{
	return (m_pFactory->GetGlobalTypeId(pType) >= 0);
}

int VmsVocabulary::MarshalMessage( IVistaSerializable* pMsg, IVistaSerializer &rSerializer ) const 
{
	return m_pFactory->MarshalObject(pMsg, rSerializer);
}

IVistaSerializable* VmsVocabulary::UnMarshalMessage( IVistaDeSerializer &rDeSerializer ) const
{
	return m_pFactory->UnmarshalObject(rDeSerializer);
}




/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/