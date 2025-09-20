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
#include "fake_ice_transport/cfake_ice_define.h"
#include "cjsep_transport_controller_test.h"

#include "api/jsep_session_description.h"
#include <thread>
#include <chrono>
using namespace libice;

JsepTransportControllerTest jsep_transport_test;
std::unique_ptr<cricket::SessionDescription> description;


void test_jsep_transport_controller_init()
{
	jsep_transport_test.CreateJsepTransportController(webrtc::JsepTransportController::Config());
	description = jsep_transport_test.CreateSessionDescriptionWithoutBundle();

}



void GetRtpTransport()
{
	
	
	auto jsep_desc = std::make_unique<webrtc::JsepSessionDescription>(webrtc::SdpType::kAnswer);

	jsep_desc->Initialize(std::move(description) , "2323", "version1.0");


	std::string sdp;
	jsep_desc->ToString(&sdp);
	//description->
	/*EXPECT_TRUE*/(jsep_transport_test.transport_controller_
		->SetLocalDescription(webrtc::SdpType::kOffer, jsep_desc->description()/*description.get()*/)
		.ok());
	auto audio_rtp_transport = jsep_transport_test.transport_controller_->GetRtpTransport(kAudioMid1);
	auto video_rtp_transport = jsep_transport_test.transport_controller_->GetRtpTransport(kVideoMid1);
	jsep_transport_test.transport_controller_->GetRtpTransport(kAudioMid2);
	//EXPECT_NE(nullptr, audio_rtp_transport);
	//EXPECT_NE(nullptr, video_rtp_transport);
	//EXPECT_NE(audio_rtp_transport, video_rtp_transport);
	// Return nullptr for non-existing ones.
	//EXPECT_EQ(nullptr, transport_controller_->GetRtpTransport(kAudioMid2));
}


void  SetIceConfig()
{
	 (jsep_transport_test.transport_controller_
		->SetLocalDescription(webrtc::SdpType::kOffer, description.get())
		.ok());

	 jsep_transport_test.transport_controller_->SetIceConfig(
		 jsep_transport_test.CreateIceConfig(kTimeout, cricket::GATHER_CONTINUALLY));
	FakeDtlsTransport* fake_audio_dtls = static_cast<FakeDtlsTransport*>(
		jsep_transport_test.transport_controller_->GetDtlsTransport(kAudioMid1));
	//ASSERT_NE(nullptr, fake_audio_dtls);
	//EXPECT_EQ(kTimeout,
		fake_audio_dtls->fake_ice_transport()->receiving_timeout()/*)*/;
	/*EXPECT_TRUE*/(fake_audio_dtls->fake_ice_transport()->gather_continually());

	// Test that value stored in controller is applied to new transports.
	jsep_transport_test.AddAudioSection(description.get(), kAudioMid2, kIceUfrag1, kIcePwd1,
		cricket::ICEMODE_FULL, cricket::CONNECTIONROLE_ACTPASS,
		nullptr);

	/*EXPECT_TRUE*/(jsep_transport_test.transport_controller_
		->SetLocalDescription(webrtc::SdpType::kOffer, description.get())
		.ok());
	fake_audio_dtls = static_cast<FakeDtlsTransport*>(
		jsep_transport_test.transport_controller_->GetDtlsTransport(kAudioMid2));
	//ASSERT_NE(nullptr, fake_audio_dtls);
	//EXPECT_EQ(kTimeout,
		fake_audio_dtls->fake_ice_transport()->receiving_timeout()/*)*/;
	/*EXPECT_TRUE*/(fake_audio_dtls->fake_ice_transport()->gather_continually());
}

void  MaybeStartGathering()
{
	 
	 (jsep_transport_test.transport_controller_
		->SetLocalDescription(webrtc::SdpType::kOffer, description.get())
		.ok());
	// After setting the local description, we should be able to start gathering
	// candidates.
	 jsep_transport_test.transport_controller_->MaybeStartGathering();
	 //cricket::kIceGatheringGathering
	//EXPECT_EQ_WAIT(cricket::kIceGatheringGathering, gathering_state_, kTimeout);
	//EXPECT_EQ(1, gathering_state_signal_count_);
}

void  AddRemoveRemoteCandidates()
{
	 
	jsep_transport_test.transport_controller_->SetLocalDescription(webrtc::SdpType::kOffer,
		description.get());
	jsep_transport_test.transport_controller_->SetRemoteDescription(webrtc::SdpType::kAnswer,
		description.get());
	auto fake_audio_dtls = static_cast<FakeDtlsTransport*>(
		jsep_transport_test.transport_controller_->GetDtlsTransport(kAudioMid1));
	//ASSERT_NE(nullptr, fake_audio_dtls);
	cricket::Candidates candidates;
	candidates.push_back(
		jsep_transport_test.CreateCandidate(kAudioMid1, cricket::ICE_CANDIDATE_COMPONENT_RTP));
	/*EXPECT_TRUE*/(
		jsep_transport_test.transport_controller_->AddRemoteCandidates(kAudioMid1, candidates).ok());
	/*EXPECT_EQ*///(1U,
	int size =	 fake_audio_dtls->fake_ice_transport()->remote_candidates().size()/*)*/;

	/*EXPECT_TRUE*/(jsep_transport_test.transport_controller_->RemoveRemoteCandidates(candidates).ok());
	//EXPECT_EQ(0U,
	size =	fake_audio_dtls->fake_ice_transport()->remote_candidates().size()/*)*/;
}




 


int main(int argc, char * argv[])
{
	test_jsep_transport_controller_init();
	//GetRtpTransport();


	//SetIceConfig();
	//MaybeStartGathering();
	AddRemoveRemoteCandidates();
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return  EXIT_SUCCESS;
}