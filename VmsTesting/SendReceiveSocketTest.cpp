// $Id$
#include "TestingUtils.h"

#include <gtest/gtest.h>

#include <VmsBase/VmsSendSocket.h>
#include <VmsBase/VmsReceiveSocket.h>
#include <VmsBase/VmsVocabulary.h>

#include <VmsBase/VmsSocketFactory.h>
#include <VmsZMQBindings/VmsZMQSocketCoreFactory.h>


class SendReceiveSocketTest : public ::testing::Test
{
public:
	SendReceiveSocketTest() {};

	virtual ~SendReceiveSocketTest() {};

	void SetUp()
	{
		//create&setup test vocabulary
		m_pVocabulary = new VmsVocabulary;
		TestMsg oMsg;
		m_pVocabulary->RegisterMessage(&oMsg, new TestMsg::TCreator);

		//create&setup factories
		m_pCoreFactory = new VmsZMQSocketCoreFactory();
		m_pSocketFactory = new VmsSocketFactory();
		m_pCoreFactory->RegisterDefaultZMQProtocols(m_pSocketFactory);

		m_pContext = m_pCoreFactory->GetZMQContext();
	}

	void TearDown()
	{
		delete m_pSocketFactory;
		delete m_pCoreFactory;
		delete m_pVocabulary;
	}


protected:
	VmsVocabulary *m_pVocabulary;
	VmsSocketFactory *m_pSocketFactory;
	VmsZMQSocketCoreFactory *m_pCoreFactory;

	void *m_pContext;
};

/**
 * Test plain fire-and-forget sending
 */
TEST_F(SendReceiveSocketTest, TestPlainSender)
{
	//setup sender socket BEFORE setting up the receiver
	VmsSendSocket *pSender = m_pSocketFactory->CreateSendSocket(STR_TEST_SOCKET, m_pVocabulary);
	//BAD HACK TO FIX "late subscriber" issue ==> will be fixed with proper handshake soon
	VistaTimeUtils::Sleep(50);
	
	//setup receiving end in separate thread
	PartnerThread *pReceiver = 
		new PartnerThread(PartnerThread::PLAIN_RECEIVER_NO_ACK, m_pContext, m_pSocketFactory, m_pVocabulary, 0);
	pReceiver->Run();
	//BAD HACK TO FIX "late subscriber" issue ==> will be fixed with proper handshake soon
	VistaTimeUtils::Sleep(50);

	TestMsg *pMsg = new TestMsg(STR_TEST_MSG);
	pSender->Send(pMsg);
	delete pMsg;
	
	pReceiver->Join();
	ASSERT_TRUE(pReceiver->GetMsgText() == STR_TEST_MSG);

	delete pReceiver;
	delete pSender;
}

/**
 * Test the plain receiver
 */
TEST_F(SendReceiveSocketTest, TestPlainReceiver)
{
	PartnerThread *pSender = 
		new PartnerThread(PartnerThread::PLAIN_SENDER_NO_ACK, m_pContext, m_pSocketFactory, m_pVocabulary, 0);
	pSender->Run();
	pSender->WaitForIsListening();
	
	VmsReceiveSocket *pReceiver = m_pSocketFactory->CreateReceiveSocket(STR_TEST_SOCKET, m_pVocabulary);
	//BAD HACK TO FIX "late subscriber" issue ==> will be fixed with proper handshake soon
	VistaTimeUtils::Sleep(50);

	TestMsg *pMsg = dynamic_cast<TestMsg*>(pReceiver->Receive());
	ASSERT_TRUE(pMsg != NULL);
	ASSERT_TRUE(pMsg->GetMsgText() == STR_TEST_MSG);
	delete pMsg;

	delete pReceiver;
	
	pSender->Join();
	delete pSender;
}

/**
 * Test connection between sender and receiver, both in their Vms flavor
 */
TEST_F(SendReceiveSocketTest, TestVmsPairing)
{
	//setup sender socket BEFORE setting up the receiver
	VmsSendSocket *pSender =m_pSocketFactory->CreateSendSocket(STR_TEST_SOCKET, m_pVocabulary);
	//BAD HACK TO FIX "late subscriber" issue ==> will be fixed with proper handshake soon
	VistaTimeUtils::Sleep(50);

	//setup receiving end in separate thread
	PartnerThread *pReceiver = 
		new PartnerThread(PartnerThread::VMS_RECEIVER_NO_ACK, m_pContext, m_pSocketFactory, m_pVocabulary, 0);
	pReceiver->Run();
	//BAD HACK TO FIX "late subscriber" issue ==> will be fixed with proper handshake soon
	VistaTimeUtils::Sleep(50);

	TestMsg *pMsg = new TestMsg(STR_TEST_MSG);
	pSender->Send(pMsg);
	delete pMsg;

	pReceiver->Join();
	ASSERT_TRUE(pReceiver->GetMsgText() == STR_TEST_MSG);

	delete pReceiver;
	delete pSender;
}

/**
 *	Test non-blocking receive with waiting interval
 */
TEST_F(SendReceiveSocketTest, TestNonblockingReceive)
{
	PartnerThread *pSender = 
		new PartnerThread(PartnerThread::PLAIN_SENDER_NO_ACK, m_pContext, m_pSocketFactory, m_pVocabulary, 100);
	pSender->Run();
	pSender->WaitForIsListening();

	VmsReceiveSocket *pReceiver = m_pSocketFactory->CreateReceiveSocket(STR_TEST_SOCKET, m_pVocabulary);
	//BAD HACK TO FIX "late subscriber" issue ==> will be fixed with proper handshake soon
	VistaTimeUtils::Sleep(50);

	//first non-blocking receive should return with NULL ==> we get a message only after 500 ms
	TestMsg *pMsg = dynamic_cast<TestMsg*>(pReceiver->TryReceive(50));
	ASSERT_TRUE(pMsg == NULL);

	VistaTimeUtils::Sleep(100);
	
	//...now after waiting some more there should be one!
	pMsg = dynamic_cast<TestMsg*>(pReceiver->TryReceive(50));
	ASSERT_TRUE(pMsg != NULL);
	ASSERT_TRUE(pMsg->GetMsgText() == STR_TEST_MSG);
	delete pMsg;

	delete pReceiver;

	pSender->Join();
	delete pSender;
}


int main( int argc, char** argv )
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 