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




#include "libice/ice_transport_factory.h"

#include <memory>
#include <utility>

#include "libice/ice_transport_internal.h"
#include "libice/p2p_constants.h"
#include "libice/p2p_transport_channel.h"
#include "libice/port_allocator.h"
#include "rtc_base/thread.h"

namespace libice {

namespace {

// This implementation of IceTransportInterface is used in cases where
// the only reference to the P2PTransport will be through this class.
// It must be constructed, accessed and destroyed on the signaling thread.
class IceTransportWithTransportChannel : public IceTransportInterface {
 public:
  IceTransportWithTransportChannel(
      std::unique_ptr<libice::IceTransportInternal> internal)
      : internal_(std::move(internal)) {}

  ~IceTransportWithTransportChannel() override {
    RTC_DCHECK_RUN_ON(&thread_checker_);
  }

  libice::IceTransportInternal* internal() override {
    RTC_DCHECK_RUN_ON(&thread_checker_);
    return internal_.get();
  }

 private:
  const webrtc::SequenceChecker thread_checker_{};
  const std::unique_ptr<libice::IceTransportInternal> internal_
      RTC_GUARDED_BY(thread_checker_);
};

}  // namespace

rtc::scoped_refptr<IceTransportInterface> CreateIceTransport(
	libice::PortAllocator* port_allocator) {
  IceTransportInit init;
  init.set_port_allocator(port_allocator);
  return CreateIceTransport(std::move(init));
}

rtc::scoped_refptr<IceTransportInterface> CreateIceTransport(
    IceTransportInit init) {
  if (init.async_resolver_factory()) {
    // Backwards compatibility mode
    return rtc::make_ref_counted<IceTransportWithTransportChannel>(
        std::make_unique<libice::P2PTransportChannel>(
            "", libice::ICE_CANDIDATE_COMPONENT_RTP, init.port_allocator(),
            init.async_resolver_factory(), init.event_log()));
  } else {
    return rtc::make_ref_counted<IceTransportWithTransportChannel>(
		libice::P2PTransportChannel::Create(
            "", libice::ICE_CANDIDATE_COMPONENT_RTP, init.port_allocator(),
            init.async_dns_resolver_factory(), init.event_log()));
  }
}

}  // namespace webrtc
