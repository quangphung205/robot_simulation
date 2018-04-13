/**
 * @file arena_entity.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ARENA_ENTITY_H_
#define SRC_ARENA_ENTITY_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include "src/common.h"
#include "src/entity_type.h"
#include "src/params.h"
#include "src/pose.h"
#include "src/rgb_color.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A Food class from which all Arena entities inherit.
 *
 * All entities know how to:
 *
 * 1. Update themselves at each timestep (i.e. in accordance with current
 * velocity and position).
 *
 * 2. Reset themselves to a newly constructed state. So that the user can
 * click the reset button to restart the game. Alternatively, the game
 * will be reset if the robot has won/lost.
 *
 * Please note that here use the upper-left coordinate, which means that the
 * origin point (0.0,0.0) is at the upper left.
 *
 * All arena entities are circular.
 *
 */
class ArenaEntity {
 public:
  /**
   * @brief ArenaEntity constructor initialized with default values from params.h
   */
  ArenaEntity() : pose_(DEFAULT_POSE), color_(DEFAULT_COLOR) {}

  /**
   * @brief Default destructor -- as defined by compiler.
   */
  virtual ~ArenaEntity() = default;

  /**
   * @brief Perform whatever updates needed for a particular entity after 1
   * timestep (updating position, changing color, etc.).
   *
   * @param[in] dt is time elapsed since the last update. Unused.
   */
  virtual void TimestepUpdate(__unused unsigned int dt) {}

  /**
   * @brief Reset entity to a newly constructed state.
   */
  virtual void Reset() {}

  /**
   * @brief Get the name of the entity for visualization and for debugging.
   *
   * @return Name of the entity. Each entity type hard codes its name
   * (e.g. "Robot").
   */
  virtual std::string get_name() const = 0;

  /**
   * @brief Get the position of the entity
   *
   * @return current position of the entity
   */
  const Pose &get_pose() const { return pose_; }

  /**
   * @brief Set the position of the entity
   *
   * @param[pose] the new position
   */
  void set_pose(const Pose &pose) { pose_ = pose; }

  /**
   * @brief Setter method for position within entity pose variable.
   *
   * @param[inx] x coordinate
   * @param[iny] y coordinate
   */
  void set_position(const double inx, const double iny) {
    pose_.x = inx;
    pose_.y = iny;
  }

  /**
   * @brief Setter method for heading within entity pose variable.
   *
   * @param[t] new heading angle
   */
  void set_heading(const double t) {pose_.theta = t;}

  /**
   * @brief get the current heading angle of the entity
   *
   * @return current heading angle
   */
  double get_heading() const { return pose_.theta; }
  /**
   * @brief Setter for heading within pose, but change is relative to current
   * value.
   *
   * @param[in] delta by which to modify current heading. Can be positive
   * or negative.
   */
  void RelativeChangeHeading(const double delta) {
    pose_.theta += delta;
  }

  /**
   * @brief get the color of the entity
   *
   * @return the color value
   */
  const RgbColor &get_color() const { return color_; }

  /**
   * @brief set the color of the entity
   *
   * @param[color] new color
   */
  void set_color(const RgbColor &color) { color_ = color; }

  /**
   * @brief get the radius of the entity
   *
   * @return current radius
   */
  double get_radius() const { return radius_; }

  /**
   * @brief set the radius of the entity
   *
   * @param[radius] new radius
   */
  void set_radius(double radius) { radius_ = radius; }

  /**
   * @brief get the type of the entity
   *
   * @return current type
   */
  EntityType get_type() const { return type_; }

  /**
   * @brief set the type of the entity
   *
   * @param[et] new type
   */
  void set_type(EntityType et) { type_ = et; }

  /**
   * @brief get the id of the entity
   *
   * @return current id
   */
  int get_id() const { return id_; }

  /**
   * @brief set the id of the entity
   *
   * @param[id] new id
   */
  void set_id(int id) { id_ = id; }

  /**
   * @brief Getter method for determining if entity can move or not.
   *
   * @return the mobility of the entity
   */
  bool is_mobile(void) { return is_mobile_; }

  /**
   * @brief Setter method for indicating if entity can move or not.
   *
   * @param[value] new mobility value
   */
  void set_mobility(bool value) { is_mobile_ = value; }

 protected:
  double radius_{DEFAULT_RADIUS};
  Pose pose_;
  RgbColor color_;
  EntityType type_{kEntity};
  int id_{-1};
  bool is_mobile_{false};
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_ENTITY_H_
