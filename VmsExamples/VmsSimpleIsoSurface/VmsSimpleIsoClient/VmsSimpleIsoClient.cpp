/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	MessagingClient.cpp RRRR   WWWWWWWW  TT  HHHHHH  */
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

/*============================================================================*/
/* INCLUDES																	  */
/*============================================================================*/
#include "VmsSimpleIsoClient.h"

#include <RegisterSimpleIsoVoc.h>
#include <RequestIsosurfaceMsg.h>
#include <TerminateMsg.h>
#include <UpdatePolyDataMsg.h>

//#include <VmsSimpleIsoVocabulary/RegisterSimpleIsoVoc.h>
//#include <VmsSimpleIsoVocabulary/RequestIsosurfaceMsg.h>
//#include <VmsSimpleIsoVocabulary/TerminateMsg.h>
//#include <VmsSimpleIsoVocabulary/UpdatePolyDataMsg.h>

#include <VmsBase/VmsSocketFactory.h>
#include <VmsBase/VmsVocabulary.h>
#include <VmsBase/VmsSendRequestSocket.h>
#include <VmsBase/VmsReceiveSocket.h>

#include <VmsZMQBindings//VmsZMQSocketCoreFactory.h>

#include <VistaFlowLibAux/VfaBaseApplication.h>
#include <VistaFlowLibAux/VfaSystemByContextUpdater.h>

#include <VistaFlowLib/Visualization/VflRenderNode.h>

#include <VistaVisExt/Data/VveDiscreteDataTyped.h>
#include <VistaVisExt/IO/VveSerializableVtkData.h>

#include <VistaKernel/VistaSystem.h>

#include <VistaInterProcComm/Connections/VistaByteBufferDeSerializer.h>
#include <VistaInterProcComm/Connections/VistaByteBufferSerializer.h>

#include <VistaAspects/VistaAspectsUtils.h>
#include <VistaAspects/VistaExplicitCallbackInterface.h>

#include <VistaBase/VistaStreamUtils.h>

#include <vtkPolyData.h>

#include <zmq.h>

#include <cstdlib>
#include <cassert>


class ChangeIsoVal : public IVistaExplicitCallbackInterface
{
public:
	ChangeIsoVal(VmsSimpleIsoClient *pApp, double dInc) : m_pApp(pApp), m_dInc(dInc) {}
	virtual ~ChangeIsoVal(){}

	virtual bool Do()
	{
		double dVal = m_pApp->GetIsoVal();
		m_pApp->SendIsosurfaceRequest(dVal+m_dInc);
		return true;
	}
protected:

private:
	VmsSimpleIsoClient *m_pApp;
	double m_dInc;
};


class PullIsosurfaceUpdate : public IVistaExplicitCallbackInterface
{
public:
	PullIsosurfaceUpdate(VmsSimpleIsoClient *pApp) : m_pApp(pApp) {}
	virtual ~PullIsosurfaceUpdate(){}

	virtual bool Do() 
	{
		m_pApp->CheckForIsosurfaceUpdate();
		return true;
	}

	
protected:

private:
	VmsSimpleIsoClient *m_pApp;
};
 
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsSimpleIsoClient::VmsSimpleIsoClient(int argc, char *argv[])
	:	m_strRequestSocketName(""),
		m_strDataSocketName(""),
		m_dIsoVal(0.0),
		m_pSystem(new VistaSystem),
		m_pApp(NULL),
		m_pTM(NULL),
		m_pData(NULL),
		m_pRenderer(NULL),
		m_pCoreFactory(NULL),
		m_pSocketFactory(NULL),
		m_pVocabulary(NULL),
		m_pRequestSocket(NULL),
		m_pDataSocket(NULL)
{
	vstr::GetStreamManager()->SetErrorPrefix("*** ERROR *** ");
	vstr::GetStreamManager()->SetWarningPrefix("*** WARNING *** ");

	this->ParseCLIParams(argc, argv);
	this->SetupCommunication();
	this->InitRendering(argc, argv);
	this->SetupInteraction();
}


