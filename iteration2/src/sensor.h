/**
 * @file sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SENSOR_H_
#define SRC_SENSOR_H_

#include "src/observer.h"
#include "src/params.h"
#include <iostream>

NAMESPACE_BEGIN(csci3081);

class Sensor : public Observer{
public:
  Sensor(Subject *s) : Observer(s), state_() {}
  Sensor(Sensor& s) = default;
  virtual void Update(const State state) override {state_ = state; }
  virtual void calculateReading() {}

  State get_state() { return state_; }
  void set_state(State state) { state_ = state; }

  double get_reading() { return reading_; }
  void set_reading(double reading) { reading_ = reading; }

  double get_vel_delta() { return velocity_delta_; }
  void set_vel_delta(double delta) { velocity_delta_ = delta; }

  int get_wheel_type() { return wheel_type_; }
  void set_wheel_type(int wType) { wheel_type_ = wType; }

  int get_connection_type() { return connection_type_; }
  void set_connection_type(int cType) { connection_type_ = cType; }

  ArenaEntity* get_host() { return host_; }
  void set_host(ArenaEntity *ent) { host_ = ent; }

  Sensor& operator=(const Sensor& other) = default;
  virtual ~Sensor() {}
protected:
  State state_;
  ArenaEntity * host_{NULL};
  double reading_{0};
  double velocity_delta_{0};
  int wheel_type_{LEFT_WHEEL};  // left or right wheel
  int connection_type_{PLUS_CONNECTION};  // + or -
};

NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_H_
