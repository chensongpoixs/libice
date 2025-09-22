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


#ifndef _C_P2P_BASE_TRANSPORT_INFO_H_
#define _C_P2P_BASE_TRANSPORT_INFO_H_

#include <string>
#include <vector>

#include "ice/candidate.h"
#include "ice/p2p_constants.h"
#include "ice/transport_description.h"
#include "rtc_base/helpers.h"

namespace libice {

// A TransportInfo is NOT a transport-info message.  It is comparable
// to a "ContentInfo". A transport-infos message is basically just a
// collection of TransportInfos.
struct TransportInfo {
	TransportInfo() = default;
	~TransportInfo() = default;

  TransportInfo(const std::string& content_name,
                const TransportDescription& description)
      : content_name(content_name), description(description) {}

  std::string content_name;
  TransportDescription description;
};

//typedef std::vector<TransportInfo> TransportInfos;

}  // namespace cricket

#endif  // P2P_BASE_TRANSPORT_INFO_H_
