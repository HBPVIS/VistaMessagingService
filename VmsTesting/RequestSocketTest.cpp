// $Id$
#include "TestingUtils.h"

#include <gtest/gtest.h>

#include <VmsBase/VmsSendRequestSocket.h>
#include <VmsBase/VmsAnswerRequestSocket.h>

#include <VmsBase/VmsSocketFactory.h>
#include <VmsZMQBindings/VmsZMQSocketCoreFactory.h>

/**
 * Test fixture
 */
class RequestSocketTest : public ::testing::Test
{
public:
	RequestSocketTest() {};

	virtual ~RequestSocketTest() {};

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
 * Test the client side for synchronous mode i.e. wait for incoming ack immediately.
 */
TEST_F(RequestSocketTest, TestSendRequestSync)
{
	//setup receiving end before! creating the sender and make sure it is listening
	PartnerThread *pServer = 
		new PartnerThread(PartnerThread::PLAIN_RECEIVER_W_ACK, m_pContext, m_pSocketFactory, m_pVocabulary, 0);
	pServer->Run();
	pServer->WaitForIsListening();
	
	//setup sender socket and test in synchronous mode
	VmsSendRequestSocket *pRequestSocket = 
		m_pSocketFactory->CreateSendRequestSocket(STR_TEST_SOCKET, m_pVocabulary);

	TestMsg *pMsg = new TestMsg(STR_TEST_MSG);
	TestMsg *pAck = dynamic_cast<TestMsg*>(pRequestSocket->SendRequestWithAck(pMsg));

	ASSERT_TRUE(pAck != NULL);
	ASSERT_TRUE(pServer->GetMsgText() == STR_TEST_MSG);
	ASSERT_TRUE(pAck->GetMsgText() == STR_TEST_ACK);

	delete pAck;
	delete pRequestSocket;

	pServer->Join();
	delete pServer;
}


/**
 * Test the client side for asynchronous mode i.e. do a separate receive ack.
 */
TEST_F(RequestSocketTest, TestSendRequestAsync)
{
	//setup receiving end before! creating the sender and make sure it is listening
	PartnerThread *pServer = 
		new PartnerThread(PartnerThread::PLAIN_RECEIVER_W_ACK, m_pContext, m_pSocketFactory, m_pVocabulary, 0);
	pServer->Run();
	pServer->WaitForIsListening();

	//setup sender socket and test in asynchronous mode
	VmsSendRequestSocket *pRequestSocket =
		m_pSocketFactory->CreateSendRequestSocket(STR_TEST_SOCKET, m_pVocabulary);

	TestMsg *pMsg = new TestMsg(STR_TEST_MSG);
	pRequestSocket->SendRequest(pMsg);

	//...
	//client code could do something useful now before finally accepting the ack below...
	//...

	TestMsg *pAck = dynamic_cast<TestMsg*>(pRequestSocket->ReceiveAck());

	ASSERT_TRUE(pAck != NULL);
	ASSERT_TRUE(pServer->GetMsgText() == STR_TEST_MSG);
	ASSERT_TRUE(pAck->GetMsgText() == STR_TEST_ACK);

	delete pAck;
	delete pRequestSocket;

	pServer->Join();
	delete pServer;
}

/**
 * Test the server side i.e. receive incoming request, "do something" and 
 * eventually send an ack back.
 */
TEST_F(RequestSocketTest, TestAnswerRequestSocket)
{
	//setup server-side socket
	VmsAnswerRequestSocket *pServerSocket = 
		m_pSocketFactory->CreateAnswerRequestSocket(STR_TEST_SOCKET, m_pVocabulary);

	//setup sender end after! creating the receiver
	PartnerThread *pClient = 
		new PartnerThread(PartnerThread::PLAIN_SENDER_W_ACK, m_pContext, m_pSocketFactory, m_pVocabulary, 0);
	pClient->Run();
		
	TestMsg *pMsg = dynamic_cast<TestMsg*>(pServerSocket->ReceiveRequest());
	ASSERT_TRUE(pMsg->GetMsgText() == STR_TEST_MSG);
	
	//...
	//actual server code would now compute something or otherwise "think" about an useful answer
	//...
	delete pMsg;

	TestMsg *pAck = new TestMsg(STR_TEST_ACK);
	pServerSocket->SendAck(pAck);
	delete pAck;
	
	//join the client back in
	pClient->Join();

	//make sure the client got the correct ACK
	ASSERT_TRUE(pClient->GetMsgText() == STR_TEST_ACK);

	delete pClient;
	delete pServerSocket;
}

TEST_F(RequestSocketTest, TestPairing)
{
	//setup receiving end before! creating the sender and make sure it is listening
	PartnerThread *pServer = 
		new PartnerThread(PartnerThread::VMS_RECEIVER_W_ACK, m_pContext, m_pSocketFactory, m_pVocabulary, 0);
	pServer->Run();
	pServer->WaitForIsListening();

	//setup sender socket and test in asynchronous mode
	VmsSendRequestSocket *pRequestSocket =
		m_pSocketFactory->CreateSendRequestSocket(STR_TEST_SOCKET, m_pVocabulary);

	TestMsg *pMsg = new TestMsg(STR_TEST_MSG);
	pRequestSocket->SendRequest(pMsg);
	delete pMsg;

	//...
	//client code could do something useful now before finally accepting the ack below...
	//...

	TestMsg *pAck = dynamic_cast<TestMsg*>(pRequestSocket->ReceiveAck());

	ASSERT_TRUE(pAck != NULL);
	ASSERT_TRUE(pAck->GetMsgText() == STR_TEST_ACK);

	delete pAck;
	delete pRequestSocket;

	pServer->Join();
	ASSERT_TRUE(pServer->GetMsgText() == STR_TEST_MSG);
	
	delete pServer;
}


int main( int argc, char** argv )
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 




