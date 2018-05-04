/**
 * @file sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SENSOR_H_
#define SRC_SENSOR_H_

#include <iostream>
#include "src/observer.h"
#include "src/params.h"
#include "src/pose.h"
#include "src/robot.h"

NAMESPACE_BEGIN(csci3081);

class Robot;
/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class to implement the Observer Pattern
 *
 * Sensors will be observers and arena will be the subject
 */
class Sensor : public Observer{
 public:
  /**
   * @brief Default constructor
   */
  explicit Sensor(Subject *s) : Observer(s), state_() {}

  /**
   * @brief Default constructor
   */
  Sensor(const Sensor &s) = default;

  /**
   * @brief update the state
   *
   * @param state new state
   */
  void Update(const State state) override {state_ = state; }

  /**
   * @brief update position of the sensor based on robot position
   */
  virtual void UpdatePosition() {}

  /**
   * @brief calculate the reading of sensor
   */
  virtual void calculateReading() {}

  /**
   * @brief get the state of the sensor
   *
   * @return current state
   */
  State get_state() const { return state_; }

  /**
   * @brief set the state of the Sensor
   *
   * @param state new state
   */
  void set_state(State state) { state_ = state; }

  /**
   * @brief get the reading of the sensor
   *
   * @return current reading
   */
  double get_reading() const { return reading_; }

  /**
   * @brief set the reading of the Sensor
   *
   * @param reading new reading
   */
  void set_reading(double reading) { reading_ = reading; }

  /**
   * @brief get the velocity delta of the sensor
   *
   * @return current velocity delta
   */
  double get_vel_delta() const { return updated_velocity_; }

  /**
   * @brief set the velocity delta of the Sensor
   *
   * @param delta new velocity delta
   */
  void set_vel_delta(double delta) { updated_velocity_ = delta; }

  /**
   * @brief get the wheel type of the sensor
   *
   * @return current wheel type
   */
  int get_wheel_type() const { return wheel_type_; }

  /**
   * @brief set the wheel type of the Sensor
   *
   * @param wType new wheel type
   */
  void set_wheel_type(int wType) { wheel_type_ = wType; }

  /**
   * @brief get the connection type of the sensor
   *
   * @return current connection type
   */
  int get_connection_type() const { return connection_type_; }

  /**
   * @brief set the connection type of the Sensor
   *
   * @param cType new connection type
   */
  void set_connection_type(int cType) { connection_type_ = cType; }

  /**
   * @brief get the host of the sensor
   *
   * @return current host
   */
  Robot* get_host() const { return host_; }

  /**
   * @brief set the host of the Sensor
   *
   * @param ent new host
   */
  void set_host(Robot *ent) { host_ = ent; }

  /**
   * @brief get the side of the sensor
   *
   * @return current side
   */
  int get_side() const {return side_; }

  /**
   * @brief set the side of the Sensor
   *
   * @param side new side
   */
  void set_side(int side) { side_ = side; }

  /**
   * @brief overload assignment operator
   */
  Sensor& operator=(const Sensor& other) = default;

  /**
   * @brief Default destructor
   */
  virtual ~Sensor() {}

  /**
   * @brief get position of the Sensor
   *
   * @return current position
   */
  const Pose &get_pose() const { return pose_; }

  /**
   * @brief set the position of the sensor
   *
   * @param pose new position
   */
  void set_pose(const Pose &pose) { pose_ = pose; }

  /**
   * @brief Setter method for position within entity pose variable.
   */
  void set_position(const double inx, const double iny) {
    pose_.x = inx;
    pose_.y = iny;
  }

  /**
   * @brief Update sensitivity of the sensor
   *
   * @param bs base sensitivity
   * @param ds distance sensitivity
   */
  void UpdateSensitivity(const double bs, const double ds) override {
    base_sensitivity_ = bs;
    coefficient_sensitivity_ = ds;
  }

 protected:
  State state_;
  Robot * host_{NULL};
  double reading_{0};
  double updated_velocity_{0};
  Pose pose_{0, 0};
  int wheel_type_{LEFT_WHEEL};  // left or right wheel
  int connection_type_{PLUS_CONNECTION};  // + or -
  int side_{LEFT_SIDE};
  double base_sensitivity_{SENSOR_SENSITIVITY};
  double coefficient_sensitivity_{DISTANCE_SENSITIVITY};
};

NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_H_
