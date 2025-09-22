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


#ifndef _C_PC_SESSION_DESCRIPTION_H_
#define _C_PC_SESSION_DESCRIPTION_H_

#include <stddef.h>
#include <stdint.h>

#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "api/crypto_params.h"
#include "libmedia/media_types.h"
#include "libmedia/rtp_parameters.h"
#include "libmedia/rtp_transceiver_direction.h"
#include "libmedia/rtp_transceiver_interface.h"
//#include "media/base/codec.h"
//#include "media/base/media_channel.h"
//#include "media/base/media_constants.h"
//#include "media/base/rid_description.h"
//#include "media/base/stream_params.h"
#include "ice/transport_description.h"
#include "ice/transport_info.h"
#include "libmedia/media_protocol_names.h"
#include "p2p_peerconnection/simulcast_description.h"
#include "rtc_base/checks.h"
#include "rtc_base/socket_address.h"
#include "rtc_base/system/rtc_export.h"
#include "libmedia/ccodec.h"
#include "libmedia/stream_params.h"
#include "api/jsep.h"
#include "libmedia/ccodec.h"
namespace libice {

typedef std::vector<libmedia::AudioCodec> AudioCodecs;
typedef std::vector<libmedia::VideoCodec> VideoCodecs;
typedef std::vector<cricket::CryptoParams> CryptoParamsVec;
typedef std::vector<libmedia::RtpExtension> RtpHeaderExtensions;

// RTC4585 RTP/AVPF
extern const char kMediaProtocolAvpf[];
// RFC5124 RTP/SAVPF
extern const char kMediaProtocolSavpf[];

extern const char kMediaProtocolDtlsSavpf[];

// Options to control how session descriptions are generated.
const int kAutoBandwidth = -1;

class AudioContentDescription;
class VideoContentDescription;
class SctpDataContentDescription;
class UnsupportedContentDescription;

// Describes a session description media section. There are subclasses for each
// media type (audio, video, data) that will have additional information.
struct MediaContentDescription {

public:
	MediaContentDescription() = default;
	virtual ~MediaContentDescription() = default;
	virtual libmedia::MediaType type() const = 0;
	virtual libmedia::MediaType type() = 0;
	virtual AudioContentDescription* as_audio() { return nullptr; }
	virtual const AudioContentDescription* as_audio() const { return nullptr; }

	// Try to cast this media description to a VideoContentDescription. Returns
	// nullptr if the cast fails.
	virtual VideoContentDescription* as_video() { return nullptr; }
	virtual const VideoContentDescription* as_video() const { return nullptr; }
	/*std::unique_ptr<MediaContentDescription> Clone() const {
		return absl::WrapUnique(CloneInternal());
	}*/
 
	// Copy function that returns a raw pointer. Caller will assert ownership.
	// Should only be called by the Clone() function. Must be implemented
	// by each final subclass.
	//virtual MediaContentDescription* CloneInternal() const = 0;
	enum ExtmapAllowMixed { kNo, kSession, kMedia };
  bool rtcp_mux_ = false;
  bool rtcp_reduced_size_ = false;
  bool remote_estimate_ = false;
  int bandwidth_ = kAutoBandwidth;
  std::string bandwidth_type_ = libmedia::kApplicationSpecificBandwidth;
  std::string protocol_;
  std::vector<cricket::CryptoParams> cryptos_;
  std::vector<libmedia::RtpExtension> rtp_header_extensions_;
  bool rtp_header_extensions_set_ = false;
  libmedia::StreamParamsVec send_streams_;
  bool conference_mode_ = false;
  libmedia::RtpTransceiverDirection direction_ =
	  libmedia::RtpTransceiverDirection::kSendRecv;
  rtc::SocketAddress connection_address_;
  ExtmapAllowMixed extmap_allow_mixed_enum_ = kMedia;

  SimulcastDescription simulcast_;
  std::vector<libmedia::RidDescription> receive_rids_;
//  MediaType media_type_ = MEDIA_TYPE_AUDIO;
};

template <class C>
struct MediaContentDescriptionImpl : public MediaContentDescription { 
  typedef C CodecType; 
  std::vector<C> codecs_;
  void BuildRtpMap(MediaContentDescriptionImpl<C>* media_content,
	  std::stringstream& ss);
  void BuildSsrc(MediaContentDescriptionImpl<C>* media_content,
	  std::stringstream& ss);
};

struct AudioContentDescription : public MediaContentDescriptionImpl<libmedia::AudioCodec> {
   
	 
	virtual libmedia::MediaType type() const { return libmedia::MEDIA_TYPE_AUDIO; };
	virtual libmedia::MediaType type()   { return libmedia::MEDIA_TYPE_AUDIO; };
	virtual AudioContentDescription* as_audio() { return this; }
	virtual const AudioContentDescription* as_audio() const { return this; }

	/*virtual AudioContentDescription* CloneInternal() const {
		return new AudioContentDescription(*this);
	}*/
	
};

struct VideoContentDescription : public MediaContentDescriptionImpl<libmedia::VideoCodec> {
	 
