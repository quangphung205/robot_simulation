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
class WheelVelocityTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    // Initialize light sensor
    struct arena_params params;
    Arena * arena = new Arena(&params);
    factory = new EntityFactory(arena);

    fear_robot = dynamic_cast<Robot*>(factory->CreateEntity(kFearRobot));
    explore_robot = dynamic_cast<Robot*>(factory->CreateEntity(kExploreRobot));

    left_fear = dynamic_cast<LightSensor*>(fear_robot->get_sensor_list()[2]);
    right_fear = dynamic_cast<LightSensor*>(fear_robot->get_sensor_list()[3]);

    left_explore = dynamic_cast<LightSensor*>(explore_robot->get_sensor_list()[2]);
    right_explore = dynamic_cast<LightSensor*>(explore_robot->get_sensor_list()[3]);

    fear_mhb = fear_robot->get_motion_handler();
    explore_mhb = explore_robot->get_motion_handler();

    state = State(new Food());

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

  EntityFactory *factory;
  Robot *fear_robot;
  Robot *explore_robot;
  LightSensor *left_fear;
  LightSensor *right_fear;
  LightSensor *left_explore;
  LightSensor *right_explore;

  State state;

  MotionHandlerRobot fear_mhb = MotionHandlerRobot(NULL);
  MotionHandlerRobot explore_mhb = MotionHandlerRobot(NULL);

  MotionHandlerRobot mhb = MotionHandlerRobot(NULL);

  Sensor *left_sensor1;
  Sensor *right_sensor1;
  Robot *robot1;
  MotionHandlerRobot mhb1 = MotionHandlerRobot(NULL);

};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/
TEST_F(WheelVelocityTest, Constructor) {
  Robot *robot = dynamic_cast<Robot*>(factory->CreateEntity(kRobot));
  MotionHandlerRobot mhb = robot->get_motion_handler();
  EXPECT_EQ(mhb.get_velocity().left, 1) << "\nFAIL left wheel constructor fails";
  EXPECT_EQ(mhb.get_velocity().right, 1) << "\nFAIL right wheel constructor fails";
}

TEST_F(WheelVelocityTest, VelocitySetterGetter) {
  Robot *robot = dynamic_cast<Robot*>(factory->CreateEntity(kRobot));
  MotionHandlerRobot mhb = robot->get_motion_handler();
  mhb.set_velocity(2,4);
  EXPECT_EQ(mhb.get_velocity().left, 2) << "\nFAIL left wheel setter/getter fails";
  EXPECT_EQ(mhb.get_velocity().right, 4) << "\nFAIL right wheel setter/getter fails";
}

TEST_F(WheelVelocityTest, ZeroReading) {
  /*
   * The motion handler uses the reading from the sensor and determines how to
     change the correspoding wheel velocity that the sensor is connecting to
   */

  // test that the velocity should stay the same when the reading is 0 (when the stimuli
  // is very far away from the robot
  Robot *robot = dynamic_cast<Robot*>(factory->CreateEntity(kRobot));
  robot->SetSpeed(0,0);  // set the velocity of the robot
  MotionHandlerRobot mhb = robot->get_motion_handler();
  left_sensor->set_reading(0);  // set the reading to the left sensor
  left_sensor->Update(state);
  right_sensor->set_reading(0);  // set the reading to the right sensor
  right_sensor->Update(state);

  EXPECT_EQ(mhb.get_velocity().left, 0) << "\nFAIL left wheel velocity changed upon 0 reading";
  EXPECT_EQ(mhb.get_velocity().right, 0) << "\nFAIL right wheel velocity changed upon 0 reading";
}

TEST_F(WheelVelocityTest, FearRobotLeftLight) {
  /*
   * The motion handler uses the reading from the sensor and determines how to
     change the correspoding wheel velocity that the sensor is connecting to
   */
  fear_robot->SetSpeed(1,1);
  fear_robot->set_position(100,100);
  fear_robot->set_heading(0.0);
  Light *light = new Light();
  light->set_position(110, 30);
  State *s = new State(light);

  left_fear->UpdatePosition();
  left_fear->Update(*s);

  right_fear->UpdatePosition();
  right_fear->Update(*s);
  MotionHandlerRobot fear_mhb = fear_robot->get_motion_handler();


  EXPECT_LE(fear_mhb.get_velocity().left, ROBOT_MAX_SPEED) << "\nFAIL left velocity exceeds max limit";
  EXPECT_LE(fear_mhb.get_velocity().right, ROBOT_MAX_SPEED) << "\nFAIL right velocity exceeds max limit";
  EXPECT_GT(fear_mhb.get_velocity().left, fear_mhb.get_velocity().right) << "\nFAIL left wheel less than right wheel";
}

