/**
 * @file subject.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_SUBJECT_H_
#define SRC_SUBJECT_H_

#include <iostream>
#include <vector>
#include "src/observer.h"
#include "src/state.h"

NAMESPACE_BEGIN(csci3081);

class Observer;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class to implement the Observer Pattern
 *
 * Sensors will be observers and arena will be the subject
 */
class Subject {
 public:
  /**
   * @brief default constructor for Subject class
   */
  Subject() : state_(), observers_() {}
  void registerObserver(Observer *obs) {
    observers_.push_back(obs);
  }

  /**
   * @brief notify all observers in the list
   */
  virtual void notify() {}

  /**
   * @brief default destructor for Subject class
   */
  virtual ~Subject() {}

 protected:
  State state_;  // contains stimulus information
  std::vector<Observer *> observers_;  // list of observers
};

NAMESPACE_END(csci3081);

#endif  // SRC_SUBJECT_H_
