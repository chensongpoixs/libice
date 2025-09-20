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
#ifndef  __C_FAKE_ICE_TRANSPORT_H_
#define  __C_FAKE_ICE_TRANSPORT_H_



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

namespace libice {



	class FakeIceTransport : public cricket::IceTransportInternal {
	public:
		explicit FakeIceTransport(const std::string& name,
			int component,
			rtc::Thread* network_thread = nullptr);
		// Must be called either on the network thread, or after the network thread
		// has been shut down.
		~FakeIceTransport() override;

		// If async, will send packets by "Post"-ing to message queue instead of
		// synchronously "Send"-ing.
		void SetAsync(bool async);
		void SetAsyncDelay(int delay_ms);

		// SetWritable, SetReceiving and SetDestination are the main methods that can
		// be used for testing, to simulate connectivity or lack thereof.
		void SetWritable(bool writable);
		void SetReceiving(bool receiving);

		// Simulates the two transports connecting to each other.
		// If `asymmetric` is true this method only affects this FakeIceTransport.
		// If false, it affects `dest` as well.
		void SetDestination(FakeIceTransport* dest, bool asymmetric = false);

		void SetTransportState(webrtc::IceTransportState state,
			cricket::IceTransportState legacy_state);

		void SetConnectionCount(size_t connection_count);

		void SetCandidatesGatheringComplete();

		// Convenience functions for accessing ICE config and other things.
		int receiving_timeout() const;
		bool gather_continually() const;
		const cricket::Candidates& remote_candidates() const;

		// Fake IceTransportInternal implementation.
		const std::string& transport_name() const override;
		int component() const override;
		uint64_t IceTiebreaker() const;
		cricket::IceMode remote_ice_mode() const;
		const std::string& ice_ufrag() const;
		const std::string& ice_pwd() const;
		const std::string& remote_ice_ufrag() const;
		const std::string& remote_ice_pwd() const;
		const cricket::IceParameters& ice_parameters() const;
		const cricket::IceParameters& remote_ice_parameters() const;

		cricket::IceTransportState GetState() const override;

		webrtc::IceTransportState GetIceTransportState() const override;

		void SetIceRole(cricket::IceRole role) override;
		cricket::IceRole GetIceRole() const override;
		void SetIceTiebreaker(uint64_t tiebreaker) override;
		void SetIceParameters(const cricket::IceParameters& ice_params) override;
		void SetRemoteIceParameters(const cricket::IceParameters& params) override;

		void SetRemoteIceMode(cricket::IceMode mode) override;

		void MaybeStartGathering() override;

		cricket::IceGatheringState gathering_state() const override;

		void SetIceConfig(const cricket::IceConfig& config) override;

		void AddRemoteCandidate(const cricket::Candidate& candidate) override;
		void RemoveRemoteCandidate(const cricket::Candidate& candidate) override;

		void RemoveAllRemoteCandidates() override;

		bool GetStats(cricket::IceTransportStats* ice_transport_stats) override;

		absl::optional<int> GetRttEstimate() override;

		const cricket::Connection* selected_connection() const override;
		absl::optional<const cricket::CandidatePair> GetSelectedCandidatePair()
			const override;

		// Fake PacketTransportInternal implementation.
		bool writable() const override;
		bool receiving() const override;
		// If combine is enabled, every two consecutive packets to be sent with
		// "SendPacket" will be combined into one outgoing packet.
		void combine_outgoing_packets(bool combine);
		int SendPacket(const char* data,
			size_t len,
			const rtc::PacketOptions& options,
			int flags) override;

		int SetOption(rtc::Socket::Option opt, int value) override;
		bool GetOption(rtc::Socket::Option opt, int* value) override;

		int GetError() override;

		rtc::CopyOnWriteBuffer last_sent_packet();

		absl::optional<rtc::NetworkRoute> network_route() const override;
		void SetNetworkRoute(absl::optional<rtc::NetworkRoute> network_route);

	private:
		void set_writable(bool writable);

		void set_receiving(bool receiving);

		void SendPacketInternal(const rtc::CopyOnWriteBuffer& packet);

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
}
#endif // __C_FAKE_ICE_TRANSPORT_H_