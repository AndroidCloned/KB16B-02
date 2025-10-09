# Artisan Coffee Roaster Hotkey Integration - Developer Implementation Guide

## Overview
This package contains all the code changes needed to implement Artisan Coffee Roaster control functionality into any QMK-compatible keyboard/macropad project. The implementation provides direct control over heater power, air flow, drum speed, and other roaster functions through keyboard shortcuts.

## Files Included
- `artisan_keycodes.h` - Custom keycode definitions
- `artisan_functions.c` - Core functionality implementation  
- `artisan_keymap_example.c` - Example keymap integration
- `artisan_encoder_example.c` - Encoder integration example
- `integration_guide.md` - Step-by-step implementation guide

---

## 1. Custom Keycodes (`artisan_keycodes.h`)

```c
/*
 * Artisan Coffee Roaster Control Keycodes
 * Add these to your enum custom_keycodes in keymap.c
 */

enum artisan_keycodes {
    // Heater Power Control
    M_HP_INC = SAFE_RANGE,      // Increment heater power by 5%
    M_HP_DEC,                   // Decrement heater power by 5%
    M_HP_25,                    // Set heater power to 25%
    M_HP_50,                    // Set heater power to 50%
    M_HP_75,                    // Set heater power to 75%
    M_HP_100,                   // Set heater power to 100%
    
    // Air Control
    M_AIR_INC,                  // Increment air by 5%
    M_AIR_DEC,                  // Decrement air by 5%
    
    // Drum Control  
    M_DRUM_INC,                 // Increment drum by 5%
    M_DRUM_DEC,                 // Decrement drum by 5%
    
    // Temperature Control
    M_TEMP_INC,                 // Increment temperature setpoint
    M_TEMP_DEC,                 // Decrement temperature setpoint
    
    // Roast Control
    M_ROAST_START,              // Start roasting
    M_ROAST_STOP,               // Stop roasting
    M_ROAST_EMERGENCY,          // Emergency stop
    M_ROAST_PROFILE,            // Load roast profile
};
```

---

## 2. Core Functions (`artisan_functions.c`)

```c
/*
 * Artisan Coffee Roaster Control Functions
 * Add these functions to your keymap.c file
 */

// Global variables for tracking roaster state
uint8_t artisan_heater_value = 50;  // Default heater value (50%)
uint8_t artisan_air_value = 50;    // Default air value (50%)
uint8_t artisan_drum_value = 50;    // Default drum value (50%)

// Send HP command to Artisan (Heater Power)
void send_hp_command(uint8_t hp_value) {
    tap_code(KC_H);  // HP command
    if (hp_value >= 100) {
        tap_code(KC_1);
        tap_code(KC_0);
        tap_code(KC_0);
    } else if (hp_value >= 10) {
        tap_code(KC_0 + (hp_value / 10));  // Tens digit
        tap_code(KC_0 + (hp_value % 10));  // Ones digit
    } else {
        tap_code(KC_0);
        tap_code(KC_0 + hp_value);         // Single digit
    }
}

// Send Air command to Artisan
void send_air_command(uint8_t air_value) {
    tap_code(KC_A);  // Air command
    if (air_value >= 100) {
        tap_code(KC_1);
        tap_code(KC_0);
        tap_code(KC_0);
    } else if (air_value >= 10) {
        tap_code(KC_0 + (air_value / 10));  // Tens digit
        tap_code(KC_0 + (air_value % 10));  // Ones digit
    } else {
        tap_code(KC_0);
        tap_code(KC_0 + air_value);         // Single digit
    }
}

// Send Drum command to Artisan
void send_drum_command(uint8_t drum_value) {
    tap_code(KC_D);  // Drum command
    if (drum_value >= 100) {
        tap_code(KC_1);
        tap_code(KC_0);
        tap_code(KC_0);
    } else if (drum_value >= 10) {
        tap_code(KC_0 + (drum_value / 10));  // Tens digit
        tap_code(KC_0 + (drum_value % 10));  // Ones digit
    } else {
        tap_code(KC_0);
        tap_code(KC_0 + drum_value);         // Single digit
    }
}
```

---

## 3. Process Record User Cases (`artisan_keymap_example.c`)

