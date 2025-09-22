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
				   date:  2025-09-19



 ******************************************************************************/
#ifndef  __C_FAKE_STUN_H_
#define  __C_FAKE_STUN_H_



#include <iostream>
#include <memory>
#include "absl/types/optional.h"
#include "api/ice_transport_factory.h"
#include "p2p/base/default_ice_transport_factory.h"
#include "p2p/base/basic_async_resolver_factory.h"
#include "p2p/base/basic_packet_socket_factory.h"
#include "p2p/client/basic_port_allocator.h"
#include "pc/session_description.h"
#include "p2p/base/dtls_transport_factory.h"
#include "p2p/base/dtls_transport.h"
#include "absl/algorithm/container.h"
#include "rtc_base/task_queue.h"
#include "absl/algorithm/algorithm.h"
#include "rtc_base/fake_ssl_identity.h"
#include "pc/jsep_transport_controller.h"

#include "fake_ice_transport/cfake_ice_define.h" 
#include "absl/memory/memory.h"
#include "rtc_base/byte_buffer.h"
#include "rtc_base/ip_address.h"
#include "rtc_base/logging.h"
#include "fake_stun/test_client.h"
#include "p2p/base/stun_server.h"
#include "rtc_base/thread.h"
#include "rtc_base/virtual_socket_server.h"
namespace libice {

	namespace {
		const rtc::SocketAddress server_addr("192.168.3.2", 3478);
		const rtc::SocketAddress client_addr("192.168.3.2", 1234);
	}  // namespace


	class StunServerTest  {
	public:
		StunServerTest() : ss_(new rtc::VirtualSocketServer()), network_(ss_.get()) {}
		virtual void SetUp() {
			server_.reset(
				new cricket::StunServer(rtc::AsyncUDPSocket::Create(ss_.get(), server_addr)));
			client_.reset(new   TestClient(
				absl::WrapUnique(rtc::AsyncUDPSocket::Create(ss_.get(), client_addr))));

			network_.Start();
		}
		void Send(const cricket::StunMessage& msg) {
			rtc::ByteBufferWriter buf;
			msg.Write(&buf);
			Send(buf.Data(), static_cast<int>(buf.Length()));
		}
		void Send(const char* buf, int len) {
			client_->SendTo(buf, len, server_addr);
		}
		bool ReceiveFails() { return (client_->CheckNoPacket()); }
		cricket::StunMessage* Receive() {
			cricket::StunMessage* msg = NULL;
			std::unique_ptr< TestClient::Packet> packet =
				client_->NextPacket(TestClient::kTimeoutMs);
			if (packet) {
				rtc::ByteBufferReader buf(packet->buf, packet->size);
				msg = new cricket::StunMessage();
				msg->Read(&buf);
			}
			return msg;
		}

	private:
		std::unique_ptr<rtc::VirtualSocketServer> ss_;
		rtc::Thread network_;
		std::unique_ptr<cricket::StunServer> server_;
		std::unique_ptr<TestClient> client_;
	};
	 
}
#endif // __C_FAKE_DTLS_TRANSPORT_FACTORY_H_