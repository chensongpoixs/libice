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

#include "fake_port_allocator/cfake_port_allocator_session.h"


namespace libice
{
	FakePortAllocatorSession::FakePortAllocatorSession(cricket::PortAllocator* allocator,
		rtc::Thread* network_thread,
		rtc::PacketSocketFactory* factory,
		const std::string& content_name,
		int component,
		const std::string& ice_ufrag,
		const std::string& ice_pwd)
		: PortAllocatorSession(content_name,
			component,
			ice_ufrag,
			ice_pwd,
			allocator->flags()),
		network_thread_(network_thread),
		factory_(factory),
		ipv4_network_("network",
			"unittest",
			rtc::IPAddress(INADDR_LOOPBACK),
			32),
		ipv6_network_("network",
			"unittest",
			rtc::IPAddress(in6addr_loopback),
			64),
		port_(),
		port_config_count_(0),
		stun_servers_(allocator->stun_servers()),
		turn_servers_(allocator->turn_servers()) {
		ipv4_network_.AddIP(rtc::IPAddress(INADDR_LOOPBACK));
		ipv6_network_.AddIP(rtc::IPAddress(in6addr_loopback));
	}

	void FakePortAllocatorSession::SetCandidateFilter(uint32_t filter)   {
		candidate_filter_ = filter;
	}

	void FakePortAllocatorSession::StartGettingPorts()   {
		if (!port_) {
			rtc::Network& network =
				(rtc::HasIPv6Enabled() && (flags() & cricket::PORTALLOCATOR_ENABLE_IPV6))
				? ipv6_network_
				: ipv4_network_;
			port_.reset(TestUDPPort::Create(network_thread_, factory_, &network, 0, 0,
				username(), password(), std::string(),
				false));
			RTC_DCHECK(port_);
			port_->SubscribePortDestroyed(
				[this](cricket::PortInterface* port) { OnPortDestroyed(port); });
			AddPort(port_.get());
		}
		++port_config_count_;
		running_ = true;
	}

	void FakePortAllocatorSession::StopGettingPorts()   { running_ = false; }
	bool FakePortAllocatorSession::IsGettingPorts()  { return running_; }
	void FakePortAllocatorSession::ClearGettingPorts()  { is_cleared = true; }
	bool FakePortAllocatorSession::IsCleared() const  { return is_cleared; }

	void FakePortAllocatorSession::RegatherOnFailedNetworks()  {
		SignalIceRegathering(this, cricket::IceRegatheringReason::NETWORK_FAILURE);
	}

	std::vector<cricket::PortInterface*> FakePortAllocatorSession::ReadyPorts() const  {
		return ready_ports_;
	}
	std::vector<cricket::Candidate> FakePortAllocatorSession::ReadyCandidates() const  {
		return candidates_;
	}
	void FakePortAllocatorSession::PruneAllPorts()  { port_->Prune(); }
	bool FakePortAllocatorSession::CandidatesAllocationDone() const  { return allocation_done_; }

	int FakePortAllocatorSession::port_config_count() { return port_config_count_; }

	const cricket::ServerAddresses& FakePortAllocatorSession::stun_servers() const { return stun_servers_; }

	const std::vector<cricket::RelayServerConfig>& FakePortAllocatorSession::turn_servers() const {
		return turn_servers_;
	}

	uint32_t FakePortAllocatorSession::candidate_filter() const { return candidate_filter_; }

	int FakePortAllocatorSession::transport_info_update_count() const {
		return transport_info_update_count_;
	}
 
	void FakePortAllocatorSession::UpdateIceParametersInternal()  {
		// Since this class is a fake and this method only is overridden for tests,
		// we don't need to actually update the transport info.
		++transport_info_update_count_;
	}
	 
	void FakePortAllocatorSession::AddPort(cricket::Port* port) {
		port->set_component(component());
		port->set_generation(generation());
		port->SignalPortComplete.connect(this,
			&FakePortAllocatorSession::OnPortComplete);
		port->PrepareAddress();
		ready_ports_.push_back(port);
		SignalPortReady(this, port);
		port->KeepAliveUntilPruned();
	}
	void FakePortAllocatorSession::OnPortComplete(cricket::Port* port) {
		const std::vector<cricket::Candidate>& candidates = port->Candidates();
		candidates_.insert(candidates_.end(), candidates.begin(), candidates.end());
		SignalCandidatesReady(this, candidates);

		allocation_done_ = true;
		SignalCandidatesAllocationDone(this);
	}
	void FakePortAllocatorSession::OnPortDestroyed(cricket::PortInterface* port) {
		// Don't want to double-delete port if it deletes itself.
		port_.release();
	}


}