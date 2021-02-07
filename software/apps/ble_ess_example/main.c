// BLE Service example app
//
// Creates a BLE environmental sensing service

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

static simple_ble_service_t environmental_sensing_service = {{
  .uuid128 = {0xFB,0x34,0x9B,0x5F,0x80,0x00,0x00,0x80,
              0x00,0x10,0x00,0x00,0x1A,0x18,0x00,0x00}
}};

static simple_ble_char_t temperature_state_char = {.uuid16 = 0x2a6e};
static int16_t temperature_val = 900;

static simple_ble_char_t sensor_location_state_char = {.uuid16 = 0x2a5d};
static uint8_t sensor_location_val = 0x02;

static simple_ble_char_t rest_hr_state_char = {.uuid16 = 0x2a92};
static uint8_t rest_hr_val = 58;

static simple_ble_char_t lang_state_char = {.uuid16 = 0x2aa2};
static char lang_val[3] = "en";

static simple_ble_char_t floor_no_state_char = {.uuid16 = 0x2ab2};
static int16_t floor_no_val = 900;

static simple_ble_char_t blood_pressure_state_char = {.uuid16 = 0x1810};
static uint8_t blood_pressure_val = 69;

static simple_ble_char_t max_hr_state_char = {.uuid16 = 0x2a8d};
static uint8_t max_hr_val = 69;

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
  printf("Got write to a characteristic!\n");
}

int main(void) {

  printf("Board started. Initializing BLE: \n");

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&environmental_sensing_service);
  simple_ble_add_characteristic(1, 0, 0, 0,
    sizeof(temperature_val), (uint8_t*)&temperature_val, 
    &environmental_sensing_service, &temperature_state_char);
  simple_ble_add_characteristic(1, 0, 0, 0,
    sizeof(sensor_location_val), (uint8_t*)&sensor_location_val, 
    &environmental_sensing_service, &sensor_location_state_char);
  simple_ble_add_characteristic(1, 0, 0, 0,
    sizeof(rest_hr_val), (uint8_t*)&rest_hr_val, 
    &environmental_sensing_service, &rest_hr_state_char);
  simple_ble_add_characteristic(1, 0, 0, 0,
    sizeof(max_hr_val), (uint8_t*)&max_hr_val, 
    &environmental_sensing_service, &max_hr_state_char);
  simple_ble_add_characteristic(1, 0, 0, 0,
    sizeof(lang_val), (uint8_t*)&lang_val, 
    &environmental_sensing_service, &lang_state_char);
  simple_ble_add_characteristic(1, 0, 0, 0,
    sizeof(blood_pressure_val), (uint8_t*)&blood_pressure_val, 
    &environmental_sensing_service, &blood_pressure_state_char);

  // Start Advertising
  simple_ble_adv_only_name();

  while(1) {
    power_manage();
  }
}