```c
/*
 * Process Record User Cases for Artisan Control
 * Add these cases to your process_record_user switch statement
 */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Heater Power Control
        case M_HP_INC:
            if (record->event.pressed) {
                if (artisan_heater_value < 100) {
                    artisan_heater_value += 5;  // Increment by 5%
                    send_hp_command(artisan_heater_value);
                }
            }
            break;
            
        case M_HP_DEC:
            if (record->event.pressed) {
                if (artisan_heater_value > 0) {
                    artisan_heater_value -= 5;  // Decrement by 5%
                    send_hp_command(artisan_heater_value);
                }
            }
            break;
            
        case M_HP_25:
            if (record->event.pressed) {
                artisan_heater_value = 25;
                send_hp_command(artisan_heater_value);
            }
            break;
            
        case M_HP_50:
            if (record->event.pressed) {
                artisan_heater_value = 50;
                send_hp_command(artisan_heater_value);
            }
            break;
            
        case M_HP_75:
            if (record->event.pressed) {
                artisan_heater_value = 75;
                send_hp_command(artisan_heater_value);
            }
            break;
            
        case M_HP_100:
            if (record->event.pressed) {
                artisan_heater_value = 100;
                send_hp_command(artisan_heater_value);
            }
            break;
            
        // Air Control
        case M_AIR_INC:
            if (record->event.pressed) {
                if (artisan_air_value < 100) {
                    artisan_air_value += 5;  // Increment by 5%
                    send_air_command(artisan_air_value);
                }
            }
            break;
            
        case M_AIR_DEC:
            if (record->event.pressed) {
                if (artisan_air_value > 0) {
                    artisan_air_value -= 5;  // Decrement by 5%
                    send_air_command(artisan_air_value);
                }
            }
            break;
            
        // Drum Control
        case M_DRUM_INC:
            if (record->event.pressed) {
                if (artisan_drum_value < 100) {
                    artisan_drum_value += 5;  // Increment by 5%
                    send_drum_command(artisan_drum_value);
                }
            }
            break;
            
        case M_DRUM_DEC:
            if (record->event.pressed) {
                if (artisan_drum_value > 0) {
                    artisan_drum_value -= 5;  // Decrement by 5%
                    send_drum_command(artisan_drum_value);
                }
            }
            break;
            
        // Temperature Control
        case M_TEMP_INC:
            if (record->event.pressed) {
                tap_code(KC_T);  // Temperature command
                tap_code(KC_UP); // Increment
            }
            break;
            
        case M_TEMP_DEC:
            if (record->event.pressed) {
                tap_code(KC_T);  // Temperature command
                tap_code(KC_DOWN); // Decrement
            }
            break;
            
        // Roast Control
        case M_ROAST_START:
            if (record->event.pressed) {
                tap_code(KC_S);  // Start command
            }
            break;
            
        case M_ROAST_STOP:
            if (record->event.pressed) {
                tap_code(KC_S);  // Stop command (same as start, toggles)
            }
            break;
            
        case M_ROAST_EMERGENCY:
            if (record->event.pressed) {
                tap_code(KC_E);  // Emergency stop
            }
            break;
            
        case M_ROAST_PROFILE:
            if (record->event.pressed) {
                tap_code(KC_P);  // Profile command
            }
            break;
            
        default:
            return true; // Process other keycodes normally
    }
    return false; // Skip further processing
}
```

---

## 4. Encoder Integration (`artisan_encoder_example.c`)

```c
/*
 * Encoder Integration for Artisan Control
 * Add to your encoder_map array
 */

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    // Example for Layer 2 (roaster control layer)
    [_2] = { 
        ENCODER_CCW_CW(TO(_1), TO(_3)),           // Layer navigation
        ENCODER_CCW_CW(M_HP_DEC, M_HP_INC),       // Heater power control
        ENCODER_CCW_CW(M_AIR_DEC, M_AIR_INC)      // Air control
    },
    
    // Example for Layer 3 (advanced roaster control)
    [_3] = { 
        ENCODER_CCW_CW(TO(_2), TO(_4)),           // Layer navigation
        ENCODER_CCW_CW(M_DRUM_DEC, M_DRUM_INC),  // Drum control
        ENCODER_CCW_CW(M_TEMP_DEC, M_TEMP_INC)   // Temperature control
    },
};
```

---

## 5. Keymap Layout Example

