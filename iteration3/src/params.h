/**
 * @file params.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_PARAMS_H_
#define SRC_PARAMS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
/*
 * @file. Constructors for classes should use reasonable default values as
 * defined here. An exception might be if the value is 0.
 */

// graphics
#define X_DIM 1024
#define Y_DIM 768
#define TEXT_BOX_WIDTH 50
#define GUI_MENU_WIDTH 180
#define GUI_MENU_GAP 10

// arena
#define N_FOODS 4
#define N_LIGHTS 4
#define N_FEAR_ROBOTS 5
#define N_AGGRESSIVE_ROBOTS 0
#define N_EXPLORE_ROBOTS 5
#define N_LOVE_ROBOTS 0
#define MAXIMUM_LIGHTS 8
#define ARENA_X_DIM X_DIM
#define ARENA_Y_DIM Y_DIM
#define COLLISION_TOLERANCE 0.0005

// game status
#define WON 0
#define LOST 1
#define PLAYING 2
#define PAUSING 3
#define STOPPED 4

// entity
#define DEFAULT_POSE \
  { 200, 200, 0}
#define DEFAULT_COLOR \
  { 255, 255, 255 }
#define DEFAULT_RADIUS 20

// mobile entity
#define STARTING_VELOCITY 0.0

// robot
#define ROBOT_ANGLE_DELTA 1
#define ROBOT_SPEED_DELTA 1
#define ROBOT_COLLISION_DELTA 1
#define ROBOT_MIN_RADIUS 8
#define ROBOT_MAX_RADIUS 14
#define ROBOT_RADIUS 30
#define ROBOT_INIT_POS \
  { 500, 200 , 30}
#define ROBOT_COLOR \
  { 0, 0, 255 }

#define ROBOT_HEADING 270
#define ROBOT_INIT_SPEED 0
#define ROBOT_MAX_SPEED 5
#define ROBOT_MIN_SPEED 0.7
#define ROBOT_MAX_ANGLE 360
#define ROBOT_HUNGRY_TIME 30
#define ROBOT_REALLY_HUNGRY_TIME 120
#define ROBOT_STARVING_TIME 250

// Food
#define FOOD_RADIUS 20
#define FOOD_COLLISION_DELTA 1
#define FOOD_INIT_POS \
  { 400, 400 }
#define FOOD_COLOR \
  { 255, 0, 0 }

// light
#define LIGHT_POSITION \
  { 200, 200 }
#define LIGHT_RADIUS 30
#define LIGHT_MIN_RADIUS 20
#define LIGHT_MAX_RADIUS 30
#define LIGHT_COLOR \
  { 255, 255, 255 }

// sensor
#define LEFT_WHEEL 0
#define RIGHT_WHEEL 1
#define PLUS_CONNECTION 0
#define MINUS_CONNECTION 1
#define LEFT_SIDE 0
#define RIGHT_SIDE 1
#define ON 1
#define OFF 0

#endif  // SRC_PARAMS_H_
