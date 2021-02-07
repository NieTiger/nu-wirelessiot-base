// BLE Service example app
//
// Creates a BLE service and blinks an LED

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "simple_ble.h"

#include "nrf52840dk.h"

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
  // c0:98:e5:4e:xx:xx
  .platform_id       = 0x4E,    // used as 4th octect in device BLE address
  .device_id         = 0xAABB,
  .adv_name          = "CS397/497", // used in advertisements if there is room
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

// LED service
static simple_ble_service_t led_service = {{
  .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
              0xB5,0x4D,0x22,0x2B,0x88,0x10,0xE6,0x32}
}};

static simple_ble_char_t led_state_char = {.uuid16 = 0x1089};
static bool led_state = false;


// Print service
// prints the byte written by the client to RTT
static simple_ble_service_t print_service = {{
  .uuid128 = {0x3f,0xbf,0x78,0x96,0xfe,0x6f,0x89,0x98,
              0xe3,0xdf,0xfa,0x52,0x60,0x1f,0x31,0xf7}
}};

static simple_ble_char_t print_state_char = {.uuid16 = 0x108a};
static bool print_state = false;


// Button service
// Notifies client when a button is pressed
static simple_ble_service_t button_service = {{
  .uuid128 = {0x4e,0x80,0x3d,0xd8,0xe6,0x54,0x70,0x7c,
              0x62,0x02,0xde,0x5a,0x60,0x1f,0x33,0xd3}
}};

static simple_ble_char_t button_state_char = {.uuid16 = 0x108b};
static uint8_t button_val = 0;



/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {

  // Check LED characteristic
  if (simple_ble_is_char_event(p_ble_evt, &led_state_char)) {
    printf("Got write to LED characteristic!\n");

    // Use value written to control LED
    if (led_state != 0) {
      printf("Turning on LED!\n");
      nrf_gpio_pin_clear(LED1);
    } else {
      printf("Turning off LED!\n");
      nrf_gpio_pin_set(LED1);
    }
  }
  
  // Check Print service
  if (simple_ble_is_char_event(p_ble_evt, &print_state_char)) {
      printf("Print service: 0x%X\n", print_state);
  }
}


static inline void handle_button(const int button, bool *button_state, int button_i) {
  if (nrf_gpio_pin_read(button)) {
    // if (*button_state) {
      // printf("Button %d released\n", button_i);
    // }
    *button_state = false;
  } else {
    if (!*button_state) {
      printf("Button %d pressed\n", button_i);
      button_val = button_i;
      simple_ble_notify_char(&button_state_char);
    }
    *button_state = true;
  }
}


int main(void) {

  printf("Board started. Initializing BLE: \n");

  // Setup LED GPIO
  nrf_gpio_cfg_output(LED1);

  // Setup button inputs
  nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  // LED service
  simple_ble_add_service(&led_service);
  simple_ble_add_characteristic(1, 1, 0, 0,
      sizeof(led_state), (uint8_t*)&led_state,
      &led_service, &led_state_char);

  // Print service
  simple_ble_add_service(&print_service);
  simple_ble_add_characteristic(1, 1, 0, 0,
      sizeof(print_state), (uint8_t*)&print_state,
      &print_service, &print_state_char);

  // Button service
  simple_ble_add_service(&button_service);
  simple_ble_add_characteristic(1, 0, 1, 0,
      sizeof(button_val), (uint8_t*)&button_val,
      &button_service, &button_state_char);

  // Setup variables
  bool b1 = false;
  bool b2 = false;
  bool b3 = false;
  bool b4 = false;

  // Start Advertising
  simple_ble_adv_only_name();

  while(1) {
    handle_button(BUTTON1, &b1, 1);
    handle_button(BUTTON2, &b2, 2);
    handle_button(BUTTON3, &b3, 3);
    handle_button(BUTTON4, &b4, 4);
  }
}

