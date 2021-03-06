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
    invi_dt_(0) {
  set_type(kRobot);
  set_color(ROBOT_COLOR);
  set_pose(ROBOT_INIT_POS);
  set_radius(ROBOT_RADIUS);
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Robot::TimestepUpdate(unsigned int dt) {
  if (isInvincible_) {
    RgbColor color;
    switch (invi_dt_) {
    case 0:
      color.Set(kGreen);
      set_color(color);
      break;
    case 40:
      color.Set(kPurple);
      set_color(color);
      break;
    case 80:
      color.Set(kBlue);
      set_color(color);
      break;
    default:
      break;
    }

    if (invi_dt_ == 80) {
      isInvincible_ = false;
      invi_dt_ = 0;
    }
    invi_dt_++;
  }
  // Update heading as indicated by touch sensor
  motion_handler_.UpdateVelocity();

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());

  // Reset Sensor for next cycle
  sensor_touch_->Reset();
} /* TimestepUpdate() */

void Robot::Reset() {
  set_pose(ROBOT_INIT_POS);
  motion_handler_.set_max_speed(ROBOT_MAX_SPEED);
  motion_handler_.set_max_angle(ROBOT_MAX_ANGLE);
  sensor_touch_->Reset();
} /* Reset() */

void Robot::HandleCollision(EntityType object_type, ArenaEntity * object) {
  if (object_type != kObstacle)
    sensor_touch_->HandleCollision(object_type, object);
  if (!isInvincible_) {
    if (object_type != kBase) {
      isInvincible_ = true;
      invi_dt_ = 0;
    }
  }
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

NAMESPACE_END(csci3081);
