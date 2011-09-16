
#include <iostream>
#include <zmq.hpp>
#include <VmsStringMsg.h>
#include <VmsMsg.h>
#include <VmsSerialMsgCodec.h>
#include <VmsMsgReceiver.h>
#include <VmsMsgSender.h>
#include <VmsEndpointFactory.h>
#include "gtest/gtest.h"
//For threading
#include <stdio.h>
#include <VistaInterProcComm/Concurrency/VistaThreadFunction.h>



zmq::context_t context(2);

/*
 * Example
 */
TEST(sample_test_case, sample_test) {	
     EXPECT_EQ(1, 1); 
}  

/*
 * Simple Test on VmsSerialMsgCodec. The string "TEST" is encoded and decoded using VmsStringMsg. It is checked
 * if the output is the same is the input and it is expected that the buffersize is greater than zero
 */
TEST(VmsSerialMsgCodec, encode_decode) {
		//encode message into suitable byte stream
		std::string message("TEST");
		VmsStringMsg::Register();
		VmsSerialMsgCodec * codec = new VmsSerialMsgCodec();
		VmsStringMsg * msg = new VmsStringMsg(message);
		VmsMsg *pMsg = NULL;
		ubyte8 *pBuffer = NULL;
		size_t iSize = 0;
		codec->Encode(msg, pBuffer, iSize);
		//Expect that the size is > 0
		EXPECT_LT(0, iSize); 
		codec->Decode(pBuffer, iSize, pMsg);
		VmsStringMsg *pStringMsg = dynamic_cast<VmsStringMsg*>(pMsg);
		////Test if message is the same
		std::string enc_string = pStringMsg->GetString();
		EXPECT_STREQ(message.c_str(), pStringMsg->GetString().c_str()); 
		delete codec;
		delete msg;
		delete pMsg;
}


void ReceiverThread(int *iArg)
  {
	VmsEndpointFactory oFactory = VmsEndpointFactory(&context);
	VmsMsgReceiver *pReceiver = oFactory.CreateReceiver("inproc://127.0.0.1:5000");
	VmsMsg *pIncoming = pReceiver->ReceiveMsg();
	assert(pIncoming != NULL);
	VmsStringMsg *pStringMsg = dynamic_cast<VmsStringMsg*>(pIncoming);
	EXPECT_STREQ("TESTMESSAGE", pStringMsg->GetString().c_str());
	delete pReceiver;
  }

void SenderThread(int *iArg)
 {
	VmsEndpointFactory oFactory = VmsEndpointFactory(&context);
	VmsMsgSender *pSender = oFactory.CreateSender("inproc://127.0.0.1:5000","inproc://127.0.0.1:5001");
	VmsStringMsg *pFinalMsg = new VmsStringMsg("TESTMESSAGE");
	pSender->SendMsg(pFinalMsg);
	delete pSender;
 }

TEST(VmsEndpointFactory, Test_internal_zeromq_communication){
	int iArg = 1;
	int aArg = 2;
	VmsStringMsg::Register();
	VistaThreadFunction<int> createReceiver(ReceiverThread,&iArg);
	VistaThreadFunction<int> createSender(SenderThread,&aArg);
	createReceiver.Run();
	sleep(1);
	createSender.Run();
	createSender.Join();
	createReceiver.Join();
}


int main(int argc, char** argv)  {
	  //Initialize the google testing framework
      testing::InitGoogleTest(&argc, argv);
      std::cout << "Starting all testcases "<< std::endl;
      RUN_ALL_TESTS();
      std::cout << "All testcases finished"<< std::endl;
}
