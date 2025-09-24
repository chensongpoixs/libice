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

#ifndef _C_API_TRANSPORT_NETWORK_TYPES_H_
#define _C_API_TRANSPORT_NETWORK_TYPES_H_
#include <stdint.h>

#include <vector>

#include "absl/types/optional.h"
#include "api/units/data_rate.h"
#include "api/units/data_size.h"
#include "api/units/time_delta.h"
#include "api/units/timestamp.h"

namespace libice {

// Configuration

// Represents constraints and rates related to the currently enabled streams.
// This is used as input to the congestion controller via the StreamsConfig
// struct.
struct BitrateAllocationLimits {
  // The total minimum send bitrate required by all sending streams.
  webrtc::DataRate min_allocatable_rate = webrtc::DataRate::Zero();
  // The total maximum allocatable bitrate for all currently available streams.
  webrtc::DataRate max_allocatable_rate = webrtc::DataRate::Zero();
  // The max bitrate to use for padding. The sum of the per-stream max padding
  // rate.
  webrtc::DataRate max_padding_rate = webrtc::DataRate::Zero();
};

// Use StreamsConfig for information about streams that is required for specific
// adjustments to the algorithms in network controllers. Especially useful
// for experiments.
struct StreamsConfig {
  StreamsConfig();
  StreamsConfig(const StreamsConfig&);
  ~StreamsConfig();
  webrtc::Timestamp at_time = webrtc::Timestamp::PlusInfinity();
  absl::optional<bool> requests_alr_probing;
  absl::optional<double> pacing_factor;

  // TODO(srte): Use BitrateAllocationLimits here.
  absl::optional<webrtc::DataRate> min_total_allocated_bitrate;
  absl::optional<webrtc::DataRate> max_padding_rate;
  absl::optional<webrtc::DataRate> max_total_allocated_bitrate;
};

struct TargetRateConstraints {
  TargetRateConstraints();
  TargetRateConstraints(const TargetRateConstraints&);
  ~TargetRateConstraints();
  webrtc::Timestamp at_time = webrtc::Timestamp::PlusInfinity();
  absl::optional<webrtc::DataRate> min_data_rate;
  absl::optional<webrtc::DataRate> max_data_rate;
  // The initial bandwidth estimate to base target rate on. This should be used
  // as the basis for initial OnTargetTransferRate and OnPacerConfig callbacks.
  absl::optional<webrtc::DataRate> starting_rate;
};

// Send side information

struct NetworkAvailability {
	webrtc::Timestamp at_time = webrtc::Timestamp::PlusInfinity();
  bool network_available = false;
};

struct NetworkRouteChange {
  NetworkRouteChange();
  NetworkRouteChange(const NetworkRouteChange&);
  ~NetworkRouteChange();
  webrtc::Timestamp at_time = webrtc::Timestamp::PlusInfinity();
  // The TargetRateConstraints are set here so they can be changed synchronously
  // when network route changes.
  TargetRateConstraints constraints;
};

struct PacedPacketInfo {
  PacedPacketInfo();
  PacedPacketInfo(int probe_cluster_id,
                  int probe_cluster_min_probes,
                  int probe_cluster_min_bytes);

  bool operator==(const PacedPacketInfo& rhs) const;

