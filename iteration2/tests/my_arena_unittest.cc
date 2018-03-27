/*
  *  CAN WE TRACK DOWN THE BUG WHERE THE ROBOT FLIES THROUGH OBSTACLES?? ***
  **************************************************************************
void Arena::AdjustEntityOverlap(ArenaMobileEntity * const mobile_e,
   ArenaEntity *const other_e) {
     double delta_x = other_e->get_pose().x - mobile_e->get_pose().x;
     double delta_y = other_e->get_pose().y - mobile_e->get_pose().y;
     double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
     double distance_to_move =
       mobile_e->get_radius() + other_e->get_radius() - distance_between;
     double angle = atan(delta_y/delta_x);
     mobile_e->set_position(
       mobile_e->get_pose().x+cos(angle)*distance_to_move,
       mobile_e->get_pose().y+sin(angle)*distance_to_move);
 }
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include "src/arena.h"
#include "src/arena_params.h"
#include "src/arena_entity.h"
#include "src/robot.h"
#include "src/base.h"

#ifdef MYARENA_TESTS

 /******************************************************
* TEST FEATURE SetUp
*******************************************************/
class ArenaTest : public ::testing::Test {

 protected:
  virtual void SetUp() {
    // Initialize Arena for Collections of TESTS
    csci3081::arena_params aparams;
    arena = new csci3081::Arena(&aparams);

    // >>>> FILL THIS IN TO INITIALIZE ROBOT AND BASE
    robot = new csci3081::Robot;
    base = new csci3081::Base;

    // Let's make sure they are a known size
    radius = 20;
    x2radius = 40;
    offset = 25;
    robot->set_radius(radius);
    base->set_radius(radius);
  }

  // The Arena is going to act upon a robot and a base
  // Well, we could use any mobile entity with any other entity for our tests.
  csci3081::Arena * arena;
  csci3081::Robot * robot;
  csci3081::Base * base;
  // On second thought, this vector is not necessary.
  std::vector<csci3081::ArenaEntity*> entities;

  // Rather than rely on the defuault radius value, let's set it to be sure.
  double radius;
  // 2x radius to set the entities apart from each other
  double x2radius;
  // the deltaX and deltaY to put things at the "corner" of an entity.
  double offset;

};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

/** With a radius of 20 for each of robot and base, to place the base
  * overlapping at an angle of 45 degrees: The offset of the base from the
  * robot is deltaX = deltaY = 25
  *
  * After it is moved, both the x and y should have moved away from
  * the center of the robot (if moving towards, that's the bug)
  **/

TEST_F(ArenaTest, IsCollidingFalse) {
  // Test if IsColliding is detecting collision
  robot->set_position(200,200);

  // X-axis to right of robot
  base->set_position(200+x2radius+5,200);
  EXPECT_FALSE(arena->IsColliding(robot,base)) << "\n FAIL not colliding";
}

TEST_F(ArenaTest, IsCollidingAxes) {
  // Test if IsColliding is detecting collision
  robot->set_position(200,200);

  // X-axis to right of robot
  base->set_position(200+x2radius-5,200);
  EXPECT_TRUE(arena->IsColliding(robot,base)) << "\n FAIL x-axis right";

  // X-axis to left of robot
  base->set_position(200-x2radius+5,200);
  EXPECT_TRUE(arena->IsColliding(robot,base)) << "\n FAIL x-axis left";

  // Y-axis above the ROBOT
  base->set_position(200,200-x2radius+5);
  EXPECT_TRUE(arena->IsColliding(robot,base)) << "\n FAIL y-axis above";

  // Y-axis below the ROBOT
  base->set_position(200,200+x2radius-5);
  EXPECT_TRUE(arena->IsColliding(robot,base)) << "\n FAIL y-axis below";
}

TEST_F(ArenaTest, IsCollidingCorners) {
  // Test if IsColliding is detecting collision
  robot->set_position(200,200);

  // quadrant 1
  base->set_position(200+offset,200-offset);
  EXPECT_TRUE(arena->IsColliding(robot,base)) << "\n FAIL q1 corner";

  // quadrant 2
  base->set_position(200-offset,200-offset);
  EXPECT_TRUE(arena->IsColliding(robot,base)) << "\n FAIL q2 corner";

  // quadrant 3
  base->set_position(200-offset,200+offset);
  EXPECT_TRUE(arena->IsColliding(robot,base)) << "\n FAIL q3 corner";

  // quadrant 4
  base->set_position(200+offset,200+offset);
  EXPECT_TRUE(arena->IsColliding(robot,base)) << "\n FAIL q4 corner";
}

TEST_F(ArenaTest, AdjustOverlapQuadrants) {
  // Test if adjusting overlapped objects correctly
  // The robot is the first argument, thus it will be moved

  // quadrant 1
  base->set_position(200+offset,200-offset);
  robot->set_position(200,200);
  arena->AdjustEntityOverlap(robot, base);
  EXPECT_LT(robot->get_pose().x, 200) << "\n FAIL quadrant 1, x";
  EXPECT_GT(robot->get_pose().y, 200) << "\n FAIL quadrant 1, y";

  // quadrant 2
  base->set_position(200-offset,200-offset);
  robot->set_position(200,200);
  arena->AdjustEntityOverlap(robot, base);
  EXPECT_GT(robot->get_pose().x, 200) << "\n FAIL quadrant 2, x";
  EXPECT_GT(robot->get_pose().y, 200) << "\n FAIL quadrant 2, y";

  // quadrant 3
  base->set_position(200-offset,200+offset);
  robot->set_position(200,200);
  arena->AdjustEntityOverlap(robot, base);
  EXPECT_GT(robot->get_pose().x, 200) << "\n FAIL quadrant 3, x";
  EXPECT_LT(robot->get_pose().y, 200) << "\n FAIL quadrant 3, y";

  // quadrant 4
  base->set_position(200+offset,200+offset);
  robot->set_position(200,200);
  arena->AdjustEntityOverlap(robot, base);
  EXPECT_LT(robot->get_pose().x, 200) << "\n FAIL quadrant 4, x";
  EXPECT_LT(robot->get_pose().y, 200) << "\n FAIL quadrant 4, y";
}

#endif
#ifndef MYARENA_TESTS

TEST_F(ArenaTest, AdjustOverlapAxes) {
  // Test if adjusting overlapped objects correctly
  // The robot is the first argument, thus it will be moved

  // X-axis to right of robot
  robot->set_position(200,200);
  base->set_position(200+x2radius-5,200);
  arena->AdjustEntityOverlap(robot, base);
  EXPECT_LT(robot->get_pose().x, 200) << "\n FAIL x-axis right";

  // X-axis to left of robot
  robot->set_position(200,200);
  base->set_position(200-x2radius+5,200);
  arena->AdjustEntityOverlap(robot, base);
  EXPECT_GT(robot->get_pose().x, 200) << "\n FAIL x-axis left";

  // Y-axis above the ROBOT
  robot->set_position(200,200);
  base->set_position(200,200-x2radius+5);
  arena->AdjustEntityOverlap(robot, base);
  EXPECT_GT(robot->get_pose().y, 200) << "\n FAIL y-axis above";

  // Y-axis below the ROBOT
  base->set_position(200,200+x2radius-5);
  arena->AdjustEntityOverlap(robot, base);
  EXPECT_LT(robot->get_pose().y, 200) << "\n FAIL y-axis below";
}

#endif /* MY_ARENA_TESTS */
