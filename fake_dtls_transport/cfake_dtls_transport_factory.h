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
#ifndef  __C_FAKE_DTLS_TRANSPORT_FACTORY_H_
#define  __C_FAKE_DTLS_TRANSPORT_FACTORY_H_



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
}
#endif // __C_FAKE_DTLS_TRANSPORT_FACTORY_H_