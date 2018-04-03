/**
 * @file light_sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#include "src/light_sensor.h"
#include "src/sensor.h"
#include <iostream>

NAMESPACE_BEGIN(csci3081);

LightSensor::LightSensor(Subject *s, int wType, int cType) : Sensor(s) {
  wheel_type_ = wType;
  connection_type_ = cType;
}

void LightSensor::calculateReading() {
  Pose pose = state_.ent_->get_pose();
  double ent_rad = state_.ent_->get_radius();
  double distance = std::sqrt(std::pow(2, pose.x - pose_.x)
                     + std::pow(2, pose.y - pose_.y)) - ent_rad;
  reading_ = 1200/(std::pow(1.08, distance));
  if (reading_ > 1000)
    reading_ = 1000;
}

void LightSensor::Update(const State state) {
  state_ = state;
}

void LightSensor::UpdatePosition() {
  if (host_ != NULL) {
    double theta;
    if (side_ == LEFT_SIDE) {
      theta = host_->get_heading() - OFF_ANGLE;
    } else {
      theta = host_->get_heading() + OFF_ANGLE;
    }

    pose_.x = (host_->get_radius() * std::cos(theta * M_PI / 180.0)) + host_->get_pose().x;
    pose_.y = (host_->get_radius() * std::sin(theta * M_PI / 180.0)) + host_->get_pose().y;
  }
}

NAMESPACE_END(csci3081);
