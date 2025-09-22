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



#ifndef _C_PC_CONNECTION_CONTEXT_H_
#define _C_PC_CONNECTION_CONTEXT_H_

#include <memory>
#include <string>

#include "api/call/call_factory_interface.h"
#include "api/media_stream_interface.h"
#include "api/peer_connection_interface.h"
#include "api/ref_counted_base.h"
#include "api/scoped_refptr.h"
#include "api/sequence_checker.h"
#include "api/transport/sctp_transport_factory_interface.h"
#include "api/transport/webrtc_key_value_config.h"
#include "media/base/media_engine.h"
#include "ice/basic_packet_socket_factory.h"
#include "pc/channel_manager.h"
#include "rtc_base/checks.h"
#include "rtc_base/network.h"
#include "rtc_base/network_monitor_factory.h"
#include "rtc_base/rtc_certificate_generator.h"
#include "rtc_base/thread.h"
#include "rtc_base/thread_annotations.h"

namespace rtc {
class BasicNetworkManager;
//class BasicPacketSocketFactory;
}  // namespace rtc

namespace libice {

class RtcEventLog;

 
class ConnectionContext  
    : public rtc::RefCountedNonVirtual<ConnectionContext> {
 public:
  
  static rtc::scoped_refptr<ConnectionContext> Create();
   
  ConnectionContext(const ConnectionContext&) = delete;
  ConnectionContext& operator=(const ConnectionContext&) = delete;

  

  cricket::ChannelManager* channel_manager() const;

  rtc::Thread* signaling_thread() { return signaling_thread_; }
  const rtc::Thread* signaling_thread() const { return signaling_thread_; }
  rtc::Thread* worker_thread() { return worker_thread_; }
  const rtc::Thread* worker_thread() const { return worker_thread_; }
  rtc::Thread* network_thread() { return network_thread_; }
  const rtc::Thread* network_thread() const { return network_thread_; }

 
  rtc::BasicNetworkManager* default_network_manager() {
   // RTC_DCHECK_RUN_ON(signaling_thread_);
    return default_network_manager_.get();
  }
  libice::BasicPacketSocketFactory* default_socket_factory() {
    //RTC_DCHECK_RUN_ON(signaling_thread_);
    return default_socket_factory_.get();
  }
  
 protected:
  explicit ConnectionContext();

  friend class rtc::RefCountedNonVirtual<ConnectionContext>;
  ~ConnectionContext();

 private:

  std::unique_ptr<rtc::Thread> owned_network_thread_;
  std::unique_ptr<rtc::Thread> owned_worker_thread_;
  std::unique_ptr<rtc::Thread> owned_signaling_thread_ ;
  rtc::Thread* const network_thread_;
  rtc::Thread* const worker_thread_;
  rtc::Thread* const signaling_thread_; 
  std::unique_ptr<rtc::NetworkMonitorFactory> const network_monitor_factory_ ;
  std::unique_ptr<rtc::BasicNetworkManager> default_network_manager_ ; 

  std::unique_ptr<libice::BasicPacketSocketFactory> default_socket_factory_ ; 
  webrtc::ScopedTaskSafety signaling_thread_safety_;
};

}  // namespace webrtc

#endif  // PC_CONNECTION_CONTEXT_H_
