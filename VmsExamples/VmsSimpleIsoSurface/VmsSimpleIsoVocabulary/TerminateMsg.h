/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	TerminateMsg.h RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef TERMINATEMSG_H
#define TERMINATEMSG_H

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
namespace VmsSimpleIsoVocabulary
{
	/**
	 * just an empty message indicating that termination 
	 */
	class SIMPLEISOVOC_API TerminateMsg : public IVistaSerializable
	{
	public:
		typedef VistaVanillaCreator<TerminateMsg, IVistaSerializable> TCreator;

		TerminateMsg();
		virtual ~TerminateMsg();

		virtual int Serialize( IVistaSerializer & ) const;

		virtual int DeSerialize( IVistaDeSerializer & );

		virtual std::string GetSignature() const;

	protected:
	private:
	};
}

#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/