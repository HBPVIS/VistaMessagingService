//*************************************************
//						RECEIVER
//*************************************************
#include <zmq.hpp>

#include <VmsStringMsg.h>
#include <VmsMsgReceiver.h>
#include <VmsEndpointFactory.h>

#include <stdio.h>

/**
 * main routine for standalon inter-process receiver
 */
int main(int argc, char *argv[])
{
	if(argc<2)
	{
		printf("Usage: receiver <service url>\n");
		return -1;
	}

	//register message types
	//NOTE: make sure that this is done consistently - i.e. in the same order -
	//      on client and server side.
	VmsStringMsg::Register();
	
	//create a context for 1 thread since we are always running standalone
	//with no other threads attached
	zmq::context_t oContext(2);


	//determine if we are talking a remote or local setup
	std::string strServiceURL = argv[1];
	if(strServiceURL.find("tcp",0,3)!=0)
	{
		//we have no tcp socket -> bail out
		printf("*** ERROR *** Receiver works only for tcp communication right now!\n");
		return -1;
	}

	//create the receiving end of the connection
	VmsEndpointFactory oFactory = VmsEndpointFactory(&oContext);
	VmsMsgReceiver *pReceiver = oFactory.CreateReceiver(strServiceURL);
	assert(pReceiver != NULL);

	int iNumMsgs = 0;
	bool done = false;
	//all right, we are good to go
	//receive messages in endless loop
	while(!done)
	{
		VmsMsg *pIncoming = pReceiver->ReceiveMsg();
		assert(pIncoming != NULL);

		//print message info
		++iNumMsgs;
		printf("[Receiver] Received message #%d\n", iNumMsgs);
		//try to convert to string message and output content
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

	return 0;
}

#if 0
int main()
{
	zmq::context_t oContext(1);

	//setup the request replier in order to receive connection
	//requests
	printf("[Receiver] setting up replier socket\n");
	zmq::socket_t oReplier(oContext, ZMQ_REP);
	oReplier.bind("tcp://134.130.70.92:14143");
	printf("\tDONE!\n");

	//receive "connect to publisher" request
	printf("[Receiver] waiting for incoming request\n");
	zmq::message_t req;
	oReplier.recv(&req);
	size_t req_size = req.size();
	char* req_buff = new char[req_size+1];
	req_buff[req_size] = '\0';
	memcpy(req_buff, req.data(), req_size);
	std::string strPubAdr(req_buff);
	delete[] req_buff;
	printf("\tDONE!\n");
	printf("[Receiver] publisher @ %s\n", strPubAdr.c_str());

	//setup subscriber and subscribe to everything
	zmq::socket_t oSubscriber(oContext, ZMQ_SUB);
	oSubscriber.connect(strPubAdr.c_str());
	oSubscriber.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);


	//blocking wait for dummy incoming 
	printf("[Receiver] waiting for first message from publisher\n");
	zmq::message_t dummy;
	oSubscriber.recv(&dummy);
	printf("\tDONE!\n");

	//send ack in order to stop publisher from talking rubbish
	zmq::message_t ack(1);
	oReplier.send(ack);

	//do another go around as a barrier type of operation
	zmq::message_t barrier;
	oReplier.recv(&barrier);

	//read any left-over rubbish from the subscriber socket
	zmq::pollitem_t oPollSock;
	oPollSock.socket = (void*)oSubscriber;
	oPollSock.events = ZMQ_POLLIN;
	
	//give it 500 ms to time out here -> this make sure there's nothing left
	while(zmq_poll(&oPollSock, 1, 500) > 0)
	{
		zmq::message_t dump_it;
		oSubscriber.recv(&dump_it);
	}

	//finally signal that we are good to go
	zmq::message_t goahead(1);
	oReplier.send(goahead);


	//now the two sockets are connected -> therefore get 
	//going with the message stuff
	printf("[Receiver] receiving actual messages\n");
	while(1)
	{
		zmq::message_t msg;
		oSubscriber.recv(&msg);

		size_t size = msg.size();
		char *pMsg = new char[size+1];
		memcpy(pMsg, msg.data(), msg.size());
		pMsg[size] = '\0';

		printf("message\t> %s\n", pMsg);
	}

}
#endif