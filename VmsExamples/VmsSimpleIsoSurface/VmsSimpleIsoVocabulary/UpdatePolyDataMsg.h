/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	UpdatePolyDataMsg.h RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef UPDATEPOLYDATAMSG_H
#define UPDATEPOLYDATAMSG_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class vtkPolyData;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "SimpleIsoVocConfig.h"
#include <VistaAspects/VistaSerializable.h>
#include <VistaAspects/VistaGenericFactory.h>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
namespace VmsSimpleIsoVocabulary
{
	/**
	 *	This message transports a single vtkPolyData which is used to update
	 *	the geometry of isosurfaces.
	 */
	class SIMPLEISOVOC_API UpdatePolyDataMsg : public IVistaSerializable
	{
	public:
		typedef VistaVanillaCreator<UpdatePolyDataMsg, IVistaSerializable> TCreator;

		UpdatePolyDataMsg();
		UpdatePolyDataMsg(vtkPolyData *pPolyData);
		virtual ~UpdatePolyDataMsg();

		vtkPolyData *GetData();

		virtual int Serialize( IVistaSerializer & ) const;

		virtual int DeSerialize( IVistaDeSerializer & );

		virtual std::string GetSignature() const;

	private:
		vtkPolyData *m_pPolyData;
	};
}


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/