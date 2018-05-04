/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "src/arena.h"
#include "src/arena_params.h"
#include "src/arena_entity.h"
#include "src/robot.h"
#include "src/sensor.h"
#include "src/light_sensor.h"
#include "src/params.h"
#include "src/state.h"

#ifdef LIGHT_SENSOR_TESTS

NAMESPACE_BEGIN(csci3081);

 /******************************************************
* TEST FEATURE SetUp
*******************************************************/
class LightSensorTest : public ::testing::Test {

 protected:
  virtual void SetUp() {
    // Initialize light sensor for Collections of TESTS
    struct arena_params params;
    Arena * arena = new Arena(&params);

    factory = new EntityFactory(arena);
    robot = dynamic_cast<Robot*>(factory->CreateEntity(kFearRobot));

    left_sensor = dynamic_cast<LightSensor*>(robot->get_sensor_list()[2]);
    right_sensor = dynamic_cast<LightSensor*>(robot->get_sensor_list()[3]);

    light = new Light;
    state = State(light);
  }

  Sensor * sensor;
  LightSensor *left_sensor;
  LightSensor *right_sensor;
  ArenaEntity *entity;
  Robot *robot;
  EntityFactory *factory;
  State state;
  Light * light;
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/
TEST_F(LightSensorTest, Constructor) {
  /*
   * Following the Observer pattern, the arena would be a subject and the sensor
     would be an observer. Create an arena first so that the sensor can register
     itselt to the arena subject.
   * The second parameter is the left/right wheel that the sensor is connecting to
   * The last parameter is the type of connection plus (increase velocity) or
     minus (decrease velocity)
   */
  struct csci3081::arena_params params;
  csci3081::Arena * arena = new csci3081::Arena(&params);
  sensor = new csci3081::LightSensor(arena, RIGHT_WHEEL, MINUS_CONNECTION);

  // check if the constructor assigns subject_ correctly
  EXPECT_EQ(sensor->get_subject(), arena) << "\nFAIL not assigning to subject_";

  // check if the constructor assigns wheel_type_ correctly
  EXPECT_EQ(sensor->get_wheel_type(), RIGHT_WHEEL) << "\nFAIL not assigning to wheel_type_";

  // check if the constructor assigns connection_type_ correctly
  EXPECT_EQ(sensor->get_connection_type(), MINUS_CONNECTION) << "\nFAIL not assigning to connection_type_";
}

TEST_F(LightSensorTest, DefaultConstructor) {
  /*
   * Following the Observer pattern, the arena would be a subject and the sensor
     would be an observer. Create an arena first so that the sensor can register
     itselt to the arena subject.
   * The default constructor does not need the type of wheel the sensor is
     connecting to or the type of connection (+/-)
   */
  struct arena_params params;
  csci3081::Arena * arena = new Arena(&params);
  sensor = new LightSensor(arena);

  // check if the default constructor assigns subject_ correctly
  EXPECT_EQ(sensor->get_subject(), arena) << "\nFAIL not assigning to subject_";

  // The default constructor will assign LEFT_WHEEL to wheel_type_
  // check if the default constructor assigns wheel_type_ correctly
  EXPECT_EQ(sensor->get_wheel_type(), LEFT_WHEEL) << "\nFAIL not assigning to wheel_type_";

  // The default constructor will assign PLUS_CONNECTION to connection_type_
  // check if the default constructor assigns connection_type_ correctly
  EXPECT_EQ(sensor->get_connection_type(), PLUS_CONNECTION) << "\nFAIL not assigning to connection_type_";
}

TEST_F(LightSensorTest, StateGetterSetter) {
  State state;
  left_sensor->set_state(state);
  EXPECT_EQ(left_sensor->get_state().ent_, state.ent_) << "\nFAIL not assigning to state_";
}

TEST_F(LightSensorTest, ReadingGetterSetter) {
  left_sensor->set_reading(205.94);
  EXPECT_EQ(left_sensor->get_reading(), 205.94) << "\nFAIL getter/setter for reading fails";
}

TEST_F(LightSensorTest, Update) {
  // Update method will update the member state_ of the sensor to the new state
  left_sensor->Update(state);
  EXPECT_EQ(left_sensor->get_state().ent_, state.ent_) << "\nFAIL not assigning to state_";
}

TEST_F(LightSensorTest, CalculateReading) {
  // calculate the reading based on the current state
  // state will contain the position of the stimuli
  // reading ranges form 0 to 1000
  Light *l = new Light;
  State *s = new State(l);

  Robot *robot = new Robot;
  right_sensor->set_host(robot);

  // test the case when the robot and the stimuli is at normal distance
  robot->set_position(100, 100);
  l->set_position(400, 400);
  right_sensor->Update(*s);
  right_sensor->calculateReading();
  EXPECT_LE(right_sensor->get_reading(), 1000) << "\nFAIL reading exceeds 1000 limit at normal distance";
  EXPECT_GE(right_sensor->get_reading(), 0) << "\nFAIL reading exceeds 0 limit at normal distance";

  // test the case when the robot and the stimuli is very far away (boundary case)
  robot->set_position(0,0);
  l->set_position(500,500);
  right_sensor->Update(*s);
  right_sensor->calculateReading();
  EXPECT_LE(right_sensor->get_reading(), 1000) << "\nFAIL reading exceeds 1000 limit at far distance";
  EXPECT_GE(right_sensor->get_reading(), 0) << "\nFAIL reading exceeds 0 limit at far distance";

  // test the case when the robot and the stimuli is very close to each other (boundary case)
  robot->set_position(200, 200);
  l->set_position(250, 250);
  right_sensor->Update(*s);
  right_sensor->calculateReading();
  EXPECT_LE(right_sensor->get_reading(), 1000) << "\nFAIL reading exceeds 1000 limit at close distance";
  EXPECT_GE(right_sensor->get_reading(), 0) << "\nFAIL reading exceeds 0 limit at close distance";

  // create an additional stimuli for testing
  Light *l2 = new Light;
  State *s2 = new State(l2);

  robot->set_position(200, 200);
  // set the position of the first stimuli. It is at normal distance to the robot
  l->set_position(400, 400);
  right_sensor->Update(*s);
  right_sensor->calculateReading();

  // set the position for the second stimuli. It will be nearer to the robot than the first stimuli
  l2->set_position(250, 250);
  right_sensor->Update(*s2);
  right_sensor->calculateReading();
  EXPECT_LE(right_sensor->get_reading(), 1000) << "\nFAIL reading exceeds 1000 limit with 2 stimulus";
  EXPECT_GE(right_sensor->get_reading(), 0) << "\nFAIL reading exceeds 0 limit with 2 stimulus";
}

TEST_F(LightSensorTest, CloseLight) {
  // calculate the reading based on the current state
  // state will contain the position of the stimuli
  // reading ranges form 0 to 1000
  Light *light = new Light;
  State *s = new State(light);

  // test the case when the robot and the stimuli is at normal distance
  robot->set_position(100, 100);
  light->set_position(150, 150);
  left_sensor->Update(*s);

  EXPECT_LE(left_sensor->get_reading(), 1000) << "\nFAIL reading exceeds 1000 limit at close distance";
  EXPECT_GE(left_sensor->get_reading(), 0) << "\nFAIL reading exceeds 0 limit at close distance";
}

TEST_F(LightSensorTest, FarLight) {
  // calculate the reading based on the current state
  // state will contain the position of the stimuli
  // reading ranges form 0 to 1000
  Light *light = new Light;
  State *s = new State(light);

  // test the case when the robot and the stimuli is at normal distance
  robot->set_position(50, 50);
  light->set_position(700, 700);
  left_sensor->Update(*s);

  EXPECT_LE(left_sensor->get_reading(), 1000) << "\nFAIL reading exceeds 1000 limit at far distance";
  EXPECT_GE(left_sensor->get_reading(), 0) << "\nFAIL reading exceeds 0 limit at far distance";
}

TEST_F(LightSensorTest, OneFarOneNear) {
  // calculate the reading based on the current state
  // state will contain the position of the stimuli
  // reading ranges form 0 to 1000
  // test the case when the robot and the stimuli is at normal distance
  robot->set_position(50, 50);
  left_sensor->UpdatePosition();

  Light *light1 = new Light;
  light1->set_position(100, 100);
  State *s1 = new State(light1);
  left_sensor->Update(*s1);
  double old_reading = left_sensor->get_reading();

  Light *light2 = new Light;
  light2->set_position(700, 700);
  State *s2 = new State(light2);
  left_sensor->Update(*s2);
  double new_reading = left_sensor->get_reading();

  EXPECT_LE(old_reading, 1000) << "\nFAIL reading exceeds 1000 limit at close distance";
  EXPECT_GE(old_reading, 0) << "\nFAIL reading exceeds 0 limit at close distance";

  EXPECT_LE(new_reading, 1000) << "\nFAIL reading exceeds 1000 limit at far distance";
  EXPECT_GE(new_reading, 0) << "\nFAIL reading exceeds 0 limit at far distance";

  EXPECT_NE(old_reading, new_reading) << "\nFAIL reading does not change with 2 stimulus";
}

TEST_F(LightSensorTest, TwoNearLights) {
  // calculate the reading based on the current state
  // state will contain the position of the stimuli
  // reading ranges form 0 to 1000
  // test the case when the robot and the stimuli is at normal distance
  robot->set_position(50, 50);

  Light *light1 = new Light;
  light1->set_position(100, 100);
  State *s1 = new State(light1);
  left_sensor->Update(*s1);
  double old_reading = left_sensor->get_reading();

  Light *light2 = new Light;
  light2->set_position(101, 101);
  State *s2 = new State(light2);
  left_sensor->Update(*s2);
  double new_reading = left_sensor->get_reading();

  EXPECT_LE(old_reading, 1000) << "\nFAIL reading exceeds 1000 limit at close distance";
  EXPECT_GE(old_reading, 0) << "\nFAIL reading exceeds 0 limit at close distance";

  EXPECT_LE(new_reading, 1000) << "\nFAIL reading exceeds 1000 limit at close distance";
  EXPECT_GE(new_reading, 0) << "\nFAIL reading exceeds 0 limit at close distance";

  EXPECT_NE(old_reading, new_reading) << "\nFAIL reading does not change with 2 close stimulus";
}

TEST_F(LightSensorTest, TwoFarLights) {
  // calculate the reading based on the current state
  // state will contain the position of the stimuli
  // reading ranges form 0 to 1000
  // test the case when the robot and the stimuli is at normal distance
  robot->set_position(50, 50);

  Light *light1 = new Light;
  light1->set_position(700, 700);
  State *s1 = new State(light1);
  left_sensor->Update(*s1);
  double old_reading = left_sensor->get_reading();

  Light *light2 = new Light;
  light2->set_position(702, 702);
  State *s2 = new State(light2);
  left_sensor->Update(*s2);
  double new_reading = left_sensor->get_reading();

  EXPECT_LE(old_reading, 1000) << "\nFAIL reading exceeds 1000 limit at far distance";
  EXPECT_GE(old_reading, 0) << "\nFAIL reading exceeds 0 limit at far distance";

  EXPECT_LE(new_reading, 1000) << "\nFAIL reading exceeds 1000 limit at far distance";
  EXPECT_GE(new_reading, 0) << "\nFAIL reading exceeds 0 limit at far distance";

  EXPECT_NE(old_reading, new_reading) << "\nFAIL reading does not change with 2 far stimulus";
}

TEST_F(LightSensorTest, TwoLightsWithEqualDistance) {
  // calculate the reading based on the current state
  // state will contain the position of the stimuli
  // reading ranges form 0 to 1000
  // test the case when the robot and the stimuli is at normal distance
  robot->set_position(50, 50);

  Light *light1 = new Light;
  light1->set_position(100, 80);
  State *s1 = new State(light1);
  left_sensor->Update(*s1);
  double old_reading = left_sensor->get_reading();

  Light *light2 = new Light;
  light2->set_position(100, 20);
  State *s2 = new State(light2);
  left_sensor->Update(*s2);
  double new_reading = left_sensor->get_reading();

  EXPECT_LE(old_reading, 1000) << "\nFAIL reading exceeds 1000 limit at normal distance";
  EXPECT_GE(old_reading, 0) << "\nFAIL reading exceeds 0 limit at normal distance";

  EXPECT_LE(new_reading, 1000) << "\nFAIL reading exceeds 1000 limit at normal distance";
  EXPECT_GE(new_reading, 0) << "\nFAIL reading exceeds 0 limit at normal distance";

  EXPECT_NE(old_reading, new_reading) << "\nFAIL reading does not change with 2 normal stimulus";
}

NAMESPACE_END(csci3081);

#endif /* LIGHT_SENSOR_TESTS */
