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
    struct arena_params *params,
    Arena * arena, Controller * controller) :
    GraphicsApp(
        params->x_dim + GUI_MENU_WIDTH + GUI_MENU_GAP * 2,
        params->y_dim,
        "Robot simulation"),
    controller_(controller),
    arena_(arena) {
  auto *gui = new nanogui::FormHelper(screen());

  nanogui::ref<nanogui::Window> window =
      gui->addWindow(
          Eigen::Vector2i(params->x_dim, 0),
          "Menu");

  // GUI configuration
  // vvvvvvvvvvvv    ADDED THIS ONE LINE to register the window  vvvvvvvvvvvv
  // gui->addGroup creates a heading within the window
  window->setLayout(new nanogui::GroupLayout());
  // End of GUI configuration

  gui->addGroup("Simulation Control");

  // vvvvvvvvvv  ADDED BELOW HERE (from nanogui example1.cc)   vvvvvvvvvvvvvvv

  gui->addGroup("Arena Configuration");

  // Creating a panel impacts the layout. Widgets, sliders, buttons can be
  // assigned to either the window or the panel.
  nanogui::Widget *panel = new nanogui::Widget(window);

  // *************** SLIDER 1 ************************//
  new nanogui::Label(panel, "Number of Robots", "sans-bold");
  nanogui::Slider *slider = new nanogui::Slider(panel);
  // The starting value (range is from 0 to 1)
  // Note that below the displayed value is 10* slider value.
  slider->setValue(1.0f);
  slider->setFixedWidth(100);
  slider->setRange(std::make_pair(0.1f, 1.0f));

  // Display the corresponding value of the slider in this textbox
  nanogui::TextBox *text_box = new nanogui::TextBox(panel);
  text_box->setFixedSize(nanogui::Vector2i(60, 25));
  text_box->setFontSize(20);
  text_box->setValue("10");

  // This is the lambda function called while the user is moving the slider
  slider->setCallback(
    [text_box](float value) {
      text_box->setValue(std::to_string(static_cast<int>(value*10)));
    });
  // This is the lambda function called once the user is no longer manipulating
  // the slider.
  // Note robot_count_ is set, which is a graphics_arena_ variable in this
  // version, although
  // you should communicate that value to the controller so that it can
  // configure the Arena.

  slider->setFinalCallback(
    [&](float value) {
      arena_->params_->n_robots = static_cast<int>(value * 10);
    });

  // *************** SLIDER 2 ************************//
  new nanogui::Label(panel, "Number of Lights", "sans-bold");
  nanogui::Slider *slider2 = new nanogui::Slider(panel);
  slider2->setValue(0.8f);
  slider2->setFixedWidth(100);

  nanogui::TextBox *text_box2 = new nanogui::TextBox(panel);
  text_box2->setFixedSize(nanogui::Vector2i(60, 25));
  text_box2->setFontSize(20);
  text_box2->setValue("4");

  slider2->setCallback(
    [text_box2](float value) {
      text_box2->setValue(std::to_string(static_cast<int>(value*5)));
    });

  slider2->setFinalCallback(
    [&](float value) {
      arena_->params_->n_lights = static_cast<int>(value * 5);
    });

  // *************** SLIDER 3 ************************//
  new nanogui::Label(panel, "Robot ratio", "sans-bold");
  nanogui::Slider *slider3 = new nanogui::Slider(panel);
  slider3->setValue(0.5f);
  slider3->setFixedWidth(100);

  nanogui::TextBox *text_box3 = new nanogui::TextBox(panel);
  text_box3->setFixedSize(nanogui::Vector2i(60, 25));
  text_box3->setFontSize(20);
  text_box3->setValue("0.5");

  slider3->setCallback(
    [text_box3](float value) {
      char buff[4];
      snprintf(buff, sizeof(buff), "%.1f", value);
      text_box3->setValue(buff);
    });

  slider3->setFinalCallback(
    [&](float value) {
      arena_->params_->ratio_ = value;
    });

  // *************** SLIDER 4 ************************//
  new nanogui::Label(panel, "Sensor sensitivity", "sans-bold");
  nanogui::Slider *slider4 = new nanogui::Slider(panel);
  slider4->setValue(0.503f);
  slider4->setFixedWidth(100);

  nanogui::TextBox *text_box4 = new nanogui::TextBox(panel);
  text_box4->setFixedSize(nanogui::Vector2i(60, 25));
  text_box4->setFontSize(20);
  text_box4->setValue("1.006");

  slider4->setCallback(
    [text_box4](float value) {
      char buff[10];
      snprintf(buff, sizeof(buff), "%.3f", value * 2);
      text_box4->setValue(buff);
    });

  slider4->setFinalCallback(
    [&](float value) {
      arena_->params_->base_sensitivity_ = value * 2;
      arena_->UpdateSensitivity();
    });

  // add food button
  food_button_ =
    gui->addButton(
      "Food: On",
      std::bind(&GraphicsArenaViewer::OnFoodBtnPressed, this));

  // *************** SLIDER 5 ************************//
  new nanogui::Label(panel, "Number of Foods", "sans-bold");
  nanogui::Slider *slider5 = new nanogui::Slider(panel);
  slider5->setValue(0.8f);
  slider5->setFixedWidth(100);

  nanogui::TextBox *text_box5 = new nanogui::TextBox(panel);
  text_box5->setFixedSize(nanogui::Vector2i(60, 25));
  text_box5->setFontSize(20);
  text_box5->setValue("4");

  slider5->setCallback(
    [text_box5](float value) {
      text_box5->setValue(std::to_string(static_cast<int>(value*5)));
    });

  slider5->setFinalCallback(
    [&](float value) {
      if (has_food_) {
        arena_->params_->n_foods = static_cast<int>(value * 5);
      }
    });

  // Lays out all the components with "15" units of inbetween spacing
  panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
                     nanogui::Alignment::Middle, 0, 15));

  // ^^^^^^^^ ADDED TO HERE (modification of nanogui example1.cc)  ^^^^^^

  screen()->performLayout();
  // End of GUI configuration

  playing_button_ =
    gui->addButton(
      "Play",
      std::bind(&GraphicsArenaViewer::OnPlayingBtnPressed, this));

  // Without fixing the width, the button will span the entire window
  gui->addButton("New game",
             std::bind(&GraphicsArenaViewer::OnNewGameBtnPressed, this));
  game_status_button_ = gui->addGroup("Status: Playing");
  screen()->performLayout();
}

