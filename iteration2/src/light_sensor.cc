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
  double distance = std::sqrt(std::pow(pose.x - pose_.x, 2)
                     + std::pow(pose.y - pose_.y, 2)) - ent_rad;
  //printf("\nDis = %lf\n", distance);
  reading_ = 1010/(std::pow(1.006, distance));
  if (reading_ > MAX_READ) {
    reading_ = MAX_READ;
  }
}

void LightSensor::Update(const State state) {
  if (state.ent_->get_type() == kLight) {
    state_ = state;
    calculateReading();
    if (connection_type_ == PLUS_CONNECTION) {
      velocity_delta_ = reading_ / 200;
    } else {
      velocity_delta_ = (MAX_READ - reading_) / 200;
    }

    if (velocity_delta_ < ROBOT_MIN_SPEED) {
      velocity_delta_ += ROBOT_MIN_SPEED;
    }

    if (host_->get_old_angle() <= 0) {
      if (side_ == LEFT_SIDE) {
        //printf("Left read = %lf\n", reading_);
      } else {
        //printf("Right read = %lf\n", reading_);
      }
      if (wheel_type_ == LEFT_WHEEL) {
        //printf("Left vel = %lf\n", velocity_delta_);
        host_->UpdateLeftWheel(velocity_delta_);
      } else {
        //printf("Right vel = %lf\n", velocity_delta_);
        host_->UpdateRightWheel(velocity_delta_);
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

    pose_.x = (host_->get_radius() * std::cos(theta * M_PI / 180.0)) + host_->get_pose().x;
    pose_.y = (host_->get_radius() * std::sin(theta * M_PI / 180.0)) + host_->get_pose().y;

/*
    if (side_ == LEFT_SIDE) {
      std::cout << "Left pose = " << pose_.x << ", " << pose_.y << std::endl;
    } else {
      std::cout << "Right pose = " << pose_.x << ", " << pose_.y << std::endl;
    }
    */
  }
}

NAMESPACE_END(csci3081);
