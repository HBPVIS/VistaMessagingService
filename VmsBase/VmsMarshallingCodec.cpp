/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMarshallingCodec.cpp		 RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsMarshallingCodec.h"

#include "VmsVocabulary.h"

#include <cassert>
#include <cstddef>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsMarshallingCodec::VmsMarshallingCodec(VmsVocabulary *pVoc)
	:	m_pVocabulary(pVoc)
{
	assert(pVoc != NULL);
}

VmsMarshallingCodec::~VmsMarshallingCodec()
{ }

int VmsMarshallingCodec::EncodeMsg( IVistaSerializable *pMsg, IVistaSerializer &rSerializer ) const
{
	return m_pVocabulary->MarshalMessage(pMsg, rSerializer);
}

IVistaSerializable * VmsMarshallingCodec::DecodeMsg( IVistaDeSerializer &rDeSer ) const
{
	return m_pVocabulary->UnMarshalMessage(rDeSer);
}


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/


