/*
 * vector_statistics.cpp
 *
 *  Created on: Feb 20, 2016
 *      Author: Cody W. Eilar <Cody.Eilar@Gmail.com>
 */

#include "vector_statistics.h"
#include <vector>
#include <cmath>

namespace oflow {
std::vector<double> VectorStatistics::Magnitude() {
  std::vector<double> mag;
  mag.reserve(points_[0].size());
  for (size_t i = 0; i < points_[0].size(); ++i) {
    mag.push_back(std::sqrt((points_[0][i].x - points_[1][i].x) *
                                (points_[0][i].x - points_[1][i].x) +
                            (points_[0][i].y - points_[1][i].y) *
                                (points_[0][i].y - points_[1][i].y)));
  }
  return mag;
}

std::vector<double> VectorStatistics::Orientation() {
  std::vector<double> theta;
  theta.reserve(points_[0].size());
  for (size_t i = 0; i < points_[0].size(); ++i) {
    double dot_prod =
        points_[0][i].x * points_[1][i].x + points_[0][i].y * points_[1][i].y;
    double mag = std::sqrt(points_[0][i].x * points_[0][i].x +
                           points_[0][i].y * points_[0][i].y) *
                 std::sqrt(points_[1][i].x * points_[1][i].x +
                           points_[1][i].y * points_[1][i].y);
    theta.push_back(std::acos(dot_prod / mag));
  }
  return theta;
}
}
