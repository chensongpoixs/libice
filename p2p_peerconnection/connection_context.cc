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


#include "p2p_peerconnection/connection_context.h"

#include <string>
#include <type_traits>
#include <utility>

#include "api/transport/field_trial_based_config.h"
#include "media/sctp/sctp_transport_factory.h"
#include "rtc_base/helpers.h"
#include "rtc_base/task_utils/to_queued_task.h"
#include "rtc_base/time_utils.h"

namespace libice {

namespace {

rtc::Thread* MaybeStartThread(rtc::Thread* old_thread,
                              const std::string& thread_name,
                              bool with_socket_server,
                              std::unique_ptr<rtc::Thread>& thread_holder) {
  if (old_thread) {
    return old_thread;
  }
  if (with_socket_server) {
    thread_holder = rtc::Thread::CreateWithSocketServer();
  } else {
    thread_holder = rtc::Thread::Create();
  }
  thread_holder->SetName(thread_name, nullptr);
  thread_holder->Start();
  return thread_holder.get();
}

rtc::Thread* MaybeWrapThread(rtc::Thread* signaling_thread,
                             bool& wraps_current_thread) {
  wraps_current_thread = false;
  if (signaling_thread) {
    return signaling_thread;
  }
  auto this_thread = rtc::Thread::Current();
  if (!this_thread) {
    // If this thread isn't already wrapped by an rtc::Thread, create a
    // wrapper and own it in this class.
    this_thread = rtc::ThreadManager::Instance()->WrapCurrentThread();
    wraps_current_thread = true;
  }
  return this_thread;
}
 

}  // namespace

// Static
rtc::scoped_refptr<ConnectionContext> ConnectionContext::Create() {
  return new ConnectionContext();
}

ConnectionContext::ConnectionContext()
    : network_thread_(MaybeStartThread(nullptr,
                                       "pc_network_thread",
                                       true,
                                       owned_network_thread_)),
      worker_thread_(MaybeStartThread(nullptr,
                                      "pc_worker_thread",
                                      false,
                                      owned_worker_thread_)),
      signaling_thread_(MaybeStartThread(nullptr, "pc_signalig_thread", false, owned_signaling_thread_)),
      network_monitor_factory_( std::move(nullptr ))
{

	 
	if (network_thread_->IsCurrent())
	{
		RTC_DCHECK_RUN_ON(signaling_thread_);
		rtc::InitRandom(rtc::Time32());

		// If network_monitor_factory_ is non-null, it will be used to create a
		// network monitor while on the network thread.
		default_network_manager_ = std::make_unique<rtc::BasicNetworkManager>(
			nullptr, network_thread()->socketserver());

		// TODO(bugs.webrtc.org/13145): Either require that a PacketSocketFactory
		// always is injected (with no need to construct this default factory), or get
		// the appropriate underlying SocketFactory without going through the
		// rtc::Thread::socketserver() accessor.
		default_socket_factory_ = std::make_unique<libice::BasicPacketSocketFactory>(
			network_thread()->socketserver());
	}
	else
	{

		network_thread_->PostTask(RTC_FROM_HERE, [this]() {
			RTC_DCHECK_RUN_ON(network_thread_);
			rtc::InitRandom(rtc::Time32());

			// If network_monitor_factory_ is non-null, it will be used to create a
			// network monitor while on the network thread.
			default_network_manager_ = std::make_unique<rtc::BasicNetworkManager>(
				nullptr, network_thread()->socketserver());

			// TODO(bugs.webrtc.org/13145): Either require that a PacketSocketFactory
			// always is injected (with no need to construct this default factory), or get
			// the appropriate underlying SocketFactory without going through the
			// rtc::Thread::socketserver() accessor.
			default_socket_factory_ = std::make_unique<libice::BasicPacketSocketFactory>(
				network_thread()->socketserver());

		});
	}
	

	//worker_thread_->Invoke<void>(RTC_FROM_HERE, [&]() {
	//	channel_manager_ = cricket::ChannelManager::Create(
	//		std::move(dependencies->media_engine),
	//		/*enable_rtx=*/true, worker_thread(), network_thread());
	//});

	// Set warning levels on the threads, to give warnings when response
	// may be slower than is expected of the thread.
	// Since some of the threads may be the same, start with the least
	// restrictive limits and end with the least permissive ones.
	// This will give warnings for all cases.
	//signaling_thread_->SetDispatchWarningMs(100);
	//worker_thread_->SetDispatchWarningMs(30);
	//network_thread_->SetDispatchWarningMs(10);
 

 
}

ConnectionContext::~ConnectionContext() {
  //RTC_DCHECK_RUN_ON(signaling_thread_);
  worker_thread_->Invoke<void>(RTC_FROM_HERE,
                               [&]() { /*channel_manager_.reset(nullptr);*/ });

  // Make sure `worker_thread()` and `signaling_thread()` outlive
  // `default_socket_factory_` and `default_network_manager_`.
  default_socket_factory_ = nullptr;
  default_network_manager_ = nullptr;

  //if (wraps_current_thread_)
  //  rtc::ThreadManager::Instance()->UnwrapCurrentThread();
  RTC_LOG(LS_INFO) << "context  free ....";
 /* network_thread_->Stop();
  worker_thread_->Stop();
  signaling_thread_->Stop();*/
  RTC_LOG(LS_INFO) << "context work thread exit ok !!!!";
}

//cricket::ChannelManager* ConnectionContext::channel_manager() const {
//  return channel_manager_.get();
//}

}  // namespace webrtc
