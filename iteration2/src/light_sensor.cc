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

}

void LightSensor::Update(const State state) {
  state_ = state;
}

NAMESPACE_END(csci3081);
