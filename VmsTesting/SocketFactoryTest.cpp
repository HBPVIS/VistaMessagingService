// $Id$

#include <gtest/gtest.h>

#include <VmsBase/VmsSocketFactory.h>

#include <VmsBase/VmsVocabulary.h>

#include <VmsBase/VmsSendSocket.h>
#include <VmsBase/VmsReceiveSocket.h>
#include <VmsBase/VmsSendRequestSocket.h>
#include <VmsBase/VmsAnswerRequestSocket.h>

#include <VmsZMQBindings/VmsZMQSocketCoreFactory.h>

const std::string STR_INPROC_SOCKET = "inproc://MyTestSocket";
const std::string STR_TCP_SOCKET = "tcp://127.0.0.1:5555";
const std::string STR_INVALID_SOCKET = "tcp://some.infamous.address.com:5555";
const std::string STR_INVALID_PROTOCOL_SOCKET = "someproto://some.infamous.address.com:5555";

/**
 *	TEST FIXTURE
 *
 *	Note:	This set of tests intentionally only validates the creation of 
 *			socket cores via the VmsSocketFactory. The cores' functionality 
 *			is ensured by a separate test.
 */
class SocketFactoryTest : public ::testing::Test
{
public:
	SocketFactoryTest() {};

	virtual ~SocketFactoryTest() {};

	void SetUp()
	{
		m_pVocabulary = new VmsVocabulary();

		m_pCoreFactory = new VmsZMQSocketCoreFactory();
		m_pFactory = new VmsSocketFactory();
		
		m_pCoreFactory->RegisterDefaultZMQProtocols(m_pFactory);
	}

	void TearDown()
	{
		delete m_pFactory;
		delete m_pCoreFactory;
		delete m_pVocabulary;
	}


protected:
	VmsVocabulary *m_pVocabulary;
	VmsZMQSocketCoreFactory *m_pCoreFactory;
	VmsSocketFactory *m_pFactory;
};

/**
 *	Just create a sender and see that we get a valid object
 */
TEST_F(SocketFactoryTest, CreateVmsSendSocket)
{
	VmsSendSocket *pSocket = m_pFactory->CreateSendSocket(STR_INPROC_SOCKET, m_pVocabulary);
	ASSERT_TRUE(pSocket != NULL);
	delete pSocket;
}

/**
 *	Just create a receiver and see that we get a valid object
 */
TEST_F(SocketFactoryTest, CreateVmsReceiveSocket)
{
	//need a sending end first in this case...
	VmsSendSocket *pSender = m_pFactory->CreateSendSocket(STR_INPROC_SOCKET, m_pVocabulary);

	VmsReceiveSocket *pSocket = m_pFactory->CreateReceiveSocket(STR_INPROC_SOCKET, m_pVocabulary);
	ASSERT_TRUE(pSocket != NULL);
	
	delete pSocket;
	delete pSender;
}

/**
 *	Just create a request receiver and see that we get a valid object
 */
TEST_F(SocketFactoryTest, CreateVmsAnswerRequestSocket)
{
	VmsAnswerRequestSocket *pSocket = m_pFactory->CreateAnswerRequestSocket(STR_INPROC_SOCKET, m_pVocabulary);
	ASSERT_TRUE(pSocket != NULL);
	delete pSocket;
}

/**
 *	Just create a request sender and see that we get a valid object
 */
TEST_F(SocketFactoryTest, CreateVmsSendRequestSocket)
{
	//need the server end first in order for this to work
	VmsAnswerRequestSocket *pServer = m_pFactory->CreateAnswerRequestSocket(STR_INPROC_SOCKET, m_pVocabulary);

	VmsSendRequestSocket *pSocket = m_pFactory->CreateSendRequestSocket(STR_INPROC_SOCKET, m_pVocabulary);
	ASSERT_TRUE(pSocket != NULL);
	
	delete pSocket;
	delete pServer;
}

/**
 *	Try to create a socket with an invalid address and ensure that this fails gracefully.
 */
TEST_F(SocketFactoryTest, CheckInvalidSocketAddress)
{
	VmsSendSocket *pSocket = m_pFactory->CreateSendSocket(STR_INVALID_SOCKET, m_pVocabulary);
	ASSERT_TRUE(pSocket == NULL);
}

/**
 *	Try to create a socket with an unknown protocol
 */
TEST_F(SocketFactoryTest, CheckInvalidProtocol)
{
	VmsSendSocket *pSocket = m_pFactory->CreateSendSocket(STR_INVALID_PROTOCOL_SOCKET, m_pVocabulary);
	ASSERT_TRUE(pSocket == NULL);
}

int main( int argc, char** argv )
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 