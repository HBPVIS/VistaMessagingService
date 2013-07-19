/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	RequestIsosurfaceMsg.cpp RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "RequestIsosurfaceMsg.h"

#include <VistaAspects/VistaSerializer.h>
#include <VistaAspects/VistaDeSerializer.h>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
namespace VmsSimpleIsoVocabulary
{
	RequestIsosurfaceMsg::RequestIsosurfaceMsg()
		:	m_dIsoVal(0.0)
	{ }

	RequestIsosurfaceMsg::RequestIsosurfaceMsg( const double dIsoVal )
		:	m_dIsoVal(dIsoVal)
	{

	}

	RequestIsosurfaceMsg::~RequestIsosurfaceMsg()
	{ }


	double RequestIsosurfaceMsg::GetIsoVal() const
	{
		return m_dIsoVal;
	}

	int RequestIsosurfaceMsg::Serialize(IVistaSerializer& rSerializer) const
	{
		int iNumBytes = rSerializer.WriteDouble(m_dIsoVal);
		return iNumBytes;
	}

	int RequestIsosurfaceMsg::DeSerialize(IVistaDeSerializer& rDeSerializer)
	{
		int iNumBytes = rDeSerializer.ReadDouble(m_dIsoVal);
		return iNumBytes;
	}

	std::string RequestIsosurfaceMsg::GetSignature() const
	{
		return "__RequestIsosurfaceMsg__";
	}

}

/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/