VmsSimpleIsoClient::~VmsSimpleIsoClient()
{
	//kill the backend
	this->SendTerminateRequest();

	if(m_pRenderer != NULL)
	{
		m_pApp->GetRenderNode()->RemoveRenderable(m_pRenderer);
		delete m_pRenderer;
	}
	vtkPolyData *pData = m_pData->GetTypedLevelDataByLevelIndex(0)->GetData();
	if(pData)
		pData->Delete();
	delete m_pData;
	delete m_pTM;
	delete m_pApp;
	delete m_pSystem;
	
	delete m_pRequestSocket;
	delete m_pDataSocket;

	delete m_pVocabulary;

	delete m_pSocketFactory;
	delete m_pCoreFactory;
}


void VmsSimpleIsoClient::Run()
{
	m_pSystem->Run();
}


double VmsSimpleIsoClient::GetIsoVal() const
{
	return m_dIsoVal;
}


bool VmsSimpleIsoClient::SendIsosurfaceRequest( double dTargetIsoVal )
{
	m_dIsoVal = dTargetIsoVal;
	VmsSimpleIsoVocabulary::RequestIsosurfaceMsg oMsg(m_dIsoVal);
	
	VmsSimpleIsoVocabulary::RequestIsosurfaceMsg *pAck = 
		dynamic_cast<VmsSimpleIsoVocabulary::RequestIsosurfaceMsg*>(m_pRequestSocket->SendRequestWithAck(&oMsg));
	if(pAck == NULL || pAck->GetIsoVal() != m_dIsoVal)
		return false;
	return true;
}


bool VmsSimpleIsoClient::SendTerminateRequest()
{
	VmsSimpleIsoVocabulary::TerminateMsg oMsg;
	
	VmsSimpleIsoVocabulary::TerminateMsg *pAck = 
		dynamic_cast<VmsSimpleIsoVocabulary::TerminateMsg*>(m_pRequestSocket->SendRequestWithAck(&oMsg));
	if(pAck == NULL)
		return false;
	return true;
}


void VmsSimpleIsoClient::CheckForIsosurfaceUpdate()
{
	//try to read a message here 
	VmsSimpleIsoVocabulary::UpdatePolyDataMsg *pMsg = 
		dynamic_cast<VmsSimpleIsoVocabulary::UpdatePolyDataMsg*>(m_pDataSocket->TryReceive(5));

	//if that worked ==> process it
	if(pMsg != NULL && pMsg->GetData())
	{
		vtkPolyData *pData = pMsg->GetData();
		vtkPolyData *pOld = m_pData->GetTypedLevelDataByLevelIndex(0)->SetData(pData);
		if(pOld != NULL)
			pOld->Delete();

		vstr::out() << "Received new isosurface (#pts: " << pData->GetNumberOfPoints();
		vstr::out()<< ", #polys: " << pData->GetNumberOfPolys() << ")" << std::endl;

		//make the renderer aware of the new data
		m_pRenderer->ClearCache();
		m_pRenderer->Update();

		//never forget to get rid of the message!
		delete pMsg;
	}
}


void VmsSimpleIsoClient::ParseCLIParams( int argc, char *argv[] )
{
	for(int a=1; a<argc; ++a)
	{
		std::string strArg = argv[a];
		if(strArg == "-servicesocket")
		{
			assert(a+1<argc);
			m_strRequestSocketName = argv[a+1];
			++a;
			vstr::out() << "sending @ " << m_strRequestSocketName << std::endl;
		}
		else if(strArg == "-datasocket")
		{
			assert(a+1<argc);
			m_strDataSocketName = argv[a+1];
			++a;
			vstr::out() << "receiving @ " << m_strDataSocketName << std::endl;
		}
		else if(strArg == "-isoval")
		{
			assert(a+1<argc);
			m_dIsoVal = VistaAspectsConversionStuff::ConvertToDouble(argv[a+1]);
			vstr::out() << "Isovalue defaults to > " << m_dIsoVal << std::endl;
			++a;
		}
	}
	
	if(m_strRequestSocketName.empty())
	{
		vstr::errp() << "No request socket CLI param given!" << std::endl;
		exit(-1);
	}
	if(m_strDataSocketName.empty())
	{
		vstr::errp() << "No data socket CLI param given!" << std::endl;
		exit(-1);
	}
}


