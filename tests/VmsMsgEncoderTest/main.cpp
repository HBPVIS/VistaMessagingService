
#include "gtest/gtest.h"

#include <VmsMsg.h>
#include <VmsStringMsg.h>
#include <VmsSerialMsgCodec.h>
#include <VmsMsgReceiver.h>
#include <VmsMsgSender.h>
#include <VmsEndpointFactory.h>

#include <VistaInterProcComm/Concurrency/VistaThreadFunction.h>
#include <VistaAspects/VistaMarshalledObjectFactory.h>
#include <VistaBase/VistaBaseTypes.h>
#include <VistaBase/VistaTimeUtils.h>

#include <iostream>
#include <stdio.h>

#include <zmq.hpp>




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
TEST(VmsSerialMsgCodec, encode_decode) 
{
		//encode message into suitable byte stream
		std::string strTestMsg("TEST");

		VmsSerialMsgCodec * pCodec = new VmsSerialMsgCodec();
		
		VmsStringMsg * pInMsg = new VmsStringMsg(strTestMsg);
		
		VistaType::byte *pBuffer = NULL;
		size_t iSize = 0;
		bool bret = pCodec->Encode(pInMsg, pBuffer, iSize);
		//Expect that encode returned true
		EXPECT_TRUE(bret, 0);
		//Expect that the size is > 0
		EXPECT_GT(iSize, 0); 
		
		//start decoding again
		VmsMsg *pOutMsg = NULL;
		bret = pCodec->Decode(pBuffer, iSize, pOutMsg);
		//expect that decode returned true
		EXPECT_TRUE(bret);

		//Test if message is the same
		VmsStringMsg *pStringMsg = dynamic_cast<VmsStringMsg*>(pOutMsg);
		EXPECT_STREQ(pInMsg->GetString().c_str(), pStringMsg->GetString().c_str()); 
		
		delete pCodec;
		
		delete pInMsg;
		delete pOutMsg;
}


void ReceiverThread(int *iArg)
  {
	VmsEndpointFactory oFactory = VmsEndpointFactory(&context);
	VmsMsgReceiver *pReceiver = oFactory.CreateReceiver("inproc://servicesocket");
	VmsMsg *pIncoming = pReceiver->ReceiveMsg();
	assert(pIncoming != NULL);
	VmsStringMsg *pStringMsg = dynamic_cast<VmsStringMsg*>(pIncoming);
	EXPECT_STREQ("TESTMESSAGE", pStringMsg->GetString().c_str());
	delete pReceiver;
  }

void SenderThread(int *iArg)
 {
	VmsEndpointFactory oFactory = VmsEndpointFactory(&context);
	VmsMsgSender *pSender = oFactory.CreateSender("inproc://servicesocket","inproc://publishersocket");
	VmsStringMsg *pFinalMsg = new VmsStringMsg("TESTMESSAGE");
	pSender->SendMsg(pFinalMsg);
	delete pSender;
 }

TEST(VmsEndpointFactory, Test_internal_zeromq_communication)
{
	int iArg = 1;
	int aArg = 2;
	
	VistaThreadFunction<int> createReceiver(ReceiverThread,&iArg);
	createReceiver.Run();

	VistaTimeUtils::Sleep(10);
	
	VistaThreadFunction<int> createSender(SenderThread,&aArg);
	createSender.Run();
	createSender.Join();
	
	createReceiver.Join();
}


int main(int argc, char** argv)  {
	  //Initialize the google testing framework
      testing::InitGoogleTest(&argc, argv);

	  //register message types for the entire run
	  //This uses the VistaMarshalledObjectFactory for a general approach to marshall the 
	  //messages. This approach works with ALL serializables, however, the registration
	  //gets a little cumbersome. 
	  VistaMarshalledObjectFactory *pFac = VistaMarshalledObjectFactory::GetSingleton();
	  VmsStringMsg oMsg;
	  pFac->RegisterType(&oMsg, new VistaVanillaCreator<IVistaSerializable,VmsStringMsg>());


      std::cout << "Starting all testcases "<< std::endl;
      RUN_ALL_TESTS();
      std::cout << "All testcases finished"<< std::endl;
}
