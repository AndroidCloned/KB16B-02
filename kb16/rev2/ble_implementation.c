#include "ble_analysis.h"
#include "quantum.h"
#include "uart.h"
#include "gpio.h"

// BLE state management
static ble_state_t ble_current_state = BLE_STATE_DISCONNECTED;
static bool ble_initialized = false;
static bool ble_mode_active = false; // true = BLE mode, false = USB mode

// PCB LED 2 control for BLE status indication
// LED 2 is the green LED visible on the PCB (not on BLE module)
static bool led2_blinking = false;
static uint16_t led2_blink_timer = 0;
static uint16_t led2_blink_interval = 500; // 500ms blink interval

// Switch detection variables
static bool sw1_detected = false;
static bool sw2_detected = false;
static pin_t sw1_pin = PIN_UNASSIGNED;
static pin_t sw2_pin = PIN_UNASSIGNED;

// UART configuration for MS50SFA BLE communication
void ble_init(void) {
    if (ble_initialized) return;
    
    // Initialize UART for MS50SFA BLE module communication
    // Note: Green LED is from PCB LED 2, not the BLE module itself
    
    // Set up GPIO pins for BLE
    setPinOutput(BLE_POWER_PIN);
    writePinHigh(BLE_POWER_PIN);  // Power on MS50SFA BLE module
    
    // Initialize UART (placeholder - needs actual implementation)
    // uart_init(BLE_UART_TX_PIN, BLE_UART_RX_PIN, 115200);
    
    // Send initialization command to BLE module
    // This is the key part that needs reverse engineering
    ble_cmd_t init_cmd = {
        .cmd = BLE_CMD_INIT,
        .len = 0,
        .checksum = 0
    };
    
    // Send command (placeholder)
    // ble_send_command(&init_cmd);
    
    ble_current_state = BLE_STATE_DISCONNECTED;
    ble_initialized = true;
}

void ble_send_key(uint8_t keycode, bool pressed) {
    if (!ble_initialized || ble_current_state != BLE_STATE_CONNECTED) {
        return;
    }
    
    // Create key command
    ble_cmd_t key_cmd = {
        .cmd = BLE_CMD_SEND_KEY,
        .len = 2,
        .data = {keycode, pressed ? 1 : 0},
        .checksum = 0
    };
    
    // Calculate checksum (placeholder algorithm)
    key_cmd.checksum = key_cmd.cmd ^ key_cmd.len ^ key_cmd.data[0] ^ key_cmd.data[1];
    
    // Send command (placeholder)
    // ble_send_command(&key_cmd);
    
    // For now, just toggle LED to show BLE activity
    #ifdef RGB_MATRIX_ENABLE
    if (pressed) {
        rgb_matrix_set_color(0, 0, 255, 0);  // Green for key press
    }
    #endif
}

void ble_enter_pairing_mode(void) {
    if (!ble_initialized) return;
    
    ble_cmd_t pair_cmd = {
        .cmd = BLE_CMD_PAIR,
        .len = 0,
        .checksum = BLE_CMD_PAIR
    };
    
    // Send pairing command (placeholder)
    // ble_send_command(&pair_cmd);
    
    ble_current_state = BLE_STATE_PAIRING;
    
    // Visual feedback for pairing mode
    #ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_color_all(255, 255, 0);  // Yellow for pairing
    #endif
}

void ble_sleep(void) {
    if (!ble_initialized) return;
    
    ble_cmd_t sleep_cmd = {
        .cmd = BLE_CMD_SLEEP,
        .len = 0,
        .checksum = BLE_CMD_SLEEP
    };
    
    // Send sleep command (placeholder)
    // ble_send_command(&sleep_cmd);
    
    ble_current_state = BLE_STATE_SLEEPING;
    
    // Power down BLE module
    writePinLow(BLE_POWER_PIN);
}

void ble_wake(void) {
    if (!ble_initialized) return;
    
    // Power up BLE module
    writePinHigh(BLE_POWER_PIN);
    
    ble_cmd_t wake_cmd = {
        .cmd = BLE_CMD_WAKE,
        .len = 0,
        .checksum = BLE_CMD_WAKE
    };
    
    // Send wake command (placeholder)
    // ble_send_command(&wake_cmd);
    
    ble_current_state = BLE_STATE_DISCONNECTED;
}

bool ble_is_connected(void) {
    return ble_current_state == BLE_STATE_CONNECTED;
}

ble_state_t ble_get_state(void) {
    return ble_current_state;
}

// Placeholder function to send commands to BLE module
void ble_send_command(ble_cmd_t* cmd) {
    // This is where we need to implement the actual UART communication
    // The protocol and command format need to be reverse engineered
    
    // For debugging, we can use the console output
    #ifdef CONSOLE_ENABLE
    uprintf("BLE CMD: 0x%02X, Len: %d, Data: ", cmd->cmd, cmd->len);
    for (int i = 0; i < cmd->len; i++) {
        uprintf("0x%02X ", cmd->data[i]);
    }
    uprintf("Checksum: 0x%02X\n", cmd->checksum);
    #endif
}

