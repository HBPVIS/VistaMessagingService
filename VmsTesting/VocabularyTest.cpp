// $Id$
#include <VmsBase/VmsVocabulary.h>

#include <VistaInterProcComm/Connections/VistaByteBufferDeSerializer.h>
#include <VistaInterProcComm/Connections/VistaByteBufferSerializer.h>


#include <VistaAspects/VistaSerializable.h>
#include <VistaAspects/VistaSerializer.h>
#include <VistaAspects/VistaDeSerializer.h>
#include <VistaAspects/VistaGenericFactory.h>

#include <gtest/gtest.h>

#include <string>

/** dummy message for simple identity testing */
class DummyMessage : public IVistaSerializable
{
public:
	/** typedef the creator for easy access */
	typedef VistaVanillaCreator<DummyMessage, IVistaSerializable> TCreator;
	
	/** need an empty c'tor for simple creation upon unmarshalling */
	DummyMessage() : m_iInt(0), m_strString("") {}
	
	DummyMessage(const int iTest, const std::string &strTest) 
		: m_iInt(iTest), m_strString(strTest) 
	{}
	virtual ~DummyMessage(){}

	bool operator==(const DummyMessage& rOther )
	{
		return rOther.m_iInt == this->m_iInt && rOther.m_strString == this->m_strString;
	}

	virtual int Serialize( IVistaSerializer& rSerializer) const
	{
		//for simplicity, we omit proper error handling here (i.e. checking the return of each write)
		int iNumBytesWritten = 0;
		iNumBytesWritten += rSerializer.WriteInt32(m_iInt);
		iNumBytesWritten += rSerializer.WriteInt32(static_cast<VistaType::uint32>(m_strString.size()));
		iNumBytesWritten += rSerializer.WriteString(m_strString);
		return iNumBytesWritten;
	}

	virtual int DeSerialize( IVistaDeSerializer& rDeSerializer) 
	{
		//for simplicity, we omit proper error handling here (i.e. checking the return of each write)
		int iNumBytesRead = 0;
		iNumBytesRead += rDeSerializer.ReadInt32(m_iInt);
		int iLen = 0;
		iNumBytesRead += rDeSerializer.ReadInt32(iLen);
		iNumBytesRead += rDeSerializer.ReadString(m_strString, iLen);
		return iNumBytesRead;
	}

	virtual std::string GetSignature() const
	{
		return "__DummyMessage__";
	}

protected:
private:
	int m_iInt;
	std::string m_strString;
};

class VocabularyTest : public ::testing::Test
{
public:
	VocabularyTest() {};

	virtual ~VocabularyTest() {};

	void SetUp()
	{
		m_pVocabulary = new VmsVocabulary();
	}

	void TearDown()
	{
		delete m_pVocabulary;
	}


protected:
	VmsVocabulary *m_pVocabulary;
private:

};

/**
 *
 */
TEST_F(VocabularyTest, TestRegistration)
{
	DummyMessage oMsg(12,"Hello World!");
	ASSERT_FALSE(m_pVocabulary->HasMessage(&oMsg));

	ASSERT_GE(m_pVocabulary->RegisterMessage(&oMsg, new DummyMessage::TCreator()), 0);

	ASSERT_TRUE(m_pVocabulary->HasMessage(&oMsg));
}

TEST_F(VocabularyTest, TestMessageMarshalling)
{
	DummyMessage oMsg(12, "Hello World!");
	m_pVocabulary->RegisterMessage(&oMsg, new DummyMessage::TCreator());

	//write message 
	VistaByteBufferSerializer oSer;
	int iNumBytes = m_pVocabulary->MarshalMessage(&oMsg, oSer);
	ASSERT_GT(iNumBytes, 0);

	//read message 
	VistaByteBufferDeSerializer oDeSer;
	oDeSer.SetBuffer(oSer.GetBuffer(), oSer.GetBufferSize());
	DummyMessage *pResult = dynamic_cast<DummyMessage*>(m_pVocabulary->UnMarshalMessage(oDeSer));

	//**magically** message should be identical 
	ASSERT_TRUE(pResult != NULL);
	ASSERT_TRUE(*pResult == oMsg);

	//remember: client code has to free the result!
	delete pResult;
}

TEST_F(VocabularyTest, TestMarshallingUnregisteredMsg)
{
	DummyMessage oMsg(12, "Hello World!");

	//write message 
	VistaByteBufferSerializer oSer;
	int iNumBytes = m_pVocabulary->MarshalMessage(&oMsg, oSer);
	ASSERT_TRUE(iNumBytes == -1);

	//read message 
	VistaByteBufferDeSerializer oDeSer;
	oDeSer.SetBuffer(oSer.GetBuffer(), oSer.GetBufferSize());
	DummyMessage *pResult = dynamic_cast<DummyMessage*>(m_pVocabulary->UnMarshalMessage(oDeSer));

	//**magically** message should be identical 
	ASSERT_TRUE(pResult == NULL);
}

int main( int argc, char** argv )
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
} 