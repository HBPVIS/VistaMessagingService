/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsInPlaceMsgCodec.cpp	     RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsInPlaceMsgCodec.h"

#include <memory.h>
#include <cassert>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
bool VmsInPlaceMsgCodec::Encode(VmsMsg *pInMsg,
								VistaType::ubyte8 *&pBuffer, 
								size_t &iSize) const
{	
	//this is an in-place encoder, i.e. we just want to send
	//the pointer to the message content without further ado...
	iSize = sizeof(pInMsg);
	pBuffer = new VistaType::ubyte8[iSize];
	//copy the pointer to the message
	memcpy(pBuffer, &pInMsg, sizeof(pInMsg));
	return true;
}



void VmsInPlaceMsgCodec::GiveUpOwnership(VmsMsg *pInMsg) const
{
	//in case of an in-place encode we actually just transfer ownership
	//and don't want to do anything to the message
	return;
}



bool VmsInPlaceMsgCodec::Decode(VistaType::ubyte8 *pBuffer, 
								const size_t iSize,
								VmsMsg *&pOutMsg) const 
{
	//sanity check based on size ==> pointer type length should be the 
	//same as we stay in the same platform
	assert(iSize == sizeof(pOutMsg));

	//finally copy output ptr
	memcpy(&pOutMsg, pBuffer, sizeof(pOutMsg));
	return true;
}



/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