```c
/*
 * Example Keymap Layout for Roaster Control Layer
 * Add to your keymaps array
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // ... other layers ...
    
    // Layer 2: Basic Roaster Control
    [_2] = LAYOUT(
        M_HP_25,  M_HP_50,  M_HP_75,  M_HP_100,
        M_HP_INC, M_HP_DEC, M_AIR_INC, M_AIR_DEC,
        M_DRUM_INC, M_DRUM_DEC, M_TEMP_INC, M_TEMP_DEC,
        M_ROAST_START, M_ROAST_STOP, M_ROAST_EMERGENCY, M_ROAST_PROFILE
    ),
    
    // ... other layers ...
};
```

---

## 6. Alternative Implementation - TAB Navigation Method

```c
/*
 * Alternative implementation using TAB navigation
 * Use this if direct commands don't work with your Artisan setup
 */

        case M_HP_INC:
            if (record->event.pressed) {
                // Method: Use TAB navigation + UP arrow
                tap_code(KC_TAB);  // Cycle to next slider
                tap_code(KC_TAB);  // Continue cycling
                tap_code(KC_TAB);  // Should reach heater slider (4th slider)
                tap_code(KC_UP);   // Increment heater power
            }
            break;
            
        case M_HP_DEC:
            if (record->event.pressed) {
                // Method: Use TAB navigation + DOWN arrow
                tap_code(KC_TAB);  // Cycle to next slider
                tap_code(KC_TAB);  // Continue cycling
                tap_code(KC_TAB);  // Should reach heater slider (4th slider)
                tap_code(KC_DOWN); // Decrement heater power
            }
            break;
```

---

## 7. RGB Lighting Integration (Optional)

```c
/*
 * Optional RGB feedback for roaster control
 * Add to your layer change or key press handlers
 */

void update_roaster_rgb(void) {
    // Color code based on heater power level
    if (artisan_heater_value >= 75) {
        rgb_matrix_set_color_all(255, 0, 0);    // Red for high heat
    } else if (artisan_heater_value >= 50) {
        rgb_matrix_set_color_all(255, 165, 0);  // Orange for medium heat
    } else if (artisan_heater_value >= 25) {
        rgb_matrix_set_color_all(255, 255, 0);  // Yellow for low heat
    } else {
        rgb_matrix_set_color_all(0, 255, 0);    // Green for minimal heat
    }
}
```

---

## Implementation Steps

### Step 1: Add Custom Keycodes
1. Copy the `enum artisan_keycodes` section to your `keymap.c`
2. Add it to your existing `enum custom_keycodes`

### Step 2: Add Global Variables
1. Copy the global variables section to your `keymap.c`
2. Place them near the top of your file

### Step 3: Add Helper Functions
1. Copy the `send_hp_command`, `send_air_command`, and `send_drum_command` functions
2. Add them to your `keymap.c` file

### Step 4: Add Process Record Cases
1. Copy all the `case` statements to your `process_record_user` function
2. Add them to your existing `switch` statement

### Step 5: Configure Keymap Layout
1. Assign the custom keycodes to keys in your keymap layout
2. Create a dedicated roaster control layer if desired

### Step 6: Configure Encoders (Optional)
1. Add encoder mappings for roaster control functions
2. Test encoder functionality

### Step 7: Test and Customize
1. Compile and flash your firmware
2. Test each function with Artisan
3. Adjust key combinations as needed
4. Add RGB feedback if desired

---

## Compatibility Notes

- **Tested with**: Kaleido M2s roaster via Artisan software
- **Compatible with**: Any QMK-compatible keyboard/macropad
- **Software**: Artisan Coffee Roaster Software
- **Protocol**: Keyboard shortcuts (H, A, D commands)

## Troubleshooting

1. **Commands not working**: Try the TAB navigation method instead
2. **Wrong increments**: Adjust the increment values (currently 5%)
3. **Key conflicts**: Modify key combinations to avoid conflicts
4. **Timing issues**: Add `wait_ms()` calls if needed

## Customization Options

- **Increment values**: Change from 5% to any value
- **Key combinations**: Modify H, A, D commands as needed
- **Additional functions**: Add more roaster-specific controls
- **RGB feedback**: Implement visual feedback for roaster state
- **OLED display**: Show current roaster values on display
