/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	UpdatePolyDataMsg.cpp RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "UpdatePolyDataMsg.h"

#include <VistaAspects/VistaSerializer.h>
#include <VistaAspects/VistaDeSerializer.h>

#include <VistaVisExt/IO/VveSerializableVtkData.h>

#include <vtkPolyData.h>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
namespace VmsTestVocabulary
{
	UpdatePolyDataMsg::UpdatePolyDataMsg()
		: m_pPolyData(NULL)
	{ }

	UpdatePolyDataMsg::UpdatePolyDataMsg( vtkPolyData *pPolyData )
		: m_pPolyData(pPolyData)
	{

	}

	UpdatePolyDataMsg::~UpdatePolyDataMsg()
	{
		//we just transport the poly data, we don't manage it!
	}


	vtkPolyData * UpdatePolyDataMsg::GetData()
	{
		return m_pPolyData;
	}


	int UpdatePolyDataMsg::Serialize( IVistaSerializer& rSerializer) const
	{
		VveSerializableVtkData oContainer;
		oContainer.AddDataObject(m_pPolyData);
		return oContainer.Serialize(rSerializer);
	}

	int UpdatePolyDataMsg::DeSerialize( IVistaDeSerializer& rDeSerializer )
	{
		VveSerializableVtkData oContainer;
		int iSize = oContainer.DeSerialize(rDeSerializer);
		if(iSize > 0)
			m_pPolyData = vtkPolyData::SafeDownCast(oContainer.GetDataObject(0));
		else
			m_pPolyData = NULL;
		return iSize;
	}

	std::string UpdatePolyDataMsg::GetSignature() const
	{
		return "__UpdatePolyDataMsg__";
	}
}
/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/