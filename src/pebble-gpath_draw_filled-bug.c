#include <pebble.h>

static Window *window;
static Layer *my_layer;

#define TOP_MARGIN 30
#define HEIGHT 100

void layer_update_proc(Layer *layer, GContext *ctx) {
  static GPoint points[3] = {
    { .x = 10, .y = 0 },
    { .x = 124, .y = 0 },
    { .x = 57, .y = HEIGHT }
  };
  static GPathInfo pathinfo = { .num_points = 3, .points = points };
  GPath *path = gpath_create(&pathinfo);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  gpath_draw_outline(ctx, path);
  gpath_draw_filled(ctx, path);
  gpath_destroy(path);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  my_layer = layer_create((GRect) { .origin = { 0,TOP_MARGIN }, .size = { bounds.size.w, HEIGHT} });
  layer_set_update_proc(my_layer, layer_update_proc);
  layer_add_child(window_layer, my_layer);
}

static void window_unload(Window *window) {
  layer_destroy(my_layer);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
