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



#ifndef _C_TRANSPORT_CONNECTIONER_H_
#define _C_TRANSPORT_CONNECTIONER_H_
#include "rtc_base/third_party/sigslot/sigslot.h"
#include "p2p_peerconnection/csession_description.h"
#include "ice/ice_transport_interface.h"
#include "ice/dtls_transport.h"
#include "p2p_peerconnection/connection_context.h"
#include "ice/dtls_transport_internal.h"
namespace libice
{
	class transport_controller : public sigslot::has_slots<>
	{
	public:
		transport_controller(rtc::scoped_refptr<libice::ConnectionContext>   context);
		virtual ~transport_controller();

		 
	public:

		int  set_remote_sdp(SessionDescription * desc);
		int  set_local_sdp(SessionDescription * desc);



		int set_remote_candidate(const libice::Candidate& candidate);

		int  send_rtp_packet(const std::string & transport_name, const char * data, size_t len);
		int  send_rtcp_packet(const std::string& transport_name, const char * data, size_t len);



	public:
		rtc::scoped_refptr<libice::IceTransportInterface> CreateIceTransport(
			const std::string& transport_name, bool rtcp);

		std::shared_ptr<libice::DtlsTransportInternal> CreateDtlsTransport(
			libice::ContentInfo* content_info,
			libice::IceTransportInternal* ice);


	public:
		// dtls  callback

		void  OnTransportWritableState_n(
			libice::PacketTransportInternal* transport);

		void  OnTransportReceivingState_n(
			libice::PacketTransportInternal* transport);

		void  OnTransportGatheringState_n(
			libice::IceTransportInternal* transport);

		void  OnTransportCandidateGathered_n(
			libice::IceTransportInternal* transport,
			const libice::Candidate& candidate);

		void OnTransportCandidateError_n(
			libice::IceTransportInternal* transport,
			const libice::IceCandidateErrorEvent& event);
		void OnTransportCandidatesRemoved_n(
			libice::IceTransportInternal* transport,
			const libice::Candidates& candidates);
		void   OnTransportCandidatePairChanged_n(
			const libice::CandidatePairChangeEvent& event);




		void   OnTransportRoleConflict_n(
			libice::IceTransportInternal* transport);

		void   OnTransportStateChanged_n(
			libice::IceTransportInternal* transport);


		void  UpdateAggregateStates_n();

		void OnDtlsHandshakeError(rtc::SSLHandshakeError error);
	private:

		void on_ice_stae();
		void on_read_packet(const char * data, size_t len, int64_t ts);
		//void on_ice_dtls_state(libice::IceDtlsTransportState ice_state);

	private:
		rtc::scoped_refptr<libice::ConnectionContext>   context_;
		std::unique_ptr<webrtc::AsyncDnsResolverFactoryInterface> async_dns_resolver_factory_;
		std::shared_ptr < libice::PortAllocator>    port_allocator_ = nullptr;
		std::unique_ptr<libice::IceTransportFactory>  ice_transport_factory_ = nullptr;
		webrtc::CryptoOptions  crypto_options_;
		libice::IceRole ice_role_ = libice::ICEROLE_CONTROLLING;
		uint64_t ice_tiebreaker_ = rtc::CreateRandomId64();
		rtc::scoped_refptr<rtc::RTCCertificate> certificate_;
		libice::IceConfig ice_config_;

		std::map<std::string, rtc::scoped_refptr<libice::IceTransportInterface>>  ices_;

		std::map<std::string, std::shared_ptr< libice::DtlsTransportInternal>>   dtls_transports_;
	};

}

#endif //   _C_PC_SESSION_DESCRIPTION_H_
 