/*****************************************************************************
 * Member Functions
 ****************************************************************************/

// This is the primary driver for state change in the arena.
// It will be called at each iteration of nanogui::mainloop()
void GraphicsArenaViewer::UpdateSimulation(double dt) {
  int status = arena_->get_game_status();

  if (status == STOPPED) {
    game_status_button_->setCaption("Status: Stopped");
  } else if (status == NEWGAME) {
    game_status_button_->setCaption("Status: New game");
  } else if (status == LOST) {
    game_status_button_->setCaption("Game status: Lost");
  } else if (status == PAUSING) {
    paused_ = true;
    playing_button_->setCaption("Play");
    game_status_button_->setCaption("Status: Pausing");
  } else {
    paused_ = false;
    game_status_button_->setCaption("Status: Playing");
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
      || arena_->get_game_status() == LOST) {
    return;
  }

  Communication key_value = kNone;

  if (arena_->get_game_status() == NEWGAME
    || arena_->get_game_status() == STOPPED
    || arena_->get_game_status() == PAUSING) {
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
  if (paused_) {
    return;
  }

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
  double l_theta = (-40 + robot->get_pose().theta) * M_PI / 180.0;
  double lx = robot->get_radius() * std::cos(l_theta) + robot->get_pose().x;
  double ly = robot->get_radius() * std::sin(l_theta) + robot->get_pose().y;
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
  double r_theta = (40 + robot->get_pose().theta) * M_PI / 180.0;
  nvgTranslate(ctx,
               static_cast<float>(robot->get_radius() * std::cos(r_theta)
                  + robot->get_pose().x),
               static_cast<float>(robot->get_radius() * std::sin(r_theta)
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

void GraphicsArenaViewer::OnFoodBtnPressed() {
  if (has_food_) {
    food_button_->setCaption("Food: Off");
    arena_->params_->n_foods = 0;
  } else {
    food_button_->setCaption("Food: On");
    arena_->params_->n_foods = N_FOODS;
  }
  has_food_ = !has_food_;
}

NAMESPACE_END(csci3081);
