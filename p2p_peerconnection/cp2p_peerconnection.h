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



#ifndef _C_P2P_PEER_CONNECTION_H_
#define _C_P2P_PEER_CONNECTION_H_
#include "rtc_base/third_party/sigslot/sigslot.h"
#include "p2p_peerconnection/csession_description.h"
#include "p2p_peerconnection/ctransport_controller.h"
namespace libice
{
	struct RTCOfferAnswerOptions {
		bool send_audio = true;
		bool send_video = true;
		bool recv_audio = true;
		bool recv_video = true;
		bool use_rtp_mux = true;
		bool use_rtcp_mux = true;
		bool dtls_on = true;
	};

	class p2p_peer_connection : public sigslot::has_slots<>
	{
	public:
		p2p_peer_connection();
		virtual ~p2p_peer_connection();

		int set_remote_sdp(const std::string& sdp);
		std::string create_answer(const RTCOfferAnswerOptions& options,
			const std::string& stream_id);
	public:

		rtc::scoped_refptr<libice::ConnectionContext> GetContext() { return context_; };
		const rtc::scoped_refptr<libice::ConnectionContext> GetContext() const  { return context_; };
	private:
		rtc::scoped_refptr<libice::ConnectionContext> context_;
		std::unique_ptr<libice::SessionDescription> remote_desc_;
		std::unique_ptr<libice::SessionDescription> local_desc_;

		std::unique_ptr<transport_controller>  transport_controller_;

		webrtc::ScopedTaskSafety signaling_thread_safety_;

		uint32_t local_audio_ssrc_ = 0;
		uint32_t local_video_ssrc_ = 0;
		uint32_t local_video_rtx_ssrc_ = 0;
		uint8_t video_pt_ = 0;
		uint8_t video_rtx_pt_ = 0;
	};

}

#endif //   _C_PC_SESSION_DESCRIPTION_H_
 
