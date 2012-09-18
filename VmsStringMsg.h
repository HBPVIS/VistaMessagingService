/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsStringMsg.h				 RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSSTRINGMSG_H
#define VMSSTRINGMSG_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/


/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsConfig.h"
#include "VmsMsg.h"
#include <VistaAspects/VistaGenericFactory.h>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 * simple message with simple string payload
 */
class VMSAPI VmsStringMsg : public VmsMsg
{
public:
	VmsStringMsg();
	VmsStringMsg(const std::string &strMsg);
	virtual ~VmsStringMsg();

	/**
	 * type conversion to string
	 */
	operator std::string();
	
	std::string GetString() const;

	void SetString(const std::string &str);
	
	///**
	// * 
	// */
	virtual std::string GetSignature() const;

	virtual int SerializeMsgContent(IVistaSerializer &oSer) const;
    
	virtual int DeSerializeMsgContent(IVistaDeSerializer &oDeser);

protected:
	//hide default copy c'tor
	VmsStringMsg(const VmsStringMsg& other);

private:
	std::string m_strMsg;
};


class VMSAPI VmsStringMsgCreator : public VmsMsgCreator
{
public:
	VmsStringMsg* CreateInstance()
	{
		return new VmsStringMsg();
	}

	void DestroyInstance( IVistaSerializable * pMsg)
	{
		if(pMsg!=0)
			delete (VmsStringMsg*)pMsg;
	}
};


#endif // Include guard.
/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
