// Copyright [2015] Pebble Technology

#if defined(PBL_RECT)

#include <pebble.h>
#include "./ui.h"

static uint8_t s_number_time_units;

static uint16_t prv_get_stroke_width(Layer *layer) {
  GRect bounds = layer_get_unobstructed_bounds(layer);
  return ((bounds.size.w / s_number_time_units) / 2) - PADDING;
}

static uint8_t prv_calculate_inset(Layer *layer, uint8_t arc_id) {
  return (prv_get_stroke_width(layer) + PADDING) * arc_id;
}

static void prv_draw_border(GContext *ctx, Layer *layer, uint8_t arc_id,
                            uint8_t segment, uint8_t total_segments) {
  // Calculate x, y, width, height of border
  uint8_t inset = prv_calculate_inset(layer, arc_id);
  GRect bounds = layer_get_unobstructed_bounds(layer);
  GPoint layer_center = grect_center_point(&bounds);

  uint16_t x = layer_center.x - (bounds.size.w / 2) + inset;
  uint16_t y = layer_center.y - (bounds.size.h / 2) + inset;
  uint16_t width = layer_center.x + (bounds.size.w / 2) - (inset * 2);
  uint16_t height = layer_center.y + (bounds.size.h / 2) - (inset * 2);
  uint16_t stroke_width = prv_get_stroke_width(layer);

  // Calculate max no of pixels in perimeter less the size of the border at
  // the four corners
  uint16_t perimeter_max = ((width * 2) + (height * 2)) - (stroke_width * 4);

  // Calculate the corners
  uint16_t top_right = width / 2;
  uint16_t bottom_right = top_right + height - stroke_width;
  uint16_t bottom_left = bottom_right + width - stroke_width;
  uint16_t top_left = bottom_left + height - stroke_width;

  // Calculate our current position around the perimeter
  uint16_t perimeter_position = (segment * perimeter_max) / total_segments;

  // Make sure we don't exceed maximum
  if (perimeter_position > perimeter_max) {
    perimeter_position = perimeter_max;
  }

  // Prefill any completed sides
  // Prefill top right
  if (perimeter_position > top_right) {
    graphics_fill_rect(ctx,
                       GRect((width / 2) + x, y, width / 2, stroke_width),
                       0,
                       GCornerNone);
  }

  // Prefill right
  if (perimeter_position > bottom_right) {
    graphics_fill_rect(ctx,
                       GRect(width - stroke_width + x,
                             stroke_width + y,
                             stroke_width,
                             height - stroke_width),
                       0,
                       GCornerNone);
  }

  // Prefill bottom
  if (perimeter_position > bottom_left) {
    graphics_fill_rect(ctx,
                       GRect(x,
                             height - stroke_width + y,
                             width - stroke_width,
                             stroke_width),
                       0,
                       GCornerNone);
  }

  // Fill left
  if (perimeter_position > top_left) {
    graphics_fill_rect(ctx,
                       GRect(x, y, stroke_width, height - stroke_width),
                       0,
                       GCornerNone);
  }

  // Draw from the last filled side to our current position
  if (perimeter_position >= top_left) {
    // TOP LEFT to TOP MIDDLE
    graphics_fill_rect(ctx,
                       GRect(stroke_width + x,
                             y,
                             perimeter_position - top_left,
                             stroke_width),
                       0,
                       GCornerNone);
  } else if (perimeter_position <= top_right) {
    // TOP MIDDLE to TOP RIGHT
    graphics_fill_rect(ctx,
                       GRect((width / 2) + x,
                             y,
                             perimeter_position,
                             stroke_width),
                       0,
                       GCornerNone);
  } else if (perimeter_position <= bottom_right) {
    // TOP RIGHT to BOTTOM RIGHT
    graphics_fill_rect(ctx,
                       GRect(width - stroke_width + x,
                             stroke_width + y,
                             stroke_width,
                             perimeter_position - top_right),
                       0,
                       GCornerNone);
  } else if (perimeter_position <= bottom_left) {
    // BOTTOM RIGHT to BOTTOM LEFT
    graphics_fill_rect(ctx,
                       GRect(width - stroke_width -
                                (perimeter_position - bottom_right) + x,
                             height - stroke_width + y,
                             perimeter_position - bottom_right,
                             stroke_width),
                       0,
                       GCornerNone);
  } else if (perimeter_position < top_left) {
    // BOTTOM LEFT to TOP LEFT
    graphics_fill_rect(ctx,
                       GRect(x,
                             height - stroke_width -
                                (perimeter_position - bottom_left) + y,
                             stroke_width,
                             perimeter_position - bottom_left),
                       0,
                       GCornerNone);
  }
}

void set_number_time_units(uint8_t number_time_units) {
  s_number_time_units = number_time_units;
}

// Handle representation of seconds
void draw_seconds(GContext *ctx, uint8_t seconds, Layer *layer) {
  prv_draw_border(ctx, layer, 0, seconds + 1, 60);
}

// Handle representation of minutes
void draw_minutes(GContext *ctx, uint8_t minutes, Layer *layer) {
  prv_draw_border(ctx, layer, 1, minutes + 1, 60);
}

// Handle representation of hours
void draw_hours(GContext *ctx, uint8_t hours, Layer *layer) {
  if (hours > 12) { hours -= 12; }
  if (hours == 0) { hours = 12; }

  prv_draw_border(ctx, layer, 2, hours, 12);
}

#endif
