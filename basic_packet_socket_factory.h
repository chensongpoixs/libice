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

#ifndef _C_P2P_BASE_BASIC_PACKET_SOCKET_FACTORY_H_
#define _C_P2P_BASE_BASIC_PACKET_SOCKET_FACTORY_H_

#include <string>

#include "api/packet_socket_factory.h"
#include "rtc_base/socket.h"

namespace libice {

class rtc::SocketFactory;

class BasicPacketSocketFactory : public rtc::PacketSocketFactory {
 public:
  explicit BasicPacketSocketFactory(rtc::SocketFactory* socket_factory);
  ~BasicPacketSocketFactory() override;

  rtc::AsyncPacketSocket* CreateUdpSocket(const rtc::SocketAddress& local_address,
                                     uint16_t min_port,
                                     uint16_t max_port) override;
  rtc::AsyncListenSocket* CreateServerTcpSocket(const rtc::SocketAddress& local_address,
                                           uint16_t min_port,
                                           uint16_t max_port,
                                           int opts) override;
  rtc::AsyncPacketSocket* CreateClientTcpSocket(
      const rtc::SocketAddress& local_address,
      const rtc::SocketAddress& remote_address,
      const rtc::ProxyInfo& proxy_info,
      const std::string& user_agent,
      const rtc::PacketSocketTcpOptions& tcp_options) override;

  rtc::AsyncResolverInterface* CreateAsyncResolver() override;

 private:
  int BindSocket(rtc::Socket* socket,
                 const rtc::SocketAddress& local_address,
                 uint16_t min_port,
                 uint16_t max_port);

  rtc::SocketFactory* socket_factory_;
};

}  // namespace rtc

#endif  // P2P_BASE_BASIC_PACKET_SOCKET_FACTORY_H_
