/**
 * @file robot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/robot.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Robot::Robot() :
    motion_handler_(this),
    motion_behavior_(this),
    lives_(9),
    isInvincible_(false),
    invi_dt_(0),
    sensor_list_() {
  set_type(kRobot);
  set_color(ROBOT_COLOR);
  set_pose(ROBOT_INIT_POS);
  set_radius(ROBOT_RADIUS);
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Robot::TimestepUpdate(unsigned int dt) {
  // Update heading as indicated by touch sensor
  motion_handler_.UpdateVelocity();

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());

  // Update the position for all sensors attached to the robot
  for(Sensor *s : sensor_list_) {
    s->UpdatePosition();
  }
} /* TimestepUpdate() */

void Robot::Reset() {
  set_pose(ROBOT_INIT_POS);
  motion_handler_.set_max_speed(ROBOT_MAX_SPEED);
  motion_handler_.set_max_angle(ROBOT_MAX_ANGLE);
  sensor_touch_->Reset();
} /* Reset() */

void Robot::HandleCollision(EntityType object_type, ArenaEntity * object) {
  if (get_old_angle() > 0) {
    SetSpeed(1, 1);
    set_old_angle(0);
  } else if (object_type != kLight) {
    SetSpeed(-1, -1.2);
    old_angle_ = pose_.theta;
  }

  if (false)
    sensor_touch_->HandleCollision(object_type, object);

  /*
  if (object_type != kLight)
    sensor_touch_->HandleCollision(object_type, object);
    */
    /*
  if (!isInvincible_) {
    if (object_type != kBase) {
      isInvincible_ = true;
      invi_dt_ = 0;
    }
  }
  */
}

void Robot::IncreaseSpeed() {
  motion_handler_.IncreaseSpeed();
}
void Robot::DecreaseSpeed() {
  motion_handler_.DecreaseSpeed();
}
void Robot::TurnRight() {
  motion_handler_.TurnRight();
}
void Robot::TurnLeft() {
  motion_handler_.TurnLeft();
}

void Robot::SetSpeed(double lv, double rv) {
  motion_handler_.SetSpeed(lv, rv);
}

void Robot::UpdateLeftWheel(double lv) {
  motion_handler_.UpdateLeftWheel(lv);
}
void Robot::UpdateRightWheel(double rv) {
  motion_handler_.UpdateRightWheel(rv);
}

NAMESPACE_END(csci3081);
