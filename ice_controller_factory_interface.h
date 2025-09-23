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


#ifndef _C_P2P_BASE_ICE_CONTROLLER_FACTORY_INTERFACE_H_
#define _C_P2P_BASE_ICE_CONTROLLER_FACTORY_INTERFACE_H_

#include <memory>
#include <string>

#include "libice/ice_controller_interface.h"
#include "libice/ice_transport_internal.h"

namespace libice {

// struct with arguments to IceControllerFactoryInterface::Create
struct IceControllerFactoryArgs {
  std::function<IceTransportState()> ice_transport_state_func;
  std::function<IceRole()> ice_role_func;
  std::function<bool(const Connection*)> is_connection_pruned_func;
  const IceFieldTrials* ice_field_trials;
  std::string ice_controller_field_trials;
};

class IceControllerFactoryInterface {
 public:
  virtual ~IceControllerFactoryInterface() = default;
  virtual std::unique_ptr<IceControllerInterface> Create(
      const IceControllerFactoryArgs& args) = 0;
};

}  // namespace cricket

#endif  // P2P_BASE_ICE_CONTROLLER_FACTORY_INTERFACE_H_
