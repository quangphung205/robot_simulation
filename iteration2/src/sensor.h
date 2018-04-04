/**
 * @file sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SENSOR_H_
#define SRC_SENSOR_H_

#include "src/observer.h"
#include "src/params.h"
#include "src/pose.h"
#include "src/robot.h"
#include <iostream>

NAMESPACE_BEGIN(csci3081);

class Robot;

class Sensor : public Observer{
public:
  Sensor(Subject *s) : Observer(s), state_() {}
  Sensor(Sensor& s) = default;

  virtual void Update(const State state) override {state_ = state; }
  virtual void UpdatePosition() {}
  virtual void calculateReading() {}

  State get_state() const { return state_; }
  void set_state(State state) { state_ = state; }

  double get_reading() const { return reading_; }
  void set_reading(double reading) { reading_ = reading; }

  double get_vel_delta() const { return velocity_delta_; }
  void set_vel_delta(double delta) { velocity_delta_ = delta; }

  int get_wheel_type() const { return wheel_type_; }
  void set_wheel_type(int wType) { wheel_type_ = wType; }

  int get_connection_type() const { return connection_type_; }
  void set_connection_type(int cType) { connection_type_ = cType; }

  Robot* get_host() const { return host_; }
  void set_host(Robot *ent) { host_ = ent; }

  int get_side() const {return side_; }
  void set_side(int side) { side_ = side; }

  Sensor& operator=(const Sensor& other) = default;
  virtual ~Sensor() {}

  const Pose &get_pose() const { return pose_; }
  void set_pose(const Pose &pose) { pose_ = pose; }

  /**
   * @brief Setter method for position within entity pose variable.
   */
  void set_position(const double inx, const double iny) {
    pose_.x = inx;
    pose_.y = iny;
  }

protected:
  State state_;
  Robot * host_{NULL};
  double reading_{0};
  double velocity_delta_{0};
  Pose pose_{0, 0};
  int wheel_type_{LEFT_WHEEL};  // left or right wheel
  int connection_type_{PLUS_CONNECTION};  // + or -
  int side_{LEFT_SIDE};
};

NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_H_
