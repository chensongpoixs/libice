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
#ifndef  __C_FAKE_ICE_TRANSPORT_FACTORY_H_
#define  __C_FAKE_ICE_TRANSPORT_FACTORY_H_

#include "fake_ice_transport/cfake_ice_define.h"
#include "fake_ice_transport/cfake_ice_transport.h" 
#include "fake_ice_transport/cfake_ice_transport_wrapper.h"



namespace libice {


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
}

#endif // __C_FAKE_ICE_TRANSPORT_FACTORY_H_