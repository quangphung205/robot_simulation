/**
 * @file state.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_STATE_H_
#define SRC_STATE_H_

#include "src/arena_entity.h"
#include <iostream>

NAMESPACE_BEGIN(csci3081);

struct State {
public:
  ArenaEntity *ent_;

  State() : ent_(NULL) {}
  State(ArenaEntity *e) : ent_(e) {}

  State& operator=(const State& other) = default;
};

NAMESPACE_END(csci3081);

#endif
