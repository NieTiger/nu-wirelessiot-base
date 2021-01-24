// BLE Scanning app
//
// Receives BLE advertisements

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "ble_advdata.h"
#include "ble_gap.h"
#include "nrf_soc.h"
#include "simple_ble.h"

#include "nrf52840dk.h"

// BLE configuration
// This is mostly irrelevant since we are scanning only
static simple_ble_config_t ble_config = {
        // BLE address is c0:98:e5:4e:00:02
        .platform_id       = 0x4E,    // used as 4th octet in device BLE address
        .device_id         = 0x0002,  // used as the 5th and 6th octet in the device BLE address, you will need to change this for each device you have
        .adv_name          = "CS397/497", // irrelevant in this example
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS), // send a packet once per second (minimum is 20 ms)
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS), // irrelevant if advertising only
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS), // irrelevant if advertising only
};
simple_ble_app_t* simple_ble_app;


// Callback handler for advertisement reception
void ble_evt_adv_report(ble_evt_t const* p_ble_evt) {

  // extract the fields we care about
  ble_gap_evt_adv_report_t const* adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
  uint8_t const* ble_addr = adv_report->peer_addr.addr; // array of 6 bytes of the address
  uint8_t* adv_buf = adv_report->data.p_data; // array of up to 31 bytes of advertisement payload data
  uint16_t adv_len = adv_report->data.len; // length of advertisement payload data

  // Filter our devices by BLE address
  if (ble_addr[5] == 0xC0 && ble_addr[4] == 0x98 && ble_addr[3] == 0xe5) {

    // Print BLE address
    printf("Adv from %X:%X:%X:%X:%X:%X  -  ", 
            ble_addr[0], ble_addr[1], ble_addr[2], ble_addr[3], ble_addr[4], ble_addr[5]);

    // Parse payload: length-type-value sets
    uint8_t i = 0;
    while (i < adv_len) {
      // length
      uint8_t len = adv_buf[i++] - 1; // -1 for the type octet

      // type
      uint8_t type = adv_buf[i++];

      // value
      switch (type) {
        // NOTE: In each case, i must be incremented by len
        case BLE_GAP_AD_TYPE_FLAGS:                 // flags for discoverability
          printf("Flags: 0x%02X, ", adv_buf[i++]);
          break;

        case BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE:   // Complete list of 16-bit Service Class UUIDs
          printf("16-bit Service Class UUIDs: 0x");
          while (len-- && i<adv_len) {
              printf("%02X", adv_buf[i++]);
          }
          printf(", ");
          break;

        case BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME:      // short/complete local device name
        case BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME:
          printf("Name: ");
          while (len-- && i<adv_len) {
              printf("%c", adv_buf[i++]);
          }
          printf(", ");
          break;

        case BLE_GAP_AD_TYPE_SERVICE_DATA:          // Service Data
          printf("Service data: ");
          while (len-- && i<adv_len) {
              printf("%c", adv_buf[i++]);
          }
          printf(", ");
          break;


        default:
          printf("Type (0x%02X): 0x", type);
          while (len-- && i<adv_len) {
              printf("%02X", adv_buf[i++]);
          }
          printf(", ");
      }
    }
    printf("\n");
  }
}


int main(void) {

  // Setup BLE
  // Note: simple BLE is our own library. You can find it in `nrf5x-base/lib/simple_ble/`
  simple_ble_app = simple_ble_init(&ble_config);
  advertising_stop();

  // Start scanning
  scanning_start();

  // go into low power mode
  while(1) {
    power_manage();
  }
}



