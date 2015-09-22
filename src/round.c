#if defined(PBL_ROUND)

#include <pebble.h>
#include "round.h"
#include "utils.h"

#define TIME_ANGLE(time) time * (TRIG_MAX_ANGLE / 60)
#define HOUR_ANGLE(hour) hour * (TRIG_MAX_ANGLE / 12)

// Draw an arc with given inner/outer radii 
static void draw_arc(GContext *ctx, Layer *layer, uint16_t inner_radius, uint16_t outer_radius, uint32_t end_angle) {
  if(end_angle == 0) {
    graphics_fill_radial(ctx, layer_get_center(layer), inner_radius, outer_radius, 0, TRIG_MAX_ANGLE);
  }
  else {
    graphics_fill_radial(ctx, layer_get_center(layer), inner_radius, outer_radius, 0, end_angle); 
  }
}

// Handle representation for seconds
void draw_seconds(GContext *ctx, uint8_t seconds, Layer *layer) {
  draw_arc(ctx, layer, 70, 90, TIME_ANGLE(seconds));
}

// Handle representation for minutes
void draw_minutes(GContext *ctx, uint8_t minutes, Layer *layer) {
  draw_arc(ctx, layer, 40, 60, TIME_ANGLE(minutes));
}

// Handle representation for hours
void draw_hours(GContext *ctx, uint8_t hours, Layer *layer) {
  draw_arc(ctx, layer, 10, 30, HOUR_ANGLE(hours));
}

#endif
