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
#ifndef  __C_TEST_UDP_PORT_H_
#define  __C_TEST_UDP_PORT_H_




#include <map>
#include <string>
#include <memory>
#include <string>
#include <vector>

#include "p2p/base/basic_packet_socket_factory.h"
#include "p2p/base/port_allocator.h"
#include "p2p/base/udp_port.h"
#include "rtc_base/net_helpers.h"
#include "rtc_base/thread.h"
namespace libice {
	class TestUDPPort : public cricket::UDPPort {
	public:
		static TestUDPPort* Create(rtc::Thread* thread,
			rtc::PacketSocketFactory* factory,
			rtc::Network* network,
			uint16_t min_port,
			uint16_t max_port,
			const std::string& username,
			const std::string& password,
			const std::string& origin,
			bool emit_localhost_for_anyaddress) {
			TestUDPPort* port =
				new TestUDPPort(thread, factory, network, min_port, max_port, username,
					password, origin, emit_localhost_for_anyaddress);
			if (!port->Init()) {
				delete port;
				port = nullptr;
			}
			return port;
		}

	protected:
		TestUDPPort(rtc::Thread* thread,
			rtc::PacketSocketFactory* factory,
			rtc::Network* network,
			uint16_t min_port,
			uint16_t max_port,
			const std::string& username,
			const std::string& password,
			const std::string& origin,
			bool emit_localhost_for_anyaddress)
			: UDPPort(thread,
				factory,
				network,
				min_port,
				max_port,
				username,
				password,
				origin,
				emit_localhost_for_anyaddress) {}
	};

}


#endif // __C_FAKE_PORT_ALLOCATOR_SESSION_H_