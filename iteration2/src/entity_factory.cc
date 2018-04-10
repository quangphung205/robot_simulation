/**
 * @file entity_factory.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include <ctime>
#include <iostream>

#include "src/common.h"
#include "src/entity_factory.h"
#include "src/entity_type.h"
#include "src/params.h"
#include "src/pose.h"
#include "src/rgb_color.h"
#include "src/light_sensor.h"
#include "src/food_sensor.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

EntityFactory::EntityFactory() {
  srand(time(nullptr));
}

ArenaEntity* EntityFactory::CreateEntity(EntityType etype) {
  switch (etype) {
    case (kRobot):
      return CreateRobot();
      break;
    case (kLight):
      return CreateLight();
      break;
    case (kFood):
      return CreateFood();
      break;
    case (kFearRobot):
      return CreateFearRobot();
      break;
    case (kAggressiveRobot):
      return CreateAggressiveRobot();
      break;
    case (kLoveRobot):
      return CreateLoveRobot();
      break;
    case (kExploreRobot):
      return CreateExploreRobot();
      break;
    default:
      std::cout << "FATAL: Bad entity type on creation\n";
      assert(false);
  }
  return nullptr;
}

Robot* EntityFactory::CreateRobot() {
  Robot* robot = new Robot;
  robot->set_type(kRobot);
  robot->set_color(ROBOT_COLOR);
  robot->set_pose(SetPoseRandomly());
  robot->set_radius((random() % (ROBOT_MAX_RADIUS
                  - ROBOT_MIN_RADIUS)) + ROBOT_MIN_RADIUS);
  robot->set_heading(random() % 360);
  robot->SetSpeed(1, 1);
  ++entity_count_;
  ++robot_count_;
  robot->set_id(robot_count_);

  Sensor *left_sensor = new FoodSensor(arena_, RIGHT_WHEEL, PLUS_CONNECTION);
  left_sensor->set_side(LEFT_SIDE);
  left_sensor->set_host(robot);
  left_sensor->UpdatePosition();
  arena_->registerObserver(left_sensor);

  Sensor *right_sensor = new FoodSensor(arena_, LEFT_WHEEL, PLUS_CONNECTION);
  right_sensor->set_side(RIGHT_SIDE);
  right_sensor->set_host(robot);
  right_sensor->UpdatePosition();
  arena_->registerObserver(right_sensor);

  robot->add_sensor(left_sensor);
  robot->add_sensor(right_sensor);

  return robot;
}

Robot* EntityFactory::CreateFearRobot() {
  Robot *robot = CreateRobot();
  robot->set_name("F");
  //robot->set_type(kFearRobot);

  Sensor *left_sensor = new LightSensor(arena_, LEFT_WHEEL, PLUS_CONNECTION);
  left_sensor->set_side(LEFT_SIDE);
  left_sensor->set_host(robot);
  left_sensor->UpdatePosition();
  arena_->registerObserver(left_sensor);

  Sensor *right_sensor = new LightSensor(arena_, RIGHT_WHEEL, PLUS_CONNECTION);
  right_sensor->set_side(RIGHT_SIDE);
  right_sensor->set_host(robot);
  right_sensor->UpdatePosition();
  arena_->registerObserver(right_sensor);

  robot->add_sensor(left_sensor);
  robot->add_sensor(right_sensor);
  return robot;
}

Robot* EntityFactory::CreateAggressiveRobot() {
  Robot *robot = CreateRobot();
  robot->set_name("A");
  //robot->set_type(kAggressiveRobot);

  Sensor *left_sensor = new LightSensor(arena_, RIGHT_WHEEL, PLUS_CONNECTION);
  left_sensor->set_side(LEFT_SIDE);
  left_sensor->set_host(robot);
  left_sensor->UpdatePosition();
  arena_->registerObserver(left_sensor);

  Sensor *right_sensor = new LightSensor(arena_, LEFT_WHEEL, PLUS_CONNECTION);
  right_sensor->set_side(RIGHT_SIDE);
  right_sensor->set_host(robot);
  right_sensor->UpdatePosition();
  arena_->registerObserver(right_sensor);

  robot->add_sensor(left_sensor);
  robot->add_sensor(right_sensor);
  return robot;
}

Robot* EntityFactory::CreateLoveRobot() {
  Robot *robot = CreateRobot();
  robot->set_name("L");
  //robot->set_type(kLoveRobot);

  Sensor *left_sensor = new LightSensor(arena_, LEFT_WHEEL, MINUS_CONNECTION);
  left_sensor->set_side(LEFT_SIDE);
  left_sensor->set_host(robot);
  left_sensor->UpdatePosition();
  arena_->registerObserver(left_sensor);

  Sensor *right_sensor = new LightSensor(arena_, RIGHT_WHEEL, MINUS_CONNECTION);
  right_sensor->set_side(RIGHT_SIDE);
  right_sensor->set_host(robot);
  right_sensor->UpdatePosition();
  arena_->registerObserver(right_sensor);

  robot->add_sensor(left_sensor);
  robot->add_sensor(right_sensor);
  return robot;
}

Robot* EntityFactory::CreateExploreRobot() {
  Robot *robot = CreateRobot();
  robot->set_name("E");
  //robot->set_type(kExploreRobot);

  Sensor *left_sensor = new LightSensor(arena_, RIGHT_WHEEL, MINUS_CONNECTION);
  left_sensor->set_side(LEFT_SIDE);
  left_sensor->set_host(robot);
  left_sensor->UpdatePosition();
  arena_->registerObserver(left_sensor);

  Sensor *right_sensor = new LightSensor(arena_, LEFT_WHEEL, MINUS_CONNECTION);
  right_sensor->set_side(RIGHT_SIDE);
  right_sensor->set_host(robot);
  right_sensor->UpdatePosition();
  arena_->registerObserver(right_sensor);

  robot->add_sensor(left_sensor);
  robot->add_sensor(right_sensor);
  return robot;
}

Light* EntityFactory::CreateLight() {
  Light* light = new Light;
  light->set_type(kLight);
  light->set_color(LIGHT_COLOR);
  light->set_pose(SetPoseRandomly());
  light->set_radius((random() % (LIGHT_MAX_RADIUS
               - LIGHT_MIN_RADIUS)) + LIGHT_MIN_RADIUS);
  light->set_heading(random() % 360);
  ++entity_count_;
  ++light_count_;
  light->set_id(light_count_);
  return light;
}

Food* EntityFactory::CreateFood() {
  Food* food = new Food;
  food->set_type(kFood);
  food->set_color(FOOD_COLOR);
  food->set_pose(SetPoseRandomly());
  food->set_radius(FOOD_RADIUS);
  ++entity_count_;
  ++food_count_;
  food->set_id(food_count_);
  return food;
}

Pose EntityFactory::SetPoseRandomly() {
  // Dividing arena into 19x14 grid. Each grid square is 50x50
  // return {static_cast<double>((30 + (random() % 19) * 50)),
  //      static_cast<double>((30 + (random() % 14) * 50))};
  return {static_cast<double>(50 + ((random() % 19) * 50)),
        static_cast<double>(50 + ((random() % 14) * 50))};
}

NAMESPACE_END(csci3081);
