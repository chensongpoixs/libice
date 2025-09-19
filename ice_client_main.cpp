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
//std::unique_ptr<webrtc::IceTransportFactory> ice_transport_factory;
//std::unique_ptr<webrtc::AsyncResolverFactory> async_resolver_factory;
//std::unique_ptr<cricket::PortAllocator> allocator;
//std::unique_ptr<webrtc::AsyncDnsResolverFactoryInterface> async_dns_resolver_factory;
//
//
//
/////////////////////////////////
//
//
//std::unique_ptr<rtc::BasicNetworkManager> default_network_manager_;
//std::unique_ptr<rtc::NetworkMonitorFactory> network_monitor_factory;
//std::unique_ptr<rtc::BasicPacketSocketFactory> default_socket_factory_;
//
//
//
//rtc::Thread * network_thread;
//rtc::Thread* network_thread_;
//
//cricket::ContentInfo content_info;
//
//cricket::IceConfig ice_config_;
//cricket::IceRole ice_role_ = cricket::ICEROLE_CONTROLLING;
//uint64_t ice_tiebreaker_ = rtc::CreateRandomId64();
//rtc::scoped_refptr<rtc::RTCCertificate> certificate_;
//
//std::unique_ptr<cricket::DtlsTransportInternal> CreateDtlsTransport(
//	const cricket::ContentInfo& content_info,
//	cricket::IceTransportInternal* ice) {
//	RTC_DCHECK_RUN_ON(network_thread_);
//
//	std::unique_ptr<cricket::DtlsTransportInternal> dtls;
//
//	/*if (config_.dtls_transport_factory) {
//		dtls = config_.dtls_transport_factory->CreateDtlsTransport(
//			ice, config_.crypto_options, config_.ssl_max_version);
//	}
//	else */
//	{
//		dtls = std::make_unique<cricket::DtlsTransport>(ice, config_.crypto_options,
//			config_.event_log,
//			config_.ssl_max_version);
//	}
//
//	RTC_DCHECK(dtls);
//	dtls->ice_transport()->SetIceRole(ice_role_);
//	dtls->ice_transport()->SetIceTiebreaker(ice_tiebreaker_);
//	dtls->ice_transport()->SetIceConfig(ice_config_);
//	if (certificate_)
//	{
//		bool set_cert_success = dtls->SetLocalCertificate(certificate_);
//		RTC_DCHECK(set_cert_success);
//	}
//
//	// Connect to signals offered by the DTLS and ICE transport.
//	dtls->SignalWritableState.connect(
//		this, &JsepTransportController::OnTransportWritableState_n);
//	dtls->SignalReceivingState.connect(
//		this, &JsepTransportController::OnTransportReceivingState_n);
//	dtls->ice_transport()->SignalGatheringState.connect(
//		this, &JsepTransportController::OnTransportGatheringState_n);
//	dtls->ice_transport()->SignalCandidateGathered.connect(
//		this, &JsepTransportController::OnTransportCandidateGathered_n);
//	dtls->ice_transport()->SignalCandidateError.connect(
//		this, &JsepTransportController::OnTransportCandidateError_n);
//	dtls->ice_transport()->SignalCandidatesRemoved.connect(
//		this, &JsepTransportController::OnTransportCandidatesRemoved_n);
//	dtls->ice_transport()->SignalRoleConflict.connect(
//		this, &JsepTransportController::OnTransportRoleConflict_n);
//	dtls->ice_transport()->SignalStateChanged.connect(
//		this, &JsepTransportController::OnTransportStateChanged_n);
//	dtls->ice_transport()->SignalIceTransportStateChanged.connect(
//		this, &JsepTransportController::OnTransportStateChanged_n);
//	dtls->ice_transport()->SignalCandidatePairChanged.connect(
//		this, &JsepTransportController::OnTransportCandidatePairChanged_n);
//
//	dtls->SubscribeDtlsHandshakeError(
//		[this](rtc::SSLHandshakeError error) { OnDtlsHandshakeError(error); });
//	return dtls;
//}
//
//int main(int argc, char *argv[])
//{
//	content_info.name = "audio";
//	content_info.type = cricket::MediaProtocolType::kRtp;
//
//	ice_transport_factory = std::make_unique<webrtc::DefaultIceTransportFactory>();;
//
//	async_dns_resolver_factory = std::make_unique<webrtc::WrappingAsyncDnsResolverFactory>(
//		std::make_unique<webrtc::BasicAsyncResolverFactory>());
//
//
//
//	///////////////////////////////////////////////////////////////////////////////////////////
//	// If network_monitor_factory_ is non-null, it will be used to create a
//  // network monitor while on the network thread.
//	default_network_manager_ = std::make_unique<rtc::BasicNetworkManager>(
//		network_monitor_factory.get(), network_thread->socketserver());
//
//	default_socket_factory_ = std::make_unique<rtc::BasicPacketSocketFactory>(
//		network_thread->socketserver());
//
//
//
//	allocator = std::make_unique<cricket::BasicPortAllocator>(
//		default_network_manager_.get(), default_socket_factory_.get(),
//		nullptr, nullptr);
//
//
//	/////////////////////////////////////////////////////////////
//	webrtc::IceTransportInit init;
//	init.set_port_allocator(allocator.get());
//	init.set_async_dns_resolver_factory(async_dns_resolver_factory.get());
//	//init.set_event_log(config_.event_log);
//	rtc::scoped_refptr<webrtc::IceTransportInterface>  ice_controller =     ice_transport_factory->CreateIceTransport("audio", 0, std::move(init));
//
//
//	std::unique_ptr<cricket::DtlsTransportInternal> rtp_dtls_transport =
//		CreateDtlsTransport(content_info, ice_controller->internal());
//
//	return EXIT_SUCCESS;
//}


