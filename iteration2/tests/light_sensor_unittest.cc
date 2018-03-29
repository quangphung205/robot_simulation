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
    sensor1 = new LightSensor(arena);
    sensor2 = new LightSensor(arena);

    light = new Light;
    state = State(light);
  }

  Sensor * sensor;
  LightSensor * sensor1;
  LightSensor * sensor2;
  ArenaEntity * entity;
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

TEST_F(LightSensorTest, Update) {
  // Update method will update the member state_ of the sensor to the new state
  sensor1->Update(state);
  EXPECT_EQ(sensor1->get_state().ent_, state.ent_) << "\nFAIL not assigning to state_";
}

TEST_F(LightSensorTest, CalculateReading) {
  // calculate the reading based on the current state
  // state will contain the position of the stimuli
  // reading ranges form 0 to 1000
  Light *l = new Light;  
  State *s = new State(l);

  Robot *robot = new Robot;
  sensor2->set_host(robot);

  // test the case when the robot and the stimuli is at normal distance
  robot->set_position(100, 100);
  l->set_position(400, 400);
  sensor2->Update(*s);
  sensor2->calculateReading();
  EXPECT_LE(sensor2->get_reading(), 1000) << "\nFAIL reading exceeds 1000 limit at normal distance";
  EXPECT_GE(sensor2->get_reading(), 0) << "\nFAIL reading exceeds 0 limit at normal distance";
  
  // test the case when the robot and the stimuli is very far away (boundary case)
  robot->set_position(0,0);
  l->set_position(500,500);
  sensor2->Update(*s);
  sensor2->calculateReading();
  EXPECT_LE(sensor2->get_reading(), 1000) << "\nFAIL reading exceeds 1000 limit at far distance";
  EXPECT_GE(sensor2->get_reading(), 0) << "\nFAIL reading exceeds 0 limit at far distance";

  // test the case when the robot and the stimuli is very close to each other (boundary case)
  robot->set_position(200, 200);
  l->set_position(250, 250);
  sensor2->Update(*s);
  sensor2->calculateReading();
  EXPECT_LE(sensor2->get_reading(), 1000) << "\nFAIL reading exceeds 1000 limit at close distance";
  EXPECT_GE(sensor2->get_reading(), 0) << "\nFAIL reading exceeds 0 limit at close distance";

  // create an additional stimuli for testing
  Light *l2 = new Light;  
  State *s2 = new State(l2);
  
  robot->set_position(200, 200);
  // set the position of the first stimuli. It is at normal distance to the robot
  l->set_position(400, 400);
  sensor2->Update(*s);
  sensor2->calculateReading();
  double reading = sensor2->get_reading();

  // set the position for the second stimuli. It will be nearer to the robot than the first stimuli
  l2->set_position(250, 250);
  sensor2->Update(*s2);
  sensor2->calculateReading();
  EXPECT_LE(sensor2->get_reading(), 1000) << "\nFAIL reading exceeds 1000 limit with 2 stimulus";
  EXPECT_GE(sensor2->get_reading(), 0) << "\nFAIL reading exceeds 0 limit with 2 stimulus";
  // The second reading should be greater than the first one
  EXPECT_NE(sensor2->get_reading(), reading) << "\nFAIL reading does not change with 2 stimulus";
  EXPECT_GT(sensor2->get_reading(), reading) << "\nFAIL reading does not increase with the nearer stimuli";
}

NAMESPACE_END(csci3081);

#endif /* LIGHT_SENSOR_TESTS */
