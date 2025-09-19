
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


#include "webrtc_ice_test/cfake_ice_define.h"
#include "webrtc_ice_test/cfake_ice_transport.h"


namespace libice {

  FakeIceTransport::FakeIceTransport(const std::string& name,
	int component,
	rtc::Thread* network_thread )
	: name_(name),
	component_(component),
	network_thread_(network_thread ? network_thread
		: rtc::Thread::Current()) {}
// Must be called either on the network thread, or after the network thread
// has been shut down.
  FakeIceTransport::~FakeIceTransport()   {
	if (dest_ && dest_->dest_ == this) {
		dest_->dest_ = nullptr;
	}
}

// If async, will send packets by "Post"-ing to message queue instead of
// synchronously "Send"-ing.
void FakeIceTransport::SetAsync(bool async) {
	RTC_DCHECK_RUN_ON(network_thread_);
	async_ = async;
}
void FakeIceTransport::SetAsyncDelay(int delay_ms) {
	RTC_DCHECK_RUN_ON(network_thread_);
	async_delay_ms_ = delay_ms;
}

// SetWritable, SetReceiving and SetDestination are the main methods that can
// be used for testing, to simulate connectivity or lack thereof.
void FakeIceTransport::SetWritable(bool writable) {
	RTC_DCHECK_RUN_ON(network_thread_);
	set_writable(writable);
}
void FakeIceTransport::SetReceiving(bool receiving) {
	RTC_DCHECK_RUN_ON(network_thread_);
	set_receiving(receiving);
}

// Simulates the two transports connecting to each other.
// If `asymmetric` is true this method only affects this FakeIceTransport.
// If false, it affects `dest` as well.
void FakeIceTransport::SetDestination(FakeIceTransport* dest, bool asymmetric ) {
	RTC_DCHECK_RUN_ON(network_thread_);
	if (dest == dest_) {
		return;
	}
	RTC_DCHECK(!dest || !dest_)
		<< "Changing fake destination from one to another is not supported.";
	if (dest) {
		// This simulates the delivery of candidates.
		dest_ = dest;
		set_writable(true);
		if (!asymmetric) {
			dest->SetDestination(this, true);
		}
	}
	else {
		// Simulates loss of connectivity, by asymmetrically forgetting dest_.
		dest_ = nullptr;
		set_writable(false);
	}
}

void FakeIceTransport::SetTransportState(webrtc::IceTransportState state,
	cricket::IceTransportState legacy_state) {
	RTC_DCHECK_RUN_ON(network_thread_);
	transport_state_ = state;
	legacy_transport_state_ = legacy_state;
	SignalIceTransportStateChanged(this);
}

void FakeIceTransport::SetConnectionCount(size_t connection_count) {
	RTC_DCHECK_RUN_ON(network_thread_);
	size_t old_connection_count = connection_count_;
	connection_count_ = connection_count;
	if (connection_count) {
		had_connection_ = true;
	}
	// In this fake transport channel, `connection_count_` determines the
	// transport state.
	if (connection_count_ < old_connection_count) {
		SignalStateChanged(this);
	}
}

void FakeIceTransport::SetCandidatesGatheringComplete() {
	RTC_DCHECK_RUN_ON(network_thread_);
	if (gathering_state_ != cricket::kIceGatheringComplete) {
		gathering_state_ = cricket::kIceGatheringComplete;
		SignalGatheringState(this);
	}
}

// Convenience functions for accessing ICE config and other things.
int FakeIceTransport::receiving_timeout() const {
	RTC_DCHECK_RUN_ON(network_thread_);
	return ice_config_.receiving_timeout_or_default();
}
bool FakeIceTransport::gather_continually() const {
	RTC_DCHECK_RUN_ON(network_thread_);
	return ice_config_.gather_continually();
}
const cricket::Candidates& FakeIceTransport::remote_candidates() const {
	RTC_DCHECK_RUN_ON(network_thread_);
	return remote_candidates_;
}

// Fake IceTransportInternal implementation.
const std::string& FakeIceTransport::transport_name() const   { return name_; }
int FakeIceTransport::component() const   { return component_; }
uint64_t FakeIceTransport::IceTiebreaker() const {
	RTC_DCHECK_RUN_ON(network_thread_);
	return tiebreaker_;
}
cricket::IceMode FakeIceTransport::remote_ice_mode() const {
	RTC_DCHECK_RUN_ON(network_thread_);
	return remote_ice_mode_;
}
const std::string& FakeIceTransport::ice_ufrag() const { return ice_parameters_.ufrag; }
const std::string& FakeIceTransport::ice_pwd() const { return ice_parameters_.pwd; }
const std::string& FakeIceTransport::remote_ice_ufrag() const {
	return remote_ice_parameters_.ufrag;
}
const std::string& FakeIceTransport::remote_ice_pwd() const {
	return remote_ice_parameters_.pwd;
}
const cricket::IceParameters& FakeIceTransport::ice_parameters() const { return ice_parameters_; }
const cricket::IceParameters& FakeIceTransport::remote_ice_parameters() const {
	return remote_ice_parameters_;
}

cricket::IceTransportState FakeIceTransport::GetState() const   {
	RTC_DCHECK_RUN_ON(network_thread_);
	if (legacy_transport_state_) {
		return *legacy_transport_state_;
	}

	if (connection_count_ == 0) {
		return had_connection_ ? cricket::IceTransportState::STATE_FAILED
			: cricket::IceTransportState::STATE_INIT;
	}

	if (connection_count_ == 1) {
		return cricket::IceTransportState::STATE_COMPLETED;
	}

	return cricket::IceTransportState::STATE_CONNECTING;
}

webrtc::IceTransportState FakeIceTransport::GetIceTransportState() const   {
	RTC_DCHECK_RUN_ON(network_thread_);
	if (transport_state_) {
		return *transport_state_;
	}

	if (connection_count_ == 0) {
		return had_connection_ ? webrtc::IceTransportState::kFailed
			: webrtc::IceTransportState::kNew;
	}

	if (connection_count_ == 1) {
		return webrtc::IceTransportState::kCompleted;
	}

	return webrtc::IceTransportState::kConnected;
}

void FakeIceTransport::SetIceRole(cricket::IceRole role)   {
	RTC_DCHECK_RUN_ON(network_thread_);
	role_ = role;
}
cricket::IceRole FakeIceTransport::GetIceRole() const   {
	RTC_DCHECK_RUN_ON(network_thread_);
	return role_;
}
void FakeIceTransport::SetIceTiebreaker(uint64_t tiebreaker)   {
	RTC_DCHECK_RUN_ON(network_thread_);
	tiebreaker_ = tiebreaker;
}
void FakeIceTransport::SetIceParameters(const cricket::IceParameters& ice_params)   {
	RTC_DCHECK_RUN_ON(network_thread_);
	ice_parameters_ = ice_params;
}
void FakeIceTransport::SetRemoteIceParameters(const cricket::IceParameters& params)   {
	RTC_DCHECK_RUN_ON(network_thread_);
	remote_ice_parameters_ = params;
}

void FakeIceTransport::SetRemoteIceMode(cricket::IceMode mode)   {
	RTC_DCHECK_RUN_ON(network_thread_);
	remote_ice_mode_ = mode;
}

void FakeIceTransport::MaybeStartGathering()   {
	RTC_DCHECK_RUN_ON(network_thread_);
	if (gathering_state_ == cricket::kIceGatheringNew) {
		gathering_state_ = cricket::kIceGatheringGathering;
		SignalGatheringState(this);
	}
}

cricket::IceGatheringState FakeIceTransport::gathering_state() const   {
	RTC_DCHECK_RUN_ON(network_thread_);
	return gathering_state_;
}

void FakeIceTransport::SetIceConfig(const cricket::IceConfig& config)   {
	RTC_DCHECK_RUN_ON(network_thread_);
	ice_config_ = config;
}

void FakeIceTransport::AddRemoteCandidate(const cricket::Candidate& candidate)   {
	RTC_DCHECK_RUN_ON(network_thread_);
	remote_candidates_.push_back(candidate);
}
void FakeIceTransport::RemoveRemoteCandidate(const cricket::Candidate& candidate)   {
	RTC_DCHECK_RUN_ON(network_thread_);
	auto it = absl::c_find(remote_candidates_, candidate);
	if (it == remote_candidates_.end()) {
		RTC_LOG(LS_INFO) << "Trying to remove a candidate which doesn't exist.";
		return;
	}

	remote_candidates_.erase(it);
}

void FakeIceTransport::RemoveAllRemoteCandidates()   {
	RTC_DCHECK_RUN_ON(network_thread_);
	remote_candidates_.clear();
}

bool FakeIceTransport::GetStats(cricket::IceTransportStats* ice_transport_stats)   {
	cricket::CandidateStats candidate_stats;
	cricket::ConnectionInfo candidate_pair_stats;
	ice_transport_stats->candidate_stats_list.clear();
	ice_transport_stats->candidate_stats_list.push_back(candidate_stats);
	ice_transport_stats->connection_infos.clear();
	ice_transport_stats->connection_infos.push_back(candidate_pair_stats);
	return true;
}

absl::optional<int> FakeIceTransport::GetRttEstimate()   { return absl::nullopt; }

const cricket::Connection* FakeIceTransport::selected_connection() const   { return nullptr; }
absl::optional<const cricket::CandidatePair> FakeIceTransport::GetSelectedCandidatePair()
const   {
	return absl::nullopt;
}

// Fake PacketTransportInternal implementation.
bool FakeIceTransport::writable() const   {
	RTC_DCHECK_RUN_ON(network_thread_);
	return writable_;
}
bool FakeIceTransport::receiving() const   {
	RTC_DCHECK_RUN_ON(network_thread_);
	return receiving_;
}
// If combine is enabled, every two consecutive packets to be sent with
// "SendPacket" will be combined into one outgoing packet.
void FakeIceTransport::combine_outgoing_packets(bool combine) {
	RTC_DCHECK_RUN_ON(network_thread_);
	combine_outgoing_packets_ = combine;
}
int FakeIceTransport::SendPacket(const char* data,
	size_t len,
	const rtc::PacketOptions& options,
	int flags)   {
	RTC_DCHECK_RUN_ON(network_thread_);
	if (!dest_) {
		return -1;
	}

	send_packet_.AppendData(data, len);
	if (!combine_outgoing_packets_ || send_packet_.size() > len) {
		rtc::CopyOnWriteBuffer packet(std::move(send_packet_));
		if (async_) {
			network_thread_->PostDelayedTask(
				webrtc::ToQueuedTask(task_safety_.flag(),
					[this, packet] {
				RTC_DCHECK_RUN_ON(network_thread_);
				FakeIceTransport::SendPacketInternal(packet);
			}),
				async_delay_ms_);
		}
		else {
			SendPacketInternal(packet);
		}
	}
	rtc::SentPacket sent_packet(options.packet_id, rtc::TimeMillis());
	SignalSentPacket(this, sent_packet);
	return static_cast<int>(len);
}

int FakeIceTransport::SetOption(rtc::Socket::Option opt, int value)   {
	RTC_DCHECK_RUN_ON(network_thread_);
	socket_options_[opt] = value;
	return true;
}
bool FakeIceTransport::GetOption(rtc::Socket::Option opt, int* value)   {
	RTC_DCHECK_RUN_ON(network_thread_);
	auto it = socket_options_.find(opt);
	if (it != socket_options_.end()) {
		*value = it->second;
		return true;
	}
	else {
		return false;
	}
}

int FakeIceTransport::GetError()   { return 0; }

rtc::CopyOnWriteBuffer FakeIceTransport::last_sent_packet() {
	RTC_DCHECK_RUN_ON(network_thread_);
	return last_sent_packet_;
}

absl::optional<rtc::NetworkRoute> FakeIceTransport::network_route() const   {
	RTC_DCHECK_RUN_ON(network_thread_);
	return network_route_;
}
void FakeIceTransport::SetNetworkRoute(absl::optional<rtc::NetworkRoute> network_route) {
	RTC_DCHECK_RUN_ON(network_thread_);
	network_route_ = network_route;
	network_thread_->Invoke<void>(RTC_FROM_HERE, [this] {
		RTC_DCHECK_RUN_ON(network_thread_);
		SignalNetworkRouteChanged(network_route_);
	});
}
 
	void FakeIceTransport::set_writable(bool writable)
		  {
		if (writable_ == writable) {
			return;
		}
		RTC_LOG(INFO) << "Change writable_ to " << writable;
		writable_ = writable;
		if (writable_) {
			SignalReadyToSend(this);
		}
		SignalWritableState(this);
	}

	void FakeIceTransport::set_receiving(bool receiving)
		  {
		if (receiving_ == receiving) {
			return;
		}
		receiving_ = receiving;
		SignalReceivingState(this);
	}

	void FakeIceTransport::SendPacketInternal(const rtc::CopyOnWriteBuffer& packet)
		 {
		if (dest_) {
			last_sent_packet_ = packet;
			dest_->SignalReadPacket(dest_, packet.data<char>(), packet.size(),
				rtc::TimeMicros(), 0);
		}
	}


}