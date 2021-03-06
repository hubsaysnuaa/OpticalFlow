/*
 * vector_statistics_tests.cpp
 *
 *  Created on: Feb 20, 2016
 *      Author: Cody W. Eilar <Cody.Eilar@Gmail.com>
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <array>
#include <memory>
#include "vector_statistics.h"
#include <cmath>

namespace oflow {

class VectorStatisticsFixture : public ::testing::Test {
 protected:
  void SetUp() override {
    original_image_.reset(new cv::Mat(num_rows_, num_cols_, CV_8U));
    cv::randu(*original_image_, cv::Scalar::all(0), cv::Scalar::all(255));
  }
  std::unique_ptr<cv::Mat> original_image_;
  const uint32_t num_rows_ = 32, num_cols_ = 32;
};

TEST_F(VectorStatisticsFixture, CvConstructor) {
  vector_type points_;
  VectorStatistics<> vs(points_);
}
TEST_F(VectorStatisticsFixture, BadSizes) {
  cv::Point2f a_pt{1.0, 2.0}, b_pt{2.0, 3.0};
  std::vector<cv::Point2f> list_a{a_pt}, list_b{b_pt, a_pt};
  vector_type test_vecs{{list_a, list_b}};
  ASSERT_THROW(VectorStatistics<> vs{test_vecs}, VectorStatisticsException);
}

TEST(VectorStatistics, VelocityX) {
  cv::Point2f a_pt{1.0, 2.0}, b_pt{2.0, 3.0};
  std::vector<cv::Point2f> list_a{a_pt}, list_b{b_pt};
  vector_type test_vecs{{list_a, list_b}};
  VectorStatistics<> vs{test_vecs};
  double expected_velocity_x = a_pt.x - b_pt.x;
  auto actual_velocity_x = vs.VelocityX();
  ASSERT_DOUBLE_EQ(expected_velocity_x, actual_velocity_x[0]);
}

TEST(VectorStatistics, VelocityXMat) {
  cv::Point2f a_pt{1.0, 2.0}, b_pt{2.0, 3.0};
  std::vector<cv::Point2f> list_a{a_pt}, list_b{b_pt};
  vector_type test_vecs{{list_a, list_b}};
  VectorStatistics<cv::Mat> vs{test_vecs, cv::Mat(5, 5, CV_32F)};
  double expected_velocity_x = a_pt.x - b_pt.x;
  cv::Mat actual_velocity_x = vs.VelocityX();
  ASSERT_DOUBLE_EQ(expected_velocity_x,
                   actual_velocity_x.at<float>(a_pt.y, a_pt.x));
}

TEST(VectorStatistics, VelocityY) {
  cv::Point2f a_pt{1.0, 2.0}, b_pt{2.0, 3.0};
  std::vector<cv::Point2f> list_a{a_pt}, list_b{b_pt};
  vector_type test_vecs{{list_a, list_b}};
  VectorStatistics<> vs{test_vecs};
  double expected_velocity_y = a_pt.y - b_pt.y;
  auto actual_velocity_y = vs.VelocityY();
  ASSERT_DOUBLE_EQ(expected_velocity_y, actual_velocity_y[0]);
}

TEST(VectorStatistics, VelocityYMat) {
  cv::Point2f a_pt{1.0, 2.0}, b_pt{2.0, 3.0};
  std::vector<cv::Point2f> list_a{a_pt}, list_b{b_pt};
  vector_type test_vecs{{list_a, list_b}};
  VectorStatistics<cv::Mat> vs{test_vecs, cv::Mat(5, 5, CV_32F)};
  double expected_velocity_y = a_pt.y - b_pt.y;
  cv::Mat actual_velocity_y = vs.VelocityY();
  ASSERT_DOUBLE_EQ(expected_velocity_y,
                   actual_velocity_y.at<float>(a_pt.y, a_pt.x));
}

TEST(VectorStatistics, Magnitude) {
  cv::Point2f a_pt{1.0, 2.0}, b_pt{2.0, 3.0};
  std::vector<cv::Point2f> list_a{a_pt, b_pt}, list_b{b_pt, a_pt};
  vector_type test_vecs{{list_a, list_b}};
  VectorStatistics<> vs{test_vecs};
  float mag_1 = std::sqrt(std::pow(static_cast<float>(1.0 - 2.0), 2) +
                          std::pow(static_cast<float>(2.0 - 3.0), 2));
  float mag_2 = std::sqrt(std::pow(static_cast<float>(2.0 - 1.0), 2) +
                          std::pow(static_cast<float>(3.0 - 2.0), 2));
  std::vector<float> expected_mags{mag_1, mag_2};
  ASSERT_FLOAT_EQ(expected_mags[0], vs.Magnitude()[0]);
  ASSERT_FLOAT_EQ(expected_mags[1], vs.Magnitude()[1]);
}

TEST(VectorStatistics, MagnitudeMat) {
  cv::Point2f a_pt{1.0, 2.0}, b_pt{2.0, 3.0};
  std::vector<cv::Point2f> list_a{a_pt, b_pt}, list_b{b_pt, a_pt};
  vector_type test_vecs{{list_a, list_b}};
  VectorStatistics<cv::Mat> vs{test_vecs, cv::Mat(5, 5, CV_32F)};
  float mag_1 = std::sqrt(std::pow(static_cast<float>(1.0 - 2.0), 2) +
                          std::pow(static_cast<float>(2.0 - 3.0), 2));
  float mag_2 = std::sqrt(std::pow(static_cast<float>(2.0 - 1.0), 2) +
                          std::pow(static_cast<float>(3.0 - 2.0), 2));
  std::vector<float> expected_mags{mag_1, mag_2};
  ASSERT_FLOAT_EQ(expected_mags[0], vs.Magnitude().at<float>(a_pt.y, a_pt.x));
  ASSERT_FLOAT_EQ(expected_mags[1], vs.Magnitude().at<float>(b_pt.y, b_pt.x));
}

TEST(VectorStatistics, Orientation) {
  cv::Point2f a_pt{3.0, 0.0}, b_pt{5.0, 5.0};
  std::vector<cv::Point2f> list_a{a_pt, b_pt}, list_b{b_pt, a_pt};
  vector_type test_vecs{{list_a, list_b}};
  VectorStatistics<> vs{test_vecs};
  // Orientation is 45 degrees.
  float expected_orientation = 0.7853982;
  auto actual_orientation = vs.Orientation();
  ASSERT_FLOAT_EQ(expected_orientation, actual_orientation[0]);
}

TEST(VectorStatistics, OrientationMat) {
  cv::Point2f a_pt{3.0, 0.0}, b_pt{5.0, 5.0};
  std::vector<cv::Point2f> list_a{a_pt, b_pt}, list_b{b_pt, a_pt};
  vector_type test_vecs{{list_a, list_b}};
  // Warning! if the points step outside the Mat, things could be bad.
  VectorStatistics<cv::Mat> vs{test_vecs, cv::Mat(6, 6, CV_32F)};
  // Orientation is 45 degrees.
  float expected_orientation = 0.7853982;
  cv::Mat actual_orientation = vs.Orientation();
  vs.Orientation().copyTo(actual_orientation);
  ASSERT_FLOAT_EQ(expected_orientation,
                  actual_orientation.at<float>(a_pt.y, a_pt.x));
}

}  // End namespace oflow
