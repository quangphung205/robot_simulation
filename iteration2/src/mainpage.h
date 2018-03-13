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
 * This is the documentation of the Robot Simulation Project for CSCI3081_S18 course. The goal of this software project is to develop a rudimentary robot simulator in which robot behavior is visualized within a graphics window. The exact use of the application is yet to be determined. It might be that it is used more like a video game in which users control the robots. Alternatively, it might be a test environment for experimenting with autonomous robot control using sensor feedback, or it might be a visualization of real robots operating in a remote location. Due to the ambiguity of future use, it is important that you plan for change through good design.

 * In the first iteration, the robot simulator is more like a video game. A user controls the robot with the arrow keys. The objective of the game is to touch all of the bases without running into (eventually moving) obstacles or walls. The robot is given a fixed number of lives at the beginning of the game, but it will lose a life with every collision (except for when colliding with a base).
 *
 * The GraphicArenaViewer class has a purpose to capture all user interactions with the system including button hittings, arrow hittings, etc. It then transfers all of these signals to the controller class which in turn communicates with the Arena class. The Arena class will receive the signals and change the status of its entities accordingly. The Arena class is also capable of communicating with the Controller class and the GraphicArenaViewer class to transfer data.
 *
 * The game starts off with the robot and other entities are placed randomly in the window screen. The robot is given 9 lives at the head of the game. The goal is for the robot to capture all of the bases and avoid obstacles and walls as much as possible. If the robot touches a wall or an obstacle, it will stop and lose one live. During 2 seconds after the collision, the robot would be granted a protected shield which keeps it invincible to any walls and obstacles. The invincibility status will be visual to users by color changing of the robot. If the robot touches a base, it will stop and wait for the next move from user. The base color also changes to a different color (orange at this time) to notify the user that this base is already captured. The game will be determined WIN if the robot captures all bases (3 at this time). The game will be determined LOST if the robot loses all of its lives. When the game is over (WIN or LOST), the user has to hit a "New Game" button to restart.
 *
 * The code is organized such that robot behavior in the arena (i.e. the model) is separate from visualization (i.e. the view), which allows for automated unit testing using Google Test. There is also a controller that handles the communication between the model and the view. The Robot class is composed of many other classes that control various aspects of robot behavior and environment interaction. You will need to maintain this class structure to interface with Google Tests.
 */

#endif  // SRC_MAINPAGE_H_
