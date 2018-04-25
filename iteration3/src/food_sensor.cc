/**
 * @file food_sensor.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#include <iostream>
#include "src/food_sensor.h"
#include "src/sensor.h"

NAMESPACE_BEGIN(csci3081);

FoodSensor::FoodSensor(Subject *s, int wType, int cType) : Sensor(s) {
  wheel_type_ = wType;
  connection_type_ = cType;
}

void FoodSensor::calculateReading() {
  Pose pose = state_.ent_->get_pose();
  double ent_rad = state_.ent_->get_radius();
  double distance = std::sqrt(std::pow(pose.x - pose_.x, 2)
                     + std::pow(pose.y - pose_.y, 2)) - ent_rad;

  //reading_ = 1010/(std::pow(1.006, distance));
  reading_ = coefficient_sensitivity_/(std::pow(base_sensitivity_, distance));
  if (reading_ > MAX_READ) {
    reading_ = MAX_READ;
  }

  if (distance <= 5) {
    host_->reset_hunger_time();
  }
}

void FoodSensor::Update(const State state) {
  if (state.ent_->get_type() == kFood
      && host_->get_food_sensor_status() == ON) {
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

void FoodSensor::UpdatePosition() {
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