static const int kTimeout = 100;
static const char kIceUfrag1[] = "u0001";
static const char kIcePwd1[] = "TESTICEPWD00000000000001";
static const char kIceUfrag2[] = "u0002";
static const char kIcePwd2[] = "TESTICEPWD00000000000002";
static const char kIceUfrag3[] = "u0003";
static const char kIcePwd3[] = "TESTICEPWD00000000000003";
static const char kIceUfrag4[] = "u0004";
static const char kIcePwd4[] = "TESTICEPWD00000000000004";
static const char kAudioMid1[] = "audio1";
static const char kAudioMid2[] = "audio2";
static const char kVideoMid1[] = "video1";
static const char kVideoMid2[] = "video2";
static const char kDataMid1[] = "data1";
class FakeIceTransport : public cricket::IceTransportInternal {
public:
	explicit FakeIceTransport(const std::string& name,
		int component,
		rtc::Thread* network_thread = nullptr)
		: name_(name),
		component_(component),
		network_thread_(network_thread ? network_thread
			: rtc::Thread::Current()) {}
	// Must be called either on the network thread, or after the network thread
	// has been shut down.
	~FakeIceTransport() override {
		if (dest_ && dest_->dest_ == this) {
			dest_->dest_ = nullptr;
		}
	}

	// If async, will send packets by "Post"-ing to message queue instead of
	// synchronously "Send"-ing.
	void SetAsync(bool async) {
		RTC_DCHECK_RUN_ON(network_thread_);
		async_ = async;
	}
	void SetAsyncDelay(int delay_ms) {
		RTC_DCHECK_RUN_ON(network_thread_);
		async_delay_ms_ = delay_ms;
	}

	// SetWritable, SetReceiving and SetDestination are the main methods that can
	// be used for testing, to simulate connectivity or lack thereof.
	void SetWritable(bool writable) {
		RTC_DCHECK_RUN_ON(network_thread_);
		set_writable(writable);
	}
	void SetReceiving(bool receiving) {
		RTC_DCHECK_RUN_ON(network_thread_);
		set_receiving(receiving);
	}

