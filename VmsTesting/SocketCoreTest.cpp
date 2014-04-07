// $Id$

#include <gtest/gtest.h>

#include <VmsBase/VmsMarshallingCodec.h>

#include <VmsZMQBindings/VmsZMQSocketCore.h>
#include <VmsZMQBindings/VmsZMQSocketCoreFactory.h>

#include "TestingUtils.h"

#include <zmq.h>

#include <cassert>

class SocketCoreTest : public ::testing::Test
{
public:
	SocketCoreTest() {};

	virtual ~SocketCoreTest() {};

	void SetUp()
	{
		m_pVocabulary = new VmsVocabulary;
		TestMsg oMsg;
		m_pVocabulary->RegisterMessage(&oMsg, new TestMsg::TCreator);

		m_pFactory = new VmsZMQSocketCoreFactory();
		m_pContext = m_pFactory->GetZMQContext();
	}

	void TearDown()
	{
		delete m_pFactory;
		delete m_pVocabulary;
	}


protected:
	VmsVocabulary *m_pVocabulary;
	VmsMarshallingCodec *m_pCodec;
	VmsZMQSocketCoreFactory *m_pFactory;
	void *m_pContext;
private:

};

/**
 * Test sending out a message via the zmq socket core implementation
 */
TEST_F(SocketCoreTest, TestZMQCoreSendWithAck)
{
	//create the "other end" for communication testing
	//	we know we don't need the socket factory so we pass in NULL 
	//	here which you SHOULD NOT normally do!
	PartnerThread *pServer = 
		new PartnerThread(PartnerThread::PLAIN_RECEIVER_W_ACK, m_pContext, NULL, m_pVocabulary, 0);
	pServer->Run();
	pServer->WaitForIsListening();

	//create core
	VmsSocketCore *pCore = m_pFactory->CreateSendRequestCore(STR_TEST_SOCKET, new VmsMarshallingCodec(m_pVocabulary));

	//send out test message
	TestMsg *pMsg = new TestMsg(STR_TEST_MSG);
	pCore->Send(pMsg);
	delete pMsg;

	//retrieve ack
	pMsg = dynamic_cast<TestMsg*>(pCore->Receive());
	ASSERT_TRUE(pMsg != NULL);
	ASSERT_TRUE(pMsg->GetMsgText() == STR_TEST_ACK);
	delete pMsg;

	ASSERT_TRUE(pServer->GetMsgText() == STR_TEST_MSG);

	pServer->Join();
	delete pServer;
	delete pCore;
}

/**
 * Test receiving a message via the zmq socket core implementation
 */
TEST_F(SocketCoreTest, TestZMQCoreReceiveWithAck)
{
	VmsSocketCore *pCore = m_pFactory->CreateAnswerRequestCore(STR_TEST_SOCKET, new VmsMarshallingCodec(m_pVocabulary));
		
	//create the "other end" for communication testing
	//	we know we don't need the socket factory so we pass in NULL 
	//	here which you SHOULD NOT normally do!
	PartnerThread *pClient = 
		new PartnerThread(PartnerThread::PLAIN_SENDER_W_ACK, m_pContext, NULL, m_pVocabulary, 0);
	pClient->Run();

	TestMsg *pMsg = dynamic_cast<TestMsg*>(pCore->Receive());
	ASSERT_TRUE(pMsg->GetMsgText() == STR_TEST_MSG);
	delete pMsg;

	TestMsg *pAck = new TestMsg(STR_TEST_ACK);
	pCore->Send(pAck);
	delete pAck;

	//join the client back in
	pClient->Join();

	//make sure the client got the correct ACK
	ASSERT_TRUE(pClient->GetMsgText() == STR_TEST_ACK);

	delete pClient;
	delete pCore;
}


int main( int argc, char** argv )
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 