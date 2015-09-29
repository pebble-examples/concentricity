// Copyright [2015] Pebble Technology

#if defined(PBL_RECT)

#include <pebble.h>
#include "./ui.h"

  
void draw_border(GContext *ctx, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t sw, uint8_t segment, uint8_t total_segments){
   // Calculate max no of pixels in perimeter less the size of the border at the four corners
   uint16_t perimmax = ((w*2)+(h*2)) - (sw*4);

   // Calculate the corners
   uint16_t c1 = w / 2;	      // TOP LEFT
   uint16_t c2 = c1 + h - sw;	// BOTTOM RIGHT
   uint16_t c3 = c2 + w - sw;	// BOTTOM LEFT
   uint16_t c4 = c3 + h - sw;  // TOP LEFT

   // Calculate our current position around the perimiter
   uint16_t perimpos = (segment * perimmax) / total_segments;

   // Make sure we don't exceed maximum
  if (perimpos > perimmax) {perimpos = perimmax;}
  
   // Prefill any completed sides
   if (perimpos > c1) { graphics_fill_rect(ctx, GRect((w/2)+x, y, w/2, sw), 0, GCornerNone);}	  // Prefill top right
   if (perimpos > c2) { graphics_fill_rect(ctx, GRect(w-sw+x, sw+y, sw, h-sw), 0, GCornerNone);} // prefill right
   if (perimpos > c3) { graphics_fill_rect(ctx, GRect(x, h-sw+y, w-sw, sw), 0, GCornerNone);} // prefill bottom
   if (perimpos > c4) { graphics_fill_rect(ctx, GRect(x, y, sw, h-sw), 0, GCornerNone);} // fill left

   // Draw from the last filled side to our current position
   if (perimpos >= c4) {
	  // TOP LEFT to TOP MIDDLE
	  graphics_fill_rect(ctx, GRect(sw+x, y, perimpos-c4, sw), 0, GCornerNone);
   }
  else if (perimpos <= c1) {
	  // TOP MIDDLE to TOP RIGHT
	  graphics_fill_rect(ctx, GRect((w/2)+x, y, perimpos, sw), 0, GCornerNone);
  }
  else if (perimpos <= c2) {
	  // TOP RIGHT to BOTTOM RIGHT
	  graphics_fill_rect(ctx, GRect(w-sw+x, sw+y, sw, perimpos-c1), 0, GCornerNone);
  }
  else if (perimpos <= c3) {
	  // BOTTOM RIGHT to BOTTOM LEFT
	  graphics_fill_rect(ctx, GRect(w-sw-(perimpos-c2)+x, h-sw+y, perimpos-c2, sw), 0, GCornerNone);
  }
  else if (perimpos < c4) {
	  // BOTTOM LEFT to TOP LEFT
	  graphics_fill_rect(ctx, GRect(x, h-sw-(perimpos-c3)+y, sw, perimpos-c3), 0, GCornerNone);
  }
}
  
  
// Handle representation of seconds
void draw_seconds(GContext *ctx, uint8_t seconds, Layer *layer) {
  uint8_t padding = 0;
  uint8_t stroke_width = 14;
  GRect bounds = layer_get_bounds(layer);
  GPoint layer_center = grect_center_point(&bounds);
  uint8_t width = bounds.size.w;
  uint8_t height = bounds.size.h;
  
  draw_border(ctx, layer_center.x-(width/2)+padding, layer_center.y-(height/2)+padding, layer_center.x+(width/2)-(padding*2), layer_center.y+(height/2)-(padding*2), stroke_width, seconds+1, 60);
}

// Handle representation of minutes
void draw_minutes(GContext *ctx, uint8_t minutes, Layer *layer) {
  uint8_t padding = 20;
  uint8_t stroke_width = 14;
  GRect bounds = layer_get_bounds(layer);
  GPoint layer_center = grect_center_point(&bounds);
  uint8_t width = bounds.size.w;
  uint8_t height = bounds.size.h;
  
  draw_border(ctx, layer_center.x-(width/2)+padding, layer_center.y-(height/2)+padding, layer_center.x+(width/2)-(padding*2), layer_center.y+(height/2)-(padding*2), stroke_width, minutes+1, 60);
}

// Handle representation of hours
void draw_hours(GContext *ctx, uint8_t hours, Layer *layer) {
  uint8_t padding = 40;
  uint8_t stroke_width = 14;
  GRect bounds = layer_get_bounds(layer);
  GPoint layer_center = grect_center_point(&bounds);
  uint8_t width = bounds.size.w;
  uint8_t height = bounds.size.h;
  
  if (hours > 12) { hours -= 12;}
  if (hours == 0) { hours = 12;}
  
  draw_border(ctx, layer_center.x-(width/2)+padding, layer_center.y-(height/2)+padding, layer_center.x+(width/2)-(padding*2), layer_center.y+(height/2)-(padding*2), stroke_width, hours, 12);
}

#endif
