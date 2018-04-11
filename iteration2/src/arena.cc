/**
 * @file arena.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>
#include <iostream>

#include "src/arena.h"
#include "src/arena_params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Arena::Arena(const struct arena_params *const params)
    : Subject(),
      x_dim_(params->x_dim),
      y_dim_(params->y_dim),
      factory_(new EntityFactory(this)),
      entities_(),
      mobile_entities_(),
      game_status_(PAUSING) {
  addEntitiesToArena(params);
}

void Arena::addEntitiesToArena(const struct arena_params *const params) {
  for (size_t i = 0; i < params->n_fear_robots; i++)
    AddRobot(kFearRobot);
  for (size_t i = 0; i < params->n_aggressive_robots; i++)
    AddRobot(kAggressiveRobot);
  for (size_t i = 0; i < params->n_explore_robots; i++)
    AddRobot(kExploreRobot);
  for (size_t i = 0; i < params->n_love_robots; i++)
    AddRobot(kLoveRobot);

  AddEntity(kFood, params->n_foods);
  AddEntity(kLight, params->n_lights);
}

Arena::~Arena() {
  for (auto ent : entities_) {
    delete ent;
  } /* for(ent..) */
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Arena::AddRobot(EntityType robot_type) {
  bool isColliding = false;
  Robot* robot_ = NULL;
  do {
    isColliding = false;
    robot_ = dynamic_cast<Robot*>(factory_->CreateEntity(robot_type));
    for (auto tmp : entities_) {
      if (robot_ != NULL && tmp != NULL && IsColliding2(robot_, tmp)) {
        isColliding = true;
        delete robot_;
        break;
      }
    }
  } while (isColliding);
  if (robot_ != NULL) {
    entities_.push_back(robot_);
    mobile_entities_.push_back(robot_);
  } else {
    std::cout << "Cannot create robot\n";
  }
}

void Arena::AddEntity(EntityType type, int quantity) {
  for (int i = 0; i < quantity; i++) {
    bool isColliding = false;
    ArenaEntity *tmp = NULL;
    do {
      isColliding = false;
      tmp = factory_->CreateEntity(type);
      for (auto ent : entities_) {
        if (tmp != NULL && ent != NULL && IsColliding2(tmp, ent)) {
          isColliding = true;
          delete tmp;
          break;
        }
      }
    } while (isColliding);

    if (tmp != NULL) {
      entities_.push_back(tmp);
      if (type == kLight) {
        mobile_entities_.push_back(dynamic_cast<ArenaMobileEntity*>(tmp));
      }
    } else {
      std::cout << "Cannot create entity\n";
    }
  }
}

void Arena::Reset() {
  if (factory_ != NULL)
    delete factory_;

  entities_.erase(entities_.begin(), entities_.end());
  mobile_entities_.erase(mobile_entities_.begin(), mobile_entities_.end());
  observers_.erase(observers_.begin(), observers_.end());

  struct arena_params params;
  // initialize new objects
  factory_ = new EntityFactory(this);
  addEntitiesToArena(&params);
  game_status_ = PAUSING;
} /* reset() */

// The primary driver of simulation movement. Called from the Controller
// but originated from the graphics viewer.
void Arena::AdvanceTime(double dt) {
  if (!(dt > 0)) {
    return;
  }
  UpdateEntitiesTimestep();
} /* AdvanceTime() */

void Arena::UpdateEntitiesTimestep() {
  for (auto ent : entities_) {
    if (ent->get_type() == kLight || ent->get_type() == kFood) {
      state_.ent_ = ent;
      notify();
    }

    if (ent->get_type() == kRobot) {
      Robot *robot = dynamic_cast<Robot*>(ent);
      if (robot->isStarving()) {
        game_status_ = STOPPED;
      }
    }
  }

   /* Determine if any mobile entity is colliding with wall.
   * Adjust the position accordingly so it doesn't overlap.
   */
  for (auto &ent1 : mobile_entities_) {
    ent1->TimestepUpdate(1);
    EntityType wall = GetCollisionWall(ent1);
    if (kUndefined != wall) {
      AdjustWallOverlap(ent1, wall);
      ent1->HandleCollision(wall);
    }
    /* Determine if that mobile entity is colliding with any other entity.
    * Adjust the position accordingly so they don't overlap.
    */
    for (auto &ent2 : entities_) {
      if (ent2 == ent1) { continue; }
      if (IsColliding(ent1, ent2)) {
        if (ent1->get_type() == kRobot) {
          if (ent2->get_type() == kLight || ent2->get_type() == kFood) {
            continue;
          } else {
            AdjustEntityOverlap(ent1, ent2);
            ent1->HandleCollision(ent2->get_type(), ent2);
          }
        } else {  // ent1 is an light
          if (ent2->get_type() == kLight) {
            AdjustEntityOverlap(ent1, ent2);
            ent1->HandleCollision(kLight, ent2);
          }
        }
      }
    }
  }
}  // UpdateEntitiesTimestep()


