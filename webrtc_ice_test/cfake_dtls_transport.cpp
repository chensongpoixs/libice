
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


#include "webrtc_ice_test/cfake_ice_define.h"
#include "webrtc_ice_test/cfake_dtls_transport.h"

namespace libice {

	FakeDtlsTransport::FakeDtlsTransport(FakeIceTransport* ice_transport)
		: ice_transport_(ice_transport),
		transport_name_(ice_transport->transport_name()),
		component_(ice_transport->component()),
		dtls_fingerprint_("", nullptr) {
		RTC_DCHECK(ice_transport_);
		ice_transport_->SignalReadPacket.connect(
			this, &FakeDtlsTransport::OnIceTransportReadPacket);
		ice_transport_->SignalNetworkRouteChanged.connect(
			this, &FakeDtlsTransport::OnNetworkRouteChanged);
	}

	FakeDtlsTransport::FakeDtlsTransport(std::unique_ptr<FakeIceTransport> ice)
		: owned_ice_transport_(std::move(ice)),
		transport_name_(owned_ice_transport_->transport_name()),
		component_(owned_ice_transport_->component()),
		dtls_fingerprint_("", rtc::ArrayView<const uint8_t>()) {
		ice_transport_ = owned_ice_transport_.get();
		ice_transport_->SignalReadPacket.connect(
			this, &FakeDtlsTransport::OnIceTransportReadPacket);
		ice_transport_->SignalNetworkRouteChanged.connect(
			this, &FakeDtlsTransport::OnNetworkRouteChanged);
	}

	// If this constructor is called, a new fake ICE transport will be created,
	// and this FakeDtlsTransport will take the ownership.
	FakeDtlsTransport::FakeDtlsTransport(const std::string& name, int component)
		: FakeDtlsTransport(std::make_unique<FakeIceTransport>(name, component)) {
	}
	FakeDtlsTransport::FakeDtlsTransport(const std::string& name,
		int component,
		rtc::Thread* network_thread)
		: FakeDtlsTransport(std::make_unique<FakeIceTransport>(name,
			component,
			network_thread)) {}

	FakeDtlsTransport::~FakeDtlsTransport() {
		if (dest_ && dest_->dest_ == this) {
			dest_->dest_ = nullptr;
		}
	}

	// Get inner fake ICE transport.
	FakeIceTransport* FakeDtlsTransport::fake_ice_transport()
	{ 
		return ice_transport_;
	}

	// If async, will send packets by "Post"-ing to message queue instead of
	// synchronously "Send"-ing.
	void FakeDtlsTransport::SetAsync(bool async) { ice_transport_->SetAsync(async); }
	void FakeDtlsTransport::SetAsyncDelay(int delay_ms) { ice_transport_->SetAsyncDelay(delay_ms); }

	// SetWritable, SetReceiving and SetDestination are the main methods that can
	// be used for testing, to simulate connectivity or lack thereof.
	void FakeDtlsTransport::SetWritable(bool writable) {
		ice_transport_->SetWritable(writable);
		set_writable(writable);
	}
	void FakeDtlsTransport::SetReceiving(bool receiving) {
		ice_transport_->SetReceiving(receiving);
		set_receiving(receiving);
	}
	void FakeDtlsTransport::SetDtlsState(webrtc::DtlsTransportState state) {
		dtls_state_ = state;
		SendDtlsState(this, dtls_state_);
	}

	// Simulates the two DTLS transports connecting to each other.
	// If `asymmetric` is true this method only affects this FakeDtlsTransport.
	// If false, it affects `dest` as well.
	void FakeDtlsTransport::SetDestination(FakeDtlsTransport* dest, bool asymmetric) {
		if (dest == dest_) {
			return;
		}
		RTC_DCHECK(!dest || !dest_)
			<< "Changing fake destination from one to another is not supported.";
		if (dest && !dest_) {
			// This simulates the DTLS handshake.
			dest_ = dest;
			if (local_cert_ && dest_->local_cert_) {
				do_dtls_ = true;
				RTC_LOG(LS_INFO) << "FakeDtlsTransport is doing DTLS";
			}
			else {
				do_dtls_ = false;
				RTC_LOG(LS_INFO) << "FakeDtlsTransport is not doing DTLS";
			}
			SetWritable(true);
			if (!asymmetric) {
				dest->SetDestination(this, true);
			}
			// If the `dtls_role_` is unset, set it to SSL_CLIENT by default.
			if (!dtls_role_) {
				dtls_role_ = std::move(rtc::SSL_CLIENT);
			}
			SetDtlsState(webrtc::DtlsTransportState::kConnected);
			ice_transport_->SetDestination(
				static_cast<FakeIceTransport*>(dest->ice_transport()), asymmetric);
		}
		else {
			// Simulates loss of connectivity, by asymmetrically forgetting dest_.
			dest_ = nullptr;
			SetWritable(false);
			ice_transport_->SetDestination(nullptr, asymmetric);
		}
	}

	// Fake DtlsTransportInternal implementation.
	webrtc::DtlsTransportState FakeDtlsTransport::dtls_state() const { return dtls_state_; }
	const std::string& FakeDtlsTransport::transport_name() const { return transport_name_; }
	int FakeDtlsTransport::component() const { return component_; }

