/**
 * @file light_sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_LIGHT_SENSOR_H_
#define SRC_LIGHT_SENSOR_H_

#include "src/sensor.h"
#include "src/subject.h"
#include "src/entity_type.h"
#include <cmath>
#include <iostream>

#define OFF_ANGLE 40
#define MAX_READ 1000

NAMESPACE_BEGIN(csci3081);

class LightSensor : public Sensor {
public:
  LightSensor(Subject *s) : Sensor(s) {}
  LightSensor(Subject *s, int wType, int cType);
  void Update(const State state) override;
  void calculateReading() override;
  void UpdatePosition() override;
};

NAMESPACE_END(csci3081);

#endif
