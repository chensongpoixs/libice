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
#include "webrtc_ice_test/cfake_ice_define.h"
#include "webrtc_ice_test/cjsep_transport_controller_test.h"

#include "api/jsep_session_description.h"

using namespace libice;

void GetRtpTransport()
{
	JsepTransportControllerTest jsep_transport_test;
	jsep_transport_test.CreateJsepTransportController(webrtc::JsepTransportController::Config());
	std::unique_ptr<cricket::SessionDescription> description = jsep_transport_test.CreateSessionDescriptionWithoutBundle();
	
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


 


int main(int argc, char * argv[])
{
	GetRtpTransport();

	return  EXIT_SUCCESS;
}