/**
 * @file controller.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <nanogui/nanogui.h>
#include <string>

#include "src/arena_params.h"
#include "src/common.h"
#include "src/controller.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

Controller::Controller() : last_dt(0) {
  // Initialize default properties for various arena entities
  //arena_params aparams;
  params_ = new arena_params();
  //aparams.n_lights = N_LIGHTS;
  //aparams.n_foods = N_FOODS;
  //aparams.x_dim = ARENA_X_DIM;
  //aparams.y_dim = ARENA_Y_DIM;

  //arena_ = new Arena(&aparams);
  arena_ = new Arena(params_);

  // Start up the graphics (which creates the arena).
  // Run() will enter the nanogui::mainloop().
  //viewer_ = new GraphicsArenaViewer(&aparams, arena_, this);
  viewer_ = new GraphicsArenaViewer(params_, arena_, this);
  //arena_ = new Arena(&aparams);
}

void Controller::Run() { viewer_->Run(); }

void Controller::AdvanceTime(double dt) {
  last_dt += dt;
  if (last_dt >= 0.02) {
    arena_->AdvanceTime(dt);
    last_dt = 0;
  }
}

void Controller::AcceptCommunication(Communication com) {
  arena_->AcceptCommand(ConvertComm(com));
}

/** Converts communication from one source to appropriate communication to
 * the other source. For example, the viewer sends a kKeyUp communication,
 * and this translate to a kIncreaseSpeed communication to Arena.
 */
/**
  * @TODO: Complete the conversion code for all key presses.
  */
Communication Controller::ConvertComm(Communication com) {
  switch (com) {
    /*
    case (kKeyUp) :
      return kIncreaseSpeed;
    case (kKeyDown) :
      return kDecreaseSpeed;
    case (kKeyLeft) :
      return kTurnLeft;
    case (kKeyRight) :
      return kTurnRight;
      */
    case (kPlay) :
      return kPlay;
    case (kPause) :
      return kPause;
    case (kNewGame) : {
      return kReset;
    }


    default: return kNone;
  }
}

NAMESPACE_END(csci3081);
