// $Id$

#include <gtest/gtest.h>

#include "../VmsBase/VmsMarshallingCodec.h"
#include "../VmsBase/VmsSerializingCodec.h"

#include "../VmsBase/VmsVocabulary.h"

#include "TestingUtils.h"

#include <VistaInterProcComm/Connections/VistaByteBufferSerializer.h>
#include <VistaInterProcComm/Connections/VistaByteBufferSerializer.h>

class MsgCodecTest : public ::testing::Test
{
public:
	MsgCodecTest() {};

	virtual ~MsgCodecTest() {};

	void SetUp()
	{
		m_pVocabulary = new VmsVocabulary;
		TestMsg oMsg;
		m_pVocabulary->RegisterMessage(&oMsg, new TestMsg::TCreator);
	}

	void TearDown()
	{
		delete m_pVocabulary;
	}

	void CheckCodec(VmsMsgCodec *pCodec)
	{
		//encode stuff
		TestMsg oMsg("Hello World!");
		VistaByteBufferSerializer oSerializer;
		int iSize = pCodec->EncodeMsg(&oMsg, oSerializer);
		ASSERT_GT(iSize, 0);
		ASSERT_EQ(iSize, oSerializer.GetBufferSize());

		//decode again
		VistaByteBufferDeSerializer oDeSer;
		oDeSer.SetBuffer(oSerializer.GetBuffer(), oSerializer.GetBufferSize(), false);
		TestMsg *pWhatIHeard = dynamic_cast<TestMsg*>(pCodec->DecodeMsg(oDeSer));

		ASSERT_TRUE(pWhatIHeard != NULL);
		ASSERT_EQ(oMsg.GetMsgText(), pWhatIHeard->GetMsgText());
	}


protected:
	VmsVocabulary *m_pVocabulary;
private:
	
};

/**
 *	Test encoding via marshalling through the 
 *	message vocabulary.
 */
TEST_F(MsgCodecTest, TestMarshallingCodec)
{
	VmsMarshallingCodec *pCodec = new VmsMarshallingCodec(m_pVocabulary);

	this->CheckCodec(pCodec);

	delete pCodec;
}

/**
 * Test encoding via simple serialization
 */
TEST_F(MsgCodecTest, TestSerializingCodec)
{
	VmsSerializingCodec<TestMsg> *pCodec = new VmsSerializingCodec<TestMsg>(new TestMsg::TCreator);

	this->CheckCodec(pCodec);

	delete pCodec;
}


int main( int argc, char** argv )
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 
