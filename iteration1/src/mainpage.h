/**
 * @file mainpage.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_MAINPAGE_H_
#define SRC_MAINPAGE_H_

/*! \mainpage My Personal Index Page
 * \section intro_sec Introduction
 *
 * This is the documentation of the Robot Simulation Project
 * for CSCI3081_S18 course. The goal of this software project is to develop a rudimentary robot simulator in which robot behavior is visualized within a graphics window. The exact use of the application is yet to be determined. It might be that it is used more like a video game in which users control the robots. Alternatively, it might be a test environment for experimenting with autonomous robot control using sensor feedback, or it might be a visualization of real robots operating in a remote location. Due to the ambiguity of future use, it is important that you plan for change through good design.

In this iteration, you will put all the pieces in place for development including libraries, base code, unit tests with Google Test, class structure, Google style guide compliance, design documentation using UML, doxygen documentation, Git usage, and self-documenting code (i.e. good organization and naming conventions). Teaching staff will also be working diligently to provide the structure and support for development, testing, and documentation. Automation in unit testing, style compliance, in-house github testing, and documentation will make this an easier process, however it means that you will have to develop within the constraints of these systems. Often there is a learning curve and a lot of systems trouble-shooting to get it all working, so start early!

In the first iteration, the robot simulator is more like a video game. A user controls the robot with the arrow keys. The objective of the game is to touch all of the bases without running into (eventually moving) obstacles or walls. The robot is given a fixed number of lives at the beginning of the game, but it will lose a life with every collision (except for when colliding with a base).
 *
 * \section Functional Requirements
 *
 * You are being provided with a significant initial code base. The first iteration is focused on enhancing functionality and getting practiced in the software development process. It is also a drop into the depths of C++ syntax. If you have any questions about the provided code, please ask. We do not expect you to understand all the constructs in this code. Below is an outline of the required functionality.

The code is organized such that robot behavior in the arena (i.e. the model) is separate from visualization (i.e. the view), which allows for automated unit testing using Google Test. There is also a controller that handles the communication between the model and the view. The Robot class is composed of many other classes that control various aspects of robot behavior and environment interaction. You will need to maintain this class structure to interface with Google Tests.
 */

#endif  // SRC_MAINPAGE_H_
