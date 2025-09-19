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
#ifndef  __C_FAKE_ICE_DEFINE_H_
#define  __C_FAKE_ICE_DEFINE_H_



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

 
namespace libice {

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

}
#endif /// __C_FAKE_ICE_DEFINE_H_