// Function to process incoming BLE data
void ble_process_response(uint8_t* data, uint8_t len) {
    // This function should process responses from the BLE module
    // It needs to be reverse engineered based on actual BLE module responses
    
    if (len < 1) return;
    
    switch (data[0]) {
        case 0x01:  // Connection established
            ble_current_state = BLE_STATE_CONNECTED;
            #ifdef RGB_MATRIX_ENABLE
            rgb_matrix_set_color_all(0, 255, 0);  // Green for connected
            #endif
            break;
            
        case 0x02:  // Disconnected
            ble_current_state = BLE_STATE_DISCONNECTED;
            #ifdef RGB_MATRIX_ENABLE
            rgb_matrix_set_color_all(255, 0, 0);  // Red for disconnected
            #endif
            break;
            
        case 0x03:  // Pairing success
            ble_current_state = BLE_STATE_CONNECTED;
            #ifdef RGB_MATRIX_ENABLE
            rgb_matrix_set_color_all(0, 255, 255);  // Cyan for paired
            #endif
            break;
            
        default:
            break;
    }
}

// PCB LED 2 control functions for BLE status indication
void ble_set_status_led(bool on) {
    // Control PCB LED 2 (the green LED visible on the PCB)
    // This LED is likely controlled by the MCU, not the BLE module
    // We need to determine which GPIO pin controls LED 2
    
    // Placeholder - need to identify actual LED 2 pin
    // setPinOutput(LED2_PIN);
    // writePin(LED2_PIN, on ? 1 : 0);
    
    led2_blinking = false; // Stop blinking when manually setting LED
}

void ble_blink_status_led(uint16_t interval) {
    led2_blink_interval = interval;
    led2_blink_timer = timer_read();
    led2_blinking = true;
}

// Function to update LED 2 blinking (call from main loop or timer)
void ble_update_status_led(void) {
    if (!led2_blinking) return;
    
    if (timer_elapsed(led2_blink_timer) >= led2_blink_interval) {
        led2_blink_timer = timer_read();
        // Toggle LED 2 state
        // writePin(LED2_PIN, !readPin(LED2_PIN));
    }
}

// S1 button functionality - switches between USB and BLE modes
void ble_toggle_mode(void) {
    ble_mode_active = !ble_mode_active;
    
    if (ble_mode_active) {
        // Switch to BLE mode
        // Disable USB HID, enable BLE communication
        // This is where we need to implement the mode switching logic
        
        #ifdef RGB_MATRIX_ENABLE
        rgb_matrix_set_color_all(0, 255, 0);  // Green for BLE mode
        #endif
        
        // LED 2 solid on for BLE mode
        ble_set_status_led(true);
        
        // Initialize BLE if not already done
        if (!ble_initialized) {
            ble_init();
        }
        
        // Enter BLE mode (pairing if not connected)
        if (ble_current_state == BLE_STATE_DISCONNECTED) {
            ble_enter_pairing_mode();
        }
        
    } else {
        // Switch to USB mode
        // Disable BLE, enable USB HID
        
        #ifdef RGB_MATRIX_ENABLE
        rgb_matrix_set_color_all(0, 0, 255);  // Blue for USB mode
        #endif
        
        // LED 2 off for USB mode
        ble_set_status_led(false);
        
        // Put BLE module to sleep to save power
        ble_sleep();
    }
}

bool ble_is_ble_mode(void) {
    return ble_mode_active;
}

// Switch detection functions
void ble_init_switches(void) {
    // Initialize switch detection
    // This function will scan available pins to find SW1 and SW2
    
    // Available pins for switches (excluding matrix, I2C, encoders, RGB):
    // A5, A6, A8, A9, A10, A11, A12, A13, A14, A15, B2, B14
    
    sw1_detected = false;
    sw2_detected = false;
    sw1_pin = PIN_UNASSIGNED;
    sw2_pin = PIN_UNASSIGNED;
}

bool ble_is_sw1_pressed(void) {
    if (!sw1_detected || sw1_pin == PIN_UNASSIGNED) {
        return false;
    }
    
    // Read SW1 pin state
    setPinInput(sw1_pin);
    return !readPin(sw1_pin); // Assuming active low
}

bool ble_is_sw2_pressed(void) {
    if (!sw2_detected || sw2_pin == PIN_UNASSIGNED) {
        return false;
    }
    
    // Read SW2 pin state
    setPinInput(sw2_pin);
    return !readPin(sw2_pin); // Assuming active low
}

void ble_scan_switch_pins(void) {
    // Scan available pins to detect switches
    // This is a debugging function to help identify the actual switch pins
    
    pin_t test_pins[] = {A5, A6, A8, A9, A10, A11, A12, A13, A14, A15, B2, B14};
    uint8_t num_pins = sizeof(test_pins) / sizeof(pin_t);
    
    for (uint8_t i = 0; i < num_pins; i++) {
        setPinInput(test_pins[i]);
        
        // Check if pin changes state (indicates a switch)
        bool initial_state = readPin(test_pins[i]);
        wait_ms(10);
        bool current_state = readPin(test_pins[i]);
        
        if (initial_state != current_state) {
            // This pin might be a switch
            if (!sw1_detected) {
                sw1_pin = test_pins[i];
                sw1_detected = true;
            } else if (!sw2_detected) {
                sw2_pin = test_pins[i];
                sw2_detected = true;
                break; // Found both switches
            }
        }
    }
}
