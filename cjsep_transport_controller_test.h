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
#ifndef  __C_JSEP_TRANSPORT_CONTROLLER_TEST_H_
#define  __C_JSEP_TRANSPORT_CONTROLLER_TEST_H_


#include "fake_ice_transport/cfake_ice_define.h"
#include "fake_ice_transport/cfake_ice_transport.h"
#include "fake_dtls_transport/cfake_dtls_transport.h"
#include "fake_dtls_transport/cfake_dtls_transport_factory.h"
#include "fake_ice_transport/cfake_ice_transport_wrapper.h"
#include "fake_ice_transport/cfake_ice_transport_factory.h"
namespace libice {


	class JsepTransportControllerTest : public webrtc::JsepTransportController::Observer,
		//public ::testing::Test,
		public sigslot::has_slots<> {
	public:
		JsepTransportControllerTest();

		void CreateJsepTransportController(
			webrtc::JsepTransportController::Config config,
			rtc::Thread* network_thread = rtc::Thread::Current(),
			cricket::PortAllocator* port_allocator = nullptr);

		void ConnectTransportControllerSignals();

		std::unique_ptr<cricket::SessionDescription>
			CreateSessionDescriptionWithoutBundle();

		std::unique_ptr<cricket::SessionDescription>
			CreateSessionDescriptionWithBundleGroup();

		std::unique_ptr<cricket::SessionDescription>
			CreateSessionDescriptionWithBundledData();

		void AddAudioSection(cricket::SessionDescription* description,
			const std::string& mid,
			const std::string& ufrag,
			const std::string& pwd,
			cricket::IceMode ice_mode,
			cricket::ConnectionRole conn_role,
			rtc::scoped_refptr<rtc::RTCCertificate> cert);

		void AddVideoSection(cricket::SessionDescription* description,
			const std::string& mid,
			const std::string& ufrag,
			const std::string& pwd,
			cricket::IceMode ice_mode,
			cricket::ConnectionRole conn_role,
			rtc::scoped_refptr<rtc::RTCCertificate> cert);

		void AddDataSection(cricket::SessionDescription* description,
			const std::string& mid,
			cricket::MediaProtocolType protocol_type,
			const std::string& ufrag,
			const std::string& pwd,
			cricket::IceMode ice_mode,
			cricket::ConnectionRole conn_role,
			rtc::scoped_refptr<rtc::RTCCertificate> cert);

		void AddTransportInfo(cricket::SessionDescription* description,
			const std::string& mid,
			const std::string& ufrag,
			const std::string& pwd,
			cricket::IceMode ice_mode,
			cricket::ConnectionRole conn_role,
			rtc::scoped_refptr<rtc::RTCCertificate> cert);

		cricket::IceConfig CreateIceConfig(
			int receiving_timeout,
			cricket::ContinualGatheringPolicy continual_gathering_policy);

		cricket::Candidate CreateCandidate(const std::string& transport_name, int component);

		void CreateLocalDescriptionAndCompleteConnectionOnNetworkThread();

	public:
		void OnConnectionState(cricket::IceConnectionState state);

		void OnStandardizedIceConnectionState(
			webrtc::PeerConnectionInterface::IceConnectionState state);

		void OnCombinedConnectionState(
			webrtc::PeerConnectionInterface::PeerConnectionState state);

		void OnGatheringState(cricket::IceGatheringState state);

		void OnCandidatesGathered(const std::string& transport_name,
			const cricket::Candidates& candidates);

		// JsepTransportController::Observer overrides.
		bool OnTransportChanged(
			const std::string& mid,
			webrtc::RtpTransportInternal* rtp_transport,
			rtc::scoped_refptr<webrtc::DtlsTransport> dtls_transport,
			webrtc::DataChannelTransportInterface* data_channel_transport) override;

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
}

#endif // __C_JSEP_TRANSPORT_CONTROLLER_TEST_H_