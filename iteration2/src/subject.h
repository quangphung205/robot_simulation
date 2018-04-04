/**
 * @file subject.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SUBJECT_H_
#define SRC_SUBJECT_H_

#include "src/observer.h"
#include "src/state.h"
#include <iostream>
#include <vector>

NAMESPACE_BEGIN(csci3081);

class Observer;

class Subject {
public:
  Subject() : state_(), observers_() {}
  void registerObserver(Observer *obs) {
    observers_.push_back(obs);
  }

  virtual void notify() {}
  virtual ~Subject() {}
protected:
  State state_;
  std::vector<Observer *> observers_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_SUBJECT_H_
