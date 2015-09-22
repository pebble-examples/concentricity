#include <pebble.h>

#if defined(PBL_RECT)
#include "rect.h"
#elif defined(PBL_ROUND)
#include "round.h"
#endif

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

// Generate a color (randomized color on >SDK2) to use for drawing routines
static GColor get_draw_color() {
  GColor color = GColorWhite;
  
  #ifndef PBL_SDK_2
  do {
    color = GColorFromRGB(rand() % 255, rand() % 255, rand() % 255);
  } while(gcolor_equal(color, GColorBlack));
  #endif
  
  return color;
}

// Set the color for drawing routines 
static void set_color(GContext *ctx, GColor color) {
  graphics_context_set_fill_color(ctx, color);
}

// Update the watchface display
static void update_display(Layer *layer, GContext *ctx) {
  set_color(ctx, s_palette->seconds);
  draw_seconds(ctx, s_second, layer);

  set_color(ctx, s_palette->minutes);
  draw_minutes(ctx, s_minute, layer);

  set_color(ctx, s_palette->hours);
  draw_hours(ctx, s_hour % 12, layer);
  
  if(s_second == 0) { s_palette->seconds = get_draw_color(); }
  if(s_minute == 0 && s_second == 0) { s_palette->minutes = get_draw_color(); }
  if(s_hour % 12 == 0 && s_minute == 0 && s_second == 0) { s_palette->hours = get_draw_color(); }
}

// Update the current time values for the watchface
static void update_time(struct tm *tick_time) {
  s_hour = tick_time->tm_hour;
  s_minute = tick_time->tm_min;
  s_second = tick_time->tm_sec;
  layer_mark_dirty(s_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

static void window_load(Window *window) {
  s_palette = malloc(sizeof(Palette));
  *s_palette = (Palette) {
    get_draw_color(),
    get_draw_color(),
    get_draw_color()
  };
  
  s_layer = layer_create(layer_get_bounds(window_get_root_layer(s_window)));
  layer_add_child(window_get_root_layer(s_window), s_layer);
  layer_set_update_proc(s_layer, update_display);
}

static void window_unload(Window *window) {
  free(s_palette);
  layer_destroy(s_layer);
}

static void init(void) {
  s_window = window_create();
  
  #ifdef PBL_SDK_2
  window_set_fullscreen(s_window, true);
  #endif
  
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_set_background_color(s_window, GColorBlack);
  window_stack_push(s_window, true);

  time_t start = time(NULL);
  srand(102784);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  update_time(localtime(&start));
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
