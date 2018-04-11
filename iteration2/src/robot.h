/**
 * @file robot.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <time.h>
#include <string>
#include <vector>
#include "src/arena_mobile_entity.h"
#include "src/common.h"
#include "src/motion_handler_robot.h"
#include "src/motion_behavior_differential.h"
#include "src/entity_type.h"
#include "src/sensor.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

class Sensor;
/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing a robot within the arena.
 *
 * Robots are composed of a motion handler, motion behavior, and touch sensor.
 * These classes interact to maintain the pose (position and heading) of the
 * robot. At each time step, the wheel velocities are used to calculate the
 * next pose of the robot. The handler manages the pose and user requests. The
 * behavior calculates the new pose Foodd on wheel velocities.
 *
 * Robots can be controlled through keypress, which modify wheel velocities.
 *
 * The touch sensor is activated when the robot collides with an object.
 * The heading is modified after a collision to move the robot away from the
 * other object.
 */
class Robot : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor using initialization values from params.h.
   */
  Robot();

  /**
   * @brief Reset the Robot to a newly constructed state (needed for reset
   * button to work in GUI).
   */
  void Reset() override;

  /**
   * @brief Update the Robot's position and velocity after the specified
   * duration has passed.
   *
   * @param dt The # of timesteps that have elapsed since the last update.
   */
  void TimestepUpdate(unsigned int dt) override;

  /**
   * @brief Handles the collision by setting the sensor to activated.
   */
  void HandleCollision(EntityType object_type,
             ArenaEntity * object = NULL) override;

  /**
   * @brief Get the name of the Robot for visualization and for debugging.
   */
  std::string get_name() const override { return name_; }
  /**
   * @brief set the name of the robot
   *
   * @param[name] new name
   */
  void set_name(std::string name) { name_ = name; }

  /**
  * @brief Command that comes from the controller, then is passed to handler.
  */
  void SetSpeed(double lv, double rv);

  /**
   * @brief udpate left wheel of the robot
   *
   * @param[lv] new left wheel velocity
   */
  void UpdateLeftWheel(double lv);

  /**
   * @brief udpate right wheel of the robot
   *
   * @param[rv] new right wheel velocity
   */
  void UpdateRightWheel(double rv);

  /**
   * @brief get motion handler of the robot
   *
   * @return motion handler of the robot
   */
  MotionHandlerRobot get_motion_handler() { return motion_handler_; }

  /**
   * @brief get motion behavior of the robot
   *
   * @return motion behavior of the robot
   */
  MotionBehaviorDifferential get_motion_behavior() { return motion_behavior_; }

  /**
   * @brief turn off food sensor
   */
  void turn_food_sensor_off() { food_sensor_status_ = OFF; }

  /**
   * @brief turn on food sensor
   */
  void turn_food_sensor_on() { food_sensor_status_ = ON; }

  /**
   * @brief turn off light sensor
   */
  void turn_light_sensor_off() { food_sensor_status_ = OFF; }

  /**
   * @brief turn on light sensor
   */
  void turn_light_sensor_on() { food_sensor_status_ = ON; }

  /**
   * @brief get food sensor status
   *
   * @return current status of food sensor
   */
  int get_food_sensor_status() { return food_sensor_status_; }

  /**
   * @brief get light sensor status
   *
   * @return current status of light sensor
   */
  int get_light_sensor_status() { return light_sensor_status_; }

  /**
   * @brief add new sensor to a sensor list
   *
   * @param[s] new pointer to a sensor
   */
  void add_sensor(Sensor *s) { sensor_list_.push_back(s); }

  /**
   * @brief get starving status
   *
   * @return current starving status
   */
  bool isStarving() { return starving_; }

  /**
   * @brief get hunger time
   *
   * @return current hunger time
   */
  int get_hunger_time() const { return hunger_time_; }

  /**
   * @brief reset hunger time to 0
   */
  void reset_hunger_time();

 private:
  // Manages pose and wheel velocities that change with time and collisions.
  MotionHandlerRobot motion_handler_;

  // Calculates changes in pose Foodd on elapsed time and wheel velocities.
  MotionBehaviorDifferential motion_behavior_;

  // Name of the robot
  std::string name_{"R"};

  // store a list of all types of sensor
  std::vector<Sensor *> sensor_list_;

  // food sensor status
  int food_sensor_status_{ON};

  // light sensor status
  int light_sensor_status_{ON};

  // time to the next hungry moment
  int hunger_time_{0};

  // the beginning of hungry interval
  clock_t hunger_start_point_{0};

  // the end of hungry interval
  clock_t hunger_end_point_{0};

  // starving status of the robot
  bool starving_{false};
};

NAMESPACE_END(csci3081);

#endif  // SRC_ROBOT_H_
