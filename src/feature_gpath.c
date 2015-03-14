/*
 * main.c
 * Creates a Window, Layer and assigns an `update_proc` to draw
 * the 'P' in the Pebble logo.
 */

#include <pebble.h>
#include "Utils.h"

static Window *s_main_window;
static Layer *hands_layer;
static int32_t hourAngle,minuteAngle,secondAngle;
static GBitmap *s_example_bitmap;
static BitmapLayer *s_bitmap_layer;

static void hands_update_proc(Layer *this_layer, GContext *ctx)
{
  drawHands(this_layer,ctx,hourAngle,minuteAngle,secondAngle);
}

static void main_window_load(Window *window)
{
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  //create Background
  APP_LOG(APP_LOG_LEVEL_INFO, "Window size: %d %d",window_bounds.size.w, window_bounds.size.h);
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  if (s_bitmap_layer==NULL)
  {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to create bitmap");
  }
  s_example_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BCKGRND);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_example_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));
  bitmap_layer_set_alignment(s_bitmap_layer, GAlignCenter);
  
  // Create Hands
  hands_layer = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  layer_add_child(window_layer, hands_layer);

  // Set the update_proc
  layer_set_update_proc(hands_layer, hands_update_proc);

}

static void main_window_unload(Window *window)
{
  // Destroy Layer
  layer_destroy(hands_layer);
  //destroy background
  gbitmap_destroy(s_example_bitmap);
  bitmap_layer_destroy(s_bitmap_layer);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  calculateNewHandAngles(tick_time, 
                         units_changed, 
                         &hourAngle,
                         &minuteAngle, 
                         &secondAngle);
  layer_mark_dirty(hands_layer);
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
