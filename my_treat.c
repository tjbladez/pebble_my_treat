#include <pebble.h>
#define YOUR_TURN true

static Window *window;
static TextLayer *label_layer;
static TextLayer *body_layer;
static TextLayer *instruction_layer;
static bool main_scene;

static void show_payee() {
  text_layer_set_text(label_layer, "Your Turn?");
  if (persist_read_bool(YOUR_TURN)) {
    text_layer_set_text(body_layer, "Yes, pay up!");
  } else {
    text_layer_set_text(body_layer, "Nah, you are good.");
  }
  main_scene = false;
  text_layer_set_text(instruction_layer,"Press 'select' to update it");
}

static void restore_default(void *timer_data){
  main_scene = true;
  window_set_background_color(window, GColorWhite);
  text_layer_set_text(instruction_layer, "Press anything to see whose turn it is to pay.");
  text_layer_set_text(label_layer, "My Treat");
  text_layer_set_text(body_layer, "");
  text_layer_set_text_color(label_layer, GColorBlack);
  text_layer_set_text_color(instruction_layer, GColorBlack);
  text_layer_set_text_color(body_layer, GColorBlack);
}

static void update_payee() {
  window_set_background_color(window, GColorBlack);
  text_layer_set_text(body_layer, "Order updated!");
  text_layer_set_text_color(body_layer, GColorWhite);
  persist_write_bool(YOUR_TURN, !persist_read_bool(YOUR_TURN));
  vibes_short_pulse();
  app_timer_register(3000, restore_default, window);
}
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (main_scene){
    show_payee();
  } else {
    update_payee();
  }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  show_payee();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  show_payee();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  label_layer = text_layer_create(GRect(0, 6, 144, 40));
  text_layer_set_background_color(label_layer, GColorClear);
  text_layer_set_text_color(label_layer, GColorBlack);
  text_layer_set_text(label_layer, "My Treat");
  text_layer_set_font(label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(label_layer, GTextAlignmentCenter);

  body_layer = text_layer_create(GRect(0, 46, 144, 40));
  text_layer_set_background_color(body_layer, GColorClear);
  text_layer_set_text_color(body_layer, GColorBlack);
  text_layer_set_font(body_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(body_layer, GTextAlignmentCenter);

  instruction_layer = text_layer_create(GRect(10, 86, 124, 80));
  text_layer_set_background_color(instruction_layer, GColorClear);
  text_layer_set_text_color(instruction_layer, GColorBlack);
  text_layer_set_font(instruction_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(instruction_layer, GTextAlignmentCenter);
  text_layer_set_text(instruction_layer, "Press anything to see whose turn it is to pay.");

  layer_add_child(window_layer, text_layer_get_layer(label_layer));
  layer_add_child(window_layer, text_layer_get_layer(body_layer));
  layer_add_child(window_layer, text_layer_get_layer(instruction_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(label_layer);
  text_layer_destroy(body_layer);
}

static void initialize_variables() {
  if (!persist_exists(YOUR_TURN)){
    persist_write_bool(YOUR_TURN,true);
  }
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  main_scene = true;
  initialize_variables();
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
