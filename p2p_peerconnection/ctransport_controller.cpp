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



#include "p2p_peerconnection/ctransport_controller.h"
#include "rtc_base/third_party/sigslot/sigslot.h"
#include "ice/basic_async_resolver_factory.h"
#include "ice/ice_transport_factory.h"
#include "ice/basic_port_allocator.h"
#include "ice/default_ice_transport_factory.h"
#include "p2p_peerconnection/connection_context.h"
#include "ice/ice_credentials_iterator.h"
namespace libice
{
	transport_controller::transport_controller(  rtc::Thread*   t,   rtc::Thread* s
		, rtc::BasicNetworkManager* default_network_manager,
		libice::BasicPacketSocketFactory* default_socket_factory)
		: network_thread_(t)
		, signalie_thread_(s)
		, async_dns_resolver_factory_(std::make_unique<libice::WrappingAsyncDnsResolverFactory>(
            std::make_unique<libice::BasicAsyncResolverFactory>()))
		, ice_transport_factory_(std::make_unique<libice::DefaultIceTransportFactory>())
		, crypto_options_()
		, ices_()
		, dtls_transports_()
	{
		 
		if (network_thread_->IsCurrent())
		{
			port_allocator_ = std::make_shared<libice::BasicPortAllocator>(
				default_network_manager, default_socket_factory,
				nullptr);
			port_allocator_->Initialize();
		}
		else
		{
			network_thread_->PostTask(webrtc::ToQueuedTask(signaling_thread_safety_.flag(), [this, default_network_manager, default_socket_factory]() {
				RTC_DCHECK_RUN_ON(network_thread_);
				port_allocator_ = std::make_shared<libice::BasicPortAllocator>(
					default_network_manager, default_socket_factory,
					nullptr);
				port_allocator_->Initialize();
			}));
		}
			
	 
		
		//BasicAsyncResolverFactory
	}
	transport_controller::~transport_controller()
	{
	}
	int transport_controller::set_remote_sdp(SessionDescription * desc)
	{
		if (!desc)
		{
			return -1;
		}
		//context_->network_thread()->Invoke<void>( RTC_FROM_HERE,[=](){
			for (size_t i = 0; i < desc->contents_.size(); ++i) {
				std::string mid = desc->contents_[i].name;
				//ContentInfo content = desc->contents_[i];
				RTC_LOG(LS_INFO) << desc->content_groups_[0].ToString();
				if (/*desc->HasGroup(mid) &&*/ mid != desc->contents_[0].name) {
					continue;
				}

			


			
					// 创建ICE transport
				// RTCP, 默认开启a=rtcp:mux
				//ice_agent_->CreateDtlsTransport(mid, 1); // 1: RTP
					
						

				auto  pi =  network_thread_->Invoke<std::pair<rtc::scoped_refptr<libice::IceTransportInterface>, std::shared_ptr<libice::DtlsTransportInternal> > >(RTC_FROM_HERE, [&] {
					rtc::scoped_refptr<libice::IceTransportInterface> ice = CreateIceTransport(desc->contents_[i].name, /*rtcp=*/false);
						//  ice 
						RTC_LOG(LS_INFO) << "create ice  name " << mid;
						

						std::shared_ptr<libice::DtlsTransportInternal> rtp_dtls_transport =
							CreateDtlsTransport(&desc->contents_[i], ice->internal());
						
						return std::make_pair(ice, rtp_dtls_transport);
					});


					
				rtc::scoped_refptr<libice::IceTransportInterface> ice = pi.first;
				std::shared_ptr<libice::DtlsTransportInternal> rtp_dtls_transport = pi.second;
				ices_.insert(std::make_pair(mid, ice));
				dtls_transports_.insert(std::make_pair(mid, rtp_dtls_transport));
				// 设置ICE param
				TransportInfo* td = desc->GetTransportInfoByName(mid);
				if (td) 
				{
					libice::IceParameters  remote_ice_parameter;
					remote_ice_parameter.pwd = td->description.ice_pwd;
					remote_ice_parameter.ufrag = td->description.ice_ufrag;
					//td->description.r
					//content.media_description()
				
					 

					 network_thread_->PostTask(ToQueuedTask(signaling_thread_safety_.flag(), [this, remote_ice_parameter, ice, rtp_dtls_transport, td]() {
						 RTC_DCHECK_RUN_ON(network_thread_);
						 ice->internal()->SetRemoteIceParameters(remote_ice_parameter);
						rtp_dtls_transport->SetRemoteFingerprint(td->description.identity_fingerprint->algorithm,
							td->description.identity_fingerprint->digest.cdata(),
							td->description.identity_fingerprint->digest.size()
						);
					}));
					//ice_agent_->SetRemoteIceParams(mid, 1, libice::IceParameters( td->ice_ufrag, td->ice_pwd));
					//if (td->identity_fingerprint)
					{
						/*	 std::string  fingerprint((char *)td->identity_fingerprint->digest.data()
							 , td->identity_fingerprint->digest.size());*/
						//ice_agent_->SetRemoteFingerprint(td->alg
						//	, td->fingerprint);

						//ice_agent_->SetIceParams
					}
					//auto dtls = _get_dtls_transport(mid);
					//if (dtls) {
					//	dtls->set_remote_fingerprint(td->identity_fingerprint->algorithm,
					//		td->identity_fingerprint->digest.cdata(),
					//		td->identity_fingerprint->digest.size());
					//}
					//ice_agent_->UseDtlsSrtp(mid);
				}

				// 设置ICE candidate
				/*for (auto candidate : content->candidates()) {
					ice_agent_->AddRemoteCandidate(mid, 1, candidate);
				}*/
			}
		//});
		return 0;
	}
	int transport_controller::set_local_sdp(SessionDescription * desc, rtc::scoped_refptr<rtc::RTCCertificate> certificate)
	{

		if (!desc) {
			return -1;
		}
		for (size_t i = 0; i < desc->contents_.size(); ++i) {
			std::string mid = desc->contents_[i].name;
			//ContentInfo content = desc->contents_[i];
			if (mid != desc->contents_[0].name/*desc->HasGroup(mid) && mid != desc->content_groups_[0].semantics_*/) {
				continue;
			}
			TransportInfo* td = desc->GetTransportInfoByName(mid);
			//auto td = desc->GetTransportInfo(mid);
			if (td) {
			/*	ice_agent_->SetIceParams(mid, 1,
					ice::IceParameters(td->ice_ufrag, td->ice_pwd));*/

				libice::IceParameters  local_ice_parameter;
				local_ice_parameter.pwd = td->description.ice_pwd;
				local_ice_parameter.ufrag = td->description.ice_ufrag;
				//td->description.r
				//content.media_description()

				network_thread_->PostTask(ToQueuedTask(signaling_thread_safety_.flag(), [mid, local_ice_parameter, this, certificate]() {
					RTC_DCHECK_RUN_ON(network_thread_);
					ices_[mid]->internal()->SetIceParameters(local_ice_parameter);
				//	auto ssl_fingerprint = rtc::SSLFingerprint::CreateFromRfc4572(alg, fingerprint);
					dtls_transports_[mid]->SetLocalCertificate(certificate);
				}));
				

				
			}
		}

		 
			if (network_thread_->IsCurrent())
			{
				//	context_->network_thread()->PostTask(ToQueuedTask(signaling_thread_safety_.flag(), [this]() {
				ices_["audio"]->internal()->MaybeStartGathering();
				//}));
			}
			else
			{
				network_thread_->PostTask(ToQueuedTask(signaling_thread_safety_.flag(), [this]() {
					RTC_DCHECK_RUN_ON(network_thread_);
					ices_["audio"]->internal()->MaybeStartGathering();
				}));
			}
		 
		
		
		
		return 0;
	}
	int transport_controller::set_remote_candidate(const libice::Candidate & candidate)
	{
		if (network_thread_->IsCurrent())
		{
			//	context_->network_thread()->PostTask(ToQueuedTask(signaling_thread_safety_.flag(), [this]() {
			ices_["audio"]->internal()->AddRemoteCandidate(candidate);
			//}));
		}
		else
		{
			network_thread_->PostTask(ToQueuedTask(signaling_thread_safety_.flag(), [this, candidate]() {
				RTC_DCHECK_RUN_ON(network_thread_);
				ices_["audio"]->internal()->AddRemoteCandidate(candidate);
			}));
		}

		
		return 0;
	}
	int transport_controller::send_rtp_packet(const std::string & transport_name, const char * data, size_t len)
	{
		return 0;
	}
	int transport_controller::send_rtcp_packet(const std::string & transport_name, const char * data, size_t len)
	{
		return 0;
	}
	rtc::scoped_refptr<libice::IceTransportInterface> transport_controller::CreateIceTransport(const std::string & transport_name, bool rtcp)
	{


		int component = rtcp ? libice::ICE_CANDIDATE_COMPONENT_RTCP
			: libice::ICE_CANDIDATE_COMPONENT_RTP;

		IceTransportInit init;
		init.set_port_allocator(port_allocator_.get());
		init.set_async_dns_resolver_factory(async_dns_resolver_factory_.get());
		//init.set_event_log(config_.event_log);
		return ice_transport_factory_->CreateIceTransport(
			transport_name, component, std::move(init));
		//return rtc::scoped_refptr<libice::IceTransportInterface>();
	}
	std::shared_ptr<libice::DtlsTransportInternal> transport_controller::CreateDtlsTransport(
		 libice::ContentInfo * content_info, libice::IceTransportInternal * ice)
	{
	//	RTC_DCHECK_RUN_ON(context_->signaling_thread());


		std::shared_ptr<libice::DtlsTransportInternal> dtls;

		
		dtls = std::make_shared<libice::DtlsTransport>(ice, crypto_options_,
				nullptr,
			rtc::SSL_PROTOCOL_DTLS_12/*config_.ssl_max_version*/);
		

		RTC_DCHECK(dtls);
		dtls->ice_transport()->SetIceRole(ice_role_);
		dtls->ice_transport()->SetIceTiebreaker(ice_tiebreaker_);
		dtls->ice_transport()->SetIceConfig(ice_config_);
		if (certificate_) {
			bool set_cert_success = dtls->SetLocalCertificate(certificate_);
			RTC_DCHECK(set_cert_success);
		}

		// Connect to signals offered by the DTLS and ICE transport.
		dtls->SignalWritableState.connect(
			this, &transport_controller::OnTransportWritableState_n);
		dtls->SignalReceivingState.connect(
			this, &transport_controller::OnTransportReceivingState_n);
		dtls->ice_transport()->SignalGatheringState.connect(
			this, &transport_controller::OnTransportGatheringState_n);
		dtls->ice_transport()->SignalCandidateGathered.connect(
			this, &transport_controller::OnTransportCandidateGathered_n);
		dtls->ice_transport()->SignalCandidateError.connect(
			this, &transport_controller::OnTransportCandidateError_n);
		dtls->ice_transport()->SignalCandidatesRemoved.connect(
			this, &transport_controller::OnTransportCandidatesRemoved_n);
		dtls->ice_transport()->SignalRoleConflict.connect(
			this, &transport_controller::OnTransportRoleConflict_n);
		dtls->ice_transport()->SignalStateChanged.connect(
			this, &transport_controller::OnTransportStateChanged_n);
		dtls->ice_transport()->SignalIceTransportStateChanged.connect(
			this, &transport_controller::OnTransportStateChanged_n);
		dtls->ice_transport()->SignalCandidatePairChanged.connect(
			this, &transport_controller::OnTransportCandidatePairChanged_n);

		dtls->SubscribeDtlsHandshakeError(
			[this](rtc::SSLHandshakeError error) { OnDtlsHandshakeError(error); });


		return dtls;
		return std::unique_ptr<libice::DtlsTransportInternal>();
	}
	void transport_controller::on_ice_stae()
	{
	}
	void transport_controller::on_read_packet(const char * data, size_t len, int64_t ts)
	{
	}




