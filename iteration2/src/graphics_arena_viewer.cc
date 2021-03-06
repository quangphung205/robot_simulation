/**
 * @file graphics_arena_viewer.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <vector>
#include <iostream>

#include "src/graphics_arena_viewer.h"
#include "src/arena_params.h"
#include "src/rgb_color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
GraphicsArenaViewer::GraphicsArenaViewer(
    const struct arena_params *const params,
    Arena * arena, Controller * controller) :
    GraphicsApp(
        params->x_dim + GUI_MENU_WIDTH + GUI_MENU_GAP * 2,
        params->y_dim,
        "Robot Simulation"),
    controller_(controller),
    arena_(arena) {
  auto *gui = new nanogui::FormHelper(screen());
  nanogui::ref<nanogui::Window> window =
      gui->addWindow(
          Eigen::Vector2i(10 + GUI_MENU_GAP, 10),
          "Menu");

  gui->addGroup("Simulation Control");
  playing_button_ =
    gui->addButton(
      "Play",
      std::bind(&GraphicsArenaViewer::OnPlayingBtnPressed, this));
  gui->addButton("New game",
             std::bind(&GraphicsArenaViewer::OnNewGameBtnPressed, this));
  game_status_button_ = gui->addGroup("Game Status: Playing");
  screen()->performLayout();
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

// This is the primary driver for state change in the arena.
// It will be called at each iteration of nanogui::mainloop()
void GraphicsArenaViewer::UpdateSimulation(double dt) {
  int status = arena_->get_game_status();

  if (status == STOPPED) {
    game_status_button_->setCaption("Game status: Stopped");
  } else if (status == LOST) {
    game_status_button_->setCaption("Game status: Lost");
  } else if (status == PAUSING) {
    paused_ = true;
    playing_button_->setCaption("Play");
    game_status_button_->setCaption("Game status: Pausing");
  } else {
    paused_ = false;
    game_status_button_->setCaption("Game status: Playing");
    playing_button_->setCaption("Pause");
    controller_->AdvanceTime(dt);
  }
}

/*******************************************************************************
 * Handlers for User Keyboard and Mouse Events
 ******************************************************************************/
void GraphicsArenaViewer::OnPlayingBtnPressed() {
  // Not implemented. Sample code provided to show how to implement.
  if (arena_->get_game_status() == WON
      || arena_->get_game_status() == LOST)
    return;

  Communication key_value = kNone;
  if (paused_) {
    paused_ = !paused_;
    key_value = kPlay;
    playing_button_->setCaption("Pause");
    game_status_button_->setCaption("Game status: Playing");
  } else {
    paused_ = !paused_;
    key_value = kPause;
    playing_button_->setCaption("Play");
    game_status_button_->setCaption("Game status: Pausing");
  }
  controller_->AcceptCommunication(key_value);
}

void GraphicsArenaViewer::OnNewGameBtnPressed() {
  Communication key_value = kNewGame;
  controller_->AcceptCommunication(key_value);
}

/** OnSpecialKeyDown is called when the user presses down on one of the
  * special keys (e.g. the arrow keys).
  */
/**
 * @TODO: Check for arrow key presses using GLFW macros, then
 * convert to appropriate enum Communication and relay to controller
 */
void GraphicsArenaViewer::OnSpecialKeyDown(int key,
  __unused int scancode, __unused int modifiers) {
  if (paused_) return;
  Communication key_value = kNone;
    switch (key) {
      case GLFW_KEY_LEFT:
        key_value = kKeyLeft;
        break;
      case GLFW_KEY_RIGHT:
        key_value = kKeyRight;
        break;
      case GLFW_KEY_UP:
        key_value = kKeyUp;
        break;
      case GLFW_KEY_DOWN:
        key_value = kKeyDown;
        break;
      default: {}
    }
  controller_->AcceptCommunication(key_value);
}

/*******************************************************************************
 * Drawing of Entities in Arena
 ******************************************************************************/
