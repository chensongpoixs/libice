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


//#include "p2p_peerconnection/session_description.h"



#include "p2p_peerconnection/csession_description.h"
#include <utility>

#include "absl/algorithm/container.h"
#include "absl/memory/memory.h"
#include "rtc_base/checks.h"
#include "csession_description.h"
#include "libmedia/ccodec.h"
namespace libice {
 
	// RTP Profile names
// http://www.iana.org/assignments/rtp-parameters/rtp-parameters.xml
// RFC4585
	const char kMediaProtocolAvpf[] = "RTP/AVPF";
	// RFC5124
	const char kMediaProtocolDtlsSavpf[] = "UDP/TLS/RTP/SAVPF";

	// We always generate offers with "UDP/TLS/RTP/SAVPF" when using DTLS-SRTP,
	// but we tolerate "RTP/SAVPF" in offers we receive, for compatibility.
	const char kMediaProtocolSavpf[] = "RTP/SAVPF";
	// Copy operator.
	 
	std::string ContentGroup::ToString() const
	{
		rtc::StringBuilder acc;
		acc << semantics_ << "(";
		if (!content_names_.empty()) {
			for (const auto& name : content_names_) {
				acc << name << " ";
			}
		}
		acc << ")";
		return acc.Release();
	}

static void AddRtcpFbLine(const libmedia::Codec& codec,
	std::stringstream& ss)
{
	//for (const cricket::Codec  &c  : codecs)
	{
		std::vector<libmedia::FeedbackParam> params = codec.feedback_params;
		for (auto& param : params)
		{ 
			ss << "a=rtcp-fb:" << codec.id << " " << param.id_;
			if (!param.param_.empty()) {
				ss << " " << param.param_;
			}
			ss << "\r\n";
		}
		
	}
}

static void AddFmtpLine(const libmedia::Codec& codec,
	std::stringstream& ss)
{

	//for (const cricket::Codec &c : codecs)
	{
		// cricket::CodecParameterMap  params = codec.params;
		 ss << "a=fmtp:" << codec.id << " ";
		 std::string data = "";
		 for (auto param : codec.params) {
			 data += (";" + param.first + "=" + param.second);
		 }

		 //;key1=value1;key2=values
		 data = data.substr(1);
		 ss << data << "\r\n";

	}

	
}


template<class C>
inline void MediaContentDescriptionImpl<C>::BuildRtpMap(
	MediaContentDescriptionImpl<C> * media_content,
	std::stringstream & ss)
{
	for (  libmedia::Codec& codec : media_content->codecs_) {
		ss << "a=rtpmap:" << codec.id << " " << codec.name << "/"
			<< codec.clockrate;
		 if (media_content->type() == libmedia::MEDIA_TYPE_AUDIO)
		{
			//auto audio_codec = codec->AsAudio();
			  
			ss << "/" << codec.GetChannel();//audio_codec->channels;
		}
		ss << "\r\n";

		AddRtcpFbLine(codec, ss);
		AddFmtpLine(codec, ss);
	}
}

template<class C>
inline void MediaContentDescriptionImpl<C>::BuildSsrc(MediaContentDescriptionImpl<C> * media_content, std::stringstream & ss)
{

	for (const libmedia::StreamParams& stream : media_content->send_streams_) {
		// 生成ssrc group
		for (auto group : stream.ssrc_groups) {
			if (group.ssrcs.empty()) {
				continue;
			}

			ss << "a=ssrc-group:FID";
			for (auto ssrc : group.ssrcs) {
				ss << " " << ssrc;
			}
			ss << "\r\n";
		}

		// 生成ssrc
		for (auto ssrc : stream.ssrcs) {
			ss << "a=ssrc:" << ssrc << " cname:" << stream.cname << "\r\n";
			ss << "a=ssrc:" << ssrc << " msid:" << stream.stream_ids_[0]
				<< " " << stream.id << "\r\n";
		}
	}


	
}
  

 

bool SessionDescription::HasGroup(const std::string & mid)
{
	for (size_t i = 0; i < content_groups_.size(); ++i)
	{
		if (content_groups_[i].semantics_ == mid)
		{
			return true;
		}
	}
	return false;
}

TransportInfo *SessionDescription::GetTransportInfoByName(const std::string & mid)
{
	for (size_t i = 0; i < transport_infos_.size(); ++i)
	{
		if (transport_infos_[i].content_name == mid)
		{
			return  &transport_infos_[i];
		}
	}
	return nullptr;
}
 const  ContentGroup* SessionDescription::GetGroupByName(const std::string& name) const
{
	for (size_t i = 0; i < content_groups_.size(); ++i)
	{
		if (content_groups_[i].semantics_ == name)
		{
			return  &content_groups_[i];
		}
	}
	return nullptr;
}
 template<typename C>
   void SessionDescription::build_sdp(MediaContentDescriptionImpl<C>* media_content, std::stringstream & ss)
 {
	 std::string fmt;

	 for (size_t i = 0; i < media_content->codecs_.size(); ++i)
	 {
		 fmt.append(" ");
		 fmt.append(std::to_string(codec.id));
	 }

	 
 }
std::string SessionDescription::ToString()
{
	std::stringstream ss;
	// version
	ss << "v=0\r\n";
	// session origin
	// RFC 4566
	// o=<username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address>
	ss << "o=- 0 2 IN IP4 127.0.0.1\r\n";
	// session name
	ss << "s=-\r\n";
	// time description
	ss << "t=0 0\r\n";

	// 生成BUNDLE信息
	const ContentGroup* answer_bundle = GetGroupByName("BUNDLE");
	if (answer_bundle && !(answer_bundle->content_names_.empty())) {
		ss << "a=group:BUNDLE";
		//for (auto content_name : answer_bundle->content_names()) {
		for (size_t i = 0; i < answer_bundle->content_names_.size(); ++i)
		{
			ss << " " << answer_bundle->content_names_[i];
		}
		ss << "\r\n";
	}

	ss << "a=msid-semantic: WMS\r\n";

	//for (auto content : contents_) 
	for (size_t i = 0; i  < contents_.size() ; ++i)
	{
		// 生成m行
		// RFC 4566
		// m=<media> <port> <proto> <fmt>
		//MediaContentDescriptionImpl<libmedia::Codec> media =
		//build_sdp<>(contents_[i].description_.get(), ss);
		//std::string fmt;
		/*for (auto codec : content.media_description()()) {
			fmt.append(" ");
			fmt.append(std::to_string(codec->id));
		}*/
		std::string fmt;
		//MediaContentDescriptionImpl<libmedia::Codec>   media_desc;
		if (contents_[i].description_->type() == libmedia::MEDIA_TYPE_AUDIO)
		{
			AudioContentDescription  *audio =  dynamic_cast<AudioContentDescription *>(contents_[i].description_->as_audio());
			for (size_t w = 0; w < audio->codecs_.size(); ++w)
			{
				fmt.append(" ");
				fmt.append(std::to_string(audio->codecs_[w].id));
			}
		//	media_desc = audio;
		}
		else if (contents_[i].description_->type() == libmedia::MEDIA_TYPE_VIDEO)
		{
			VideoContentDescription  *video = dynamic_cast<VideoContentDescription *>(contents_[i].description_->as_video());
			for (size_t w = 0; w < video->codecs_.size(); ++w)
			{
				fmt.append(" ");
				fmt.append(std::to_string(video->codecs_[w].id));
			}
		}


		ss << "m=" << contents_[i].name << " 9 " << kMediaProtocolDtlsSavpf
			<< fmt << "\r\n";
		ss << "c=IN IP4 0.0.0.0\r\n";
		ss << "a=rtcp:9 IN IP4 0.0.0.0\r\n";

		TransportInfo* td = GetTransportInfoByName(contents_[i].name);
		if (td) {
			ss << "a=ice-ufrag:" << td->description.ice_ufrag << "\r\n";
			ss << "a=ice-pwd:" << td->description.ice_pwd << "\r\n";
			ss << "a=ice-options:trickle" << "\r\n";
			auto fp = td->description.identity_fingerprint.get();
			if (fp) {
				ss << "a=fingerprint:" << fp->algorithm << " " << fp->GetRfc4572Fingerprint()
					<< "\r\n";
				std::string connection_role;
				ConnectionRoleToString(td->description.connection_role, &connection_role);
				ss << "a=setup:" << connection_role/*connection_role_to_string( td->connection_role)*/ << "\r\n";
			}
			
		}

		ss << "a=mid:" << contents_[i].name << "\r\n";
		ss << "a=" << "sendonly"/*GetDirection(content.media_description())*/ << "\r\n";
		//if (content->rtcp_mux()) {
		//	ss << "a=rtcp-mux" << "\r\n";
		//}
		if (contents_[i].description_->type() == libmedia::MEDIA_TYPE_AUDIO)
		{
			AudioContentDescription  *audio = dynamic_cast<AudioContentDescription *>(contents_[i].description_->as_audio());
			audio->BuildRtpMap(audio, ss);
			audio->BuildSsrc(audio, ss);
		}
		else if (contents_[i].description_->type() == libmedia::MEDIA_TYPE_VIDEO)
		{
			VideoContentDescription  *video = dynamic_cast<VideoContentDescription *>(contents_[i].description_->as_video());
			video->BuildRtpMap(video, ss);
			video->BuildSsrc(video, ss);
		}
		
	}

	return ss.str();
}
 
}  // namespace cricket
