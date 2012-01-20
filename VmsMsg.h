/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsg.h				     RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSMSG_H
#define VMSMSG_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class IVistaSerializer;
class IVistaDeSerializer;
/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsConfig.h"
#include <VistaAspects/VistaSerializable.h>
#include <VistaAspects/VistaGenericFactory.h>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 * basic message interface
 * provide semantics for message type and ticket (i.e. consecutive message numbering)
 */
class VMSAPI VmsMsg : public IVistaSerializable
{
public:
	/**
	 * 
	 */
	virtual ~VmsMsg();

	/**
	 * Ticketing access for labeling successive messages
	 */
	int GetTicket() const;

	void SetTicket(const int iTicket);
	/**
	 * basic serialization strategy here
	 */
	int Serialize(IVistaSerializer &oSer) const;

	int DeSerialize(IVistaDeSerializer &oDeser);

	virtual int SerializeMsgContent(IVistaSerializer &oSer) const = 0;
    
	virtual int DeSerializeMsgContent(IVistaDeSerializer &oDeser) = 0;


protected:
	//make sure, no one creates "plain" untyped messages
	//==> Hide c'tors for outside users
	VmsMsg();

	VmsMsg(const VmsMsg& msg);

private:
	int m_iTicket;
};

class VMSAPI VmsMsgCreator : public IVistaCreator<IVistaSerializable>
{
public:
	VmsMsgCreator() {};
	virtual ~VmsMsgCreator() {};
	
	virtual VmsMsg* CreateInstance() = 0;
};


#endif // Include guard.
/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
