// $Id$
#include <VmsBase/VmsVocabulary.h>
#include <VmsBase/VmsSocketFactory.h>

#include <VmsBase/VmsAnswerRequestSocket.h>
#include <VmsBase/VmsReceiveSocket.h>

#include <VistaInterProcComm/Connections/VistaByteBufferSerializer.h>
#include <VistaInterProcComm/Connections/VistaByteBufferDeSerializer.h>
#include <VistaInterProcComm/Concurrency/VistaThread.h>
#include <VistaInterProcComm/Concurrency/VistaThreadEvent.h>

#include <VistaAspects/VistaSerializable.h>
#include <VistaAspects/VistaSerializer.h>
#include <VistaAspects/VistaDeSerializer.h>
#include <VistaAspects/VistaGenericFactory.h>

#include <VistaBase/VistaTimeUtils.h>
#include <VistaBase/VistaStreamUtils.h>

#include <zmq.h>

#include <string>
#include <cassert>
#include <cstring>

const std::string STR_TEST_SOCKET = "inproc://my_test_socket"; //"tcp://127.0.0.1:5555";
const std::string STR_TEST_MSG = "Hello World!";
const std::string STR_TEST_ACK = "Have a nice day!";

/**
 * SHARED TESTING UTILITIES FOR VMS
 */
static void DumpZMQErrorState();

/**
 * Simple test string message
 */
class TestMsg : public IVistaSerializable
{
public:
	typedef VistaVanillaCreator<TestMsg, IVistaSerializable> TCreator;

	TestMsg() : m_strMsg("") {}
	TestMsg(const std::string &strMsg) : m_strMsg(strMsg) {}
	virtual ~TestMsg(){}

	const std::string &GetMsgText() const
	{
		return m_strMsg;
	}

	virtual int Serialize( IVistaSerializer& oSer) const
	{
		oSer.WriteInt32(static_cast<int>(m_strMsg.length()));
		oSer.WriteString(m_strMsg);
		return static_cast<int>(sizeof(int)+m_strMsg.length());
	}

	virtual int DeSerialize( IVistaDeSerializer& oDeSer) 
	{
		int iLen;
		oDeSer.ReadInt32(iLen);
		oDeSer.ReadString(m_strMsg, iLen);
		return sizeof(int)+iLen;
	}

	virtual std::string GetSignature() const
	{
		return "TestMsg";
	}

private:
	std::string m_strMsg;
};

/**
 * Helper thread implementing "the other end" of communication.
 */
class PartnerThread : public VistaThread
{
public:
	enum 
	{
		PLAIN_SENDER_W_ACK,
		PLAIN_RECEIVER_W_ACK,
		VMS_RECEIVER_W_ACK,
		PLAIN_SENDER_NO_ACK,
		PLAIN_RECEIVER_NO_ACK,
		VMS_RECEIVER_NO_ACK
	};
	PartnerThread(int iMode, void *pContext, VmsSocketFactory *pFactory, VmsVocabulary *pVoc, int iSleep)
		:	m_iMode(iMode),
			m_pVocabulary(pVoc),
			m_pContext(pContext),
			m_pSocketFactory(pFactory),
			m_pIsListening(new VistaThreadEvent(true)),
			m_strMsgText(""),
			m_iSleepBeforeSend(iSleep)
	{
	}

	virtual ~PartnerThread()
	{
		delete m_pIsListening;
	}

	virtual void ThreadBody() 
	{
		switch(m_iMode)
		{
		case PLAIN_SENDER_W_ACK:
			this->SendTestMsgPlainZMQWithAck();
			break;
		case PLAIN_RECEIVER_W_ACK:
			this->ReceiveTestMsgPlainZMQWithAck();
			break;
		case VMS_RECEIVER_W_ACK:
			this->ReceiveTestMsgVmsWithAck();
			break;
		case PLAIN_SENDER_NO_ACK:
			this->SendTestMsgPlainZMQNoAck();
			break;
		case PLAIN_RECEIVER_NO_ACK:
			this->ReceiveTestMsgPlainZMQNoAck();
			break;
		case VMS_RECEIVER_NO_ACK:
			this->ReceiveTestMsgVmsNoAck();
			break;
		}
	}

	void WaitForIsListening() 
	{
		m_pIsListening->WaitForEvent(true);
		m_pIsListening->ResetThisEvent();
	}

