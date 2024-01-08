// Copyright 2023 ICUBE Laboratory, University of Strasbourg
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: Thibault Poignonec (thibault.poignonec@gmail.com)

#ifndef FD_SDK_VENDOR__FD_MODEL_HPP_
#define FD_SDK_VENDOR__FD_MODEL_HPP_

#include <Eigen/Geometry>
#include <Eigen/Dense>
typedef Eigen::Matrix<double, 6, 6, Eigen::RowMajor> Mat66;

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "fd_sdk_vendor/dhd.hpp"

namespace fd
{
/**
     * @brief Get the cartesian inertia matrix (6x6 matrix) of the fd device.
     *
     * @warning Matrix expressed in default fd_sdk frame of ref! See force dimension user manual.
     *
     * @param joint_positions Joint position of the device. At least a 3D vector, up to 7 DoF.
     * @param inertia_matrix To return the inertia matrix by ref.
     * @param device_id (opt.) ID of the fd device.
     * @return true All OK
     * @return false Failed to compute inertia matrix
     */
bool get_cartesian_inertia(
  const std::vector<double> & joint_positions,
  Eigen::Matrix<double, 6, 6> & inertia_matrix,
  char device_id = -1)
{
  // Test joint positions vector has a valid size
  if (joint_positions.size() < 3) {
    std::cerr << std::string("") + "Invalid joint positions vector!" + \
      "Size should be at least 3." << std::endl;
  }
  if (joint_positions.size() > DHD_MAX_DOF) {
    std::cerr << std::string("") + "Invalid joint positions vector!" + \
      "Size should be less that DHD_MAX_DOF (i.e., usually <= 7-8)." << std::endl;
  }
  // Copy joint positions to c array
  double c_inertia_array[6][6];
  double c_joint_positions[DHD_MAX_DOF] = {0.0};
  for (uint i = 0; i < joint_positions.size(); i++) {
    c_joint_positions[i] = joint_positions[i];
  }
  // Get inertia from robot pose
  int flag = 0;        // "Success" flag
  flag += dhdEnableExpertMode();
  flag += dhdJointAnglesToInertiaMatrix(c_joint_positions, c_inertia_array, device_id);
  flag += dhdDisableExpertMode();
  Eigen::Map<Mat66> inertia_array_map = Eigen::Map<Mat66>(&c_inertia_array[0][0]);
  inertia_matrix = inertia_array_map;
  return flag >= 0;
}

}  // namespace fd

#endif  // FD_SDK_VENDOR__FD_MODEL_HPP_
