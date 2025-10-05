#pragma once

// BLE Implementation Analysis for DOIO KB16 Wireless
// Based on reverse engineering from clownfish-og/qmk_firmware commit ee38890
// 
// HARDWARE IDENTIFIED:
// - BLE Module: MS50SFA (FCC ID: 2ABUS-MS50SFA)
// - Green LED is from LED 2 on PCB (NOT from BLE module)
// - PCB clearly labeled "BLE"
// - Module is surface-mounted with gold pads for connections
// - S1 BUTTON: Switches between USB and BLE modes
// - BLE module status unknown (no visible LED on module itself)

/*
 * HARDWARE ANALYSIS:
 * 
 * Matrix pins used:
 * - Rows: A3, A2, A1, A0, A4
 * - Cols: B15, B13, B12, B1, B0, A7, B7
 * 
 * Encoder pins used:
 * - Encoder 1: B5, B6
 * - Encoder 2: B3, B4  
 * - Encoder 3: B8, B9
 * 
 * Other pins used:
 * - I2C (OLED): B10, B11
 * - RGB: B15 (shared with matrix)
 * 
 * AVAILABLE PINS FOR BLE/UART:
 * - A5, A6, A8, A9, A10, A11, A12, A13, A14, A15
 * - B2, B14
 * 
 * PHYSICAL SWITCHES IDENTIFIED:
 * - SW1 (S1 button): Pin R (USB/BLE mode switch) - Need to identify actual pin
 * - SW2: Pin B (function unknown - possibly reset or other function) - Need to identify actual pin
 * 
 * NOTE: B12 and B11 are already used (matrix col and I2C), so SW1/SW2 must use different pins
 * 
 * LIKELY BLE PINS:
 * - A9, A10: Common UART TX/RX pins
 * - A5, A6: Alternative UART pins
 * - B2: Possible CTS/RTS or power control
 */

// Physical Switch Pin Definitions (to be determined)
// SW1 (S1 button): Pin R - USB/BLE mode switch
// SW2: Pin B - function unknown
// NOTE: Need to identify actual GPIO pins for SW1 and SW2

// BLE Communication Protocol (to be reverse engineered)
#define BLE_UART_TX_PIN A9
#define BLE_UART_RX_PIN A10
#define BLE_UART_CTS_PIN A5
#define BLE_UART_RTS_PIN A6
#define BLE_POWER_PIN B2

// BLE Command Structure (estimated)
typedef struct {
    uint8_t cmd;
    uint8_t len;
    uint8_t data[16];
    uint8_t checksum;
} ble_cmd_t;

// BLE Commands (to be reverse engineered)
#define BLE_CMD_INIT 0x01
#define BLE_CMD_PAIR 0x02
#define BLE_CMD_SEND_KEY 0x03
#define BLE_CMD_SLEEP 0x04
#define BLE_CMD_WAKE 0x05

// BLE Status
typedef enum {
    BLE_STATE_DISCONNECTED,
    BLE_STATE_CONNECTING,
    BLE_STATE_CONNECTED,
    BLE_STATE_PAIRING,
    BLE_STATE_SLEEPING
} ble_state_t;

// Function prototypes for BLE implementation
void ble_init(void);
void ble_send_key(uint8_t keycode, bool pressed);
void ble_enter_pairing_mode(void);
void ble_sleep(void);
void ble_wake(void);
bool ble_is_connected(void);
ble_state_t ble_get_state(void);

// PCB LED 2 control for BLE status indication
void ble_set_status_led(bool on);
void ble_blink_status_led(uint16_t interval);

// S1 button functionality
void ble_toggle_mode(void);
bool ble_is_ble_mode(void);

// Switch pin detection
void ble_init_switches(void);
bool ble_is_sw1_pressed(void);
bool ble_is_sw2_pressed(void);
void ble_scan_switch_pins(void);
