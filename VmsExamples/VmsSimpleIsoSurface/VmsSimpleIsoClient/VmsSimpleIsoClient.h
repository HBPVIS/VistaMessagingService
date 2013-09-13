/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	MessagingClient.h RRRR   WWWWWWWW  TT  HHHHHH  */
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

#ifndef MESSAGINGCLIENT_H
#define MESSAGINGCLIENT_H

/*============================================================================*/
/* FORWARD DECLARATIONS														  */
/*============================================================================*/
class VistaSystem;
class VfaBaseApplication;
class VveTimeMapper;
template<class T> class VveDiscreteDataTyped;
class IVflRenderable;

class VmsVocabulary;
class VmsSendRequestSocket;
class VmsReceiveSocket;
class VmsZMQSocketCoreFactory;
class VmsSocketFactory;

class IVistaSerializable;

class vtkPolyData;
/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include <VistaFlowLib/Visualization/Geometry/VflGeometryVis.h>
#include <VistaBase/VistaBaseTypes.h>

#include <string>

/*============================================================================*/
/* CLASS DEFINITION															  */
/*============================================================================*/
class VmsSimpleIsoClient
{
public:
	VmsSimpleIsoClient(int argc, char *argv[]);
	virtual ~VmsSimpleIsoClient();

	void Run();

	double GetIsoVal() const;

	bool SendIsosurfaceRequest(double dTargetIsoVal);

	bool SendTerminateRequest();

	void CheckForIsosurfaceUpdate();

protected:
	void ParseCLIParams(int argc, char *argv[]);

	void InitRendering(int argc, char *argv[]);

	void SetupCommunication();

	void SetupInteraction();

private:
	std::string m_strRequestSocketName;
	std::string m_strDataSocketName;

	double m_dIsoVal;
	
	//ViSTA/VFL stuff
	VistaSystem *m_pSystem;
	VfaBaseApplication *m_pApp;
	VveTimeMapper *m_pTM;
	VveDiscreteDataTyped<vtkPolyData> *m_pData;
	VflVtkPolyDataVis *m_pRenderer;

	VmsZMQSocketCoreFactory *m_pCoreFactory;
	VmsSocketFactory *m_pSocketFactory;
	//vocabulary for talking to the server
	VmsVocabulary *m_pVocabulary;
	VmsSendRequestSocket *m_pRequestSocket;

	VmsReceiveSocket *m_pDataSocket;
};


#endif // Include guard.


/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/