// Copyright [2015] Pebble Technology

#if defined(PBL_RECT)

#include <pebble.h>
#include "./ui.h"

#define SCALED_TIME(time) ((time) * 100) / 60
#define SCALED_HOUR(hour) ((hour) * 100) / 12

// Define struct to store the numerical values used for drawing a given border
typedef struct Border {
  uint16_t top_right;
  uint16_t right;
  uint16_t bottom;
  uint16_t left;
  uint16_t top_left;
  GRect bounds;
  uint16_t max_horizontal;
  uint16_t max_vertical;
} Border;

/*
Calculate & return a Border object for the given layer, desired padding, and
current time
*/
static Border calculate_border(Layer *layer,
                               uint8_t padding,
                               uint32_t scaled_time) {
  GRect bounds = layer_get_bounds(layer);
  // Remove padding from both ends of the side
  uint16_t max_horizontal = bounds.size.w - (2 * padding);
  // Remove padding from both ends of the side
  uint16_t max_vertical = bounds.size.h - (2 * padding);
  Border border = {0, 0, 0, 0, 0, bounds, max_horizontal, max_vertical};

  // Remove duplicated corner pixels
  uint32_t num_pixels = 2 * (max_horizontal - 1) + 2 * (max_vertical - 1);
  uint32_t progress = (scaled_time * num_pixels) / 100;

  // Check for invalid time values
  if (progress > num_pixels) {
    APP_LOG(APP_LOG_LEVEL_DEBUG,
            "Scaled time is out of bounds, expecting <%lu, got %lu",
            num_pixels,
            progress);
  }

  if (progress == 0) {
    // 12 exactly
    border.top_left = max_horizontal / 2;
    border.top_right = max_horizontal / 2;
    border.right = max_vertical;
    border.bottom = max_horizontal;
    border.left = max_vertical;
    return border;
  }
  if (progress < (max_horizontal / 2)) {
    // Between 12 and 1:30
    border.top_right = progress;
    return border;
  } else {
    border.top_right = max_horizontal / 2;
  }
  if (progress < ((max_horizontal / 2) + max_vertical)) {
    // Between 1:30 and 4:30
    border.right = progress - (max_horizontal / 2);
    return border;
  } else {
    border.right = max_vertical;
  }
  if (progress <
     (uint32_t)((max_horizontal / 2) + max_vertical + max_horizontal)) {
    // Between 4:30 and 7:30
    border.bottom = progress - (max_horizontal / 2) - max_vertical;
    return border;
  } else {
    border.bottom = max_horizontal;
  }
  if (progress <
     (uint32_t)((max_horizontal / 2) + (2 * max_vertical) + max_horizontal)) {
    // Between 7:30 and 10:30
    border.left = progress - (max_horizontal / 2) - max_vertical -
                  max_horizontal;
    return border;
  } else {
    // Between 10:30 and 12
    border.left = max_vertical;
    border.top_left = progress - (max_horizontal / 2) - (2 * max_vertical) -
                      max_horizontal;
  }
  return border;
}

// Draw a border, one segment at a time
static void draw_border(GContext *ctx,
                        Border border,
                        uint8_t padding,
                        uint8_t stroke_width) {
  // Top-left to top-middle
  graphics_fill_rect(ctx,
                     GRect(border.bounds.origin.x + padding,
                           border.bounds.origin.y + padding,
                           border.top_left,
                           stroke_width),
                     0,
                     0);

  // Top-middle to top_right
  graphics_fill_rect(ctx,
                     GRect(border.bounds.origin.x + padding +
                             (border.max_horizontal / 2),
                           border.bounds.origin.y + padding,
                           border.top_right,
                           stroke_width),
                     0,
                     0);
  // Top-left to bottom-left
  graphics_fill_rect(ctx,
                     GRect(border.bounds.origin.x + padding,
                           border.bounds.origin.y + padding +
                             (border.max_vertical - border.left),
                           stroke_width,
                           border.left),
                     0,
                     0);
  // Top-right to bottom-right
  graphics_fill_rect(ctx,
                     GRect(border.max_horizontal + padding - stroke_width,
                           border.bounds.origin.y + padding,
                           stroke_width,
                           border.right),
                     0,
                     0);
  // Bottom-left to bottom-right
  graphics_fill_rect(ctx,
                     GRect(border.bounds.origin.x + padding +
                             (border.max_horizontal - border.bottom),
                           border.max_vertical + padding - stroke_width,
                           border.bottom,
                           stroke_width),
                     0,
                     0);
}

// Handle representation of seconds
void draw_seconds(GContext *ctx, uint8_t seconds, Layer *layer) {
  uint8_t padding = 0;
  uint8_t stroke_width = 14;
  draw_border(ctx,
              calculate_border(layer, padding, SCALED_TIME(seconds)),
              padding,
              stroke_width);
}

// Handle representation of minutes
void draw_minutes(GContext *ctx, uint8_t minutes, Layer *layer) {
  uint8_t padding = 20;
  uint8_t stroke_width = 14;
  draw_border(ctx,
              calculate_border(layer, padding, SCALED_TIME(minutes)),
              padding,
              stroke_width);
}

// Handle representation of hours
void draw_hours(GContext *ctx, uint8_t hours, Layer *layer) {
  uint8_t padding = 40;
  uint8_t stroke_width = 14;
  draw_border(ctx,
              calculate_border(layer, padding, SCALED_HOUR(hours)),
              padding,
              stroke_width);
}

#endif