	const rtc::SSLFingerprint & FakeDtlsTransport::dtls_fingerprint()
	{
		// TODO: insert return statement here
		return dtls_fingerprint_;
	}



	bool FakeDtlsTransport::SetRemoteFingerprint(const std::string& alg,
		const uint8_t* digest,
		size_t digest_len) {
		dtls_fingerprint_ =
			rtc::SSLFingerprint(alg, rtc::MakeArrayView(digest, digest_len));
		return true;
	}
	bool FakeDtlsTransport::SetDtlsRole(rtc::SSLRole role) {
		dtls_role_ = std::move(role);
		return true;
	}
	bool FakeDtlsTransport::GetDtlsRole(rtc::SSLRole* role) const {
		if (!dtls_role_) {
			return false;
		}
		*role = *dtls_role_;
		return true;
	}
	bool FakeDtlsTransport::SetLocalCertificate(
		const rtc::scoped_refptr<rtc::RTCCertificate>& certificate) {
		do_dtls_ = true;
		local_cert_ = certificate;
		return true;
	}
	void FakeDtlsTransport::SetRemoteSSLCertificate(rtc::FakeSSLCertificate* cert) {
		remote_cert_ = cert;
	}
	bool FakeDtlsTransport::IsDtlsActive() const { return do_dtls_; }
	bool FakeDtlsTransport::GetSslVersionBytes(int * version) const
	{
		if (!do_dtls_) {
			return false;
		}
		*version = 0x0102;
		return true;
	}

	bool FakeDtlsTransport::GetSrtpCryptoSuite(int* crypto_suite) {
		if (!do_dtls_) {
			return false;
		}
		*crypto_suite = crypto_suite_;
		return true;
	}
	void FakeDtlsTransport::SetSrtpCryptoSuite(int crypto_suite) { crypto_suite_ = crypto_suite; }

	bool FakeDtlsTransport::GetSslCipherSuite(int* cipher_suite) {
		if (ssl_cipher_suite_) {
			*cipher_suite = *ssl_cipher_suite_;
			return true;
		}
		return false;
	}
	void FakeDtlsTransport::SetSslCipherSuite(absl::optional<int> cipher_suite) {
		ssl_cipher_suite_ = cipher_suite;
	}
	rtc::scoped_refptr<rtc::RTCCertificate> FakeDtlsTransport::GetLocalCertificate() const {
		return local_cert_;
	}
	std::unique_ptr<rtc::SSLCertChain> FakeDtlsTransport::GetRemoteSSLCertChain() const {
		if (!remote_cert_) {
			return nullptr;
		}
		return std::make_unique<rtc::SSLCertChain>(remote_cert_->Clone());
	}
	bool FakeDtlsTransport::ExportKeyingMaterial(const std::string& label,
		const uint8_t* context,
		size_t context_len,
		bool use_context,
		uint8_t* result,
		size_t result_len) {
		if (!do_dtls_) {
			return false;
		}
		memset(result, 0xff, result_len);
		return true;
	}
	void FakeDtlsTransport::set_ssl_max_protocol_version(rtc::SSLProtocolVersion version) {
		ssl_max_version_ = version;
	}
	rtc::SSLProtocolVersion FakeDtlsTransport::ssl_max_protocol_version() const {
		return ssl_max_version_;
	}

	cricket::IceTransportInternal* FakeDtlsTransport::ice_transport() { return ice_transport_; }

	// PacketTransportInternal implementation, which passes through to fake ICE
	// transport for sending actual packets.
	bool FakeDtlsTransport::writable() const { return writable_; }
	bool FakeDtlsTransport::receiving() const { return receiving_; }
	int FakeDtlsTransport::SendPacket(const char* data,
		size_t len,
		const rtc::PacketOptions& options,
		int flags) {
		// We expect only SRTP packets to be sent through this interface.
		if (flags != cricket::PF_SRTP_BYPASS && flags != 0) {
			return -1;
		}
		return ice_transport_->SendPacket(data, len, options, flags);
	}
	int FakeDtlsTransport::SetOption(rtc::Socket::Option opt, int value) {
		return ice_transport_->SetOption(opt, value);
	}
	bool FakeDtlsTransport::GetOption(rtc::Socket::Option opt, int* value) {
		return ice_transport_->GetOption(opt, value);
	}
	int FakeDtlsTransport::GetError() { return ice_transport_->GetError(); }

	absl::optional<rtc::NetworkRoute> FakeDtlsTransport::network_route() const {
		return ice_transport_->network_route();
	}

	void FakeDtlsTransport::OnIceTransportReadPacket(PacketTransportInternal* ice_,
		const char* data,
		size_t len,
		const int64_t& packet_time_us,
		int flags) {
		SignalReadPacket(this, data, len, packet_time_us, flags);
	}

	void FakeDtlsTransport::set_receiving(bool receiving) {
		if (receiving_ == receiving) {
			return;
		}
		receiving_ = receiving;
		SignalReceivingState(this);
	}

	void FakeDtlsTransport::set_writable(bool writable) {
		if (writable_ == writable) {
			return;
		}
		writable_ = writable;
		if (writable_) {
			SignalReadyToSend(this);
		}
		SignalWritableState(this);
	}

	void FakeDtlsTransport::OnNetworkRouteChanged(absl::optional<rtc::NetworkRoute> network_route) {
		SignalNetworkRouteChanged(network_route);
	}
}