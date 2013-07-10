/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	RequestIsosurfaceMsg.h RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef REQUESTISOSURFACEMSG_H
#define REQUESTISOSURFACEMSG_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/


/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "SimpleIsoVocConfig.h"
#include <VistaAspects/VistaSerializable.h>
#include <VistaAspects/VistaGenericFactory.h>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
namespace VmsTestVocabulary
{
	/**
	 * send a single isosurface value which triggers a request for an isosurface 
	 * update.
	 */
	class SIMPLEISOVOC_API RequestIsosurfaceMsg : public IVistaSerializable
	{
	public:
		typedef VistaVanillaCreator<RequestIsosurfaceMsg, IVistaSerializable> TCreator;

		RequestIsosurfaceMsg();
		RequestIsosurfaceMsg(const double dIsoVal);

		virtual ~RequestIsosurfaceMsg();

		double GetIsoVal() const;

		virtual int Serialize( IVistaSerializer & ) const;

		virtual int DeSerialize( IVistaDeSerializer & );

		virtual std::string GetSignature() const;

	protected:
	private:
		double m_dIsoVal;
	};
}


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/

