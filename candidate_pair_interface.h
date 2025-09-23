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


#ifndef _C_P2P_BASE_CANDIDATE_PAIR_INTERFACE_H_
#define _C_P2P_BASE_CANDIDATE_PAIR_INTERFACE_H_

namespace libice {

class Candidate;

class CandidatePairInterface {
 public:
  virtual ~CandidatePairInterface() {}

  virtual const Candidate& local_candidate() const = 0;
  virtual const Candidate& remote_candidate() const = 0;
};

}  // namespace cricket

#endif  // P2P_BASE_CANDIDATE_PAIR_INTERFACE_H_
