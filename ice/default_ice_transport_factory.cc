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


#include "ice/default_ice_transport_factory.h"

#include <utility>

#include "ice/basic_ice_controller.h"
#include "ice/ice_controller_factory_interface.h"

namespace {

class BasicIceControllerFactory
    : public libice::IceControllerFactoryInterface {
 public:
  std::unique_ptr<libice::IceControllerInterface> Create(
      const libice::IceControllerFactoryArgs& args) override {
    return std::make_unique<libice::BasicIceController>(args);
  }
};

}  // namespace

namespace libice {

DefaultIceTransport::DefaultIceTransport(
    std::unique_ptr<libice::P2PTransportChannel> internal)
    : internal_(std::move(internal)) {}

DefaultIceTransport::~DefaultIceTransport() {
  RTC_DCHECK_RUN_ON(&thread_checker_);
}

rtc::scoped_refptr<IceTransportInterface>
DefaultIceTransportFactory::CreateIceTransport(
    const std::string& transport_name,
    int component,
    IceTransportInit init) {
  BasicIceControllerFactory factory;
  return rtc::make_ref_counted<DefaultIceTransport>(
      libice::P2PTransportChannel::Create(
          transport_name, component, init.port_allocator(),
          init.async_dns_resolver_factory(), init.event_log(), &factory));
}

}  // namespace webrtc