  // TODO(srte): Move probing info to a separate, optional struct.
  static constexpr int kNotAProbe = -1;
  int send_bitrate_bps = -1;
  int probe_cluster_id = kNotAProbe;
  int probe_cluster_min_probes = -1;
  int probe_cluster_min_bytes = -1;
  int probe_cluster_bytes_sent = 0;
};

struct SentPacket {
	webrtc::Timestamp send_time = webrtc::Timestamp::PlusInfinity();
  // Size of packet with overhead up to IP layer.
	webrtc::DataSize size = webrtc::DataSize::Zero();
  // Size of preceeding packets that are not part of feedback.
	webrtc::DataSize prior_unacked_data = webrtc::DataSize::Zero();
  // Probe cluster id and parameters including bitrate, number of packets and
  // number of bytes.
  PacedPacketInfo pacing_info;
  // True if the packet is an audio packet, false for video, padding, RTX etc.
  bool audio = false;
  // Transport independent sequence number, any tracked packet should have a
  // sequence number that is unique over the whole call and increasing by 1 for
  // each packet.
  int64_t sequence_number;
  // Tracked data in flight when the packet was sent, excluding unacked data.
  webrtc::DataSize data_in_flight = webrtc::DataSize::Zero();
};

struct ReceivedPacket {
  webrtc::Timestamp send_time = webrtc::Timestamp::MinusInfinity();
  webrtc::Timestamp receive_time = webrtc::Timestamp::PlusInfinity();
  webrtc::DataSize size = webrtc::DataSize::Zero();
};

// Transport level feedback

struct RemoteBitrateReport {
	webrtc::Timestamp receive_time = webrtc::Timestamp::PlusInfinity();
	webrtc::DataRate bandwidth = webrtc::DataRate::Infinity();
};

struct RoundTripTimeUpdate {
	webrtc::Timestamp receive_time = webrtc::Timestamp::PlusInfinity();
	webrtc::TimeDelta round_trip_time = webrtc::TimeDelta::PlusInfinity();
  bool smoothed = false;
};

struct TransportLossReport {
	webrtc::Timestamp receive_time = webrtc::Timestamp::PlusInfinity();
	webrtc::Timestamp start_time = webrtc::Timestamp::PlusInfinity();
	webrtc::Timestamp end_time = webrtc::Timestamp::PlusInfinity();
  uint64_t packets_lost_delta = 0;
  uint64_t packets_received_delta = 0;
};

// Packet level feedback

struct PacketResult {
  class ReceiveTimeOrder {
   public:
    bool operator()(const PacketResult& lhs, const PacketResult& rhs);
  };

  PacketResult();
  PacketResult(const PacketResult&);
  ~PacketResult();

  inline bool IsReceived() const { return !receive_time.IsPlusInfinity(); }

  SentPacket sent_packet;
  webrtc::Timestamp receive_time = webrtc::Timestamp::PlusInfinity();
};

struct TransportPacketsFeedback {
  TransportPacketsFeedback();
  TransportPacketsFeedback(const TransportPacketsFeedback& other);
  ~TransportPacketsFeedback();

  webrtc::Timestamp feedback_time = webrtc::Timestamp::PlusInfinity();
  webrtc::Timestamp first_unacked_send_time = webrtc::Timestamp::PlusInfinity();
  webrtc::DataSize data_in_flight = webrtc::DataSize::Zero();
  webrtc::DataSize prior_in_flight = webrtc::DataSize::Zero();
  std::vector<PacketResult> packet_feedbacks;

  // Arrival times for messages without send time information.
  std::vector<webrtc::Timestamp> sendless_arrival_times;

  std::vector<PacketResult> ReceivedWithSendInfo() const;
  std::vector<PacketResult> LostWithSendInfo() const;
  std::vector<PacketResult> PacketsWithFeedback() const;
  std::vector<PacketResult> SortedByReceiveTime() const;
};

// Network estimation

struct NetworkEstimate {
	webrtc::Timestamp at_time = webrtc::Timestamp::PlusInfinity();
  // Deprecated, use TargetTransferRate::target_rate instead.
  webrtc::DataRate bandwidth = webrtc::DataRate::Infinity();
  webrtc::TimeDelta round_trip_time = webrtc::TimeDelta::PlusInfinity();
  webrtc::TimeDelta bwe_period = webrtc::TimeDelta::PlusInfinity();