void VmsSimpleIsoClient::InitRendering(int argc, char *argv[])
{
	m_pSystem = new VistaSystem();
	m_pSystem->IntroMsg();
	
	std::list<std::string> liSearchPaths;
	liSearchPaths.push_back("configfiles/");
	liSearchPaths.push_back("../configfiles/");
	liSearchPaths.push_back("../../configfiles/");
	m_pSystem->SetIniSearchPaths(liSearchPaths);

	if(!m_pSystem->Init(argc, argv))
	{
		vstr::errp() << "*** ERROR *** VistaSystem initialization failed" << std::endl;
		vstr::errp() << "\tBAILING OUT!" << std::endl;
		delete m_pSystem;
		m_pSystem = NULL;
		
		exit(-1);
	}

	m_pApp = new VfaBaseApplication(m_pSystem, true, true);

	m_pTM = new VveTimeMapper;
	m_pData = new VveDiscreteDataTyped<vtkPolyData>(m_pTM);
	vtkPolyData *pPolyData = vtkPolyData::New();
	m_pData->GetTypedLevelDataByLevelIndex(0)->SetData(pPolyData);

	VflVtkPolyDataVis* pRenderer = new VflVtkPolyDataVis();
	if(!pRenderer->Init())
	{
		vstr::errp() << "*** ERROR *** Renderer initialization failed" << std::endl;
		vstr::errp() << "\tBAILING OUT!" << std::endl;
		exit(-1);
	}
	pRenderer->SetData(m_pData);
	m_pApp->GetRenderNode()->AddRenderable(pRenderer);
	pRenderer->SetSurfaceShader( VistaGeometryRenderingCore::GetDefaultShader() );
	pRenderer->SetUniform("u_v4Color", 0.3f, 0.3f, 0.3f, 1.0f );
	//pRenderer->GetProperties()->SetPerformStreamUpdate(true);
	pRenderer->GetProperties()->SetEnableCullFace(false);
	m_pRenderer = pRenderer;
}


void VmsSimpleIsoClient::SetupCommunication()
{
	//create and setup factories
	m_pCoreFactory = new VmsZMQSocketCoreFactory();
	m_pSocketFactory = new VmsSocketFactory();
	//register default protocols for ZMQ
	m_pCoreFactory->RegisterDefaultZMQProtocols(m_pSocketFactory);

	//create and populate vocabulary
	//NOTE: In this case, where we have only a very limited number of options, we use
	//      the same vocabulary for the request connection and the data connection.
	//		In more complex scenarios one would separate the two.
	m_pVocabulary = new VmsVocabulary();
	VmsSimpleIsoVocabulary::RegisterMessages(m_pVocabulary);

	//configure sender
	m_pRequestSocket = m_pSocketFactory->CreateSendRequestSocket(m_strRequestSocketName, m_pVocabulary);

	//configure receiver
	m_pDataSocket = m_pSocketFactory->CreateReceiveSocket(m_strDataSocketName, m_pVocabulary);
}


void VmsSimpleIsoClient::SetupInteraction()
{
	m_pApp->RegisterAction('+', new ChangeIsoVal(this,  0.1));
	m_pApp->RegisterAction('-', new ChangeIsoVal(this, -0.1));

	m_pApp->GetUpdateHandler()->RegisterAction(new PullIsosurfaceUpdate(this));
}
/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/