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
#ifndef  __C_FAKE_PORT_ALLOCATOR_SESSION_H_
#define  __C_FAKE_PORT_ALLOCATOR_SESSION_H_




#include <map>
#include <string>

#include "p2p/base/packet_transport_internal.h"
#include "rtc_base/copy_on_write_buffer.h"
#include "fake_port_allocator/ctest_udp_port.h"
namespace libice {
	class FakePortAllocatorSession : public cricket::PortAllocatorSession {
	public:
		FakePortAllocatorSession(cricket::PortAllocator* allocator,
			rtc::Thread* network_thread,
			rtc::PacketSocketFactory* factory,
			const std::string& content_name,
			int component,
			const std::string& ice_ufrag,
			const std::string& ice_pwd);

		void SetCandidateFilter(uint32_t filter) override;

		void StartGettingPorts() override;

		void StopGettingPorts() override;
		bool IsGettingPorts() override;
		void ClearGettingPorts() override;
		bool IsCleared() const override;

		void RegatherOnFailedNetworks() override;

		std::vector<cricket::PortInterface*> ReadyPorts() const override;
		std::vector<cricket::Candidate> ReadyCandidates() const override;
		void PruneAllPorts() override;
		bool CandidatesAllocationDone() const override;

		int port_config_count();

		const cricket::ServerAddresses& stun_servers() const;

		const std::vector<cricket::RelayServerConfig>& turn_servers() const;

		uint32_t candidate_filter() const;

		int transport_info_update_count() const;

	protected:
		void UpdateIceParametersInternal() override;

	private:
		void AddPort(cricket::Port* port);
		void OnPortComplete(cricket::Port* port);
		void OnPortDestroyed(cricket::PortInterface* port);

		rtc::Thread* network_thread_;
		rtc::PacketSocketFactory* factory_;
		rtc::Network ipv4_network_;
		rtc::Network ipv6_network_;
		std::unique_ptr<cricket::Port> port_;
		int port_config_count_;
		std::vector<cricket::Candidate> candidates_;
		std::vector<cricket::PortInterface*> ready_ports_;
		bool allocation_done_ = false;
		bool is_cleared = false;
		cricket::ServerAddresses stun_servers_;
		std::vector<cricket::RelayServerConfig> turn_servers_;
		uint32_t candidate_filter_ = cricket::CF_ALL;
		int transport_info_update_count_ = 0;
		bool running_ = false;
	};

}


#endif // __C_FAKE_PORT_ALLOCATOR_SESSION_H_