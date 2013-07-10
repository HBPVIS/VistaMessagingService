/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	MessagingServer.h RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef MESSAGINGSERVER_H
#define MESSAGINGSERVER_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VmsSocketFactory;
class VmsZMQSocketCoreFactory;
class VmsVocabulary;
class VmsAnswerRequestSocket;
class VmsSendSocket;

class IVistaSerializable;

class vtkPolyData;
/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
 #include <string>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
class VmsSimpleIsoServer
{
public:
	VmsSimpleIsoServer(int argc, char * argv[]);
	virtual ~VmsSimpleIsoServer();

	void Run();

protected:
	void ParseCLI(int argc, char *argv[]);

	void SetupCommunication();

private:
	std::string m_strDataFileName;
	std::string m_strServiceSocketName;
	std::string m_strDataSocketName;

	//factories for setting up communication
	VmsZMQSocketCoreFactory *m_pCoreFactory;
	VmsSocketFactory *m_pSocketFactory;
	
	//vocabulary for talking to the client
	VmsVocabulary *m_pVocabulary;
	
	//the service socket is used to receive incoming requests
	VmsAnswerRequestSocket *m_pServiceSocket;

	//the data socket is used to send result data to the client
	VmsSendSocket *m_pDataSocket;
	
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/