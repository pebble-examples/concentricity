// Copyright [2015] Pebble Technology

#include <pebble.h>
#include "./ui.h"

// Define struct to store colors for each time unit
typedef struct Palette {
  GColor seconds;
  GColor minutes;
  GColor hours;
} Palette;

static Window *s_window;
static Layer *s_layer;
static Palette *s_palette;

static uint8_t s_hour;
static uint8_t s_minute;
static uint8_t s_second;

// Set the color for drawing routines
static void prv_set_color(GContext *ctx, GColor color) {
  graphics_context_set_fill_color(ctx, color);
}

// Update the watchface display
static void prv_update_display(Layer *layer, GContext *ctx) {
  prv_set_color(ctx, s_palette->seconds);
  draw_seconds(ctx, s_second, layer);

  prv_set_color(ctx, s_palette->minutes);
  draw_minutes(ctx, s_minute, layer);

  prv_set_color(ctx, s_palette->hours);
  draw_hours(ctx, s_hour % 12, layer);
}

// Update the current time values for the watchface
static void prv_update_time(struct tm *tick_time) {
  s_hour = tick_time->tm_hour;
  s_minute = tick_time->tm_min;
  s_second = tick_time->tm_sec;
  layer_mark_dirty(s_layer);
}

static void prv_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  prv_update_time(tick_time);
}

static void prv_unobstructed_area_did_change_handler(void *context) {
  layer_mark_dirty(s_layer);
}

static void prv_window_load(Window *window) {
  s_palette = malloc(sizeof(Palette));
  *s_palette = (Palette) {
      PBL_IF_COLOR_ELSE(GColorRichBrilliantLavender, GColorWhite),
      PBL_IF_COLOR_ELSE(GColorVividViolet, GColorWhite),
      PBL_IF_COLOR_ELSE(GColorBlueMoon, GColorWhite)
  };
  set_number_time_units(3);

  s_layer = layer_create(layer_get_bounds(window_get_root_layer(s_window)));
  layer_add_child(window_get_root_layer(s_window), s_layer);
  layer_set_update_proc(s_layer, prv_update_display);

  unobstructed_area_service_subscribe((UnobstructedAreaHandlers){
      .did_change = prv_unobstructed_area_did_change_handler
  }, NULL);
}

static void prv_window_unload(Window *window) {
  unobstructed_area_service_unsubscribe();
  free(s_palette);
  layer_destroy(s_layer);
}

static void prv_init(void) {
  s_window = window_create();

  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload
  });
  window_set_background_color(s_window, GColorBlack);
  window_stack_push(s_window, true);

  time_t start = time(NULL);
  tick_timer_service_subscribe(SECOND_UNIT, prv_tick_handler);
  prv_update_time(localtime(&start));  // NOLINT(runtime/threadsafe_fn)
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