TEST_F(WheelVelocityTest, FearRobotRightLight) {
  /*
   * The motion handler uses the reading from the sensor and determines how to
     change the correspoding wheel velocity that the sensor is connecting to
   */
  fear_robot->SetSpeed(1,1);
  fear_robot->set_position(100,100);
  fear_robot->set_heading(0.0);

  Light *light = new Light();
  light->set_position(120, 200);
  State *s = new State(light);

  left_fear->UpdatePosition();
  left_fear->Update(*s);

  right_fear->UpdatePosition();
  right_fear->Update(*s);

  MotionHandlerRobot fear_mhb = fear_robot->get_motion_handler();

  EXPECT_LE(fear_mhb.get_velocity().left, ROBOT_MAX_SPEED) << "\nFAIL left velocity exceeds max limit";
  EXPECT_LE(fear_mhb.get_velocity().right, ROBOT_MAX_SPEED) << "\nFAIL right velocity exceeds max limit";
  EXPECT_LT(fear_mhb.get_velocity().left, fear_mhb.get_velocity().right) << "\nFAIL left wheel greater than right wheel";
}

TEST_F(WheelVelocityTest, FearRobotCenterLight) {
  /*
   * The motion handler uses the reading from the sensor and determines how to
     change the correspoding wheel velocity that the sensor is connecting to
   */
  fear_robot->SetSpeed(1,1);
  fear_robot->set_position(100,100);
  fear_robot->set_heading(0.0);
  Light *light = new Light();
  light->set_position(120, 100);
  State *s = new State(light);

  left_fear->UpdatePosition();
  left_fear->Update(*s);

  right_fear->UpdatePosition();
  right_fear->Update(*s);
  MotionHandlerRobot fear_mhb = fear_robot->get_motion_handler();


  EXPECT_LE(fear_mhb.get_velocity().left, ROBOT_MAX_SPEED) << "\nFAIL left velocity exceeds max limit";
  EXPECT_LE(fear_mhb.get_velocity().right, ROBOT_MAX_SPEED) << "\nFAIL right velocity exceeds max limit";
  EXPECT_EQ(fear_mhb.get_velocity().left, fear_mhb.get_velocity().right) << "\nFAIL left wheel not equal to right wheel";
}

TEST_F(WheelVelocityTest, ExploreRobotLeftLight) {
  /*
   * The motion handler uses the reading from the sensor and determines how to
     change the correspoding wheel velocity that the sensor is connecting to
   */
  explore_robot->SetSpeed(3,3);
  explore_robot->set_position(100,100);
  explore_robot->set_heading(0.0);
  Light *light = new Light();
  light->set_position(110, 30);
  State *s = new State(light);

  left_explore->UpdatePosition();
  left_explore->Update(*s);

  right_explore->UpdatePosition();
  right_explore->Update(*s);
  MotionHandlerRobot explore_mhb = explore_robot->get_motion_handler();


  EXPECT_LE(explore_mhb.get_velocity().left, ROBOT_MAX_SPEED) << "\nFAIL left velocity exceeds max limit";
  EXPECT_LE(explore_mhb.get_velocity().right, ROBOT_MAX_SPEED) << "\nFAIL right velocity exceeds max limit";
  EXPECT_GT(explore_mhb.get_velocity().left, explore_mhb.get_velocity().right) << "\nFAIL left wheel less than right wheel";
}

TEST_F(WheelVelocityTest, ExploreRobotRightLight) {
  /*
   * The motion handler uses the reading from the sensor and determines how to
     change the correspoding wheel velocity that the sensor is connecting to
   */
  explore_robot->SetSpeed(3,3);
  explore_robot->set_position(100,100);
  explore_robot->set_heading(0.0);

  Light *light = new Light();
  light->set_position(120, 200);
  State *s = new State(light);

  left_explore->UpdatePosition();
  left_explore->Update(*s);

  right_explore->UpdatePosition();
  right_explore->Update(*s);

  MotionHandlerRobot explore_mhb = explore_robot->get_motion_handler();

  EXPECT_LE(explore_mhb.get_velocity().left, ROBOT_MAX_SPEED) << "\nFAIL left velocity exceeds max limit";
  EXPECT_LE(explore_mhb.get_velocity().right, ROBOT_MAX_SPEED) << "\nFAIL right velocity exceeds max limit";
  EXPECT_LT(explore_mhb.get_velocity().left, explore_mhb.get_velocity().right) << "\nFAIL left wheel greater than right wheel";
}

TEST_F(WheelVelocityTest, ExploreRobotCenterLight) {
  /*
   * The motion handler uses the reading from the sensor and determines how to
     change the correspoding wheel velocity that the sensor is connecting to
   */
  explore_robot->SetSpeed(3,3);
  explore_robot->set_position(100,100);
  explore_robot->set_heading(0.0);
  Light *light = new Light();
  light->set_position(120, 100);
  State *s = new State(light);

  left_explore->UpdatePosition();
  left_explore->Update(*s);

  right_explore->UpdatePosition();
  right_explore->Update(*s);
  MotionHandlerRobot explore_mhb = explore_robot->get_motion_handler();


  EXPECT_LT(explore_mhb.get_velocity().left, ROBOT_MAX_SPEED) << "\nFAIL left velocity exceeds max limit";
  EXPECT_LT(explore_mhb.get_velocity().right, ROBOT_MAX_SPEED) << "\nFAIL right velocity exceeds max limit";
  EXPECT_EQ(explore_mhb.get_velocity().left, explore_mhb.get_velocity().right) << "\nFAIL left wheel not equal to right wheel";
}

NAMESPACE_END(csci3081);

#endif /* MOTION_BEHAVIOR_TESTS */
