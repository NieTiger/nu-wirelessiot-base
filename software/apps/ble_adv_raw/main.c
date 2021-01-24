// BLE Advertisement Raw app
//
// Sends a BLE advertisement with raw bytes

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_util.h"
#include "simple_ble.h"

#include "nrf52840dk.h"

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:4e:xx:xx
        .platform_id       = 0x4E,   // used as 4th octect in device BLE address
        .device_id         = 0xAABB, // must be unique on each device you program!
        .adv_name          = "CS397/497", // used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;


int main(void) {

  printf("Board started. Initializing BLE: \n");

  // Setup BLE
  // Note: simple BLE is our own library. You can find it in `nrf5x-base/lib/simple_ble/`
  simple_ble_app = simple_ble_init(&ble_config);

  // Start Advertising
  uint8_t ble_data_len = 3 + 8 + 3 + 4;
  uint8_t ble_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX] = {
    0x02, 0x01, 0x06, // flags: 0x06
    0x07, 0x09, 0x43, 0x53, 0x5f, 0x49, 0x6f, 0x54, // Complete Local Name: CS_IoT
    0x02, 0x0A, 0x00, // TX Power Level: default 0dbm
    0x03, 0xFF, 0xc0, 0x98, // Manufacturer Specific Data: c0:98
  };

  simple_ble_adv_raw(ble_data, ble_data_len);

  printf("Started BLE advertisements\n");

  while(1) {
    power_manage();
  }
}

