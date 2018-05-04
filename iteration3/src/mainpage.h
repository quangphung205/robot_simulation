/**
 * @file mainpage.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_MAINPAGE_H_
#define SRC_MAINPAGE_H_

/*! \mainpage My Personal Index Page
 * \section intro_sec Technical Users
 *
 * This program uses the MVC (Module - Viewer - Controller) model to create a robot simulation.
 The controller handles all communication between the Module, which is the arena, and Viewer,
 which is the graphics. The arena contains different types of entities including: robots, lights,
 and foods. Lights and foods are considered stimulus in the simulation which directly affect the
 motion of robots. The robots so far are implemented with four sensors: two for lights sensors and
 two for foods sensors. All sensors get the reading directly from all stimulus in the arena and
 then convert the readings into wheel velocity. Each sensor connects to a particular wheel and impacts
 that wheel in a specific way. The combination of different connections and impacts of each sensor
 creates four different kinds of robots which follow the model of Braitenburg Vehicles. They are
 Fear, Aggression, Love, and Explore.
 *
 * Sensors are implemented by the Observer Pattern in which the arena is the Subject whose purpose is simply
 to notify all sensors about the stimulus; and Sensors
 are the Observers who will wait for the arena to receive the updated information of the stimulus.
 The arena notifies the corresponding sensors based on the stimuli. For example, the arena will notify
 light sensors about lights only. The same happens with food sensors. The sensor, in turn, will receive
 the information from the arena about the stimuli, calculate the reading based on the distance to the
 stimuli, and generate a corresponding velocity. The sensor then updates the new velocity to the wheel
 that it is connecting to. The robot itself will be responsible for checking the new updated velocity
 to ensure it is within the acceptable range. It the new velocity happens to exceed the desired limit,
 the robot simply adjusts it to the set maximum velocity; or if it goes below the limit, the robot sets
 it to the minimum one.
 *
 * The different models of the robot (e.g. Fear, Aggressive, Love, Exploratory) in the arena provide
 a good condition to apply the strategy pattern. The idea is that we would create a new general class
 for different models of robot called RobotModel class. This class will have all information about
 how many sensors the robot has, including light sensors and food sensors, how sensors are connecting
 to both wheels (e.g. direct connection or crossed connection), how sensors impact the velocity of both
 wheels (e.g. positive connection or negative connection). After creating this new class, the robot
 class now would have an additional member which is a pointer to the RobotModel class. This member helps
 to determine what kind of model the robot is simulating. For each model of the Braitenberg vehicles,
 we will add a new class and let it inherit from the RobotModel class. This new class connects all
 sensors properly to both wheels to reflect the correction motion of this model based on the Braitenberg
 vehicles. When we want to create a particular robot, for example a robot exhibits Fear behavior, we
 simply need to initialize the pointer to the RobotModel class. There are many advantages of this approach.
 First, a programmer can add as many new models as they want for the robot in the future. All they need
 to do is to create a new class for that model and make it inherit from the RobotModel class. They also
 need to initialize the robot_model_ pointer to the new added class in the Robot constructor. Another
 advantage is that we can change the model of any robot easily at any time in the simulation.
 For example, when the robot is very hungry, it should exhibit aggressive behavior to the food.
 We can implement this by reinitialize the robot_model_ pointer to the RobotAggressive class.
 *
 * In order to add a new stimuli to the simulator, the programmer needs to create
 a new class for that stimuli and let it inherit from both the ArenaMobileEntity class (if it is a
 mobile stimuli) or the ArenaImmobileEntity class (if it is an immobile stimuli) and the Subject class
 which is part of a Observer Pattern. The programmer also needs to implement all method from the Subject
 class because the Subject class only acts as an interface. This will include overriding the Notify
 method which allows it to send its information (e.g. position) to all of its observers (e.g. water
 sensors). If it is a mobile entity and it has a different motion behavior, the programmer also needs
 to create two new classes for the motion handler and motion behavior of the stimuli. The new stimuli
 class then requires to have these new motion controllers as its members.
 *
 * \section sub_sec Non-Technical Users
 *
 * The purpose of this program is to simulate the behavior of the robot with the effect of the surrounding
 stimulus. The program consists of the arena which contains other objects. There are three different kinds
 of entities such as robots, lights, and foods. Foods are immobile entities which only stay in place for
 the whole simulation. Robots and lights are mobile entities which can move around and collide with other
 entities in the arena. Lights only collide with the wall and themselves. Robots are able to pass through
 lights and foods.
 *
 * Each robot contains four sensors, two for light sensors and two for food sensors. Each
 sensor has different connection types and impacts on the wheels of the robots. These sensor will
 affect the motion behavior of the robot by the position of the stimulus in the arena.
 When the game starts, there are 10 robots, 8 lights, and 4 foods in the arena by default. A user can change
 the quantity of these entities by adjusting the corresponding sliders on the controll board. 5 robots will
 fear the lights which means they will try to avoid lights as much as possible. Other five robots will
 explore the light which means they will try to come closer to the light.After 30 seconds, the robot will
 get hungry and start to search for foods. Each robot is equipped with 2 foods sensor which helps it to
 actively find foods in the arena. At this stage, robots' light sensors are still active so that the robots
 still express their default behavior toward lights.
 *
 * The robot consumes food when it gets within 5 pixels of food entity.
 If a robot has not consumed any food for 120 seconds, it will become very hungry and start to ignore all
 lights in the arena. At this stage, the robot only focuses on food. If the robot could not consume any food
 within 150 seconds, it is considered starving and the game will end. At this stage, the user has to start
 the new game. In order to run this program, user must locate the executable file named arenaviewer in
 the build directory. The user then run the following command: ./arenaviewer in the command prompt.
 The interface enables the user to configure the simulation. There are different sliders for the number of
 robots, lights, and foods in the arena. Additionally, there is an option to turn on/off food in case the
 user only wants to test the behavior of robots with lights. There is a slider to control the sensitivity of
 robot's sensors with respect to the distance with the stimulus. After configuring the area, the user clicks
 new game to start the simulation. The user can only configure at the beginning of the simulation or when
 the simulation ends except that the users are still able to adjust the sensitivity of sensors. If no food
 option was selected, robots will not ever get hungry.
 */

#endif  // SRC_MAINPAGE_H_
