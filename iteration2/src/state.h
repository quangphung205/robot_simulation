/**
 * @file state.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_STATE_H_
#define SRC_STATE_H_

#include <iostream>
#include "src/arena_entity.h"

NAMESPACE_BEGIN(csci3081);
/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class contains all information about the stimulus
 */
struct State {
 public:
  ArenaEntity *ent_;  // contains stimuli information

  /**
   * @brief default constructor for State structure
   */
  State() : ent_(NULL) {}

  /**
   * @brief default constructor for State structure
   */
  explicit State(ArenaEntity *e) : ent_(e) {}

  /**
   * @brief overload the assignment operator
   */
  State& operator=(const State& other) = default;
};

NAMESPACE_END(csci3081);

#endif  // SRC_STATE_H_
