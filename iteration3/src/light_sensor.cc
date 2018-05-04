/**
 * @file light_sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#include <iostream>
#include "src/light_sensor.h"
#include "src/sensor.h"

NAMESPACE_BEGIN(csci3081);

LightSensor::LightSensor(Subject *s, int wheel_type, int connect_type)
  : Sensor(s) {
  wheel_type_ = wheel_type;
  connection_type_ = connect_type;
}

void LightSensor::calculateReading() {
  Pose pose = state_.ent_->get_pose();
  double ent_rad = state_.ent_->get_radius();
  double distance = std::sqrt(std::pow(pose.x - pose_.x, 2)
                     + std::pow(pose.y - pose_.y, 2)) - ent_rad;

  reading_ = coefficient_sensitivity_/(std::pow(base_sensitivity_, distance));
  if (reading_ > MAX_READ) {
    reading_ = MAX_READ;
  }
}

void LightSensor::Update(const State state) {
  if (state.ent_->get_type() == kLight
      && host_->get_light_sensor_status() == ON) {
    state_ = state;
    calculateReading();
    if (connection_type_ == PLUS_CONNECTION) {
      updated_velocity_ = reading_ / 200;
    } else {
      updated_velocity_ = (MAX_READ - reading_) / 200;
    }

    if (updated_velocity_ < ROBOT_MIN_SPEED) {
      updated_velocity_ += ROBOT_MIN_SPEED;
    }

    if (host_->get_old_angle() <= 0) {
      if (wheel_type_ == LEFT_WHEEL) {
        host_->UpdateLeftWheel(updated_velocity_);
      } else {
        host_->UpdateRightWheel(updated_velocity_);
      }
    }
  }
}

void LightSensor::UpdatePosition() {
  if (host_ != NULL) {
    double theta;
    if (side_ == LEFT_SIDE) {
      theta = host_->get_heading() - OFF_ANGLE;
    } else {
      theta = host_->get_heading() + OFF_ANGLE;
    }

    pose_.x = (host_->get_radius() * std::cos(theta * M_PI / 180.0))
         + host_->get_pose().x;
    pose_.y = (host_->get_radius() * std::sin(theta * M_PI / 180.0))
         + host_->get_pose().y;
  }
}

NAMESPACE_END(csci3081);
