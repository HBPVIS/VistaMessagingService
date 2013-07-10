/*============================================================================*/
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*============================================================================*/
/*                                             .                              */
/*                                               RRRR WW  WW   WTTTTTTHH  HH  */
/*                                               RR RR WW WWW  W  TT  HH  HH  */
/*      Header   :	MessagingServer.cpp RRRR   WWWWWWWW  TT  HHHHHH  */
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
#include "VmsSimpleIsoServer.h"

#include <RegisterSimpleIsoVoc.h>
#include <RequestIsosurfaceMsg.h>
#include <TerminateMsg.h>
#include <UpdatePolyDataMsg.h>

//#include <VmsSimpleIsoVocabulary/RegisterVocabulary.h>
//#include <VmsSimpleIsoVocabulary/RequestIsosurfaceMsg.h>
//#include <VmsSimpleIsoVocabulary/TerminateMsg.h>
//#include <VmsSimpleIsoVocabulary/UpdatePolyDataMsg.h>

#include <VmsBase/VmsSocketFactory.h>
#include <VmsBase/VmsVocabulary.h>
#include <VmsBase/VmsAnswerRequestSocket.h>
#include <VmsBase/VmsSendSocket.h>

#include <VmsZMQBindings//VmsZMQSocketCoreFactory.h>


#include <VistaInterProcComm/Connections/VistaByteBufferSerializer.h>
#include <VistaInterProcComm/Connections/VistaByteBufferDeSerializer.h>

#include <VistaTools/VistaStreams.h>

#include <vtkDataSetReader.h>
#include <vtkContourFilter.h>

#include <zmq.h>
#include <zmq.hpp>

#include <iostream>
#include <cassert>
/*============================================================================*/
/* IMPLEMENTATION															  */
/*============================================================================*/
VmsSimpleIsoServer::VmsSimpleIsoServer( int argc, char * argv[] )
	:	m_strDataFileName(""),
		m_strServiceSocketName(""),
		m_strDataSocketName(""),
		m_pVocabulary(NULL),
		m_pCoreFactory(NULL),
		m_pSocketFactory(NULL),
		m_pServiceSocket(NULL),
		m_pDataSocket(NULL)
{
	vstr::GetStreamManager()->SetErrorPrefix("*** ERROR *** ");
	vstr::GetStreamManager()->SetWarningPrefix("*** WARNING *** ");

	this->ParseCLI(argc, argv);
	this->SetupCommunication();
}


VmsSimpleIsoServer::~VmsSimpleIsoServer()
{
	delete m_pServiceSocket;
	delete m_pDataSocket;
	delete m_pVocabulary;
	delete m_pSocketFactory;
	delete m_pCoreFactory;
}


void VmsSimpleIsoServer::Run()
{
	if(m_strServiceSocketName.empty() || m_strDataFileName.empty())
		return;

	//load data
	vtkDataSetReader *pReader = vtkDataSetReader::New();
	vstr::out() << "Reading data..." << std::endl;
	pReader->SetFileName(m_strDataFileName.c_str());
	pReader->Update();
	vstr::out() << "\tDONE!" << std::endl;

	vtkContourFilter *pFilter = vtkContourFilter::New();
	pFilter->SetInputConnection(pReader->GetOutputPort());

	vstr::out() << "Entering server loop" << std::endl;

	//cache the message tags for easy access
	const type_info& TERMINATE_TAG = typeid(VmsTestVocabulary::TerminateMsg);
	const type_info& UPDATE_ISO_TAG = typeid(VmsTestVocabulary::RequestIsosurfaceMsg);
	
	//enter service loop:
	//as long as there are new isovalues ==> we'll answer with the corresponding surface
	while(true)
	{
		IVistaSerializable *pMsg = m_pServiceSocket->ReceiveRequest();
		if(pMsg == NULL)
		{
			vstr::warnp() << "Intercepted invalid or empty message!" << std::endl;
			continue;
		}	
		
		//we have nothing to add -> just ping-pong the message as ack
		m_pServiceSocket->SendAck(pMsg);
		
		const type_info& rMsgType = typeid(*pMsg);

		if(rMsgType == TERMINATE_TAG)
		{
			delete pMsg;
			break;
		}
		else if(rMsgType == UPDATE_ISO_TAG)
		{
			double dIsoVal = static_cast<VmsTestVocabulary::RequestIsosurfaceMsg*>(pMsg)->GetIsoVal();
			vstr::out() << "Updating for " << dIsoVal << std::endl;
			pFilter->SetValue(0, dIsoVal);
			pFilter->Update();
			vstr::out() << "\tDONE!" << std::endl;

			VmsTestVocabulary::UpdatePolyDataMsg oMsg(pFilter->GetOutput());
			m_pDataSocket->Send(&oMsg);
		}
		else
		{
			vstr::errp() << "Undefined message!" << std::endl;
			vstr::errp() << "typeid name is " << typeid(*pMsg).name() << std::endl;
		}
		
		//clean up properly!
		delete pMsg;
	}
	
	vstr::out() << "Left server loop" << std::endl;

	//cleanup
	pFilter->Delete();
	pReader->Delete();
}


void VmsSimpleIsoServer::ParseCLI( int argc, char *argv[] )
{
	for(int a=1; a<argc; ++a)
	{
		std::string strArg = argv[a];
		if(strArg == "-servicesocket")
		{
			assert(a+1<argc);
			m_strServiceSocketName = argv[a+1];
			++a;
			vstr::out() << "receiving requests @ " << m_strServiceSocketName << std::endl;
		}
		else if(strArg == "-datasocket")
		{
			assert(a+1<argc);
			m_strDataSocketName = argv[a+1];
			++a;
			vstr::out() << "sending data @ " << m_strDataSocketName << std::endl;
		}
		else if(strArg == "-datafile")
		{
			assert(a+1<argc);
			m_strDataFileName = argv[a+1];
			vstr::out() << "File to read > " << m_strDataFileName << std::endl;
			++a;
		}
	}

	//error indication
	if(m_strDataFileName.empty())
	{
		vstr::errp() << "No data file name CLI param given!" << std::endl;
		exit(-1);
	}
	if(m_strServiceSocketName.empty())
	{
		vstr::errp() << "No service socket CLI param given!" << std::endl;
		exit(-1);
	}
	if(m_strDataSocketName.empty())
	{
		vstr::errp() << "No data socket CLI param given!" << std::endl;
		exit(-1);
	}
}


void VmsSimpleIsoServer::SetupCommunication()
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
	VmsTestVocabulary::RegisterMessages(m_pVocabulary);

	m_pServiceSocket = m_pSocketFactory->CreateAnswerRequestSocket(m_strServiceSocketName, m_pVocabulary);	
	
	m_pDataSocket = m_pSocketFactory->CreateSendSocket(m_strDataSocketName, m_pVocabulary);
}
/*============================================================================*/
/* END OF FILE																  */
/*============================================================================*/