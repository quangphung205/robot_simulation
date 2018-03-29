/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include "src/arena.h"
#include "src/arena_params.h"
#include "src/arena_entity.h"
#include "src/robot.h"
#include "src/sensor.h"
#include "src/light_sensor.h"
#include "src/params.h"
#include "src/state.h"

#ifdef MOTION_BEHAVIOR_TESTS

NAMESPACE_BEGIN(csci3081);

 /******************************************************
* TEST FEATURE SetUp
*******************************************************/
class MotionBehaviorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    // Initialize light sensor
    struct arena_params params;
    Arena * arena = new Arena(&params);

    // This is a +/direct robot which means left sensor connects to left wheel
    // and changes its velocity positively, right sensor connects to right wheel
    // and changes its velocity negatively
    left_sensor = new LightSensor(arena, LEFT_WHEEL, PLUS_CONNECTION);
    right_sensor = new LightSensor(arena, RIGHT_WHEEL, PLUS_CONNECTION);
    robot = new Robot;
    left_sensor->set_host(robot);  // set a robot as a host object to each sensor
    right_sensor->set_host(robot);
    robot->add_sensor(left_sensor);  // add each sensor to the robot
    robot->add_sensor(right_sensor);
    mhb = robot->get_motion_handler();  // get robot motion handler for testing

    // This is a -/direct robot which means left sensor connects to left wheel
    // and changes its velocity negatively, right sensor connects to right wheel
    // and changes its velocity positively
    left_sensor1 = new LightSensor(arena, LEFT_WHEEL, MINUS_CONNECTION);
    right_sensor1 = new LightSensor(arena, RIGHT_WHEEL, MINUS_CONNECTION);
    robot1 = new Robot;
    left_sensor1->set_host(robot1);  // set a robot as a host object to each sensor
    right_sensor1->set_host(robot1);
    robot1->add_sensor(left_sensor1);  // add each sensor to the robot
    robot1->add_sensor(right_sensor1);
    mhb1 = robot1->get_motion_handler();  // get robot motion handler for testing
  }

  Sensor *left_sensor;
  Sensor *right_sensor;
  Robot *robot;
  MotionHandlerRobot mhb = MotionHandlerRobot(NULL);

  Sensor *left_sensor1;
  Sensor *right_sensor1;
  Robot *robot1;
  MotionHandlerRobot mhb1 = MotionHandlerRobot(NULL);

};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/
TEST_F(MotionBehaviorTest, UpdateVelocity) {
  /*
   * The motion handler uses the reading from the sensor and determines how to
     change the correspoding wheel velocity that the sensor is connecting to
   */

  // test that the velocity should stay the same when the reading is 0 (when the stimuli
  // is very far away from the robot
  robot->SetSpeed(0,0);  // set the velocity of the robot
  left_sensor->set_reading(0);  // set the reading to the left sensor
  right_sensor->set_reading(0);  // set the reading to the right sensor
  EXPECT_EQ(mhb.get_velocity().left, 0) << "\nFAIL left wheel velocity changed upon 0 reading";
  EXPECT_EQ(mhb.get_velocity().right, 0) << "\nFAIL right wheel velocity changed upon 0 reading";

  // test that the velocity for both wheel should increase upon reading since
  // this is a +/direct robot
  robot->SetSpeed(5,5);  // set the velocity of the robot
  left_sensor->set_reading(100);  // set the reading to the left sensor
  right_sensor->set_reading(100);  // set the reading to the right sensor
  EXPECT_GT(mhb.get_velocity().left, 5) << "\nFAIL left wheel velocity doesn't change correctly upon reading";
  EXPECT_GT(mhb.get_velocity().right, 5) << "\nFAIL right wheel velocity doesn't change correctly upon reading";

  // test that the left wheel velocity should be less than the right wheel velocity
  // when the left sensor reading is smaller than the right one (for +/direct robot)
  robot->SetSpeed(5,5);  // set the velocity of the robot
  left_sensor->set_reading(100);  // set the reading to the left sensor
  right_sensor->set_reading(200);  // set the reading to the right sensor
  EXPECT_GT(mhb.get_velocity().left, 5) << "\nFAIL left wheel velocity doesn't change correctly upon reading";
  EXPECT_GT(mhb.get_velocity().right, 5) << "\nFAIL right wheel velocity doesn't change correctly upon reading";
  EXPECT_LT(mhb.get_velocity().left, mhb.get_velocity().right) << "\nFAIL left_velocity >= right_velocity whild left_reading < right_reading";

  // test that the left wheel velocity should be greater than the right wheel velocity
  // when the left sensor reading is greater than the right one (for +/direct robot)
  robot->SetSpeed(5,5);  // set the velocity of the robot
  left_sensor->set_reading(300);  // set the reading to the left sensor
  right_sensor->set_reading(200);  // set the reading to the right sensor
  EXPECT_GT(mhb.get_velocity().left, 5) << "\nFAIL left wheel velocity doesn't change correctly upon reading";
  EXPECT_GT(mhb.get_velocity().right, 5) << "\nFAIL right wheel velocity doesn't change correctly upon reading";
  EXPECT_GT(mhb.get_velocity().left, mhb.get_velocity().right) << "\nFAIL left_velocity <= right_velocity whild left_reading > right_reading";




  // test that the velocity should stay the same when the reading is 0 (when the stimuli
  // is very far away from the robot
  robot1->SetSpeed(0,0);  // set the velocity of the robot
  left_sensor1->set_reading(0);  // set the reading to the left sensor
  right_sensor1->set_reading(0);  // set the reading to the right sensor
  EXPECT_EQ(mhb1.get_velocity().left, 0) << "\nFAIL left wheel velocity changed upon 0 reading";
  EXPECT_EQ(mhb1.get_velocity().right, 0) << "\nFAIL right wheel velocity changed upon 0 reading";

  // test that the velocity for both wheel should decrease upon reading since
  // this is a -/direct robot
  robot1->SetSpeed(5,5);  // set the velocity of the robot
  left_sensor1->set_reading(100);  // set the reading to the left sensor
  right_sensor1->set_reading(100);  // set the reading to the right sensor
  EXPECT_LT(mhb1.get_velocity().left, 5) << "\nFAIL left wheel velocity doesn't change correctly upon reading";
  EXPECT_LT(mhb1.get_velocity().right, 5) << "\nFAIL right wheel velocity doesn't change correctly upon reading";

  // test that the left wheel velocity should be greater than the right wheel velocity
  // when the left sensor reading is smaller than the right one (for -/direct robot)
  robot1->SetSpeed(5,5);  // set the velocity of the robot
  left_sensor1->set_reading(100);  // set the reading to the left sensor
  right_sensor1->set_reading(200);  // set the reading to the right sensor
  EXPECT_LT(mhb1.get_velocity().left, 5) << "\nFAIL left wheel velocity doesn't change correctly upon reading";
  EXPECT_LT(mhb1.get_velocity().right, 5) << "\nFAIL right wheel velocity doesn't change correctly upon reading";
  EXPECT_GT(mhb1.get_velocity().left, mhb1.get_velocity().right) << "\nFAIL left_velocity <= right_velocity whild left_reading < right_reading";

  // test that the left wheel velocity should be less than the right wheel velocity
  // when the left sensor reading is greater than the right one (for -/direct robot)
  robot1->SetSpeed(5,5);  // set the velocity of the robot
  left_sensor1->set_reading(300);  // set the reading to the left sensor
  right_sensor1->set_reading(200);  // set the reading to the right sensor
  EXPECT_LT(mhb1.get_velocity().left, 5) << "\nFAIL left wheel velocity doesn't change correctly upon reading";
  EXPECT_LT(mhb1.get_velocity().right, 5) << "\nFAIL right wheel velocity doesn't change correctly upon reading";
  EXPECT_LT(mhb1.get_velocity().left, mhb1.get_velocity().right) << "\nFAIL left_velocity >= right_velocity whild left_reading > right_reading";
}

NAMESPACE_END(csci3081);

#endif /* MOTION_BEHAVIOR_TESTS */
