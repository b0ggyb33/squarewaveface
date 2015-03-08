/*
 * main.c
 * Creates a Window, Layer and assigns an `update_proc` to draw
 * the 'P' in the Pebble logo.
 */

#include <pebble.h>
#include "Utils.h"

static Window *s_main_window;
static Layer *s_canvas_layer;

static int32_t hourAngle,minuteAngle,secondAngle;

static GBitmap *s_example_bitmap;
static BitmapLayer *s_bitmap_layer;


static void canvas_update_proc(Layer *this_layer, GContext *ctx)
{
  GRect bounds = layer_get_bounds(this_layer);

  unsigned int radius = 45;

  // Get the center of the screen (non full-screen)
  GPoint center = GPoint(bounds.size.w / 2, (bounds.size.h / 2));

  //HourCircle
  graphics_context_set_stroke_color(ctx, GColorMelon);
  graphics_draw_circle(ctx, center, radius);
  graphics_draw_circle(ctx, center, radius-1);
  graphics_draw_circle(ctx, center, radius+1);
  //Hour Line
  graphics_context_set_stroke_color(ctx, GColorRed);
  GPoint hourEdge = getGPointFromAngle(center,radius,hourAngle);
  graphics_draw_line(ctx,center,hourEdge);

  //minute
  GPoint minuteEdge = getGPointFromAngle(hourEdge,radius/2,minuteAngle);
  graphics_context_set_stroke_color(ctx, GColorLimerick);
  graphics_draw_line(ctx,hourEdge,minuteEdge);
  graphics_context_set_stroke_color(ctx, GColorLimerick);
  graphics_draw_circle(ctx, hourEdge, radius/2);
  graphics_draw_circle(ctx, hourEdge, radius/2+1);
  graphics_draw_circle(ctx, hourEdge, radius/2-1);

  //second
  GPoint secondEdge = getGPointFromAngle(minuteEdge,radius/4,secondAngle);
  graphics_context_set_stroke_color(ctx, GColorBlue);
  graphics_draw_line(ctx,minuteEdge,secondEdge);
  graphics_context_set_stroke_color(ctx, GColorPictonBlue);
  graphics_draw_circle(ctx, minuteEdge, radius/4);
  graphics_draw_circle(ctx, minuteEdge, radius/4+1);
  graphics_draw_circle(ctx, minuteEdge, radius/4-1);

}

static void main_window_load(Window *window)
{
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  //create Background
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  bitmap_layer_set_bitmap(s_bitmap_layer, s_example_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));
  bitmap_layer_set_alignment(s_bitmap_layer, GAlignCenter);
  bitmap_layer_mark_dirty(s_bitmap_layer);
  // Create Layer
  s_canvas_layer = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  layer_add_child(window_layer, s_canvas_layer);

  // Set the update_proc
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);

}

static void main_window_unload(Window *window)
{
  // Destroy Layer
  layer_destroy(s_canvas_layer);
  //destroy background
  gbitmap_destroy(s_example_bitmap);
  bitmap_layer_destroy(s_bitmap_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  hourAngle = TRIG_MAX_ANGLE * tick_time->tm_hour / 12;
  minuteAngle = TRIG_MAX_ANGLE * tick_time->tm_min / 60;
  secondAngle = TRIG_MAX_ANGLE * tick_time->tm_sec / 60;

  hourAngle+=minuteAngle/12;
  minuteAngle+=secondAngle/60;

  layer_mark_dirty(s_canvas_layer);
}

static void init(void)
{
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);

  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

  hourAngle=0;
  minuteAngle=0;
  secondAngle=0;

  s_example_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WHITE_BCKGRND);
  //144x168
}

static void deinit(void)
{
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}
