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
#ifndef  __C_FAKE_PORT_ALLOCATOR_H_
#define  __C_FAKE_PORT_ALLOCATOR_H_




#include <map>
#include <string>

#include "p2p/base/packet_transport_internal.h"
#include "rtc_base/copy_on_write_buffer.h"
#include "fake_port_allocator/ctest_udp_port.h"
#include "fake_port_allocator/cfake_port_allocator_session.h"
namespace libice {
	class FakePortAllocator : public cricket::PortAllocator {
	public:
		// TODO(bugs.webrtc.org/13145): Require non-null `factory`.
		FakePortAllocator(rtc::Thread* network_thread,
			rtc::PacketSocketFactory* factory)
			: network_thread_(network_thread), factory_(factory) {
			if (factory_ == NULL) {
				owned_factory_.reset(new rtc::BasicPacketSocketFactory(
					network_thread_ ? network_thread_->socketserver() : nullptr));
				factory_ = owned_factory_.get();
			}

			if (network_thread_ == nullptr) {
				network_thread_ = rtc::Thread::Current();
				Initialize();
				return;
			}
			network_thread_->Invoke<void>(RTC_FROM_HERE, [this] { Initialize(); });
		}

		void SetNetworkIgnoreMask(int network_ignore_mask) override {}

		cricket::PortAllocatorSession* CreateSessionInternal(
			const std::string& content_name,
			int component,
			const std::string& ice_ufrag,
			const std::string& ice_pwd) override {
			return new FakePortAllocatorSession(this, network_thread_, factory_,
				content_name, component, ice_ufrag,
				ice_pwd);
		}

		bool initialized() const { return initialized_; }

		// For testing: Manipulate MdnsObfuscationEnabled()
		bool MdnsObfuscationEnabled() const override {
			return mdns_obfuscation_enabled_;
		}
		void SetMdnsObfuscationEnabledForTesting(bool enabled) {
			mdns_obfuscation_enabled_ = enabled;
		}

	private:
		rtc::Thread* network_thread_;
		rtc::PacketSocketFactory* factory_;
		std::unique_ptr<rtc::BasicPacketSocketFactory> owned_factory_;
		bool mdns_obfuscation_enabled_ = false;
	};

}


#endif // __C_FAKE_PORT_ALLOCATOR_H_