// Determine if the entity is colliding with a wall.
// Always returns an entity type. If not collision, returns kUndefined.
EntityType Arena::GetCollisionWall(ArenaMobileEntity *const ent) {
  if (ent->get_pose().x + ent->get_radius() >= x_dim_) {
    return kRightWall;  // at x = x_dim_
  } else if (ent->get_pose().x - ent->get_radius() <= 0) {
    return kLeftWall;  // at x = 0
  } else if (ent->get_pose().y + ent->get_radius() >= y_dim_) {
    return kBottomWall;  // at y = y_dim
  } else if (ent->get_pose().y - ent->get_radius() <= 0) {
    return kTopWall;  // at y = 0
  } else {
    return kUndefined;
  }
} /* GetCollisionWall() */

/* The entity type indicates which wall the entity is colliding with.
* This determines which way to move the entity to set it slightly off the wall. */
void Arena::AdjustWallOverlap(ArenaMobileEntity *const ent, EntityType object) {
  Pose entity_pos = ent->get_pose();
  switch (object) {
    case (kRightWall):  // at x = x_dim_
    ent->set_position(x_dim_-(ent->get_radius()+5), entity_pos.y);
    break;
    case (kLeftWall):  // at x = 0
    ent->set_position(ent->get_radius()+5, entity_pos.y);
    break;
    case (kTopWall):  // at y = 0
    ent->set_position(entity_pos.x, ent->get_radius()+5);
    break;
    case (kBottomWall):  // at y = y_dim_
    ent->set_position(entity_pos.x, y_dim_-(ent->get_radius()+5));
    break;
    default:
    {}
  }
}

/* Calculates the distance between the center points to determine overlap */
bool Arena::IsColliding(
  ArenaMobileEntity * const mobile_e,
  ArenaEntity * const other_e) {
    if (mobile_e == NULL || other_e == NULL) {
      std::cout << "Invalid arguments for IsColliding\n";
      return false;
    }
    double delta_x = other_e->get_pose().x - mobile_e->get_pose().x;
    double delta_y = other_e->get_pose().y - mobile_e->get_pose().y;
    double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
    return
      (((mobile_e->get_radius() + other_e->get_radius()) > distance_between));
    //       > COLLISION_TOLERANCE);
}

/* Calculates the distance between the center points to determine overlap */
bool Arena::IsColliding2(
  ArenaEntity * const mobile_e,
  ArenaEntity * const other_e) {
    if (mobile_e == NULL || other_e == NULL) {
      std::cout << "Invalid arguments for IsColliding2\n";
      return false;
    }
    double delta_x = other_e->get_pose().x - mobile_e->get_pose().x;
    double delta_y = other_e->get_pose().y - mobile_e->get_pose().y;
    double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
    return
      (((mobile_e->get_radius() + other_e->get_radius()) - distance_between)
       > COLLISION_TOLERANCE);
}

/* This is called when it is known that the two entities overlap.
* We determine by how much they overlap then move the mobile entity to
* the edge of the other
*/
/* @TODO: Add functionality to Pose to determine the distance distance_between two instances (e.g. overload operator -)
*/
/* @BUG: The robot will pass through the home Food on occasion. The problem
 * is likely due to the adjustment being in the wrong direction. This could
 * be because the cos/sin generate the wrong sign of the distance_to_move
 * when the collision is in a specific quadrant relative to the center of the
 * colliding entities..
 */
void Arena::AdjustEntityOverlap(ArenaMobileEntity *const mobile_e,
  ArenaEntity *const other_e) {
  //    double delta_x = other_e->get_pose().x - mobile_e->get_pose().x;
  //    double delta_y = other_e->get_pose().y - mobile_e->get_pose().y;
    double delta_x = mobile_e->get_pose().x - other_e->get_pose().x;
    double delta_y = mobile_e->get_pose().y - other_e->get_pose().y;
    double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
    double distance_to_move =
      mobile_e->get_radius() + other_e->get_radius() - distance_between;
    //    double angle = atan(delta_y/delta_x);
    double angle = atan2(delta_y, delta_x);
    if (delta_x >= 0 && delta_y >= 0) {
      angle += M_PI;
    } else if (delta_x >= 0 && delta_y <= 0) {
      angle += M_PI;
    }

    mobile_e->set_position(
      mobile_e->get_pose().x+cos(angle)*distance_to_move,
      mobile_e->get_pose().y+sin(angle)*distance_to_move);
}

// Accept communication from the controller. Dispatching as appropriate.
/** @TODO: Call the appropriate Robot functions to implement user input
  * for controlling the robot.
  */
void Arena::AcceptCommand(Communication com) {
  switch (com) {
    case(kPlay):
      game_status_ = PLAYING;
      break;
    case(kPause):
      game_status_ = PAUSING;
      break;
    case(kReset): {
      Reset();
      break;
    }
    case(kNone):
    default: break;
  }
} /* AcceptCommand */

/*
 * Print the info of all entities in the arena to the console
 */
void Arena::printEntities() {
  for (auto e : entities_) {
    if (e->get_type() == kFood) {
      printf("%lf %lf %s\n", e->get_pose().x, e->get_pose().y,
            dynamic_cast<Food*>(e)->get_captured() ? "true" : "false");
    }
  }
}

void Arena::notify() {
  for (Observer *obs : observers_) {
    obs->Update(state_);
  }
}

NAMESPACE_END(csci3081);
