/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsg.cpp					 RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsMsg.h"
#include <VistaAspects/VistaSerializer.h>
#include <VistaAspects/VistaDeSerializer.h>

/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsMsg::VmsMsg() 
: m_iTicket(-1) 
{
}


VmsMsg::VmsMsg(const VmsMsg& msg) 
: m_iTicket(msg.m_iTicket) 
{
}



VmsMsg::~VmsMsg(){}



int VmsMsg::GetTicket() const
{
	return m_iTicket;
}



void VmsMsg::SetTicket(const int iTicket)
{
	m_iTicket = iTicket;
}



int VmsMsg::Serialize(IVistaSerializer &oSer) const
{
	int iSize = 0;
	int iret = 0;
	/*
	//first of all -> write type id for direct later retrieval
	iret = oSer.WriteInt32(this->GetType());
	if(iret < 0)
	return -1;
	else
	iSize += iret;
	*/

	//write length encoded signature string 
	//for later type identification on receiver side
	std::string strSig = this->GetSignature();
	int iLen = (int) strSig.length();

	iret = oSer.WriteInt32(iLen);
	if(iret < 0)
		return -1;
	else
		iSize += iret;

	iret = oSer.WriteRawBuffer(strSig.data(), (int)strSig.length());
	if(iret < 0)
		return -1;
	else
		iSize += iret;

	iret = oSer.WriteInt32(m_iTicket);
	if(iret < 0)
		return -1;
	else
		iSize += iret;

	return iSize;
}



int VmsMsg::DeSerialize(IVistaDeSerializer &oDeser)
{
	int iSize = 0;
	int iret = 0;

	//read type identifier
	/*
	int iType = -1;
	iret = oDeser.ReadInt32(iType);
	if(iret < 0)
	return -1;
	else
	iSize += iret;
	//check type
	if(iType != this->GetType())
	{
	printf("*** ERROR *** Deserialization type id mismatch!\n");
	return -1;
	}
	*/
	//read signature
	int iLen = 0;
	iret = oDeser.ReadInt32(iLen);
	if(iret < 0)
		return -1;
	else
		iSize += iret;
	//
	char *buffer = new char[iLen+1];
	iret = oDeser.ReadRawBuffer(buffer, iLen);
	if(iret < 0)
		return -1;
	else
		iSize += iret;
	buffer[iLen] = '\0';
	std::string strSig = buffer;
	delete[] buffer;

	//check out signature for correct type!
	if(strSig != this->GetSignature())
	{
		printf("*** ERROR *** Deserialization signature mismatch!\n");
		return -1;
	}

	iret = oDeser.ReadInt32(m_iTicket);
	if(iret < 0)
		return -1;
	else
		iSize += iret;

	return iSize;
}


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
