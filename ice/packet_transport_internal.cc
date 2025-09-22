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


#include "ice/packet_transport_internal.h"

namespace libice {

PacketTransportInternal::PacketTransportInternal() = default;

PacketTransportInternal::~PacketTransportInternal() = default;

bool PacketTransportInternal::GetOption(rtc::Socket::Option opt, int* value) {
  return false;
}

absl::optional<rtc::NetworkRoute> PacketTransportInternal::network_route() const {
  return absl::optional<rtc::NetworkRoute>();
}

}  // namespace rtc
