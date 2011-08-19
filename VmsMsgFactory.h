/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsMsgFactory.h RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSMSGFACTORY_H
#define VMSMSGFACTORY_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsMsg;

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsConfig.h"
#include <VistaBase/VistaBaseTypes.h>
#include <string>
#include <map>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 * the message factory is used to generically build messages
 * based on string or int identifiers
 * 
 * @todo The general pattern here, i.e. the combination of the PROTOTYPE
 *		 and FACTORY patterns could be refactored into a general ViSTA
 *		 structure for use in ViSTA Aspects
 */
class VMSAPI VmsMsgFactory
{
public:
	/**
	 * register a new prototype
	 * NOTE: the SERIALIZABLE's signature will be used as a key
	 */
	static int RegisterPrototype(VmsMsg *pMsg);

	/**
	 * create an instance designated by a string
	 */
	static VmsMsg *CreateInstanceOfType(const std::string &strTypeDesignator);

	/**
	 * create an instance designated by a unique typeid
	 */
	static VmsMsg *CreateInstanceOfType(const int iTypeId);
	/**
	 * 
	 */
	static sint32 GetRawMsgTypeId();
protected:

private:
	static std::map<std::string, VmsMsg*> m_mapStringToPrototype;
	static std::map<int, VmsMsg*> m_mapIntToPrototype;

	/**
	 * counter that is used to assign unique message type ids to the
	 * different message types.
	 * NOTE: Valid user specific message types start with 1 as the value
	 * of 0 is reserved for RAW data content.
	 */
	static int m_iTypeIdCounter;
	static const sint32 m_iRAWMessagetTypeId;
};

#endif // Include guard.
/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
