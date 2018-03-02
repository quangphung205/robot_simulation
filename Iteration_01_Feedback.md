### Feedback for Iteration 1

Run on March 02, 04:01:47 AM.


#### Necessary Files and Structure

+ Pass: Check that directory "iteration1" exists.

+ Pass: Check that directory "iteration1/src" exists.

+ Pass: Check that directory "iteration1/docs" exists.

+ Pass: Check that file "iteration1/docs/Doxyfile" exists.


### .gitignore set up properly

+ Pass: Check that file/directory "iteration1/build" does not exist.

+ Pass: Check that file/directory "iteration/docs/html" does not exist.

+ Pass: Check that file/directory "iteration1/docs/latex" does not exist.

+ Pass: Change into directory "iteration1".


#### Doxygen

+ Pass: Generating documentation by running doxygen

+ Fail: Check that file "src/mainpage.h" exists.

     "src/mainpage.h" not found.

+ Fail: Check that file "html/classRobot.html" exists.

     "html/classRobot.html" not found.

+ Fail: Check that file "html/classObstacle.html" exists.

     "html/classObstacle.html" not found.


#### Google Style Compliance

+ Fail: Cpplint checking for for Google C++ compliance

Done processing src/wheel_velocity.h
Done processing src/arena_entity.h
Done processing src/params.h
Done processing src/sensor_touch.h
Done processing src/entity_factory.h
Done processing src/motion_handler.h
Done processing src/robot.h
Done processing src/rgb_color.h
Done processing src/common.h
Done processing src/controller.h
Done processing src/pose.h
Done processing src/obstacle.h
Done processing src/motion_behavior.h
Done processing src/arena_immobile_entity.h
Done processing src/graphics_arena_viewer.h
Done processing src/communication.h
Done processing src/arena.h
Done processing src/arena_params.h
Done processing src/base.h
Done processing src/motion_handler_robot.h
Done processing src/entity_type.h
Done processing src/arena_mobile_entity.h
Done processing src/motion_behavior_differential.h
Total errors found: 7src/motion_handler.h:42:  Missing space after ,  [whitespace/comma] [3]
src/motion_handler.h:130:  Lines should be <= 80 characters long  [whitespace/line_length] [2]
src/motion_handler.h:130:  Should have a space between // and comment  [whitespace/comments] [4]
src/base.h:75:  private: should be indented +1 space inside class Base  [whitespace/indent] [3]
src/arena_mobile_entity.h:72:  #endif line should be "#endif  // SRC_ARENA_MOBILE_ENTITY_H_"  [build/header_guard] [5]
src/motion_behavior_differential.h:87:  #endif line should be "#endif  // SRC_MOTION_BEHAVIOR_DIFFERENTIAL_H_"  [build/header_guard] [5]
src/motion_behavior_differential.h:34:  Missing space after ,  [whitespace/comma] [3]


#### Compilation on a CSE Labs machine

+ Pass: Change into directory "src".

+ Pass: make 'all' in directory '.': expect result=../build/bin/arenaviewer, timeout=500, extra_args='CS3081DIR=/classes/csel-s18c3081'

    make successful.
    Check ../build/bin/arenaviewer exists...OK: result ../build/bin/arenaviewer found


#### Unit Tests

+ Pass: Change into directory "../../".

+ Pass: make 'all' in directory '.': expect result=build/bin/unittest, timeout=500, extra_args='PROJROOTDIR=../../project-phung043/iteration1/'

    make successful.
    Check build/bin/unittest exists...OK: result build/bin/unittest found

+ Fail: Run unit test 'build/bin/unittest', extra_args='', allowed_fails=0

Running main() from gtest_main.cc
[==========] Running 14 tests from 4 test cases.
[----------] Global test environment set-up.
[----------] 4 tests from IntegrationTest
[ RUN      ] IntegrationTest.TEST_ARROW_KEY_UP
[       OK ] IntegrationTest.TEST_ARROW_KEY_UP (0 ms)
[ RUN      ] IntegrationTest.TEST_ARROW_KEY_DOWN
[       OK ] IntegrationTest.TEST_ARROW_KEY_DOWN (0 ms)
[ RUN      ] IntegrationTest.TEST_LEFT_KEY
[       OK ] IntegrationTest.TEST_LEFT_KEY (0 ms)
[ RUN      ] IntegrationTest.TEST_RIGHT_KEY
[       OK ] IntegrationTest.TEST_RIGHT_KEY (0 ms)
[----------] 4 tests from IntegrationTest (0 ms total)

[----------] 3 tests from RobotTest
[ RUN      ] RobotTest.RobotStopOnObstacleCollision
robot_unittest.cc:55: Failure
Expected equality of these values:
  std::make_tuple(v.left, v.right)
    Which is: (1, 1)
  std::make_tuple(0, 0)
    Which is: (0, 0)
Fail: Robot did not stop on collision with an Obstacle
[  FAILED  ] RobotTest.RobotStopOnObstacleCollision (0 ms)
[ RUN      ] RobotTest.DecreaseSpeedMin
[       OK ] RobotTest.DecreaseSpeedMin (0 ms)
[ RUN      ] RobotTest.IncreaseSpeedMax
[       OK ] RobotTest.IncreaseSpeedMax (0 ms)
[----------] 3 tests from RobotTest (0 ms total)

[----------] 2 tests from BaseTest
[ RUN      ] BaseTest.Constructor
[       OK ] BaseTest.Constructor (0 ms)
[ RUN      ] BaseTest.ChangeColorOnCaptured
[       OK ] BaseTest.ChangeColorOnCaptured (0 ms)
[----------] 2 tests from BaseTest (0 ms total)

[----------] 5 tests from ArenaTest
[ RUN      ] ArenaTest.Constructor
arena_unittest.cc:65: Failure
Expected: ((int)entities.size()) >= (8), actual: 4 vs 8
FAIL: Arena:Constructor - There should be at least 8 entities
arena_unittest.cc:67: Failure
Expected: ((int)obstacles.size()) >= (4), actual: 0 vs 4
FAIL: Arena:Constructor - There should be at least 4 obstacles in the arena.
[  FAILED  ] ArenaTest.Constructor (0 ms)
[ RUN      ] ArenaTest.HomeBaseEqualSize
[       OK ] ArenaTest.HomeBaseEqualSize (0 ms)
[ RUN      ] ArenaTest.ObstacleNotEqualSize
arena_unittest.cc:80: Failure
Expected: ((int)obstacles.size()) > (0), actual: 0 vs 0
[  FAILED  ] ArenaTest.ObstacleNotEqualSize (0 ms)
[ RUN      ] ArenaTest.Win
[       OK ] ArenaTest.Win (0 ms)
[ RUN      ] ArenaTest.LossOnHittingWall
[       OK ] ArenaTest.LossOnHittingWall (0 ms)
[----------] 5 tests from ArenaTest (0 ms total)

[----------] Global test environment tear-down
[==========] 14 tests from 4 test cases ran. (0 ms total)
[  PASSED  ] 11 tests.
[  FAILED  ] 3 tests, listed below:
[  FAILED  ] RobotTest.RobotStopOnObstacleCollision
[  FAILED  ] ArenaTest.Constructor
[  FAILED  ] ArenaTest.ObstacleNotEqualSize

 3 FAILED TESTS