void GraphicsArenaViewer::DrawRobot(NVGcontext *ctx,
                                     const Robot *const robot) {
  // translate and rotate all graphics calls that follow so that they are
  // centered, at the position and heading of this robot
  nvgSave(ctx);
  nvgTranslate(ctx,
               static_cast<float>(robot->get_pose().x),
               static_cast<float>(robot->get_pose().y));
  nvgRotate(ctx,
            static_cast<float>(robot->get_pose().theta * M_PI / 180.0));

  // robot's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0, 0.0, static_cast<float>(robot->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(robot->get_color().r, robot->get_color().g,
                       robot->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // robot id text label
  nvgSave(ctx);
  nvgRotate(ctx, static_cast<float>(M_PI / 2.0));
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  char buff[10];
  int hunger_time = robot->get_hunger_time();
  snprintf(buff, sizeof(buff), "%d", hunger_time);
  nvgText(ctx, 0.0, 0.0, buff, nullptr);

  // robot's left sensor
  nvgRestore(ctx);
  nvgRestore(ctx);
  nvgSave(ctx);
  double lTheta = (-40 + robot->get_pose().theta) * M_PI / 180.0;
  double lx = robot->get_radius() * std::cos(lTheta) + robot->get_pose().x;
  double ly = robot->get_radius() * std::sin(lTheta) + robot->get_pose().y;
  nvgTranslate(ctx,
               static_cast<float>(lx),
               static_cast<float>(ly));
  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0, 0.0, 3);
  nvgFillColor(ctx, nvgRGBA(255, 255, 100, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // robot's right sensor
  nvgRestore(ctx);
  nvgSave(ctx);
  double rTheta = (40 + robot->get_pose().theta) * M_PI / 180.0;
  nvgTranslate(ctx,
               static_cast<float>(robot->get_radius() * std::cos(rTheta)
                  + robot->get_pose().x),
               static_cast<float>(robot->get_radius() * std::sin(rTheta)
                  + robot->get_pose().y));

  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0, 0.0, 3);
  nvgFillColor(ctx, nvgRGBA(255, 255, 100, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  nvgRestore(ctx);
}
void GraphicsArenaViewer::DrawArena(NVGcontext *ctx) {
  nvgBeginPath(ctx);
  // Creates new rectangle shaped sub-path.
  nvgRect(ctx, 0, 0, arena_->get_x_dim(), arena_->get_y_dim());
  nvgStrokeColor(ctx, nvgRGBA(255, 255, 255, 255));
  nvgStroke(ctx);
}

void GraphicsArenaViewer::DrawEntity(NVGcontext *ctx,
                                       const ArenaEntity *const entity) {
  // light's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx,
            static_cast<float>(entity->get_pose().x),
            static_cast<float>(entity->get_pose().y),
            static_cast<float>(entity->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(entity->get_color().r, entity->get_color().g,
                       entity->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // light id text label
  nvgSave(ctx);
  nvgTranslate(ctx,
               static_cast<float>(entity->get_pose().x),
               static_cast<float>(entity->get_pose().y));
  nvgRotate(ctx,
               static_cast<float>(entity->get_pose().theta * M_PI / 180.0));
  nvgRotate(ctx, static_cast<float>(M_PI / 2.0));
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, 0, 0,
          entity->get_name().c_str(), nullptr);
  nvgRestore(ctx);
}

void GraphicsArenaViewer::DrawUsingNanoVG(NVGcontext *ctx) {
  // initialize text rendering settings
  nvgFontSize(ctx, 18.0f);
  nvgFontFace(ctx, "sans-bold");
  nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  DrawArena(ctx);
  std::vector<ArenaEntity *> entities = arena_->get_entities();
  for (auto &entity : entities) {
    if (entity->get_type() == kLight || entity->get_type() == kFood) {
      DrawEntity(ctx, entity);
    } else {
      DrawRobot(ctx, dynamic_cast<Robot*>(entity));
    }
  } /* for(i..) */
}

NAMESPACE_END(csci3081);
