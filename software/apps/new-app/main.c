// new-app
//
// Use buttons and switches to control LEDs

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrfx_glue.h"
#include "nrfx_gpiote.h"
#include "nrfx_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

// Pin definitions
#include "nrf52840dk.h"

static inline void handle_button_led(const int button, const int led,
                              bool *button_state, int button_i) {
  if (nrf_gpio_pin_read(button)) {
    nrf_gpio_pin_set(led);
    if (*button_state) {
      printf("Button %d released\n", button_i);
    }
    *button_state = false;
  } else {
    nrf_gpio_pin_clear(led);
    if (!*button_state) {
      printf("Button %d pressed\n", button_i);
    }
    *button_state = true;
  }
}

int main(void) {

  // Initialize buttons and LEDs
  nrf_gpio_cfg_output(LED1);
  nrf_gpio_cfg_output(LED2);
  nrf_gpio_cfg_output(LED3);
  nrf_gpio_cfg_output(LED4);
  nrf_gpio_pin_set(LED1);
  nrf_gpio_pin_set(LED2);
  nrf_gpio_pin_set(LED3);
  nrf_gpio_pin_set(LED4);
  nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);

  // Initialize RTT library
  ret_code_t error_code = NRF_SUCCESS;
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // Setup variables
  bool b1 = false;
  bool b2 = false;
  bool b3 = false;
  bool b4 = false;

  // Enter main loop.
  while (1) {
    handle_button_led(BUTTON1, LED1, &b1, 1);
    handle_button_led(BUTTON2, LED2, &b2, 2);
    handle_button_led(BUTTON3, LED3, &b3, 3);
    handle_button_led(BUTTON4, LED4, &b4, 4);
  }
}
