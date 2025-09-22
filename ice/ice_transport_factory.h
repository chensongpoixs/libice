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



#ifndef _C_API_ICE_TRANSPORT_FACTORY_H_
#define _C_API_ICE_TRANSPORT_FACTORY_H_

#include "ice/ice_transport_interface.h"
#include "api/scoped_refptr.h"
#include "rtc_base/system/rtc_export.h"

namespace libice {
class PortAllocator;
}  // namespace cricket

namespace libice {

// Static factory for an IceTransport object that can be created
// without using a webrtc::PeerConnection.
// The returned object must be accessed and destroyed on the thread that
// created it.
// The PortAllocator must outlive the created IceTransportInterface object.
// TODO(steveanton): Remove in favor of the overload that takes
// IceTransportInit.
RTC_EXPORT rtc::scoped_refptr<IceTransportInterface> CreateIceTransport(
    libice::PortAllocator* port_allocator);

// Static factory for an IceTransport object that can be created
// without using a webrtc::PeerConnection.
// The returned object must be accessed and destroyed on the thread that
// created it.
// `init.port_allocator()` is required and must outlive the created
//     IceTransportInterface object.
// `init.async_resolver_factory()` and `init.event_log()` are optional, but if
//     provided must outlive the created IceTransportInterface object.
RTC_EXPORT rtc::scoped_refptr<IceTransportInterface> CreateIceTransport(
    IceTransportInit);

}  // namespace webrtc

#endif  // API_ICE_TRANSPORT_FACTORY_H_