	void transport_controller::OnTransportWritableState_n(
		libice::PacketTransportInternal* transport) {
		RTC_LOG(LS_INFO) << " Transport " << transport->transport_name()
			<< " writability changed to " << transport->writable()
			<< ".";
		UpdateAggregateStates_n();
	}

	void transport_controller::OnTransportReceivingState_n(
		libice::PacketTransportInternal* transport) {
		UpdateAggregateStates_n();
	}

	void transport_controller::OnTransportGatheringState_n(
		libice::IceTransportInternal* transport) {
		UpdateAggregateStates_n();
	}

	void transport_controller::OnTransportCandidateGathered_n(
		libice::IceTransportInternal* transport,
		const libice::Candidate& candidate) {
		// We should never signal peer-reflexive candidates.
		if (candidate.type() == libice::PRFLX_PORT_TYPE) {
			RTC_NOTREACHED();
			return;
		}

		//signal_ice_candidates_gathered_.Send(
		//	transport->transport_name(), std::vector<cricket::Candidate>{candidate});
	}

	void transport_controller::OnTransportCandidateError_n(
		libice::IceTransportInternal* transport,
		const libice::IceCandidateErrorEvent& event) {
		//signal_ice_candidate_error_.Send(event);
	}
	void transport_controller::OnTransportCandidatesRemoved_n(
		libice::IceTransportInternal* transport,
		const libice::Candidates& candidates) {
		//signal_ice_candidates_removed_.Send(candidates);
	}
	void transport_controller::OnTransportCandidatePairChanged_n(
		const libice::CandidatePairChangeEvent& event) {
		//signal_ice_candidate_pair_changed_.Send(event);
	}


