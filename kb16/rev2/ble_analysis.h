#pragma once

// BLE Implementation Analysis for DOIO KB16 Wireless
// Based on reverse engineering from clownfish-og/qmk_firmware commit ee38890
// 
// HARDWARE IDENTIFIED:
// - BLE Module: MS50SFA (FCC ID: 2ABUS-MS50SFA)
// - Module has active green LED (power/activity indicator)
// - PCB clearly labeled "BLE"
// - Module is surface-mounted with gold pads for connections

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
 * LIKELY BLE PINS:
 * - A9, A10: Common UART TX/RX pins
 * - A5, A6: Alternative UART pins
 * - B2: Possible CTS/RTS or power control
 */

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
