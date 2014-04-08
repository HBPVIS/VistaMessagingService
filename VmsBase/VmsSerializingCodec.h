/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsSerializingCodec.h RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSSERIALIZINGCODEC_H
#define VMSSERIALIZINGCODEC_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/


/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsMsgCodec.h"

#include <VistaAspects/VistaSerializable.h>
#include <VistaAspects/VistaSerializer.h>
#include <VistaAspects/VistaDeSerializer.h>

#include <cassert>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 *  This codec just stores/retrieves an object's state information. Type 
 *	information is statically stored via templating, i.e. the type of a 
 *	received message is implicitly defined by the template parameter.
 *
 *	In order for this to work, two basic assumptions are made:
 *	1)	TMsgType implements IVistaSerializable
 *	2)	TMsgType features a local public type TCreator which can be used
 *		to create (w/o additional params) a new object of TMsgType.
 */
template<class TMsgType> 
class VmsSerializingCodec : public VmsMsgCodec
{
public:
	typedef typename TMsgType::TCreator TMsgCreator;

	/**
	 *	Create a new codec given the creator for the specific message type.
	 *	NULL is a valid input value for codec instances that will ONLY
	 *	be used on the sending end. However, be careful about this, because
	 *	trying to read from a codec with a NULL-initialized creator WILL
	 *	CRASH!
	 *
	 *	Note that this object will take ownership of the creator, i.e.
	 *	it will delete it after use. Hence, clients MUST NOT delete it
	 *	themselves. Moreover, it is a bad idea to share the same creator among
	 *	instances!
	 */
	VmsSerializingCodec(TMsgCreator *pCreator);
	virtual ~VmsSerializingCodec();

	/**
	 *	Store stuff by just using the IVistaSerializer interface.
	 */
	virtual int EncodeMsg( IVistaSerializable *pMsg, IVistaSerializer &rSerializer ) const;
	
	/**
	 *	Retrieve stuff by just using the IVistaSerializer interface.
	 *	A new object will be created using the creator passed upon construction.
	 *	Clients should treat this as a new, i.e. they assume responsibility for
	 *	the returned object.
	 */
	virtual IVistaSerializable * DecodeMsg( IVistaDeSerializer &rDeSer ) const;

private:
	TMsgCreator *m_pCreator;
};

template<class TMsgType>
inline VmsSerializingCodec<TMsgType>::VmsSerializingCodec( TMsgCreator *pCreator )
	: m_pCreator(pCreator)
{

}

template<class TMsgType>
VmsSerializingCodec<TMsgType>::~VmsSerializingCodec()
{
	delete m_pCreator;
}

template<class TMsgType>
int VmsSerializingCodec<TMsgType>::EncodeMsg( IVistaSerializable *pMsg, IVistaSerializer &rSerializer ) const
{
	return pMsg->Serialize(rSerializer);
}

template<class TMsgType>
IVistaSerializable * VmsSerializingCodec<TMsgType>::DecodeMsg( IVistaDeSerializer &rDeSer ) const
{
	assert(m_pCreator != NULL);

	TMsgType *pMsg = dynamic_cast<TMsgType*>(m_pCreator->CreateInstance());
	
	if(pMsg == NULL)
		return NULL;
	
	if(pMsg->DeSerialize(rDeSer) < 0)
	{
		m_pCreator->DestroyInstance(pMsg);
		return NULL;
	}
	return pMsg;
}
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
	
	

#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/
