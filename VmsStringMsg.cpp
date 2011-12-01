/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsStringMsg.cpp			 RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsStringMsg.h"

#include <VistaAspects/VistaSerializer.h>
#include <VistaAspects/VistaDeSerializer.h>

/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsStringMsg::VmsStringMsg()
:	m_strMsg("")
{ }

VmsStringMsg::VmsStringMsg(const VmsStringMsg& other)
{

}

VmsStringMsg::VmsStringMsg(const std::string &strMsg)
:	m_strMsg(strMsg) 
{
}

VmsStringMsg::~VmsStringMsg()
{ 
}



VmsStringMsg::operator std::string()
{
	return this->GetString();
}

std::string VmsStringMsg::GetString() const
{
	return m_strMsg;
}

void VmsStringMsg::SetString(const std::string &str)
{
	m_strMsg = str;
}


int VmsStringMsg::Serialize(IVistaSerializer &oSer) const
{
	int iSize = 0;
	//serialize header
	int iret = VmsMsg::Serialize(oSer);
	//rudimentary error handling 
	if(iret < 0)
		return -1;
	else
		iSize += iret;

	//write length encoded string
	iret = oSer.WriteInt32((int)m_strMsg.size());
	if(iret < 0)
		return -1;
	else
		iSize += iret;

	iret = oSer.WriteRawBuffer(m_strMsg.c_str(),(int)m_strMsg.size());
	if(iret < 0)
		return -1;
	else
		iSize += iret;

	return iSize;
}

int VmsStringMsg::DeSerialize(IVistaDeSerializer &oDeser)
{
	int iSize = 0;
	int iret = VmsMsg::DeSerialize(oDeser);
	//rudimentary error handling 
	if(iret < 0)
		return -1;
	else
		iSize += iret;

	int iLen = 0;
	iret = oDeser.ReadInt32(iLen);
	if(iret < 0)
		return -1;
	else
		iSize += iret;

	char *buffer = new char[iLen+1];
	iret = oDeser.ReadRawBuffer(buffer,iLen);
	if(iret < 0)
		return -1;
	else
		iSize += iret;
	buffer[iLen] = '\0';
	m_strMsg = buffer;
	delete[] buffer;

	return iSize;
}

std::string VmsStringMsg::GetSignature() const
{
	return "VistaStringMsg";
}





/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
