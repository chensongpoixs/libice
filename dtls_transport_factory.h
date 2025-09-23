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
				   date:  2025-09-21



 ******************************************************************************/


#ifndef _C_P2P_BASE_DTLS_TRANSPORT_FACTORY_H_
#define _C_P2P_BASE_DTLS_TRANSPORT_FACTORY_H_

#include <memory>
#include <string>

#include "libice/dtls_transport_internal.h"
#include "libice/ice_transport_internal.h"

namespace libice {

// This interface is used to create DTLS transports. The external transports
// can be injected into the JsepTransportController through it.
//
// TODO(qingsi): Remove this factory in favor of one that produces
// DtlsTransportInterface given by the public API if this is going to be
// injectable.
class DtlsTransportFactory {
 public:
  virtual ~DtlsTransportFactory() = default;

  virtual std::unique_ptr<DtlsTransportInternal> CreateDtlsTransport(
      IceTransportInternal* ice,
      const webrtc::CryptoOptions& crypto_options,
      rtc::SSLProtocolVersion max_version) = 0;
};

}  // namespace cricket

#endif  // P2P_BASE_DTLS_TRANSPORT_FACTORY_H_
