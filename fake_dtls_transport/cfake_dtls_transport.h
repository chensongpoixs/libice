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
				   date:  2025-09-19



 ******************************************************************************/
#ifndef  __C_FAKE_DTLS_TRANSPORT_H_
#define  __C_FAKE_DTLS_TRANSPORT_H_



#include <iostream>
#include <memory>
#include "absl/types/optional.h"
#include "api/ice_transport_factory.h"
#include "p2p/base/default_ice_transport_factory.h"
#include "p2p/base/basic_async_resolver_factory.h"
#include "p2p/base/basic_packet_socket_factory.h"
#include "p2p/client/basic_port_allocator.h"
#include "pc/session_description.h"
#include "p2p/base/dtls_transport_factory.h"
#include "p2p/base/dtls_transport.h"
#include "absl/algorithm/container.h"
#include "rtc_base/task_queue.h"
#include "absl/algorithm/algorithm.h"
#include "rtc_base/fake_ssl_identity.h"
#include "pc/jsep_transport_controller.h"
#include "fake_ice_transport/cfake_ice_define.h"
#include "fake_ice_transport/cfake_ice_transport.h"


namespace libice {

	//class FakeIceTransport;
	class FakeDtlsTransport : public cricket::DtlsTransportInternal {
	public:
		explicit FakeDtlsTransport(FakeIceTransport* ice_transport);

		explicit FakeDtlsTransport(std::unique_ptr<FakeIceTransport> ice);

		// If this constructor is called, a new fake ICE transport will be created,
		// and this FakeDtlsTransport will take the ownership.
		FakeDtlsTransport(const std::string& name, int component);
		FakeDtlsTransport(const std::string& name,
			int component,
			rtc::Thread* network_thread);

		~FakeDtlsTransport() override;

		// Get inner fake ICE transport.
		FakeIceTransport* fake_ice_transport();

		// If async, will send packets by "Post"-ing to message queue instead of
		// synchronously "Send"-ing.
		void SetAsync(bool async);
		void SetAsyncDelay(int delay_ms);

		// SetWritable, SetReceiving and SetDestination are the main methods that can
		// be used for testing, to simulate connectivity or lack thereof.
		void SetWritable(bool writable);
		void SetReceiving(bool receiving);
		void SetDtlsState(webrtc::DtlsTransportState state);

		// Simulates the two DTLS transports connecting to each other.
		// If `asymmetric` is true this method only affects this FakeDtlsTransport.
		// If false, it affects `dest` as well.
		void SetDestination(FakeDtlsTransport* dest, bool asymmetric = false);

		// Fake DtlsTransportInternal implementation.
		webrtc::DtlsTransportState dtls_state() const override;
		const std::string& transport_name() const override;
		int component() const override;
		const rtc::SSLFingerprint& dtls_fingerprint();
		bool SetRemoteFingerprint(const std::string& alg,
			const uint8_t* digest,
			size_t digest_len) override;
		bool SetDtlsRole(rtc::SSLRole role) override;
		bool GetDtlsRole(rtc::SSLRole* role) const override;
		bool SetLocalCertificate(
			const rtc::scoped_refptr<rtc::RTCCertificate>& certificate) override;
		void SetRemoteSSLCertificate(rtc::FakeSSLCertificate* cert);
		bool IsDtlsActive() const override;
		bool GetSslVersionBytes(int* version)  const;
		bool GetSrtpCryptoSuite(int* crypto_suite);
		void SetSrtpCryptoSuite(int crypto_suite);

		bool GetSslCipherSuite(int* cipher_suite);
		void SetSslCipherSuite(absl::optional<int> cipher_suite);
		rtc::scoped_refptr<rtc::RTCCertificate> GetLocalCertificate() const override;
		std::unique_ptr<rtc::SSLCertChain> GetRemoteSSLCertChain() const override;
		bool ExportKeyingMaterial(const std::string& label,
			const uint8_t* context,
			size_t context_len,
			bool use_context,
			uint8_t* result,
			size_t result_len) override;
		void set_ssl_max_protocol_version(rtc::SSLProtocolVersion version);
		rtc::SSLProtocolVersion ssl_max_protocol_version() const;

		cricket::IceTransportInternal* ice_transport() override;

		// PacketTransportInternal implementation, which passes through to fake ICE
		// transport for sending actual packets.
		bool writable() const override;
		bool receiving() const override;
		int SendPacket(const char* data,
			size_t len,
			const rtc::PacketOptions& options,
			int flags) override;
		int SetOption(rtc::Socket::Option opt, int value);
		bool GetOption(rtc::Socket::Option opt, int* value);
		int GetError() override;

		absl::optional<rtc::NetworkRoute> network_route() const override;

	private:
		void OnIceTransportReadPacket(PacketTransportInternal* ice_,
			const char* data,
			size_t len,
			const int64_t& packet_time_us,
			int flags);

		void set_receiving(bool receiving);

		void set_writable(bool writable);

		void OnNetworkRouteChanged(absl::optional<rtc::NetworkRoute> network_route);

		FakeIceTransport* ice_transport_;
		std::unique_ptr<FakeIceTransport> owned_ice_transport_;
		std::string transport_name_;
		int component_;
		FakeDtlsTransport* dest_ = nullptr;
		rtc::scoped_refptr<rtc::RTCCertificate> local_cert_;
		rtc::FakeSSLCertificate* remote_cert_ = nullptr;
		bool do_dtls_ = false;
		rtc::SSLProtocolVersion ssl_max_version_ = rtc::SSL_PROTOCOL_DTLS_12;
		rtc::SSLFingerprint dtls_fingerprint_;
		absl::optional<rtc::SSLRole> dtls_role_;
		int crypto_suite_ = rtc::kSrtpAes128CmSha1_80;
		absl::optional<int> ssl_cipher_suite_;

		webrtc::DtlsTransportState dtls_state_ = webrtc::DtlsTransportState::kNew;

		bool receiving_ = false;
		bool writable_ = false;
	};



}


#endif // __C_FAKE_DTLS_TRANSPORT_H_