/**
 * @file light_sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_LIGHT_SENSOR_H_
#define SRC_LIGHT_SENSOR_H_

#include <cmath>
#include <iostream>
#include "src/sensor.h"
#include "src/subject.h"
#include "src/entity_type.h"

#define OFF_ANGLE 40
#define MAX_READ 1000

NAMESPACE_BEGIN(csci3081);
/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class represents light sensor for the Robot
 */
class LightSensor : public Sensor {
 public:
  /**
   * @brief Default constructor
   */
  explicit LightSensor(Subject *s) : Sensor(s) {}

  /**
   * @brief Default constructor
   */
  LightSensor(Subject *s, int wheel_type, int connect_type);

  /**
   * @brief udpate the state of the sensor
   *
   * @param state new state
   */
  void Update(const State state) override;

  /**
   * @brief calculate the reading
   */
  void calculateReading() override;

  /**
   * @brief udpate the position of the sensor based on the robot position
   */
  void UpdatePosition() override;
};

NAMESPACE_END(csci3081);

#endif  // SRC_LIGHT_SENSOR_H_
