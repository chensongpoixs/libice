
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


#include "webrtc_ice_test/cjsep_transport_controller_test.h"

namespace libice {

	JsepTransportControllerTest::JsepTransportControllerTest() : signaling_thread_(rtc::Thread::Current())
	{
		fake_ice_transport_factory_ = std::make_unique<FakeIceTransportFactory>();
		fake_dtls_transport_factory_ = std::make_unique<FakeDtlsTransportFactory>();
	}

	void JsepTransportControllerTest::CreateJsepTransportController(
		webrtc::JsepTransportController::Config config, rtc::Thread* network_thread 
		, cricket::PortAllocator* port_allocator) 
	{
		config.transport_observer = this;
		config.rtcp_handler = [](const rtc::CopyOnWriteBuffer& packet,
			int64_t packet_time_us) { RTC_NOTREACHED(); };

		config.ice_transport_factory = fake_ice_transport_factory_.get();
		config.dtls_transport_factory = fake_dtls_transport_factory_.get();
		config.on_dtls_handshake_error_ = [](rtc::SSLHandshakeError s) {};


		transport_controller_ = std::make_unique<webrtc::JsepTransportController>(
			network_thread, port_allocator, nullptr /* async_resolver_factory */,
			config);

		network_thread->Invoke<void>(RTC_FROM_HERE, [&] {
			ConnectTransportControllerSignals(); 
		});
	}

	void JsepTransportControllerTest::ConnectTransportControllerSignals() {
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
		JsepTransportControllerTest::CreateSessionDescriptionWithoutBundle() {
		std::unique_ptr<cricket::SessionDescription> description = std::make_unique<cricket::SessionDescription>();
		AddAudioSection(description.get(), kAudioMid1, kIceUfrag1, kIcePwd1,
			cricket::ICEMODE_FULL, cricket::CONNECTIONROLE_ACTPASS,
			nullptr);
		AddVideoSection(description.get(), kVideoMid1, kIceUfrag1, kIcePwd1,
			cricket::ICEMODE_FULL, cricket::CONNECTIONROLE_ACTPASS,
			nullptr);
		return description;
	}

	std::unique_ptr<cricket::SessionDescription>
		JsepTransportControllerTest::CreateSessionDescriptionWithBundleGroup() {
		auto description = CreateSessionDescriptionWithoutBundle();
		cricket::ContentGroup bundle_group(cricket::GROUP_TYPE_BUNDLE);
		bundle_group.AddContentName(kAudioMid1);
		bundle_group.AddContentName(kVideoMid1);
		description->AddGroup(bundle_group);

		return description;
	}

	std::unique_ptr<cricket::SessionDescription>
		JsepTransportControllerTest::CreateSessionDescriptionWithBundledData() {
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

	void JsepTransportControllerTest::AddAudioSection(cricket::SessionDescription* description,
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

	void JsepTransportControllerTest::AddVideoSection(cricket::SessionDescription* description,
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

	void JsepTransportControllerTest::AddDataSection(cricket::SessionDescription* description,
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

	void JsepTransportControllerTest::AddTransportInfo(cricket::SessionDescription* description,
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

	cricket::IceConfig JsepTransportControllerTest::CreateIceConfig(
		int receiving_timeout,
		cricket::ContinualGatheringPolicy continual_gathering_policy) {
		cricket::IceConfig config;
		config.receiving_timeout = receiving_timeout;
		config.continual_gathering_policy = continual_gathering_policy;
		return config;
	}

	cricket::Candidate JsepTransportControllerTest::CreateCandidate(const std::string& transport_name, int component) {
		cricket::Candidate c;
		c.set_transport_name(transport_name);
		c.set_address(rtc::SocketAddress("192.168.1.1", 8000));
		c.set_component(component);
		c.set_protocol(cricket::UDP_PROTOCOL_NAME);
		c.set_priority(1);
		return c;
	}

	void JsepTransportControllerTest::CreateLocalDescriptionAndCompleteConnectionOnNetworkThread() {
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

	void JsepTransportControllerTest::OnConnectionState(cricket::IceConnectionState state) {
		ice_signaled_on_thread_ = rtc::Thread::Current();
		connection_state_ = state;
		++connection_state_signal_count_;
	}

	void JsepTransportControllerTest::OnStandardizedIceConnectionState(
		webrtc::PeerConnectionInterface::IceConnectionState state) {
		ice_signaled_on_thread_ = rtc::Thread::Current();
		ice_connection_state_ = state;
		++ice_connection_state_signal_count_;
	}

	void JsepTransportControllerTest::OnCombinedConnectionState(
		webrtc::PeerConnectionInterface::PeerConnectionState state) {
		RTC_LOG(LS_INFO) << "OnCombinedConnectionState: "
			<< static_cast<int>(state);
		ice_signaled_on_thread_ = rtc::Thread::Current();
		combined_connection_state_ = state;
		++combined_connection_state_signal_count_;
	}

	void JsepTransportControllerTest::OnGatheringState(cricket::IceGatheringState state) {
		ice_signaled_on_thread_ = rtc::Thread::Current();
		gathering_state_ = state;
		++gathering_state_signal_count_;
	}

	void JsepTransportControllerTest::OnCandidatesGathered(const std::string& transport_name,
		const cricket::Candidates& candidates) {
		ice_signaled_on_thread_ = rtc::Thread::Current();
		candidates_[transport_name].insert(candidates_[transport_name].end(),
			candidates.begin(), candidates.end());
		++candidates_signal_count_;
	}

	// JsepTransportController::Observer overrides.
	bool JsepTransportControllerTest::OnTransportChanged(
		const std::string& mid,
		webrtc::RtpTransportInternal* rtp_transport,
		rtc::scoped_refptr<webrtc::DtlsTransport> dtls_transport,
		webrtc::DataChannelTransportInterface* data_channel_transport) {
		changed_rtp_transport_by_mid_[mid] = rtp_transport;
		if (dtls_transport) {
			changed_dtls_transport_by_mid_[mid] = dtls_transport->internal();
		}
		else {
			changed_dtls_transport_by_mid_[mid] = nullptr;
		}
		return true;
	}
}