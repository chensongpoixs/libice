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
				   date:  2025-09-17



 ******************************************************************************/




#include <iostream>
#include <memory>

#include "api/ice_transport_factory.h"
#include "p2p/base/default_ice_transport_factory.h"
#include "p2p/base/basic_async_resolver_factory.h"
#include "p2p/base/basic_packet_socket_factory.h"
#include "p2p/client/basic_port_allocator.h"
std::unique_ptr<webrtc::IceTransportFactory> ice_transport_factory;
std::unique_ptr<webrtc::AsyncResolverFactory> async_resolver_factory;
std::unique_ptr<cricket::PortAllocator> allocator;
std::unique_ptr<webrtc::AsyncDnsResolverFactoryInterface> async_dns_resolver_factory;



///////////////////////////////


std::unique_ptr<rtc::BasicNetworkManager> default_network_manager_;
std::unique_ptr<rtc::NetworkMonitorFactory> network_monitor_factory;
std::unique_ptr<rtc::BasicPacketSocketFactory> default_socket_factory_;



rtc::Thread * network_thread;
int main(int argc, char *argv[])
{
	ice_transport_factory = std::make_unique<webrtc::DefaultIceTransportFactory>();;

	async_dns_resolver_factory = std::make_unique<webrtc::WrappingAsyncDnsResolverFactory>(
		std::make_unique<webrtc::BasicAsyncResolverFactory>());



	///////////////////////////////////////////////////////////////////////////////////////////
	// If network_monitor_factory_ is non-null, it will be used to create a
  // network monitor while on the network thread.
	default_network_manager_ = std::make_unique<rtc::BasicNetworkManager>(
		network_monitor_factory.get(), network_thread->socketserver());

	default_socket_factory_ = std::make_unique<rtc::BasicPacketSocketFactory>(
		network_thread->socketserver());



	allocator = std::make_unique<cricket::BasicPortAllocator>(
		default_network_manager_.get(), default_socket_factory_.get(),
		nullptr, nullptr);


	/////////////////////////////////////////////////////////////
	webrtc::IceTransportInit init;
	init.set_port_allocator(allocator.get());
	init.set_async_dns_resolver_factory(async_dns_resolver_factory.get());
	//init.set_event_log(config_.event_log);
	rtc::scoped_refptr<webrtc::IceTransportInterface>  ice_controller =     ice_transport_factory->CreateIceTransport("audio", 0, std::move(init));


	return EXIT_SUCCESS;
}