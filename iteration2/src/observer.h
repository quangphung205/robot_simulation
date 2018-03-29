/**
 * @file observer.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_OBSERVER_H_
#define SRC_OBSERVER_H_

#include "src/arena_entity.h"
#include "src/subject.h"
#include "src/state.h"
#include <iostream>

NAMESPACE_BEGIN(csci3081);

class Observer {
public:
  Observer(Subject *s) : subject_(s) {}
  Observer(Observer& o) : subject_(o.subject_) {}
  virtual void Update(const State) {};
  virtual ~Observer() {}

  Subject* get_subject() { return subject_; }

  Observer& operator=(const Observer& other) = default;
protected:
  Subject *subject_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_OBSERVER_H_
