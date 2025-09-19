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
#ifndef  __C_FAKE_ICE_TRANSPORT_WRAPPER_H_
#define  __C_FAKE_ICE_TRANSPORT_WRAPPER_H_




#include "webrtc_ice_test/cfake_ice_define.h"
#include "webrtc_ice_test/cfake_ice_transport.h"
#include "webrtc_ice_test/cfake_dtls_transport.h"
#include "webrtc_ice_test/cfake_dtls_transport_factory.h"
#include "webrtc_ice_test/cfake_ice_transport_wrapper.h"
namespace libice {

	class FakeIceTransportWrapper : public webrtc::IceTransportInterface {
	public:
		explicit FakeIceTransportWrapper(
			std::unique_ptr<FakeIceTransport> internal)
			: internal_(std::move(internal)) {}

		cricket::IceTransportInternal* internal() override { return internal_.get(); }

	private:
		std::unique_ptr<FakeIceTransport> internal_;
	};
}

#endif // __C_FAKE_ICE_TRANSPORT_WRAPPER_H_