/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	VmsVocabulary.h RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef VMSVOCABULARY_H
#define VMSVOCABULARY_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VistaMarshalledObjectFactory;
class IVistaSerializable;
class IVistaSerializer;
class IVistaDeSerializer;
template<class T> class IVistaCreator;
/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsBaseConfig.h"

#include <VistaBase/VistaBaseTypes.h>
/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
/**
 * A message vocabulary is a collection of messages which can be exchanged
 * between any two sockets. Sockets have to have access to the same message
 * types in order to be compatible. In case of a system, which exchanges
 * messages only within a single process, this can be ensured by using the
 * very same VmsVocabulary instance. In the more likely case of a distributed
 * system, where messages are exchanged across process boundaries, this is
 * ensured by <bf>registering the messages in the very same order</bf> on 
 * both systems.
 */
class VMSBASEAPI VmsVocabulary
{
public:
	VmsVocabulary();
	virtual ~VmsVocabulary();

	/**
	 * Add a new message type to the vocabulary.
	 *
	 * pType has to be of the specific type to be registered.
	 *
	 * Provide a creator that captures all the necessary data that
	 * needs to be passed to newly created object instances.
	 * The vocabulary will take ownership of the creator instance, i.e.
	 * clients must not delete it after this call!
	 *
	 * \return	If successful, the newly registered message's global type id 
	 *			is returned. In case of failure, the return value is -1.
	 *			Multiple registration of the same type will result in failure.
	 */
	VistaType::sint32 RegisterMessage(IVistaSerializable *pType, 
									  IVistaCreator<IVistaSerializable> *pCreator);

	/**
	 * Check if pType is "spoken" by this vocabulary 
	 */
	bool HasMessage(IVistaSerializable *pType) const;

	/**
	 * Marshall a given message, i.e. pack it up in the given serializer,
	 * including its type information.
	 * This only works, if the given message type has been registered with
	 * this vocabulary prior to this call.
	 * \return	If successful, the return value indicates the number of 
	 *			bytes written to the serializer. -1 indicates failure.
	 */
	int MarshalMessage(IVistaSerializable *pMsg, IVistaSerializer &rSerializer) const;

	/**
	 * Unmarshal a message from the given deserializer, i.e. read its 
	 * encoded type information, create an object of proper type and 
	 * read its state.
	 * NOTE that unmarshalling is equivalent to object creation, i.e. it is
	 * the client's responsibility to handle the newly created object and
	 * delete it properly.
	 * This only works, if the given message type has been registered with
	 * this vocabulary prior to this call.
	 * \return	If successful, this returns a pointer to the newly created
	 *			message object. NULL is returned in case of failure.
	 */
	IVistaSerializable *UnMarshalMessage(IVistaDeSerializer &rDeSerializer) const;


protected:

private:
	/** for now the impl. only wraps ViSTA's MarshalledObjectFactory but
	  * that might change in the future. Moreover, the Vocabulary is used
	  * as a illustrative concept in its own right.
	  */
	VistaMarshalledObjectFactory *m_pFactory;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/