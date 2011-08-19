//*************************************************
//						SENDER
//*************************************************
#include <zmq.hpp>

#include <VmsStringMsg.h>
#include <VmsMsgReceiver.h>
#include <VmsMsgSender.h>
#include <VmsEndpointFactory.h>

#include <VistaInterProcComm/Concurrency/VistaThreadLoop.h>

#include <VistaBase/VistaTimeUtils.h>

#include <stdio.h>
/*
  This demo illustrates the use of VMS for a simple sender
  receiver pair.
  It features two basic modes of operations:
  1) Use it in standalone mode. Data will be transmitted between
     the main application thread and a special receiver thread.
	 Params: inproc://servicesocket inproc://publishersocket
  2) Use it with a dedicated receiver (incorporated in this test
     case. Data will be sent across process boundaries via a TCP
	 connection. 
	 Params: tcp://yourIP:port tcp://yourIP:port+1
	 e.g. tcp://134.130.70.92:14143 tcp://134.130.70.92:14145
 
 The first parameter in the above mentioned lists gives refers to
 the socket where the receiver will be listening for incoming 
 connection requests. The second parameter determines the actual
 address, where the sender will send out its messages.
*/

  
/**
 * receiver thread for intra process communication
 */
class ReceiverThread : public VistaThread
{
public:
	ReceiverThread(zmq::context_t *pContext,
				   const std::string &strServiceURL)
		:	m_pContext(pContext),
			m_strServiceURL(strServiceURL)
	{
	}

	virtual ~ReceiverThread()
	{
	}

	virtual void ThreadBody()
	{
		VmsEndpointFactory oFactory = VmsEndpointFactory(m_pContext);
		VmsMsgReceiver *pReceiver = oFactory.CreateReceiver(m_strServiceURL);
		assert(pReceiver != NULL);

		int iNumMsgs = 0;
		bool done = false;
		//all right, we are good to go
		//receive messages  until we get a terminate token
		while(!done)
		{
			//so here we actually receive a message using our connection
			//endpoint. Its as simple as this: just pull it out. The
			//call will block until a message is received.
			VmsMsg *pIncoming = pReceiver->ReceiveMsg();
			assert(pIncoming != NULL);

			//print message info
			++iNumMsgs;
			printf("[Receiver] Received message #%d\n", iNumMsgs);
			//All the rest here is just "eye candy" i.e. we try to interpret
			//the message.
			//Try to convert to string message and output content
			VmsStringMsg *pStringMsg = dynamic_cast<VmsStringMsg*>(pIncoming);
			if(pStringMsg != NULL)
			{
				std::string strContent = pStringMsg->GetString();
				done = (strContent == "final");
				if(!done)
				{
					printf("[Receiver] Got string message!\n");
					printf("\tcontent = %s\n", strContent.c_str());
				}
				else
				{
					printf("[Receiver] Got terminate token!\n");
				}
			}
			else
			{
				printf("[Receiver] Got some other generic message!\n");
			}
			printf("\n\n");
			//remember to clear the message!
			delete pIncoming;
		}

		delete pReceiver;
	}

protected:

private:
	zmq::context_t *m_pContext;
	std::string m_strServiceURL;
};

/**
 * some more or less stupid message strings
 */
const int NUM_STRINGS = 14;
const char *MSG_STRINGS[] ={
	"Hello ",						// 0
	"World! ",						// 1
	"This ",						// 2
	"is ",							// 3
	"my ",							// 4
	"collection ",					// 5
	"of ",							// 6
	"most ",						// 7
	"useless ",						// 8
	"testing strings!\n",			// 9
	"Want ",						//10
	"another ",						//11
	"round?\n",						//12
	"Sure...\n\n"					//13
};

/**
 * general main routine for (inter-process or intra-process) sender
 */
int main(int argc, char *argv[])
{
	if(argc<3)
	{
		//NOTE:
		//1st argument is the URL at which to request new connections
		//2nd argument is the URL at which the sender will create the
		//			   publisher socket
		printf("Usage: sender <service url> <publisher url>\n");
		return -1;
	}

	//PARSE INPUT ARGUMENTS
	//determine if we are talking a remote or local setup
	std::string strServiceURL = argv[1];
	bool bIsLocalMode=false;
	printf("[Sender] service socket @ %s\n", strServiceURL.c_str());
	if(strServiceURL.find("inproc",0,6)==0)
	{
		//we are intra-process (i.e. communicating between threads
		//==> create a thread
		printf("[Sender] Entering intra-process mode.\n");
		bIsLocalMode = true;

	}
	else if(strServiceURL.find("tcp",0,3)==0)
	{
		printf("[Sender] Entering inter-process mode.\n");
		bIsLocalMode = false;
	}
	else
	{
		printf("*** ERROR *** Socket type currently not supported!\n");
		return NULL;
	}
	std::string strPublisherURL = argv[2];

	
	//register message types
	//NOTE: make sure that this is done consistently - i.e. in the same order -
	//      on client and server side.
	VmsStringMsg::Register();
	
	//create and init zmq context
	zmq::context_t oContext(2);
	
	ReceiverThread *pReceiver = NULL;
	if(bIsLocalMode)
	{
		//if we are in intra-process mode -> create the receiver
		//this should be done BEFORE the sender is created!
		//otherwise sender creation will block indefinitely!
		printf("[Sender] Starting receiver thread in local mode!\n");
		pReceiver = new ReceiverThread(&oContext, strServiceURL);
		pReceiver->Run();
		printf("\tDONE!\n");
		//wait a little here in order to allow the receiver imp to
		//get up and running before we try to connect to it!
		VistaTimeUtils::Sleep(2000);
	}

	//create sender endpoint using the input parameter as URLs
	VmsEndpointFactory oFactory = VmsEndpointFactory(&oContext);
	printf("[Sender] Creating sender endpoint!\n");
	VmsMsgSender *pSender = oFactory.CreateSender(strServiceURL, strPublisherURL);
	if(pSender != NULL)
	{
		printf("\tDONE!\n");
	}
	else
	{	
		printf("\tFAILED!\n");
		return -1;
	}

	//send test messages
	printf("[Sender] sending test messages\n\n");
	const int iNumMsg = 100;
	for(int i=0; i<iNumMsg; ++i)
	{
		std::string strMsg = MSG_STRINGS[i%NUM_STRINGS]; 
		if(!bIsLocalMode)
		{
			printf("[Sender] Sending message <%d>\n", i+1);
			printf("\t<%s>\n", strMsg.c_str());
		}
		//create a new string message for sending
		VmsStringMsg *pMsg = new VmsStringMsg(strMsg);
		//send it -> NOTE: the sender will transparently handle
		//message deletion if needed. In every case, regardless
		//of whether the message is transmitted via inter- or
		//intra-process communication, we must not touch it 
		//hereafter!
		pSender->SendMsg(pMsg);
	}
	printf("[Sender] DONE!\n\n");
 
	//cleanup and shutdown
	printf("[Sender] Shutting down\n");
	//send one final message in order to allow the client
	//to terminate correctly
	VmsStringMsg *pFinalMsg = new VmsStringMsg("final");
	pSender->SendMsg(pFinalMsg);

	if(bIsLocalMode)
	{
		//finally stop the thread
		pReceiver->Join();
		delete pReceiver;
	}

	delete pSender;
	printf("\nDONE!\n");
}