	void transport_controller::OnTransportRoleConflict_n(
		libice::IceTransportInternal* transport) {
		// Note: since the role conflict is handled entirely on the network thread,
		// we don't need to worry about role conflicts occurring on two ports at
		// once. The first one encountered should immediately reverse the role.
		libice::IceRole reversed_role = (ice_role_ == libice::ICEROLE_CONTROLLING)
			? libice::ICEROLE_CONTROLLED
			: libice::ICEROLE_CONTROLLING;
		RTC_LOG(LS_INFO) << "Got role conflict; switching to "
			<< (reversed_role == libice::ICEROLE_CONTROLLING
				? "controlling"
				: "controlled")
			<< " role.";
		//SetIceRole_n(reversed_role);
	}

	void transport_controller::OnTransportStateChanged_n(
		libice::IceTransportInternal* transport) {
		RTC_LOG(LS_INFO) << transport->transport_name() << " Transport "
			<< transport->component()
			<< " state changed. Check if state is complete.";
		UpdateAggregateStates_n();
	}

	void transport_controller::UpdateAggregateStates_n() {
		//TRACE_EVENT0("webrtc", "JsepTransportController::UpdateAggregateStates_n");
		//auto dtls_transports = GetActiveDtlsTransports();
		//libice::IceConnectionState new_connection_state =
		//	libice::kIceConnectionConnecting;
		//PeerConnectionInterface::IceConnectionState new_ice_connection_state =
		//	PeerConnectionInterface::IceConnectionState::kIceConnectionNew;
		//PeerConnectionInterface::PeerConnectionState new_combined_state =
		//	PeerConnectionInterface::PeerConnectionState::kNew;
		//cricket::IceGatheringState new_gathering_state = cricket::kIceGatheringNew;
		//bool any_failed = false;
		//bool all_connected = !dtls_transports.empty();
		//bool all_completed = !dtls_transports.empty();
		//bool any_gathering = false;
		//bool all_done_gathering = !dtls_transports.empty();

		//std::map<IceTransportState, int> ice_state_counts;
		//std::map<DtlsTransportState, int> dtls_state_counts;

		//for (const auto& dtls : dtls_transports) {
		//	any_failed = any_failed || dtls->ice_transport()->GetState() ==
		//		cricket::IceTransportState::STATE_FAILED;
		//	all_connected = all_connected && dtls->writable();
		//	all_completed =
		//		all_completed && dtls->writable() &&
		//		dtls->ice_transport()->GetState() ==
		//		cricket::IceTransportState::STATE_COMPLETED &&
		//		dtls->ice_transport()->GetIceRole() == cricket::ICEROLE_CONTROLLING &&
		//		dtls->ice_transport()->gathering_state() ==
		//		cricket::kIceGatheringComplete;
		//	any_gathering = any_gathering || dtls->ice_transport()->gathering_state() !=
		//		cricket::kIceGatheringNew;
		//	all_done_gathering =
		//		all_done_gathering && dtls->ice_transport()->gathering_state() ==
		//		cricket::kIceGatheringComplete;

		//	dtls_state_counts[dtls->dtls_state()]++;
		//	ice_state_counts[dtls->ice_transport()->GetIceTransportState()]++;
		//}

		//if (any_failed) {
		//	new_connection_state = cricket::kIceConnectionFailed;
		//}
		//else if (all_completed) {
		//	new_connection_state = cricket::kIceConnectionCompleted;
		//}
		//else if (all_connected) {
		//	new_connection_state = cricket::kIceConnectionConnected;
		//}
		//if (ice_connection_state_ != new_connection_state) {
		//	ice_connection_state_ = new_connection_state;

		//	signal_ice_connection_state_.Send(new_connection_state);
		//}

		//// Compute the current RTCIceConnectionState as described in
		//// https://www.w3.org/TR/webrtc/#dom-rtciceconnectionstate.
		//// The PeerConnection is responsible for handling the "closed" state.
		//int total_ice_checking = ice_state_counts[IceTransportState::kChecking];
		//int total_ice_connected = ice_state_counts[IceTransportState::kConnected];
		//int total_ice_completed = ice_state_counts[IceTransportState::kCompleted];
		//int total_ice_failed = ice_state_counts[IceTransportState::kFailed];
		//int total_ice_disconnected =
		//	ice_state_counts[IceTransportState::kDisconnected];
		//int total_ice_closed = ice_state_counts[IceTransportState::kClosed];
		//int total_ice_new = ice_state_counts[IceTransportState::kNew];
		//int total_ice = dtls_transports.size();

		//if (total_ice_failed > 0) {
		//	// Any RTCIceTransports are in the "failed" state.
		//	new_ice_connection_state = PeerConnectionInterface::kIceConnectionFailed;
		//}
		//else if (total_ice_disconnected > 0) {
		//	// None of the previous states apply and any RTCIceTransports are in the
		//	// "disconnected" state.
		//	new_ice_connection_state =
		//		PeerConnectionInterface::kIceConnectionDisconnected;
		//}
		//else if (total_ice_new + total_ice_closed == total_ice) {
		//	// None of the previous states apply and all RTCIceTransports are in the
		//	// "new" or "closed" state, or there are no transports.
		//	new_ice_connection_state = PeerConnectionInterface::kIceConnectionNew;
		//}
		//else if (total_ice_new + total_ice_checking > 0) {
		//	// None of the previous states apply and any RTCIceTransports are in the
		//	// "new" or "checking" state.
		//	new_ice_connection_state = PeerConnectionInterface::kIceConnectionChecking;
		//}
		//else if (total_ice_completed + total_ice_closed == total_ice ||
		//	all_completed) {
		//	// None of the previous states apply and all RTCIceTransports are in the
		//	// "completed" or "closed" state.
		//	//
		//	// TODO(https://bugs.webrtc.org/10356): The all_completed condition is added
		//	// to mimic the behavior of the old ICE connection state, and should be
		//	// removed once we get end-of-candidates signaling in place.
		//	new_ice_connection_state = PeerConnectionInterface::kIceConnectionCompleted;
		//}
		//else if (total_ice_connected + total_ice_completed + total_ice_closed ==
		//	total_ice) {
		//	// None of the previous states apply and all RTCIceTransports are in the
		//	// "connected", "completed" or "closed" state.
		//	new_ice_connection_state = PeerConnectionInterface::kIceConnectionConnected;
		//}
		//else {
		//	RTC_NOTREACHED();
		//}

		//if (standardized_ice_connection_state_ != new_ice_connection_state) {
		//	if (standardized_ice_connection_state_ ==
		//		PeerConnectionInterface::kIceConnectionChecking &&
		//		new_ice_connection_state ==
		//		PeerConnectionInterface::kIceConnectionCompleted) {
		//		// Ensure that we never skip over the "connected" state.
		//		signal_standardized_ice_connection_state_.Send(
		//			PeerConnectionInterface::kIceConnectionConnected);
		//	}
		//	standardized_ice_connection_state_ = new_ice_connection_state;
		//	signal_standardized_ice_connection_state_.Send(new_ice_connection_state);
		//}

		//// Compute the current RTCPeerConnectionState as described in
		//// https://www.w3.org/TR/webrtc/#dom-rtcpeerconnectionstate.
		//// The PeerConnection is responsible for handling the "closed" state.
		//// Note that "connecting" is only a valid state for DTLS transports while
		//// "checking", "completed" and "disconnected" are only valid for ICE
		//// transports.
		//int total_connected =
		//	total_ice_connected + dtls_state_counts[DtlsTransportState::kConnected];
		//int total_dtls_connecting =
		//	dtls_state_counts[DtlsTransportState::kConnecting];
		//int total_failed =
		//	total_ice_failed + dtls_state_counts[DtlsTransportState::kFailed];
		//int total_closed =
		//	total_ice_closed + dtls_state_counts[DtlsTransportState::kClosed];
		//int total_new = total_ice_new + dtls_state_counts[DtlsTransportState::kNew];
		//int total_transports = total_ice * 2;

		//if (total_failed > 0) {
		//	// Any of the RTCIceTransports or RTCDtlsTransports are in a "failed" state.
		//	new_combined_state = PeerConnectionInterface::PeerConnectionState::kFailed;
		//}
		//else if (total_ice_disconnected > 0) {
		//	// None of the previous states apply and any RTCIceTransports or
		//	// RTCDtlsTransports are in the "disconnected" state.
		//	new_combined_state =
		//		PeerConnectionInterface::PeerConnectionState::kDisconnected;
		//}
		//else if (total_new + total_closed == total_transports) {
		//	// None of the previous states apply and all RTCIceTransports and
		//	// RTCDtlsTransports are in the "new" or "closed" state, or there are no
		//	// transports.
		//	new_combined_state = PeerConnectionInterface::PeerConnectionState::kNew;
		//}
		//else if (total_new + total_dtls_connecting + total_ice_checking > 0) {
		//	// None of the previous states apply and all RTCIceTransports or
		//	// RTCDtlsTransports are in the "new", "connecting" or "checking" state.
		//	new_combined_state =
		//		PeerConnectionInterface::PeerConnectionState::kConnecting;
		//}
		//else if (total_connected + total_ice_completed + total_closed ==
		//	total_transports) {
		//	// None of the previous states apply and all RTCIceTransports and
		//	// RTCDtlsTransports are in the "connected", "completed" or "closed" state.
		//	new_combined_state =
		//		PeerConnectionInterface::PeerConnectionState::kConnected;
		//}
		//else {
		//	RTC_NOTREACHED();
		//}

		//if (combined_connection_state_ != new_combined_state) {
		//	combined_connection_state_ = new_combined_state;
		//	signal_connection_state_.Send(new_combined_state);
		//}

		//// Compute the gathering state.
		//if (dtls_transports.empty()) {
		//	new_gathering_state = cricket::kIceGatheringNew;
		//}
		//else if (all_done_gathering) {
		//	new_gathering_state = cricket::kIceGatheringComplete;
		//}
		//else if (any_gathering) {
		//	new_gathering_state = cricket::kIceGatheringGathering;
		//}
		//if (ice_gathering_state_ != new_gathering_state) {
		//	ice_gathering_state_ = new_gathering_state;
		//	signal_ice_gathering_state_.Send(new_gathering_state);
		//}
	}

	void transport_controller::OnDtlsHandshakeError(rtc::SSLHandshakeError error)
	{
	}

}
 