	const std::string &GetMsgText() const
	{
		return m_strMsgText;
	}

protected:
	/**
	 * Use plain ZMQ coding to send a test message and receive an ACK
	 */
	void SendTestMsgPlainZMQWithAck();
	/**
	 * Use plain ZMQ coding to receive a test message and send an ack
	 */
	void ReceiveTestMsgPlainZMQWithAck();
	/** 
	 * Same thing as above but with fully fledged Vms AnswerRequestSocket
	 */
	void ReceiveTestMsgVmsWithAck();
	/**
	 * Use plain ZMQ coding to send a test message w/o receiving a 
	 * follow-up ack.
	 */
	void SendTestMsgPlainZMQNoAck();
	/**
	 * Receive a test message in plain ZMQ w/o sending a follow-up ack
	 */
	void ReceiveTestMsgPlainZMQNoAck();
	/**
	 * Receive a test message via Vms ReceiveSocket
	 */
	void ReceiveTestMsgVmsNoAck();

private:
	int m_iMode;
	void *m_pContext;
	VmsSocketFactory *m_pSocketFactory;
	VmsVocabulary *m_pVocabulary;
	VistaThreadEvent *m_pIsListening;
	std::string m_strMsgText;
	int m_iSleepBeforeSend;
};

//*****************************************************************************
//************************* IMPLEMENTATION ************************************
//*****************************************************************************
void PartnerThread::SendTestMsgPlainZMQWithAck()
{
	void *pSocket = zmq_socket(m_pContext, ZMQ_REQ);
	assert(pSocket != NULL);

	int iret = zmq_connect(pSocket, STR_TEST_SOCKET.c_str());
	assert(iret == 0);

	//send out test msg
	TestMsg oMsg(STR_TEST_MSG);
	VistaByteBufferSerializer oSer;
	m_pVocabulary->MarshalMessage(&oMsg, oSer);

	zmq_msg_t oRawMsg;
	iret = zmq_msg_init_size(&oRawMsg, oSer.GetBufferSize());
	assert(iret == 0);

	memcpy(zmq_msg_data(&oRawMsg), oSer.GetBuffer(), oSer.GetBufferSize());

	iret = zmq_msg_send(&oRawMsg, pSocket, 0);
	assert(iret > 0);

	iret = zmq_msg_close(&oRawMsg);
	assert(iret == 0);

	zmq_msg_t oRawAck;
	iret = zmq_msg_init(&oRawAck);
	assert(iret == 0);

	//receive ack
	iret = zmq_msg_recv(&oRawAck, pSocket, 0);

	assert(iret > 0);

	//manually decode
	VistaByteBufferDeSerializer oDeSer;
	oDeSer.SetBuffer(static_cast<VistaType::byte*>(zmq_msg_data(&oRawAck)), static_cast<int>(zmq_msg_size(&oRawAck)));
	TestMsg *pAck = dynamic_cast<TestMsg*>(m_pVocabulary->UnMarshalMessage(oDeSer));
	assert(pAck != NULL);
	m_strMsgText = pAck->GetMsgText();

	iret = zmq_msg_close(&oRawAck);
	assert(iret == 0);

	iret = zmq_close(pSocket);
	assert(iret == 0);
}

void PartnerThread::ReceiveTestMsgPlainZMQWithAck()
{
	void *pSocket = zmq_socket(m_pContext, ZMQ_REP);
	assert(pSocket != NULL);

	int iret = zmq_bind(pSocket, STR_TEST_SOCKET.c_str());
	assert(iret == 0);

	m_pIsListening->SignalEvent();

	zmq_msg_t oRawMsg;
	iret = zmq_msg_init(&oRawMsg);
	assert(iret == 0);

	//receive message
	iret = zmq_msg_recv(&oRawMsg, pSocket, 0);
	assert(iret > 0);

	//manually decode
	VistaByteBufferDeSerializer oDeSer;
	oDeSer.SetBuffer(static_cast<VistaType::byte*>(zmq_msg_data(&oRawMsg)), static_cast<int>(zmq_msg_size(&oRawMsg)));
	TestMsg *pMsg = dynamic_cast<TestMsg*>(m_pVocabulary->UnMarshalMessage(oDeSer));
	assert(pMsg != NULL);
	m_strMsgText = pMsg->GetMsgText();

	iret = zmq_msg_close(&oRawMsg);
	assert(iret == 0);

	//send manual ack
	TestMsg oAck(STR_TEST_ACK);
	VistaByteBufferSerializer oSer;
	m_pVocabulary->MarshalMessage(&oAck, oSer);

	zmq_msg_t oRawAck;
	iret = zmq_msg_init_size(&oRawAck, oSer.GetBufferSize());
	assert(iret == 0);

	memcpy(zmq_msg_data(&oRawAck), oSer.GetBuffer(), oSer.GetBufferSize());

	iret = zmq_msg_send(&oRawAck, pSocket, 0);
	assert(iret > 0);

	iret = zmq_msg_close(&oRawAck);
	assert(iret == 0);

	iret = zmq_close(pSocket);
	assert(iret == 0);
}

