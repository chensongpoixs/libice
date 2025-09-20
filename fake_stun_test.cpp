/******************************************************************************
 *  Copyright (c) 2025 The CRTC project authors . All Rights Reserved.
 *
 *  Please visit https://chensongpoixs.github.io for detail
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 ******************************************************************************/
 /*****************************************************************************
				   Author: chensong
				   date:  2025-09-17



 ******************************************************************************/


#include  "fake_stun/cfake_stun.h"


#include <iostream>

using namespace libice;

StunServerTest   stun_server_test;


void  TestGood()
{
	cricket::StunMessage req;
	// kStunLegacyTransactionIdLength = 16 for legacy RFC 3489 request
	std::string transaction_id = "0123456789abcdef";
	req.SetType(cricket::STUN_BINDING_REQUEST);
	req.SetTransactionID(transaction_id);
	stun_server_test.Send(req);

	cricket::StunMessage* msg = stun_server_test.Receive();
	//ASSERT_TRUE(msg != NULL);
	//EXPECT_EQ(STUN_BINDING_RESPONSE, msg->type());
	//EXPECT_EQ(req.transaction_id(), msg->transaction_id());

	const cricket::StunAddressAttribute* mapped_addr =
		msg->GetAddress(cricket::STUN_ATTR_MAPPED_ADDRESS);
	//EXPECT_TRUE(mapped_addr != NULL);
	//EXPECT_EQ(1, mapped_addr->family());
	//EXPECT_EQ(client_addr.port(), mapped_addr->port());

	delete msg;
}

void TestGoodXorMappedAddr()
{
	cricket::StunMessage req;
	// kStunTransactionIdLength = 12 for RFC 5389 request
	// StunMessage::Write will automatically insert magic cookie (0x2112A442)
	std::string transaction_id = "0123456789ab";
	req.SetType(cricket::STUN_BINDING_REQUEST);
	req.SetTransactionID(transaction_id);
	stun_server_test.Send(req);

	cricket::StunMessage* msg = stun_server_test.Receive();
	//ASSERT_TRUE(msg != NULL);
	//EXPECT_EQ(STUN_BINDING_RESPONSE, msg->type());
	//EXPECT_EQ(req.transaction_id(), msg->transaction_id());

	const cricket::StunAddressAttribute* mapped_addr =
		msg->GetAddress(cricket::STUN_ATTR_XOR_MAPPED_ADDRESS);
	//EXPECT_TRUE(mapped_addr != NULL);
	//EXPECT_EQ(1, mapped_addr->family());
	//EXPECT_EQ(client_addr.port(), mapped_addr->port());

	delete msg;
}

void TestNoXorMappedAddr()
{
	cricket::StunMessage req;
	// kStunLegacyTransactionIdLength = 16 for legacy RFC 3489 request
	std::string transaction_id = "0123456789abcdef";
	req.SetType(cricket::STUN_BINDING_REQUEST);
	req.SetTransactionID(transaction_id);
	stun_server_test.Send(req);

	cricket::StunMessage* msg = stun_server_test.Receive();
	//ASSERT_TRUE(msg != NULL);
	//EXPECT_EQ(STUN_BINDING_RESPONSE, msg->type());
	//EXPECT_EQ(req.transaction_id(), msg->transaction_id());

	const cricket::StunAddressAttribute* mapped_addr =
		msg->GetAddress(cricket::STUN_ATTR_XOR_MAPPED_ADDRESS);
	//EXPECT_TRUE(mapped_addr == NULL);

	delete msg;
}


int main(int argc, char *argv[])
{
	TestGood();
	TestGoodXorMappedAddr();
	TestNoXorMappedAddr();

	return EXIT_SUCCESS;
}