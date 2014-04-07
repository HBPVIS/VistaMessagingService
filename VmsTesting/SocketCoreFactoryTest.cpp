// $Id$

#include <gtest/gtest.h>

#include <VmsBase/VmsVocabulary.h>
#include <VmsBase/VmsMarshallingCodec.h>

#include <VmsZMQBindings/VmsZMQSocketCoreFactory.h>
#include <VmsZMQBindings/VmsZMQSocketCore.h>


const std::string STR_INPROC_SOCKET = "inproc://MyTestSocket";
const std::string STR_TCP_SOCKET = "tcp://127.0.0.1:5555";
const std::string STR_INVALID_SOCKET = "tcp://some.infamous.address.com:5555";
/**
 *	TEST FIXTURE
 *	
 *	Note:	This test only checks whether or not socket cores
 *			are created correctly. The cores' correct functionality
 *			is tested in a separate test.
 */
class CoreFactoryTest : public ::testing::Test
{
public:
	CoreFactoryTest() {};

	virtual ~CoreFactoryTest() {};

	void SetUp()
	{
		m_pVocabulary = new VmsVocabulary();
		m_pFactory = new VmsZMQSocketCoreFactory();
	}

	void TearDown()
	{
		delete m_pFactory;
		delete m_pVocabulary;
	}


protected:
	VmsVocabulary *m_pVocabulary;
	VmsSocketCoreFactory *m_pFactory;
};

/**
 *	Just create a sender and see that we get a valid object
 */
TEST_F(CoreFactoryTest, CreateVmsSendSocketCore)
{
	//NOTE: Every socket core needs its own, distinct codec!
	VmsSocketCore *pCore= m_pFactory->CreateSendCore(STR_INPROC_SOCKET, new VmsMarshallingCodec(m_pVocabulary));
	ASSERT_TRUE(pCore != NULL);
	delete pCore;
}

/**
 *	Just create a receiver and see that we get a valid object
 */
TEST_F(CoreFactoryTest, CreateVmsReceiveSocketCore)
{
	//NOTE: Every socket core needs its own, distinct codec!
	//need a sending end first in this case...
	VmsSocketCore *pSendCore = m_pFactory->CreateSendCore(STR_INPROC_SOCKET, new VmsMarshallingCodec(m_pVocabulary));

	VmsSocketCore *pCore = m_pFactory->CreateReceiveCore(STR_INPROC_SOCKET, new VmsMarshallingCodec(m_pVocabulary));
	ASSERT_TRUE(pCore != NULL);
	
	delete pCore;
	delete pSendCore;
}

/**
 *	Just create a request receiver and see that we get a valid object
 */
TEST_F(CoreFactoryTest, CreateVmsAnswerRequestSocketCore)
{
	//NOTE: Every socket core needs its own, distinct codec!
	VmsSocketCore *pCore = m_pFactory->CreateAnswerRequestCore(STR_INPROC_SOCKET, new VmsMarshallingCodec(m_pVocabulary));
	ASSERT_TRUE(pCore != NULL);
	delete pCore;
}

/**
 *	Just create a request sender and see that we get a valid object
 */
TEST_F(CoreFactoryTest, CreateVmsSendRequestSocketCore)
{
	//NOTE: Every socket core needs its own, distinct codec!
	
	//need the server end first in order for this to work
	VmsSocketCore *pAnswerCore = m_pFactory->CreateAnswerRequestCore(STR_INPROC_SOCKET, new VmsMarshallingCodec(m_pVocabulary));

	VmsSocketCore *pCore = m_pFactory->CreateSendRequestCore(STR_INPROC_SOCKET, new VmsMarshallingCodec(m_pVocabulary));
	ASSERT_TRUE(pCore != NULL);
	
	delete pCore;
	delete pAnswerCore;
}

/**
 *	Try to create a socket with an invalid address and ensure that this fails gracefully.
 */
TEST_F(CoreFactoryTest, CheckInvalidSocketAddress)
{
	VmsSocketCore *pCore = m_pFactory->CreateSendCore(STR_INVALID_SOCKET, new VmsMarshallingCodec(m_pVocabulary));
	ASSERT_TRUE(pCore == NULL);
}

int main( int argc, char** argv )
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 