/**
 * @file motion_handler_robot.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/motion_handler_robot.h"
#include "src/motion_behavior_differential.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
// @TODO add clamped (done)

void MotionHandlerRobot::TurnLeft() {
  set_velocity(
    get_velocity().left  - get_angle_delta(),
    get_velocity().right + get_angle_delta());
}

void MotionHandlerRobot::TurnRight() {
  set_velocity(
    get_velocity().left  + get_angle_delta(),
    get_velocity().right - get_angle_delta());
}

void MotionHandlerRobot::IncreaseSpeed() {
  set_velocity(
    clamp_vel(get_velocity().left  + get_speed_delta()),
    clamp_vel(get_velocity().right + get_speed_delta()));
}

void MotionHandlerRobot::DecreaseSpeed() {
  set_velocity(
    clamp_vel(get_velocity().left  - get_speed_delta()),
    clamp_vel(get_velocity().right - get_speed_delta()));
}

void MotionHandlerRobot::SetSpeed(double lv, double rv) {
  set_velocity(clamp_vel(lv), clamp_vel(rv));
}

void MotionHandlerRobot::UpdateLeftWheel(double lv) {
  velocity_.left = clamp_vel(lv);
}

void MotionHandlerRobot::UpdateRightWheel(double rv) {
  velocity_.right = clamp_vel(rv);
}

void MotionHandlerRobot::UpdateVelocity() {
  /*
  if (entity_->get_touch_sensor()->get_output()) {
    entity_->RelativeChangeHeading(+180);
  }
  */

  if (entity_->get_old_angle() > 0) {
    if ((entity_->get_pose().theta - entity_->get_old_angle()) > 45) {
      entity_->set_old_angle(0);
      SetSpeed(1, 1);
    }
  }
}

double MotionHandlerRobot::clamp_vel(double vel) {
  // @TODO: don't go backwards (done)
  double clamped = 0.0;
  if (vel > 0) {
    clamped = (vel > get_max_speed()) ?
              get_max_speed():
              vel;
  } else {
    clamped = (vel < -get_max_speed()) ?
              -get_max_speed():
              vel;
  }
  return clamped;
} /* clamp_vel() */

NAMESPACE_END(csci3081);
