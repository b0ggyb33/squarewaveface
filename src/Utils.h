#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <pebble.h>

GPoint getGPointFromAngle(GPoint center,int radius,int angle)
{
  //secondHand.y = (-cos_lookup(second_angle) * secondHandLength / TRIG_MAX_RATIO) + center.y;
  //secondHand.x = (sin_lookup(second_angle) * secondHandLength / TRIG_MAX_RATIO) + center.x;
  GPoint edge;
  edge.y = (-cos_lookup(angle) * radius / TRIG_MAX_RATIO) + center.y;
  edge.x = (sin_lookup(angle) * radius / TRIG_MAX_RATIO) + center.x;
  return edge;
}

#endif // UTILS_H_INCLUDED