	// Simulates the two transports connecting to each other.
	// If `asymmetric` is true this method only affects this FakeIceTransport.
	// If false, it affects `dest` as well.
	void SetDestination(FakeIceTransport* dest, bool asymmetric = false) {
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

	void SetTransportState(webrtc::IceTransportState state,
		cricket::IceTransportState legacy_state) {
		RTC_DCHECK_RUN_ON(network_thread_);
		transport_state_ = state;
		legacy_transport_state_ = legacy_state;
		SignalIceTransportStateChanged(this);
	}

	void SetConnectionCount(size_t connection_count) {
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

	void SetCandidatesGatheringComplete() {
		RTC_DCHECK_RUN_ON(network_thread_);
		if (gathering_state_ != cricket::kIceGatheringComplete) {
			gathering_state_ = cricket::kIceGatheringComplete;
			SignalGatheringState(this);
		}
	}

	// Convenience functions for accessing ICE config and other things.
	int receiving_timeout() const {
		RTC_DCHECK_RUN_ON(network_thread_);
		return ice_config_.receiving_timeout_or_default();
	}
	bool gather_continually() const {
		RTC_DCHECK_RUN_ON(network_thread_);
		return ice_config_.gather_continually();
	}
	const cricket::Candidates& remote_candidates() const {
		RTC_DCHECK_RUN_ON(network_thread_);
		return remote_candidates_;
	}

	// Fake IceTransportInternal implementation.
	const std::string& transport_name() const override { return name_; }
	int component() const override { return component_; }
	uint64_t IceTiebreaker() const {
		RTC_DCHECK_RUN_ON(network_thread_);
		return tiebreaker_;
	}
	cricket::IceMode remote_ice_mode() const {
		RTC_DCHECK_RUN_ON(network_thread_);
		return remote_ice_mode_;
	}
	const std::string& ice_ufrag() const { return ice_parameters_.ufrag; }
	const std::string& ice_pwd() const { return ice_parameters_.pwd; }
	const std::string& remote_ice_ufrag() const {
		return remote_ice_parameters_.ufrag;
	}
	const std::string& remote_ice_pwd() const {
		return remote_ice_parameters_.pwd;
	}
	const cricket::IceParameters& ice_parameters() const { return ice_parameters_; }
	const cricket::IceParameters& remote_ice_parameters() const {
		return remote_ice_parameters_;
	}

	cricket::IceTransportState GetState() const override {
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

	webrtc::IceTransportState GetIceTransportState() const override {
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

	void SetIceRole(cricket::IceRole role) override {
		RTC_DCHECK_RUN_ON(network_thread_);
		role_ = role;
	}
	cricket::IceRole GetIceRole() const override {
		RTC_DCHECK_RUN_ON(network_thread_);
		return role_;
	}
	void SetIceTiebreaker(uint64_t tiebreaker) override {
		RTC_DCHECK_RUN_ON(network_thread_);
		tiebreaker_ = tiebreaker;
	}
	void SetIceParameters(const cricket::IceParameters& ice_params) override {
		RTC_DCHECK_RUN_ON(network_thread_);
		ice_parameters_ = ice_params;
	}
	void SetRemoteIceParameters(const cricket::IceParameters& params) override {
		RTC_DCHECK_RUN_ON(network_thread_);
		remote_ice_parameters_ = params;
	}

	void SetRemoteIceMode(cricket::IceMode mode) override {
		RTC_DCHECK_RUN_ON(network_thread_);
		remote_ice_mode_ = mode;
	}

	void MaybeStartGathering() override {
		RTC_DCHECK_RUN_ON(network_thread_);
		if (gathering_state_ == cricket::kIceGatheringNew) {
			gathering_state_ = cricket::kIceGatheringGathering;
			SignalGatheringState(this);
		}
	}

	cricket::IceGatheringState gathering_state() const override {
		RTC_DCHECK_RUN_ON(network_thread_);
		return gathering_state_;
	}

	void SetIceConfig(const cricket::IceConfig& config) override {
		RTC_DCHECK_RUN_ON(network_thread_);
		ice_config_ = config;
	}

	void AddRemoteCandidate(const cricket::Candidate& candidate) override {
		RTC_DCHECK_RUN_ON(network_thread_);
		remote_candidates_.push_back(candidate);
	}
	void RemoveRemoteCandidate(const cricket::Candidate& candidate) override {
		RTC_DCHECK_RUN_ON(network_thread_);
		auto it = absl::c_find(remote_candidates_, candidate);
		if (it == remote_candidates_.end()) {
			RTC_LOG(LS_INFO) << "Trying to remove a candidate which doesn't exist.";
			return;
		}

		remote_candidates_.erase(it);
	}

	void RemoveAllRemoteCandidates() override {
		RTC_DCHECK_RUN_ON(network_thread_);
		remote_candidates_.clear();
	}

	bool GetStats(cricket::IceTransportStats* ice_transport_stats) override {
		cricket::CandidateStats candidate_stats;
		cricket::ConnectionInfo candidate_pair_stats;
		ice_transport_stats->candidate_stats_list.clear();
		ice_transport_stats->candidate_stats_list.push_back(candidate_stats);
		ice_transport_stats->connection_infos.clear();
		ice_transport_stats->connection_infos.push_back(candidate_pair_stats);
		return true;
	}

	absl::optional<int> GetRttEstimate() override { return absl::nullopt; }

	const cricket::Connection* selected_connection() const override { return nullptr; }
	absl::optional<const cricket::CandidatePair> GetSelectedCandidatePair()
		const override {
		return absl::nullopt;
	}

	// Fake PacketTransportInternal implementation.
	bool writable() const override {
		RTC_DCHECK_RUN_ON(network_thread_);
		return writable_;
	}
	bool receiving() const override {
		RTC_DCHECK_RUN_ON(network_thread_);
		return receiving_;
	}
	// If combine is enabled, every two consecutive packets to be sent with
	// "SendPacket" will be combined into one outgoing packet.
	void combine_outgoing_packets(bool combine) {
		RTC_DCHECK_RUN_ON(network_thread_);
		combine_outgoing_packets_ = combine;
	}
	int SendPacket(const char* data,
		size_t len,
		const rtc::PacketOptions& options,
		int flags) override {
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

	int SetOption(rtc::Socket::Option opt, int value) override {
		RTC_DCHECK_RUN_ON(network_thread_);
		socket_options_[opt] = value;
		return true;
	}
	bool GetOption(rtc::Socket::Option opt, int* value) override {
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

	int GetError() override { return 0; }

	rtc::CopyOnWriteBuffer last_sent_packet() {
		RTC_DCHECK_RUN_ON(network_thread_);
		return last_sent_packet_;
	}

	absl::optional<rtc::NetworkRoute> network_route() const override {
		RTC_DCHECK_RUN_ON(network_thread_);
		return network_route_;
	}
	void SetNetworkRoute(absl::optional<rtc::NetworkRoute> network_route) {
		RTC_DCHECK_RUN_ON(network_thread_);
		network_route_ = network_route;
		network_thread_->Invoke<void>(RTC_FROM_HERE, [this] {
			RTC_DCHECK_RUN_ON(network_thread_);
			SignalNetworkRouteChanged(network_route_);
		});
	}

private:
	void set_writable(bool writable)
		RTC_EXCLUSIVE_LOCKS_REQUIRED(network_thread_) {
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

	void set_receiving(bool receiving)
		RTC_EXCLUSIVE_LOCKS_REQUIRED(network_thread_) {
		if (receiving_ == receiving) {
			return;
		}
		receiving_ = receiving;
		SignalReceivingState(this);
	}

	void SendPacketInternal(const rtc::CopyOnWriteBuffer& packet)
		RTC_EXCLUSIVE_LOCKS_REQUIRED(network_thread_) {
		if (dest_) {
			last_sent_packet_ = packet;
			dest_->SignalReadPacket(dest_, packet.data<char>(), packet.size(),
				rtc::TimeMicros(), 0);
		}
	}

	const std::string name_;
	const int component_;
	FakeIceTransport* dest_ RTC_GUARDED_BY(network_thread_) = nullptr;
	bool async_ RTC_GUARDED_BY(network_thread_) = false;
	int async_delay_ms_ RTC_GUARDED_BY(network_thread_) = 0;
	cricket::Candidates remote_candidates_ RTC_GUARDED_BY(network_thread_);
	cricket::IceConfig ice_config_ RTC_GUARDED_BY(network_thread_);
	cricket::IceRole role_ RTC_GUARDED_BY(network_thread_) = cricket::ICEROLE_UNKNOWN;
	uint64_t tiebreaker_ RTC_GUARDED_BY(network_thread_) = 0;
	cricket::IceParameters ice_parameters_ RTC_GUARDED_BY(network_thread_);
	cricket::IceParameters remote_ice_parameters_ RTC_GUARDED_BY(network_thread_);
	cricket::IceMode remote_ice_mode_ RTC_GUARDED_BY(network_thread_) = cricket::ICEMODE_FULL;
	size_t connection_count_ RTC_GUARDED_BY(network_thread_) = 0;
	absl::optional<webrtc::IceTransportState> transport_state_
		RTC_GUARDED_BY(network_thread_);
	absl::optional<cricket::IceTransportState> legacy_transport_state_
		RTC_GUARDED_BY(network_thread_);
	cricket::IceGatheringState gathering_state_ RTC_GUARDED_BY(network_thread_) =
		cricket::kIceGatheringNew;
	bool had_connection_ RTC_GUARDED_BY(network_thread_) = false;
	bool writable_ RTC_GUARDED_BY(network_thread_) = false;
	bool receiving_ RTC_GUARDED_BY(network_thread_) = false;
	bool combine_outgoing_packets_ RTC_GUARDED_BY(network_thread_) = false;
	rtc::CopyOnWriteBuffer send_packet_ RTC_GUARDED_BY(network_thread_);
	absl::optional<rtc::NetworkRoute> network_route_
		RTC_GUARDED_BY(network_thread_);
	std::map<rtc::Socket::Option, int> socket_options_
		RTC_GUARDED_BY(network_thread_);
	rtc::CopyOnWriteBuffer last_sent_packet_ RTC_GUARDED_BY(network_thread_);
	rtc::Thread* const network_thread_;
	webrtc::ScopedTaskSafetyDetached task_safety_;
};

class FakeIceTransportWrapper : public webrtc::IceTransportInterface {
public:
	explicit FakeIceTransportWrapper(
		std::unique_ptr<FakeIceTransport> internal)
		: internal_(std::move(internal)) {}

	cricket::IceTransportInternal* internal() override { return internal_.get(); }

private:
	std::unique_ptr<FakeIceTransport> internal_;
};
class FakeIceTransportFactory : public webrtc::IceTransportFactory {
public:
	~FakeIceTransportFactory() override = default;
	rtc::scoped_refptr<webrtc::IceTransportInterface> CreateIceTransport(
		const std::string& transport_name,
		int component,
		webrtc::IceTransportInit init) override {
		return rtc::make_ref_counted<FakeIceTransportWrapper>(
			std::make_unique<FakeIceTransport>(transport_name, component));
	}
};


class FakeDtlsTransport : public cricket::DtlsTransportInternal {
public:
	explicit FakeDtlsTransport(FakeIceTransport* ice_transport)
		: ice_transport_(ice_transport),
		transport_name_(ice_transport->transport_name()),
		component_(ice_transport->component()),
		dtls_fingerprint_("", nullptr) {
		RTC_DCHECK(ice_transport_);
		ice_transport_->SignalReadPacket.connect(
			this, &FakeDtlsTransport::OnIceTransportReadPacket);
		ice_transport_->SignalNetworkRouteChanged.connect(
			this, &FakeDtlsTransport::OnNetworkRouteChanged);
	}

	explicit FakeDtlsTransport(std::unique_ptr<FakeIceTransport> ice)
		: owned_ice_transport_(std::move(ice)),
		transport_name_(owned_ice_transport_->transport_name()),
		component_(owned_ice_transport_->component()),
		dtls_fingerprint_("", rtc::ArrayView<const uint8_t>()) {
		ice_transport_ = owned_ice_transport_.get();
		ice_transport_->SignalReadPacket.connect(
			this, &FakeDtlsTransport::OnIceTransportReadPacket);
		ice_transport_->SignalNetworkRouteChanged.connect(
			this, &FakeDtlsTransport::OnNetworkRouteChanged);
	}

	// If this constructor is called, a new fake ICE transport will be created,
	// and this FakeDtlsTransport will take the ownership.
	FakeDtlsTransport(const std::string& name, int component)
		: FakeDtlsTransport(std::make_unique<FakeIceTransport>(name, component)) {
	}
	FakeDtlsTransport(const std::string& name,
		int component,
		rtc::Thread* network_thread)
		: FakeDtlsTransport(std::make_unique<FakeIceTransport>(name,
			component,
			network_thread)) {}

	~FakeDtlsTransport() override {
		if (dest_ && dest_->dest_ == this) {
			dest_->dest_ = nullptr;
		}
	}

	// Get inner fake ICE transport.
	FakeIceTransport* fake_ice_transport() { return ice_transport_; }

	// If async, will send packets by "Post"-ing to message queue instead of
	// synchronously "Send"-ing.
	void SetAsync(bool async) { ice_transport_->SetAsync(async); }
	void SetAsyncDelay(int delay_ms) { ice_transport_->SetAsyncDelay(delay_ms); }

	// SetWritable, SetReceiving and SetDestination are the main methods that can
	// be used for testing, to simulate connectivity or lack thereof.
	void SetWritable(bool writable) {
		ice_transport_->SetWritable(writable);
		set_writable(writable);
	}
	void SetReceiving(bool receiving) {
		ice_transport_->SetReceiving(receiving);
		set_receiving(receiving);
	}
	void SetDtlsState(webrtc::DtlsTransportState state) {
		dtls_state_ = state;
		SendDtlsState(this, dtls_state_);
	}

	// Simulates the two DTLS transports connecting to each other.
	// If `asymmetric` is true this method only affects this FakeDtlsTransport.
	// If false, it affects `dest` as well.
	void SetDestination(FakeDtlsTransport* dest, bool asymmetric = false) {
		if (dest == dest_) {
			return;
		}
		RTC_DCHECK(!dest || !dest_)
			<< "Changing fake destination from one to another is not supported.";
		if (dest && !dest_) {
			// This simulates the DTLS handshake.
			dest_ = dest;
			if (local_cert_ && dest_->local_cert_) {
				do_dtls_ = true;
				RTC_LOG(LS_INFO) << "FakeDtlsTransport is doing DTLS";
			}
			else {
				do_dtls_ = false;
				RTC_LOG(LS_INFO) << "FakeDtlsTransport is not doing DTLS";
			}
			SetWritable(true);
			if (!asymmetric) {
				dest->SetDestination(this, true);
			}
			// If the `dtls_role_` is unset, set it to SSL_CLIENT by default.
			if (!dtls_role_) {
				dtls_role_ = std::move(rtc::SSL_CLIENT);
			}
			SetDtlsState(webrtc::DtlsTransportState::kConnected);
			ice_transport_->SetDestination(
				static_cast<FakeIceTransport*>(dest->ice_transport()), asymmetric);
		}
		else {
			// Simulates loss of connectivity, by asymmetrically forgetting dest_.
			dest_ = nullptr;
			SetWritable(false);
			ice_transport_->SetDestination(nullptr, asymmetric);
		}
	}

	// Fake DtlsTransportInternal implementation.
	webrtc::DtlsTransportState dtls_state() const override { return dtls_state_; }
	const std::string& transport_name() const override { return transport_name_; }
	int component() const override { return component_; }
	const rtc::SSLFingerprint& dtls_fingerprint() const {
		return dtls_fingerprint_;
	}
	bool SetRemoteFingerprint(const std::string& alg,
		const uint8_t* digest,
		size_t digest_len) override {
		dtls_fingerprint_ =
			rtc::SSLFingerprint(alg, rtc::MakeArrayView(digest, digest_len));
		return true;
	}
	bool SetDtlsRole(rtc::SSLRole role) override {
		dtls_role_ = std::move(role);
		return true;
	}
	bool GetDtlsRole(rtc::SSLRole* role) const override {
		if (!dtls_role_) {
			return false;
		}
		*role = *dtls_role_;
		return true;
	}
	bool SetLocalCertificate(
		const rtc::scoped_refptr<rtc::RTCCertificate>& certificate) override {
		do_dtls_ = true;
		local_cert_ = certificate;
		return true;
	}
	void SetRemoteSSLCertificate(rtc::FakeSSLCertificate* cert) {
		remote_cert_ = cert;
	}
	bool IsDtlsActive() const override { return do_dtls_; }
	bool GetSslVersionBytes(int* version) const override {
		if (!do_dtls_) {
			return false;
		}
		*version = 0x0102;
		return true;
	}
	bool GetSrtpCryptoSuite(int* crypto_suite) override {
		if (!do_dtls_) {
			return false;
		}
		*crypto_suite = crypto_suite_;
		return true;
	}
	void SetSrtpCryptoSuite(int crypto_suite) { crypto_suite_ = crypto_suite; }

	bool GetSslCipherSuite(int* cipher_suite) override {
		if (ssl_cipher_suite_) {
			*cipher_suite = *ssl_cipher_suite_;
			return true;
		}
		return false;
	}
	void SetSslCipherSuite(absl::optional<int> cipher_suite) {
		ssl_cipher_suite_ = cipher_suite;
	}
	rtc::scoped_refptr<rtc::RTCCertificate> GetLocalCertificate() const override {
		return local_cert_;
	}
	std::unique_ptr<rtc::SSLCertChain> GetRemoteSSLCertChain() const override {
		if (!remote_cert_) {
			return nullptr;
		}
		return std::make_unique<rtc::SSLCertChain>(remote_cert_->Clone());
	}
	bool ExportKeyingMaterial(const std::string& label,
		const uint8_t* context,
		size_t context_len,
		bool use_context,
		uint8_t* result,
		size_t result_len) override {
		if (!do_dtls_) {
			return false;
		}
		memset(result, 0xff, result_len);
		return true;
	}
	void set_ssl_max_protocol_version(rtc::SSLProtocolVersion version) {
		ssl_max_version_ = version;
	}
	rtc::SSLProtocolVersion ssl_max_protocol_version() const {
		return ssl_max_version_;
	}

	cricket::IceTransportInternal* ice_transport() override { return ice_transport_; }

	// PacketTransportInternal implementation, which passes through to fake ICE
	// transport for sending actual packets.
	bool writable() const override { return writable_; }
	bool receiving() const override { return receiving_; }
	int SendPacket(const char* data,
		size_t len,
		const rtc::PacketOptions& options,
		int flags) override {
		// We expect only SRTP packets to be sent through this interface.
		if (flags != cricket::PF_SRTP_BYPASS && flags != 0) {
			return -1;
		}
		return ice_transport_->SendPacket(data, len, options, flags);
	}
	int SetOption(rtc::Socket::Option opt, int value) override {
		return ice_transport_->SetOption(opt, value);
	}
	bool GetOption(rtc::Socket::Option opt, int* value) override {
		return ice_transport_->GetOption(opt, value);
	}
	int GetError() override { return ice_transport_->GetError(); }

	absl::optional<rtc::NetworkRoute> network_route() const override {
		return ice_transport_->network_route();
	}

private:
	void OnIceTransportReadPacket(PacketTransportInternal* ice_,
		const char* data,
		size_t len,
		const int64_t& packet_time_us,
		int flags) {
		SignalReadPacket(this, data, len, packet_time_us, flags);
	}

	void set_receiving(bool receiving) {
		if (receiving_ == receiving) {
			return;
		}
		receiving_ = receiving;
		SignalReceivingState(this);
	}

	void set_writable(bool writable) {
		if (writable_ == writable) {
			return;
		}
		writable_ = writable;
		if (writable_) {
			SignalReadyToSend(this);
		}
		SignalWritableState(this);
	}

	void OnNetworkRouteChanged(absl::optional<rtc::NetworkRoute> network_route) {
		SignalNetworkRouteChanged(network_route);
	}

	FakeIceTransport* ice_transport_;
	std::unique_ptr<FakeIceTransport> owned_ice_transport_;
	std::string transport_name_;
	int component_;
	FakeDtlsTransport* dest_ = nullptr;
	rtc::scoped_refptr<rtc::RTCCertificate> local_cert_;
	rtc::FakeSSLCertificate* remote_cert_ = nullptr;
	bool do_dtls_ = false;
	rtc::SSLProtocolVersion ssl_max_version_ = rtc::SSL_PROTOCOL_DTLS_12;
	rtc::SSLFingerprint dtls_fingerprint_;
	absl::optional<rtc::SSLRole> dtls_role_;
	int crypto_suite_ = rtc::kSrtpAes128CmSha1_80;
	absl::optional<int> ssl_cipher_suite_;

	webrtc::DtlsTransportState dtls_state_ = webrtc::DtlsTransportState::kNew;

	bool receiving_ = false;
	bool writable_ = false;
};

class FakeDtlsTransportFactory : public cricket::DtlsTransportFactory {
public:
	std::unique_ptr<cricket::DtlsTransportInternal> CreateDtlsTransport(
		cricket::IceTransportInternal* ice,
		const webrtc::CryptoOptions& crypto_options,
		rtc::SSLProtocolVersion max_version) override {
		return std::make_unique<FakeDtlsTransport>(
			static_cast<FakeIceTransport*>(ice));
	}
};


class JsepTransportControllerTest : public webrtc::JsepTransportController::Observer,
	//public ::testing::Test,
	public sigslot::has_slots<> {
public:
	JsepTransportControllerTest() : signaling_thread_(rtc::Thread::Current()) 
	{
		fake_ice_transport_factory_ = std::make_unique<FakeIceTransportFactory>();
		fake_dtls_transport_factory_ = std::make_unique<FakeDtlsTransportFactory>();
	}

	void CreateJsepTransportController(
		webrtc::JsepTransportController::Config config,
		rtc::Thread* network_thread = rtc::Thread::Current(),
		cricket::PortAllocator* port_allocator = nullptr) {
		config.transport_observer = this;
		config.rtcp_handler = [](const rtc::CopyOnWriteBuffer& packet,
			int64_t packet_time_us) { RTC_NOTREACHED(); };
		config.ice_transport_factory = fake_ice_transport_factory_.get();
		config.dtls_transport_factory = fake_dtls_transport_factory_.get();
		config.on_dtls_handshake_error_ = [](rtc::SSLHandshakeError s) {};
		transport_controller_ = std::make_unique<webrtc::JsepTransportController>(
			network_thread, port_allocator, nullptr /* async_resolver_factory */,
			config);
		network_thread->Invoke<void>(RTC_FROM_HERE,
			[&] { ConnectTransportControllerSignals(); });
	}

	void ConnectTransportControllerSignals() {
		transport_controller_->SubscribeIceConnectionState(
			[this](cricket::IceConnectionState s) {
			JsepTransportControllerTest::OnConnectionState(s);
		});
		transport_controller_->SubscribeConnectionState(
			[this](webrtc::PeerConnectionInterface::PeerConnectionState s) {
			JsepTransportControllerTest::OnCombinedConnectionState(s);
		});
		transport_controller_->SubscribeStandardizedIceConnectionState(
			[this](webrtc::PeerConnectionInterface::IceConnectionState s) {
			JsepTransportControllerTest::OnStandardizedIceConnectionState(s);
		});
		transport_controller_->SubscribeIceGatheringState(
			[this](cricket::IceGatheringState s) {
			JsepTransportControllerTest::OnGatheringState(s);
		});
		transport_controller_->SubscribeIceCandidateGathered(
			[this](const std::string& transport,
				const std::vector<cricket::Candidate>& candidates) {
			JsepTransportControllerTest::OnCandidatesGathered(transport,
				candidates);
		});
	}

	std::unique_ptr<cricket::SessionDescription>
		CreateSessionDescriptionWithoutBundle() {
		auto description = std::make_unique<cricket::SessionDescription>();
		AddAudioSection(description.get(), kAudioMid1, kIceUfrag1, kIcePwd1,
			cricket::ICEMODE_FULL, cricket::CONNECTIONROLE_ACTPASS,
			nullptr);
		AddVideoSection(description.get(), kVideoMid1, kIceUfrag1, kIcePwd1,
			cricket::ICEMODE_FULL, cricket::CONNECTIONROLE_ACTPASS,
			nullptr);
		return description;
	}

	std::unique_ptr<cricket::SessionDescription>
		CreateSessionDescriptionWithBundleGroup() {
		auto description = CreateSessionDescriptionWithoutBundle();
		cricket::ContentGroup bundle_group(cricket::GROUP_TYPE_BUNDLE);
		bundle_group.AddContentName(kAudioMid1);
		bundle_group.AddContentName(kVideoMid1);
		description->AddGroup(bundle_group);

		return description;
	}

	std::unique_ptr<cricket::SessionDescription>
		CreateSessionDescriptionWithBundledData() {
		auto description = CreateSessionDescriptionWithoutBundle();
		AddDataSection(description.get(), kDataMid1,
			cricket::MediaProtocolType::kSctp, kIceUfrag1, kIcePwd1,
			cricket::ICEMODE_FULL, cricket::CONNECTIONROLE_ACTPASS,
			nullptr);
		cricket::ContentGroup bundle_group(cricket::GROUP_TYPE_BUNDLE);
		bundle_group.AddContentName(kAudioMid1);
		bundle_group.AddContentName(kVideoMid1);
		bundle_group.AddContentName(kDataMid1);
		description->AddGroup(bundle_group);
		return description;
	}

	void AddAudioSection(cricket::SessionDescription* description,
		const std::string& mid,
		const std::string& ufrag,
		const std::string& pwd,
		cricket::IceMode ice_mode,
		cricket::ConnectionRole conn_role,
		rtc::scoped_refptr<rtc::RTCCertificate> cert) {
		std::unique_ptr<cricket::AudioContentDescription> audio(
			new cricket::AudioContentDescription());
		// Set RTCP-mux to be true because the default policy is "mux required".
		audio->set_rtcp_mux(true);
		description->AddContent(mid, cricket::MediaProtocolType::kRtp,
			/*rejected=*/false, std::move(audio));
		AddTransportInfo(description, mid, ufrag, pwd, ice_mode, conn_role, cert);
	}

	void AddVideoSection(cricket::SessionDescription* description,
		const std::string& mid,
		const std::string& ufrag,
		const std::string& pwd,
		cricket::IceMode ice_mode,
		cricket::ConnectionRole conn_role,
		rtc::scoped_refptr<rtc::RTCCertificate> cert) {
		std::unique_ptr<cricket::VideoContentDescription> video(
			new cricket::VideoContentDescription());
		// Set RTCP-mux to be true because the default policy is "mux required".
		video->set_rtcp_mux(true);
		description->AddContent(mid, cricket::MediaProtocolType::kRtp,
			/*rejected=*/false, std::move(video));
		AddTransportInfo(description, mid, ufrag, pwd, ice_mode, conn_role, cert);
	}

	void AddDataSection(cricket::SessionDescription* description,
		const std::string& mid,
		cricket::MediaProtocolType protocol_type,
		const std::string& ufrag,
		const std::string& pwd,
		cricket::IceMode ice_mode,
		cricket::ConnectionRole conn_role,
		rtc::scoped_refptr<rtc::RTCCertificate> cert) {
		RTC_CHECK(protocol_type == cricket::MediaProtocolType::kSctp);
		std::unique_ptr<cricket::SctpDataContentDescription> data(
			new cricket::SctpDataContentDescription());
		data->set_rtcp_mux(true);
		description->AddContent(mid, protocol_type,
			/*rejected=*/false, std::move(data));
		AddTransportInfo(description, mid, ufrag, pwd, ice_mode, conn_role, cert);
	}

	void AddTransportInfo(cricket::SessionDescription* description,
		const std::string& mid,
		const std::string& ufrag,
		const std::string& pwd,
		cricket::IceMode ice_mode,
		cricket::ConnectionRole conn_role,
		rtc::scoped_refptr<rtc::RTCCertificate> cert) {
		std::unique_ptr<rtc::SSLFingerprint> fingerprint;
		if (cert) {
			fingerprint = rtc::SSLFingerprint::CreateFromCertificate(*cert);
		}

		cricket::TransportDescription transport_desc(std::vector<std::string>(),
			ufrag, pwd, ice_mode,
			conn_role, fingerprint.get());
		description->AddTransportInfo(cricket::TransportInfo(mid, transport_desc));
	}

	cricket::IceConfig CreateIceConfig(
		int receiving_timeout,
		cricket::ContinualGatheringPolicy continual_gathering_policy) {
		cricket::IceConfig config;
		config.receiving_timeout = receiving_timeout;
		config.continual_gathering_policy = continual_gathering_policy;
		return config;
	}

	cricket::Candidate CreateCandidate(const std::string& transport_name, int component) {
		cricket::Candidate c;
		c.set_transport_name(transport_name);
		c.set_address(rtc::SocketAddress("192.168.1.1", 8000));
		c.set_component(component);
		c.set_protocol(cricket::UDP_PROTOCOL_NAME);
		c.set_priority(1);
		return c;
	}

	void CreateLocalDescriptionAndCompleteConnectionOnNetworkThread() {
		if (!network_thread_->IsCurrent()) {
			network_thread_->Invoke<void>(RTC_FROM_HERE, [&] {
				CreateLocalDescriptionAndCompleteConnectionOnNetworkThread();
			});
			return;
		}

		auto description = CreateSessionDescriptionWithBundleGroup();
		/*EXPECT_TRUE*/(transport_controller_
			->SetLocalDescription(webrtc::SdpType::kOffer, description.get())
			.ok());

		transport_controller_->MaybeStartGathering();
		auto fake_audio_dtls = static_cast<FakeDtlsTransport*>(
			transport_controller_->GetDtlsTransport(kAudioMid1));
		auto fake_video_dtls = static_cast<FakeDtlsTransport*>(
			transport_controller_->GetDtlsTransport(kVideoMid1));
		fake_audio_dtls->fake_ice_transport()->SignalCandidateGathered(
			fake_audio_dtls->fake_ice_transport(),
			CreateCandidate(kAudioMid1, /*component=*/1));
		fake_video_dtls->fake_ice_transport()->SignalCandidateGathered(
			fake_video_dtls->fake_ice_transport(),
			CreateCandidate(kVideoMid1, /*component=*/1));
		fake_audio_dtls->fake_ice_transport()->SetCandidatesGatheringComplete();
		fake_video_dtls->fake_ice_transport()->SetCandidatesGatheringComplete();
		fake_audio_dtls->fake_ice_transport()->SetConnectionCount(2);
		fake_video_dtls->fake_ice_transport()->SetConnectionCount(2);
		fake_audio_dtls->SetReceiving(true);
		fake_video_dtls->SetReceiving(true);
		fake_audio_dtls->SetWritable(true);
		fake_video_dtls->SetWritable(true);
		fake_audio_dtls->fake_ice_transport()->SetConnectionCount(1);
		fake_video_dtls->fake_ice_transport()->SetConnectionCount(1);
	}

public:
	void OnConnectionState(cricket::IceConnectionState state) {
		ice_signaled_on_thread_ = rtc::Thread::Current();
		connection_state_ = state;
		++connection_state_signal_count_;
	}

	void OnStandardizedIceConnectionState(
		webrtc::PeerConnectionInterface::IceConnectionState state) {
		ice_signaled_on_thread_ = rtc::Thread::Current();
		ice_connection_state_ = state;
		++ice_connection_state_signal_count_;
	}

	void OnCombinedConnectionState(
		webrtc::PeerConnectionInterface::PeerConnectionState state) {
		RTC_LOG(LS_INFO) << "OnCombinedConnectionState: "
			<< static_cast<int>(state);
		ice_signaled_on_thread_ = rtc::Thread::Current();
		combined_connection_state_ = state;
		++combined_connection_state_signal_count_;
	}

	void OnGatheringState(cricket::IceGatheringState state) {
		ice_signaled_on_thread_ = rtc::Thread::Current();
		gathering_state_ = state;
		++gathering_state_signal_count_;
	}

	void OnCandidatesGathered(const std::string& transport_name,
		const cricket::Candidates& candidates) {
		ice_signaled_on_thread_ = rtc::Thread::Current();
		candidates_[transport_name].insert(candidates_[transport_name].end(),
			candidates.begin(), candidates.end());
		++candidates_signal_count_;
	}

	// JsepTransportController::Observer overrides.
	bool OnTransportChanged(
		const std::string& mid,
		webrtc::RtpTransportInternal* rtp_transport,
		rtc::scoped_refptr<webrtc::DtlsTransport> dtls_transport,
		webrtc::DataChannelTransportInterface* data_channel_transport) override {
		changed_rtp_transport_by_mid_[mid] = rtp_transport;
		if (dtls_transport) {
			changed_dtls_transport_by_mid_[mid] = dtls_transport->internal();
		}
		else {
			changed_dtls_transport_by_mid_[mid] = nullptr;
		}
		return true;
	}

	// Information received from signals from transport controller.
	cricket::IceConnectionState connection_state_ =
		cricket::kIceConnectionConnecting;
	webrtc::PeerConnectionInterface::IceConnectionState ice_connection_state_ =
		webrtc::PeerConnectionInterface::kIceConnectionNew;
	webrtc::PeerConnectionInterface::PeerConnectionState combined_connection_state_ =
		webrtc::PeerConnectionInterface::PeerConnectionState::kNew;
	bool receiving_ = false;
	cricket::IceGatheringState gathering_state_ = cricket::kIceGatheringNew;
	// transport_name => candidates
	std::map<std::string, cricket::Candidates> candidates_;
	// Counts of each signal emitted.
	int connection_state_signal_count_ = 0;
	int ice_connection_state_signal_count_ = 0;
	int combined_connection_state_signal_count_ = 0;
	int receiving_signal_count_ = 0;
	int gathering_state_signal_count_ = 0;
	int candidates_signal_count_ = 0;

	// `network_thread_` should be destroyed after `transport_controller_`
	std::unique_ptr<rtc::Thread> network_thread_;
	std::unique_ptr<FakeIceTransportFactory> fake_ice_transport_factory_;
	std::unique_ptr<FakeDtlsTransportFactory> fake_dtls_transport_factory_;
	rtc::Thread* const signaling_thread_ = nullptr;
	rtc::Thread* ice_signaled_on_thread_ = nullptr;
	// Used to verify the SignalRtpTransportChanged/SignalDtlsTransportChanged are
	// signaled correctly.
	std::map<std::string, webrtc::RtpTransportInternal*> changed_rtp_transport_by_mid_;
	std::map<std::string, cricket::DtlsTransportInternal*>
		changed_dtls_transport_by_mid_;

	// Transport controller needs to be destroyed first, because it may issue
	// callbacks that modify the changed_*_by_mid in the destructor.
	std::unique_ptr<webrtc::JsepTransportController> transport_controller_;
};


int main(int argc, char * argv[])
{
	JsepTransportControllerTest jsep_transport_test;
	jsep_transport_test.CreateJsepTransportController(webrtc::JsepTransportController::Config());
	auto description = jsep_transport_test.CreateSessionDescriptionWithoutBundle();
	/*EXPECT_TRUE*/(jsep_transport_test.transport_controller_
		->SetLocalDescription(webrtc::SdpType::kOffer, description.get())
		.ok());
	auto audio_rtp_transport = jsep_transport_test.transport_controller_->GetRtpTransport(kAudioMid1);
	auto video_rtp_transport = jsep_transport_test.transport_controller_->GetRtpTransport(kVideoMid1);
	jsep_transport_test.transport_controller_->GetRtpTransport(kAudioMid2);
	//EXPECT_NE(nullptr, audio_rtp_transport);
	//EXPECT_NE(nullptr, video_rtp_transport);
	//EXPECT_NE(audio_rtp_transport, video_rtp_transport);
	// Return nullptr for non-existing ones.
	//EXPECT_EQ(nullptr, transport_controller_->GetRtpTransport(kAudioMid2));

	return  EXIT_SUCCESS;
}