	virtual libmedia::MediaType type() const { return libmedia::MEDIA_TYPE_VIDEO; };
	virtual libmedia::MediaType type()   { return libmedia::MEDIA_TYPE_VIDEO; };
	// Try to cast this media description to a VideoContentDescription. Returns
	// nullptr if the cast fails.
	virtual VideoContentDescription* as_video() { return this; }
	virtual const VideoContentDescription* as_video() const { return this; }
	/*virtual VideoContentDescription* CloneInternal() const {
		return new VideoContentDescription(*this);
	}*/
};

//struct SctpDataContentDescription : public MediaContentDescription {
//  
//	 
//	SctpDataContentDescription()
//	{
//		media_type_ = MEDIA_TYPE_DATA;
//	}
//  bool use_sctpmap_ = true;  // Note: "true" is no longer conformant.
//  // Defaults should be constants imported from SCTP. Quick hack.
//  int port_ = 5000;
//  // draft-ietf-mmusic-sdp-sctp-23: Max message size default is 64K
//  int max_message_size_ = 64 * 1024;
//};
//
//struct UnsupportedContentDescription : public MediaContentDescription {
// 
//	UnsupportedContentDescription()
//	{
//		media_type_ = MEDIA_TYPE_UNSUPPORTED;
//	}
//  
//  std::string media_type_str_;
//};

// Protocol used for encoding media. This is the "top level" protocol that may
// be wrapped by zero or many transport protocols (UDP, ICE, etc.).
enum   MediaProtocolType {
  kRtp,   // Section will use the RTP protocol (e.g., for audio or video).
          // https://tools.ietf.org/html/rfc3550
  kSctp,  // Section will use the SCTP protocol (e.g., for a data channel).
          // https://tools.ietf.org/html/rfc4960
  kOther  // Section will use another top protocol which is not
          // explicitly supported.
};

// Represents a session description section. Most information about the section
// is stored in the description, which is a subclass of MediaContentDescription.
// Owns the description.
struct    ContentInfo {
  
public:
	ContentInfo() = default;
	ContentInfo(const ContentInfo& o)
	{
		name = o.name;
		type = o.type;
		rejected = o.rejected;
		bundle_only = o.bundle_only;
		description_.reset(o.description_.get());
	}
	ContentInfo& operator=(const ContentInfo& o) = default;
	ContentInfo(ContentInfo&& o) = default;
	ContentInfo& operator=(ContentInfo&& o) = default;
  // TODO(bugs.webrtc.org/8620): Rename this to mid.
	std::string name = ""; //mid
  MediaProtocolType type = kRtp;
  bool rejected = false;
  bool bundle_only = false;
   
  friend class SessionDescription;
  std::unique_ptr<MediaContentDescription> description_;
//private:
	///ContentInfo(const libice::ContentInfo &c);
};

 

// This class provides a mechanism to aggregate different media contents into a
// group. This group can also be shared with the peers in a pre-defined format.
// GroupInfo should be populated only with the `content_name` of the
// MediaDescription.
struct ContentGroup {
	ContentGroup() = default;
	~ContentGroup() = default;
  // for debugging
  std::string ToString() const;
  
  std::string semantics_;
  std::vector<std::string> content_names_;
};

//typedef std::vector<ContentInfo> ContentInfos;
//typedef std::vector<ContentGroup> ContentGroups;

//const ContentInfo* FindContentInfoByName(const ContentInfos& contents,
//                                         const std::string& name);
//const ContentInfo* FindContentInfoByType(const ContentInfos& contents,
//                                         const std::string& type);

// Determines how the MSID will be signaled in the SDP. These can be used as
// flags to indicate both or none.
enum MsidSignaling {
  // Signal MSID with one a=msid line in the media section.
  kMsidSignalingMediaSection = 0x1,
  // Signal MSID with a=ssrc: msid lines in the media section.
  kMsidSignalingSsrcAttribute = 0x2
};

// Describes a collection of contents, each with its own name and
// type.  Analogous to a <jingle> or <session> stanza.  Assumes that
// contents are unique be name, but doesn't enforce that.
class  SessionDescription 
{
public:
	SessionDescription( ) = default;
	~SessionDescription(){}
	template <typename C>
	void build_sdp(MediaContentDescriptionImpl<C>* media_content,
		std::stringstream& ss);

	bool HasGroup(const std::string & mid);
	 const  ContentGroup* GetGroupByName(const std::string& name) const;
	TransportInfo *GetTransportInfoByName(const std::string & mid);
	std::string ToString();
public:
	std::vector<ContentInfo> contents_;
  std::vector<TransportInfo> transport_infos_;
  std::vector<ContentGroup> content_groups_;
  bool msid_supported_ = false;
  // Default to what Plan B would do.
  // TODO(bugs.webrtc.org/8530): Change default to kMsidSignalingMediaSection.
  int msid_signaling_ = kMsidSignalingSsrcAttribute;
  bool extmap_allow_mixed_ = false;
};

// Indicates whether a session description was sent by the local client or
// received from the remote client.
enum ContentSource { CS_LOCAL, CS_REMOTE };




}  // namespace cricket

#endif  // PC_SESSION_DESCRIPTION_H_
