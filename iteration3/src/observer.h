/**
 * @file observer.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_OBSERVER_H_
#define SRC_OBSERVER_H_

#include <iostream>
#include "src/arena_entity.h"
#include "src/subject.h"
#include "src/state.h"

NAMESPACE_BEGIN(csci3081);
/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class to implement the Observer Pattern
 *
 * Sensors will be observers and arena will be the subject
 */
class Observer {
 public:
  /**
   * @brief Default constructor
   */
  explicit Observer(Subject *s) : subject_(s) {}

  /**
   * @brief Default constructor
   */
  Observer(const Observer& o) : subject_(o.subject_) {}

  /**
   * @brief update the state
   *
   * @param state new state
   */
  virtual void Update(const State) {}

  /**
   * @brief Default destructor
   */
  virtual ~Observer() {}

  /**
   * @brief get the subject
   *
   * @return current subject
   */
  Subject* get_subject() { return subject_; }

  /**
   * @brief set the subject
   *
   * @param s new subject
   */
  void set_subject(Subject *s) { subject_ = s;}

  /**
   * @brief overload the assignment operator
   */
  Observer& operator=(const Observer& other) = default;

 protected:
  Subject *subject_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_OBSERVER_H_
