#include <pebble.h>
#include "utils.h"

// FIXME: Should this return something similar to layer_get_bounds
GPoint layer_get_center(Layer *layer) {
  GRect bounds = layer_get_bounds(layer);
  return GPoint(bounds.size.w / 2, bounds.size.h / 2);
}

uint8_t layer_get_max_radius(Layer *layer) {
  GRect bounds = layer_get_bounds(layer);
  if(bounds.size.w >= bounds.size.h) {
    return bounds.size.w / 2;
  }
  else {
    return bounds.size.h / 2;
  }
}
