#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <Message.hpp>

typedef struct ControlType {
  int8_t x;
  int8_t y;
} control_t;

//  create message
typedef sb::Message<control_t> Control;

#endif