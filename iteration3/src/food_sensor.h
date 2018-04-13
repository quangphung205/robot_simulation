/**
 * @file food_sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_FOOD_SENSOR_H_
#define SRC_FOOD_SENSOR_H_

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
 * @brief Class represents food sensor for the Robot
 */
class FoodSensor : public Sensor {
 public:
  /**
   * @brief Default constructor
   */
  explicit FoodSensor(Subject *s) : Sensor(s) {}

  /**
   * @brief Default constructor
   */
  FoodSensor(Subject *s, int wType, int cType);

  /**
   * @brief update the state of a sensor
   *
   * @param state new state
   */
  void Update(const State state) override;

  /**
   * @brief calculate the reading
   */
  void calculateReading() override;

  /**
   * @brief update the position of the sensor based on the robot position
   */
  void UpdatePosition() override;
};

NAMESPACE_END(csci3081);

#endif  // SRC_FOOD_SENSOR_H_
