/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	RegisterVocabulary.cpp RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "RegisterSimpleIsoVoc.h"

#include <VmsBase/VmsVocabulary.h>

#include "RequestIsosurfaceMsg.h"
#include "TerminateMsg.h"
#include "UpdatePolyDataMsg.h"
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
namespace VmsSimpleIsoVocabulary
{
	int RegisterMessages(VmsVocabulary *pVoc)
	{
		RequestIsosurfaceMsg oRequestIsoMsg;
		int iret = pVoc->RegisterMessage(&oRequestIsoMsg, new RequestIsosurfaceMsg::TCreator());
		if(iret < 0)
			return -1;

		TerminateMsg oTerminateMsg;
		iret = pVoc->RegisterMessage(&oTerminateMsg, new TerminateMsg::TCreator());
		if(iret < 0)
			return -1;

		UpdatePolyDataMsg oUpdatePolyDataMsg;
		iret = pVoc->RegisterMessage(&oUpdatePolyDataMsg, new UpdatePolyDataMsg::TCreator());
		if(iret < 0)
			return -1;

		return 3;
	}
}



/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/