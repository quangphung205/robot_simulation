/**
 * @file subject.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SUBJECT_H_
#define SRC_SUBJECT_H_

#include "src/observer.h"
#include <iostream>
#include <vector>

NAMESPACE_BEGIN(csci3081);

class Observer;

class Subject {
public:
  Subject() : observers_() {}
  void registerObserver(Observer *obs) {
    observers_.push_back(obs);
  }

  virtual void notify() {}
  virtual ~Subject() {}
protected:
  std::vector<Observer *> observers_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_SUBJECT_H_
