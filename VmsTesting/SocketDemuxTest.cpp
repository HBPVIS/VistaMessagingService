// $Id$
#include <gtest/gtest.h>

#include "TestingUtils.h"

#include <VmsBase/VmsSocketDemux.h>
#include <VmsZMQBindings/VmsZMQSocketDemux.h>

#include <VmsBase/VmsSendSocket.h>

#include <VmsBase/VmsSocketFactory.h>
#include <VmsZMQBindings/VmsZMQSocketCoreFactory.h>

#include <VistaAspects/VistaAspectsUtils.h>

#include <cmath>

const int NUM_CHANNELS = 5;
const int NUM_POLLS = 10;

/**
 *	Test fixture -- build the default stuff
 */
class SocketDemuxTest : public ::testing::Test
{
public:
	SocketDemuxTest() {};

	virtual ~SocketDemuxTest() {};

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

	/**
	 * Setup pairs of SendSocket->ReceiveSocket for testing
	 */
	void SetupPlainSendReceivePairs(std::vector<VmsSendSocket*> &vecSenders, 
									std::vector<VmsReceiveSocket*> &vecReceivers)
	{
		const std::string strSocketBase = "inproc://test_socket_";
		//init a number of sockets
		for(size_t i=0; i<NUM_CHANNELS; ++i)
		{
			std::string strCurrentAddress = strSocketBase + VistaAspectsConversionStuff::ConvertToString(int(i));
			vecSenders[i]   = m_pSocketFactory->CreateSendSocket(strCurrentAddress, m_pVocabulary);
			vecReceivers[i] = m_pSocketFactory->CreateReceiveSocket(strCurrentAddress, m_pVocabulary);
		}
	}

	/**
	 *	Actually perform random sends from one of the senders and check if the receiver 
	 *	intercepts it.
	 */
	void TestSendReceivePoll(std::vector<VmsSendSocket*> &vecSenders, 
						     std::vector<VmsReceiveSocket*> &vecReceivers,
				             size_t iTimeOut)
	{
		VmsZMQSocketDemux oDemux;

		for(size_t t=0; t<NUM_POLLS; ++t)
		{
			size_t iSender = static_cast<size_t>(floor(double(rand())/double(RAND_MAX)*NUM_CHANNELS));
			TestMsg oMsg(STR_TEST_MSG);
			vecSenders[iSender]->Send(&oMsg);
			
			int iReceiver = oDemux.DemultiplexSockets(vecReceivers, iTimeOut);
			
			//make sure we got the right receiver
			ASSERT_GE(iReceiver, 0);
			ASSERT_EQ(iSender, iReceiver);

			//make sure we got the right message
			TestMsg *pRecvMsg = dynamic_cast<TestMsg*>(vecReceivers[iReceiver]->Receive());
			ASSERT_TRUE(pRecvMsg != NULL);
			ASSERT_EQ(STR_TEST_MSG, pRecvMsg->GetMsgText());
			delete pRecvMsg;
		}
	}

	void CleanupPlainSendReceivePairs(std::vector<VmsSendSocket*> &vecSenders, 
									  std::vector<VmsReceiveSocket*> &vecReceivers)
	{
		for(size_t i=0; i<NUM_CHANNELS; ++i)
		{
			delete vecSenders[i];
			delete vecReceivers[i];
		}
	}

	VmsVocabulary *m_pVocabulary;
	VmsSocketFactory *m_pSocketFactory;
	VmsZMQSocketCoreFactory *m_pCoreFactory;

	void *m_pContext;
};

/**
 * Test send-receive pairing without timeout (NTO)
 */
TEST_F(SocketDemuxTest, TestDemuxForPlainReceiverNTO)
{
	std::vector<VmsSendSocket*> vecSenders(NUM_CHANNELS);
	std::vector<VmsReceiveSocket*> vecReceivers(NUM_CHANNELS);

	this->SetupPlainSendReceivePairs(vecSenders, vecReceivers);

	this->TestSendReceivePoll(vecSenders, vecReceivers, 0);

	this->CleanupPlainSendReceivePairs(vecSenders, vecReceivers);
}

/**
 * Test send-receive pairing with timeout (WTO)
 */
TEST_F(SocketDemuxTest, TestDemuxForPlainReceiverWTO)
{
	std::vector<VmsSendSocket*> vecSenders(NUM_CHANNELS);
	std::vector<VmsReceiveSocket*> vecReceivers(NUM_CHANNELS);

	this->SetupPlainSendReceivePairs(vecSenders, vecReceivers);

	this->TestSendReceivePoll(vecSenders, vecReceivers, 50);

	this->CleanupPlainSendReceivePairs(vecSenders, vecReceivers);
}

/**
 * Test message message intercept w/o having one ==> should fail
 */
TEST_F(SocketDemuxTest, TestDemuxWOMessage)
{
	std::vector<VmsSendSocket*> vecSenders(NUM_CHANNELS);
	std::vector<VmsReceiveSocket*> vecReceivers(NUM_CHANNELS);

	this->SetupPlainSendReceivePairs(vecSenders, vecReceivers);

	VmsZMQSocketDemux oDemux;
	int iReceiver = oDemux.DemultiplexSockets(vecReceivers, 50);
	ASSERT_EQ(VmsSocketDemux::DM_TIMEOUT, iReceiver);

	//cleanup
	this->CleanupPlainSendReceivePairs(vecSenders, vecReceivers);
}

int main( int argc, char** argv )
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 


