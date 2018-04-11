/**
 * @file wheel_velocity.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_WHEEL_VELOCITY_H_
#define SRC_WHEEL_VELOCITY_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A simple representation of the position/orientation of an entity
 * within the Arena.
 *
 * NOTE: Origin (0,0) is at the upper left corner of the Arena.
 */
struct WheelVelocity {
 public:
  /**
   * @brief Default constructor.
   */
  WheelVelocity()
    : left(STARTING_VELOCITY), right(STARTING_VELOCITY) {}

  /**
   * @brief Constructor
   *
   * @param[l] left volecity
   * @param[r] right velocity
   */
  WheelVelocity(double l, double r) : left(l), right(r) {}


  /**
   * @brief Default assignment operator. Simply copies the (x,y) values of
   * another Pose.
   *
   * @param other The Pose object to copy from.
   *
   * @return The left-hand-side Pose object that is now identical (in value)
   * to `other`.
   */
  WheelVelocity &operator=(const WheelVelocity &other) = default;

  double left;  // left velocity wheel
  double right;  // right velocity wheel
};

NAMESPACE_END(csci3081);

#endif  // SRC_WHEEL_VELOCITY_H_
