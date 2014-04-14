// $Id$

#include <gtest/gtest.h>

#include "../VmsBase/VmsTypedReceiveSocket.h"
#include "../VmsBase/VmsTypedSendSocket.h"

#include "../VmsBase/VmsVocabulary.h"
#include "../VmsBase/VmsSocketFactory.h"

#include "../VmsZMQBindings/VmsZMQSocketCoreFactory.h"

#include "TestingUtils.h"

#include <VistaBase/VistaTimeUtils.h>

const std::string STR_ADDRESS = "inproc://my_test_socket";



class TypedSendReceiveSocketTest : public ::testing::Test
{
public:
	TypedSendReceiveSocketTest() {};

	virtual ~TypedSendReceiveSocketTest() {};

	void SetUp()
	{
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
	}


protected:
	VmsSocketFactory *m_pSocketFactory;
	VmsZMQSocketCoreFactory *m_pCoreFactory;

	void *m_pContext;
private:

};

/**
 * Test simple ping-pong between the socket pair
 */
TEST_F(TypedSendReceiveSocketTest, TestConnection)
{
	VmsTypedSendSocket<TestMsg> *pSender = m_pSocketFactory->CreateTypedSendSocket<TestMsg>(STR_ADDRESS);
	VmsTypedReceiveSocket<TestMsg> *pReceiver = m_pSocketFactory->CreateTypedReceiveSocket<TestMsg>(STR_ADDRESS, new TestMsg::TCreator);

	TestMsg oMsg("Hello World!");
	pSender->Send(&oMsg);

	TestMsg *pWhatIHeard = pReceiver->Receive();

	EXPECT_TRUE(pWhatIHeard != NULL);
	if(pWhatIHeard != NULL)
		ASSERT_EQ(oMsg.GetMsgText(), pWhatIHeard->GetMsgText());

	delete pReceiver;
	delete pSender;
}

/**
 * Test try receive functionality
 */
TEST_F(TypedSendReceiveSocketTest, TestTryReceive)
{
	VmsTypedSendSocket<TestMsg> *pSender = m_pSocketFactory->CreateTypedSendSocket<TestMsg>(STR_ADDRESS);
	VmsTypedReceiveSocket<TestMsg> *pReceiver = m_pSocketFactory->CreateTypedReceiveSocket<TestMsg>(STR_ADDRESS, new TestMsg::TCreator);

	const int iWaitMsecs = 5;
	VistaTimer oTimer;
	VistaType::microtime tStart= oTimer.GetMicroTime();
	TestMsg *pWhatIHeard = pReceiver->TryReceive(iWaitMsecs);
	VistaType::microtime tEnd= oTimer.GetMicroTime();

	EXPECT_TRUE(pWhatIHeard == NULL);
	EXPECT_GE(1000.0*(tEnd-tStart), static_cast<double>(iWaitMsecs));

	TestMsg oMsg("Hello World!");
	pSender->Send(&oMsg);

	tStart= oTimer.GetMicroTime();
	pWhatIHeard = pReceiver->TryReceive(iWaitMsecs);
	tEnd= oTimer.GetMicroTime();

	EXPECT_TRUE(pWhatIHeard != NULL);
	EXPECT_LE(1000.0*(tEnd-tStart), static_cast<double>(iWaitMsecs));
	if(pWhatIHeard != NULL)
		ASSERT_EQ(oMsg.GetMsgText(), pWhatIHeard->GetMsgText());

	delete pReceiver;
	delete pSender;
}


int main( int argc, char** argv )
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 
