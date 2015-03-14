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

void drawClock(Layer *this_layer,
               GContext *ctx,
               GPoint centrePoint,
               GPoint handPoint,
               GColor colour,
               unsigned int clockSize)
{
  //Circle
  graphics_context_set_stroke_color(ctx, colour);
  graphics_draw_circle(ctx, centrePoint, clockSize);
  graphics_draw_circle(ctx, centrePoint, clockSize-1);
  graphics_draw_circle(ctx, centrePoint, clockSize+1);
  // Line
  graphics_context_set_stroke_color(ctx, colour);
  graphics_draw_line(ctx,centrePoint,handPoint);
}

void drawHands(Layer *this_layer, 
                      GContext *ctx,
                      int32_t hourAngle,
                      int32_t minuteAngle,
                      int32_t secondAngle)
{
  GRect bounds = layer_get_bounds(this_layer);

  unsigned int radius = 45;

  GColor hourColour,minuteColour,secondColour;
  
  #ifdef PBL_COLOR 
    hourColour=GColorRed;    
    minuteColour=GColorLimerick;
    secondColour=GColorBlue;
  #else
    hourColour = minuteColour = secondColour = GColorBlack;
  #endif
    
  // Get the center of the screen (non full-screen)
  GPoint center = GPoint(bounds.size.w / 2, (bounds.size.h / 2));
  GPoint hourHand = getGPointFromAngle(center,radius,hourAngle);
  drawClock(this_layer,ctx,center,hourHand,hourColour,radius);
  
  //minute
  GPoint minuteHand = getGPointFromAngle(hourHand,radius/2,minuteAngle);
  drawClock(this_layer,ctx,hourHand,minuteHand,minuteColour,radius/2);

  //second
  GPoint secondHand = getGPointFromAngle(minuteHand,radius/4,secondAngle);
  drawClock(this_layer,ctx,minuteHand,secondHand,secondColour,radius/4);

}


void calculateNewHandAngles(struct tm *tick_time, 
                            TimeUnits units_changed,
                            int32_t* hourAngle,
                            int32_t* minuteAngle,
                            int32_t* secondAngle
                           )
{
  *hourAngle = TRIG_MAX_ANGLE * tick_time->tm_hour / 12;
  *minuteAngle = TRIG_MAX_ANGLE * tick_time->tm_min / 60;
  *secondAngle = TRIG_MAX_ANGLE * tick_time->tm_sec / 60;

  *hourAngle+=*minuteAngle/12;
  *minuteAngle+=*secondAngle/60;
  
}

#endif // UTILS_H_INCLUDED