void PartnerThread::ReceiveTestMsgVmsWithAck()
{
	//setup server-side socket
	VmsAnswerRequestSocket *pServerSocket =
		m_pSocketFactory->CreateAnswerRequestSocket(STR_TEST_SOCKET, m_pVocabulary);
	m_pIsListening->SignalEvent();

	TestMsg *pMsg = dynamic_cast<TestMsg*>(pServerSocket->ReceiveRequest());
	m_strMsgText = pMsg->GetMsgText();

	//...
	//actual server code would now compute something or otherwise "think" about an useful answer
	//...
	delete pMsg;
	
	TestMsg *pAck = new TestMsg(STR_TEST_ACK);
	pServerSocket->SendAck(pAck);
	delete pAck;

	delete pServerSocket;
}

void PartnerThread::SendTestMsgPlainZMQNoAck()
{
	void *pSocket = zmq_socket(m_pContext, ZMQ_PUB);
	int iret = zmq_bind(pSocket, STR_TEST_SOCKET.c_str());
	assert(iret == 0);

	m_pIsListening->SignalEvent();
	//BAD HACK TO FIX "late subscriber" issue ==> will be fixed with proper handshake soon
	VistaTimeUtils::Sleep(50);

	//send out test msg
	TestMsg oMsg(STR_TEST_MSG);
	VistaByteBufferSerializer oSer;
	m_pVocabulary->MarshalMessage(&oMsg, oSer);

	zmq_msg_t oRawMsg;
	iret = zmq_msg_init_size(&oRawMsg, oSer.GetBufferSize());
	assert(iret == 0);

	memcpy(zmq_msg_data(&oRawMsg), oSer.GetBuffer(), oSer.GetBufferSize());

	if(m_iSleepBeforeSend > 0)
		VistaTimeUtils::Sleep(m_iSleepBeforeSend);

	iret = zmq_msg_send(&oRawMsg, pSocket, 0);
	assert(iret > 0);

	iret = zmq_msg_close(&oRawMsg);
	assert(iret == 0);

	iret = zmq_close(pSocket);
	assert(iret == 0);
}

void PartnerThread::ReceiveTestMsgPlainZMQNoAck()
{
	//create subscriber
	void *pSocket = zmq_socket(m_pContext, ZMQ_SUB);
	assert(pSocket != NULL);
	//connect to publisher
	int iret = zmq_connect(pSocket, STR_TEST_SOCKET.c_str());
	assert(iret == 0);
	//subscribe to everything
	iret = zmq_setsockopt(pSocket, ZMQ_SUBSCRIBE, NULL, 0);
	assert(iret == 0);

	zmq_msg_t oRawMsg;
	iret = zmq_msg_init(&oRawMsg);
	assert(iret == 0);

	//receive message
	iret = zmq_msg_recv(&oRawMsg, pSocket, 0);
	assert(iret > 0);

	//manually decode
	VistaByteBufferDeSerializer oDeSer;
	oDeSer.SetBuffer(static_cast<VistaType::byte*>(zmq_msg_data(&oRawMsg)), static_cast<int>(zmq_msg_size(&oRawMsg)));
	TestMsg *pMsg = dynamic_cast<TestMsg*>(m_pVocabulary->UnMarshalMessage(oDeSer));
	assert(pMsg != NULL);
	m_strMsgText = pMsg->GetMsgText();

	//cleanup message and socket
	iret = zmq_msg_close(&oRawMsg);
	assert(iret == 0);

	iret = zmq_close(pSocket);
	assert(iret == 0);
}

void PartnerThread::ReceiveTestMsgVmsNoAck()
{
	VmsReceiveSocket *pReceiver = 
		m_pSocketFactory->CreateReceiveSocket(STR_TEST_SOCKET, m_pVocabulary);

	TestMsg *pMsg = dynamic_cast<TestMsg*>(pReceiver->Receive());
	assert(pMsg != NULL);
	m_strMsgText = pMsg->GetMsgText();
	delete pMsg;

	delete pReceiver;
}

static void DumpZMQErrorState()
{
	int ierror = zmq_errno();
	switch(ierror)
	{
	case EAGAIN:
		vstr::errp() << "EAGAIN" << std::endl;
		break;
	case ENOTSUP:
		vstr::errp() << "ENOTSUP" << std::endl;
		break;
	case ETERM:
		vstr::errp() << "ETERM" << std::endl;
		break;
	case ENOTSOCK:
		vstr::errp() << "ENOTSOCK" << std::endl;
		break;
	case EINTR:
		vstr::errp() << "EINTR" << std::endl;
		break;
	case EFAULT:
		vstr::errp() << "EFAULT" << std::endl;
		break;
	}
}