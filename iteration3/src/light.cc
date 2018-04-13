/**
 * @file light.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/light.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Light::Light() :
  motion_handler_(this), motion_behavior_(this) {
  set_color(LIGHT_COLOR);
  set_pose(LIGHT_POSITION);
  set_radius(LIGHT_RADIUS);
  set_type(kLight);
  set_velocity(1.0, 1.0);
}

void Light::set_velocity(double lv, double rv) {
  motion_handler_.set_velocity(lv, rv);
}

void Light::TimestepUpdate(unsigned int dt) {
  /*
  if (backing_up_dt_ == 80) {
    RelativeChangeHeading(+225);
    backing_up_dt_ = 0;
  }
  if (backing_up_dt_ > 0)
    backing_up_dt_++;
    */
  // Update heading as indicated by touch sensor
  motion_handler_.UpdateVelocity();
  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());
  // Reset Sensor for next cycle
  sensor_touch_->Reset();
} /* TimestepUpdate() */

void Light::HandleCollision(EntityType object_type, ArenaEntity * object) {
  if (get_old_angle() > 0) {
    motion_handler_.SetSpeed(1, 1);
    set_old_angle(0);
  } else if (object_type != kRobot) {
    motion_handler_.SetSpeed(-1, -1.2);
    old_angle_ = pose_.theta;
  }

  if (false)
    sensor_touch_->HandleCollision(object_type, object);
}

NAMESPACE_END(csci3081);