  float loss_rate_ratio = 0;
};

// Network control

struct PacerConfig {
	webrtc::Timestamp at_time = webrtc::Timestamp::PlusInfinity();
  // Pacer should send at most data_window data over time_window duration.
	webrtc::DataSize data_window = webrtc::DataSize::Infinity();
	webrtc::TimeDelta time_window = webrtc::TimeDelta::PlusInfinity();
  // Pacer should send at least pad_window data over time_window duration.
	webrtc::DataSize pad_window = webrtc::DataSize::Zero();
	webrtc::DataRate data_rate() const { return data_window / time_window; }
	webrtc::DataRate pad_rate() const { return pad_window / time_window; }
};

struct ProbeClusterConfig {
	webrtc::Timestamp at_time = webrtc::Timestamp::PlusInfinity();
	webrtc::DataRate target_data_rate = webrtc::DataRate::Zero();
	webrtc::TimeDelta target_duration = webrtc::TimeDelta::Zero();
  int32_t target_probe_count = 0;
  int32_t id = 0;
};

struct TargetTransferRate {
	webrtc::Timestamp at_time = webrtc::Timestamp::PlusInfinity();
  // The estimate on which the target rate is based on.
  NetworkEstimate network_estimate;
  webrtc::DataRate target_rate = webrtc::DataRate::Zero();
  webrtc::DataRate stable_target_rate = webrtc::DataRate::Zero();
  double cwnd_reduce_ratio = 0;
};

// Contains updates of network controller comand state. Using optionals to
// indicate whether a member has been updated. The array of probe clusters
// should be used to send out probes if not empty.
struct NetworkControlUpdate {
  NetworkControlUpdate();
  NetworkControlUpdate(const NetworkControlUpdate&);
  ~NetworkControlUpdate();
  absl::optional<webrtc::DataSize> congestion_window;
  absl::optional<PacerConfig> pacer_config;
  std::vector<ProbeClusterConfig> probe_cluster_configs;
  absl::optional<TargetTransferRate> target_rate;
};

// Process control
struct ProcessInterval {
  ProcessInterval();
  ProcessInterval(const ProcessInterval&);
  ~ProcessInterval();
  webrtc::Timestamp at_time = webrtc::Timestamp::PlusInfinity();
  absl::optional<webrtc::DataSize> pacer_queue;
};

// Under development, subject to change without notice.
struct NetworkStateEstimate {
  double confidence = NAN;
  // The time the estimate was received/calculated.
  webrtc::Timestamp update_time = webrtc::Timestamp::MinusInfinity();
  webrtc::Timestamp last_receive_time = webrtc::Timestamp::MinusInfinity();
  webrtc::Timestamp last_send_time = webrtc::Timestamp::MinusInfinity();

  // Total estimated link capacity.
  webrtc::DataRate link_capacity = webrtc::DataRate::MinusInfinity();
  // Used as a safe measure of available capacity.
  webrtc::DataRate link_capacity_lower = webrtc::DataRate::MinusInfinity();
  // Used as limit for increasing bitrate.
  webrtc::DataRate link_capacity_upper = webrtc::DataRate::MinusInfinity();

  webrtc::TimeDelta pre_link_buffer_delay = webrtc::TimeDelta::MinusInfinity();
  webrtc::TimeDelta post_link_buffer_delay = webrtc::TimeDelta::MinusInfinity();
  webrtc::TimeDelta propagation_delay = webrtc::TimeDelta::MinusInfinity();

  // Only for debugging
  webrtc::TimeDelta time_delta = webrtc::TimeDelta::MinusInfinity();
  webrtc::Timestamp last_feed_time = webrtc::Timestamp::MinusInfinity();
  double cross_delay_rate = NAN;
  double spike_delay_rate = NAN;
  webrtc::DataRate link_capacity_std_dev = webrtc::DataRate::MinusInfinity();
  webrtc::DataRate link_capacity_min = webrtc::DataRate::MinusInfinity();
  double cross_traffic_ratio = NAN;
};
}  // namespace webrtc

#endif  // API_TRANSPORT_NETWORK_TYPES_H_
