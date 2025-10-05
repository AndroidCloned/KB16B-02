/* Copyright 2022 DOIO
 * Copyright 2022 HorrorTroll <https://github.com/HorrorTroll>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// random number generator
# include <stdlib.h>

// OLED animation
#include "../../../lib/layer_status/layer_status.h"

// Set shorter timout for OLED (default is 60 seconds!)
// this has to be modified in the driver files, check it out here:
// drivers/oled/oled_driver.h:300
// I also turned down the brighness there...


// Color map constants
struct RGB_VAL {
    const uint8_t r;
    const uint8_t g;
    const uint8_t b;
    };

// Coffee/Caffeine Theme Colors
const struct RGB_VAL COFFEE_BEAN = { .r = 0x4A, .g = 0x2C, .b = 0x1A};      // Dark coffee bean brown
const struct RGB_VAL CREAM = {.r = 0xF5, .g = 0xF0, .b = 0xE6};            // Cream/light coffee
const struct RGB_VAL ESPRESSO = {.r = 0x2F, .g = 0x1B, .b = 0x14};         // Dark espresso
const struct RGB_VAL CARAMEL = {.r = 0xD2, .g = 0x69, .b = 0x1E};          // Caramel color
const struct RGB_VAL MOCHA = {.r = 0x8B, .g = 0x45, .b = 0x13};            // Mocha brown

const struct RGB_VAL HEATER_ON = {.r = 0xFF, .g = 0x44, .b = 0x00};        // Hot orange for heater
const struct RGB_VAL HEATER_OFF = {.r = 0x66, .g = 0x66, .b = 0x66};       // Grey for heater off
const struct RGB_VAL STEAM = {.r = 0xE6, .g = 0xE6, .b = 0xFA};            // Light steam blue

// jiggler max distance from center
#define MAX_JIGGLE_DISTANCE 10

// *****************************************************
// *****************************************************
// ################## COLOR MAP(s) #####################
// *****************************************************
// *****************************************************

// Coffee-themed layer color maps
const struct RGB_VAL PROGMEM layer_1_color_map[16] = {
    CARAMEL, CREAM, CREAM, CREAM,
    CREAM, CREAM, CREAM, CREAM,
    CARAMEL, CREAM, CREAM, CREAM,
    CARAMEL, CARAMEL, CARAMEL, CARAMEL,
};

const struct RGB_VAL PROGMEM layer_2_color_map[16] = {
    CREAM, COFFEE_BEAN, COFFEE_BEAN, COFFEE_BEAN,
    COFFEE_BEAN, COFFEE_BEAN, COFFEE_BEAN, COFFEE_BEAN,
    CREAM, COFFEE_BEAN, COFFEE_BEAN, COFFEE_BEAN,
    CREAM, CREAM, CREAM, CREAM,
};

const struct RGB_VAL PROGMEM layer_3_color_map[16] = {
    CARAMEL, ESPRESSO, STEAM, HEATER_ON,
    STEAM, ESPRESSO, STEAM, HEATER_ON,
    HEATER_ON, ESPRESSO, STEAM, CARAMEL,
    MOCHA, ESPRESSO, HEATER_OFF, HEATER_OFF
};

const struct RGB_VAL PROGMEM layer_4_color_map[16] = {
    CARAMEL, ESPRESSO, ESPRESSO, COFFEE_BEAN,
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
    HEATER_OFF, ESPRESSO, ESPRESSO, COFFEE_BEAN,
};

const struct RGB_VAL PROGMEM busy_0_map[16] = {
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
    ESPRESSO, ESPRESSO, COFFEE_BEAN, COFFEE_BEAN,
    ESPRESSO, ESPRESSO, COFFEE_BEAN, COFFEE_BEAN,
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
};

const struct RGB_VAL PROGMEM busy_1_map[16] = {
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
    COFFEE_BEAN, COFFEE_BEAN, ESPRESSO, ESPRESSO,
    COFFEE_BEAN, COFFEE_BEAN, ESPRESSO, ESPRESSO,
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
};

const struct RGB_VAL PROGMEM busy_2_map[16] = {
    ESPRESSO, COFFEE_BEAN, COFFEE_BEAN, ESPRESSO,
    ESPRESSO, COFFEE_BEAN, COFFEE_BEAN, ESPRESSO,
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
};

const struct RGB_VAL PROGMEM busy_3_map[16] = {
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
    ESPRESSO, COFFEE_BEAN, COFFEE_BEAN, ESPRESSO,
    ESPRESSO, COFFEE_BEAN, COFFEE_BEAN, ESPRESSO,
};

const struct RGB_VAL PROGMEM sleep_map[16] = {
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
    ESPRESSO, ESPRESSO, ESPRESSO, ESPRESSO,
};

const struct RGB_VAL PROGMEM all_red_map[16] = {
    HEATER_ON, HEATER_ON, HEATER_ON, HEATER_ON,
    HEATER_ON, HEATER_ON, HEATER_ON, HEATER_ON,
    HEATER_ON, HEATER_ON, HEATER_ON, HEATER_ON,
    HEATER_ON, HEATER_ON, HEATER_ON, HEATER_ON,
};


// *****************************************************
// State variables and timers for complex macros
// *****************************************************

bool to_be_reset = false;

bool look_busy_on;
bool look_busy_state;
uint8_t look_busy_choice = 0;
uint16_t look_busy_timer = false;
uint16_t look_busy_interval = 300;

bool win_hold_on;
uint16_t win_hold_timer = false;
uint16_t win_hold_interval = 700;

bool tab_scroll_on;
uint16_t tab_scroll_timer = false;
uint16_t tab_scroll_interval = 700;

bool ctrl_tab_scroll_on;
uint16_t ctrl_tab_scroll_timer = false;
uint16_t ctrl_tab_scroll_interval = 700;

bool sleep_mode_on;
uint16_t sleep_mode_timer = false;
uint16_t half_minute_interval = 30000; // timer to count out 1/2 minute (approx)
uint16_t inactive_half_minutes = 0;
uint16_t sleep_mode_start_interval = 30 * 2; // Number of half minutes till of inactivity before sleepmode auto starts
// This method of timing is pretty inaccurate, so this could be off by a few minutes (theres no RTC and the timers are rough)

bool animation_done;
uint16_t animation_timer = false;
uint16_t animation_interval = 5000;

uint8_t left_stretch = 0;
uint8_t right_stretch = 0;
uint8_t up_stretch = 0;
uint8_t down_stretch = 0;

// Artisan Coffee Roaster - Kaleido M2s Heater Control
uint8_t artisan_heater_value = 50;  // Default heater value (50%)

// *****************************************************
// Declarations bc order dumbness
// *****************************************************

void set_layer_map(const struct RGB_VAL*);
uint8_t one_of_four(void);

// *****************************************************
// Macro names and what have you
// *****************************************************
enum custom_keycodes {
    // Windows navigation
    M_WIN_1 = SAFE_RANGE,   // Windows + N
    M_WIN_2,
    M_WIN_3,
    M_WIN_4,
    M_WIN_5,
    M_WIN_6,
    M_WIN_7,
    M_WIN_8,
    M_WIN_9,
    M_WIN_D,
    M_TAB_MENU,             // Opens and holds open ALT TAB
    M_DESK_RIGHT,           // Move one desktop right
    M_DESK_LEFT,            // Move one desktop left
    M_TAB_SCROLL_UP,        // Scroll wheel tab through ALT tab menu forward
    M_TAB_SCROLL_DOWN,      // Scroll wheel tabe through ALT TAB menu backward
    M_CTRL_TAB_SCROLL_UP,    // Scroll wheel tab through CTL tab menu forward (in most apps this cycles though the app tabs)
    M_CTRL_TAB_SCROLL_DOWN,    // Scroll wheel tab through CTL tab menu forward (in most apps this cycles though the app tabs)
    
    // Mouse Jiggler
    M_LOOK_BUSY,            // Spams modifiers and mouse jiggles to keep computer awake
    
    // POWER TOYS
    M_ALWAYS_TOP,           // Powertoys always on top with WIN + CTRL + T
    M_COLOR_PICKER,         // Powertoys color picker with WIN+ SHIFT + C
    M_TEXT_EXTRACT,         // Powertoys TEXT with WIN + SHIFT + T
    M_LAUNCHER,             // Powertoys launcher
    
    // VS + VS CODE
    M_RUN,                  // Run (no deubug)
    M_RUN_DEBUG,            // Run with Debug
    M_RERUN_DEBUG,          // Rerun debug
    M_STOP_DEBUG,           // Stop Debugging
    M_STEP_OVER,            // Step over line
    M_STEP_INTO,            // Step into
    M_STEP_OUT,             // Step out of 
    M_TOGGLE_BREAKPOINT,    // Toggle active breakpoint
    M_EXPLORER,             // Open file view
    M_SEARCH,               // Open Search view
    M_DEBUG_MENU,           // Open Debug/run view
    M_SHOW_HIDE,            // Hide side panel

    // Power management
    M_SLEEP_PAD,            // Put macropad into sleep mode
    M_SLEEP_ALL,            // Put computer to sleep, then sleep self
    M_QMK_RESET,            // Activate QMK Bootloader
    
    // Artisan Coffee Roaster - Kaleido M2s Heater Control
    M_HEATER_INC,           // Increment heater power
    M_HEATER_DEC,           // Decrement heater power
    M_HEATER_RESET,         // Reset heater to 0%
    M_HEATER_25,            // Set heater to 25%
    M_HEATER_50,            // Set heater to 50%
    M_HEATER_75,            // Set heater to 75%
};

// *****************************************************
// Startup function after init
// Used to set color mode
// *****************************************************
void keyboard_post_init_user(void) {
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);

    sleep_mode_timer = timer_read();
    inactive_half_minutes = 0;
    sleep_mode_on = false;
}

// *****************************************************
// Easy call to put pad into sleep mode
// *****************************************************
void enter_sleep_mode(void) {
    sleep_mode_on = true;
    animation_done = false;
    reset_animations();
    animation_timer = timer_read();
    animation_interval = 4800;
}

// *****************************************************
// Easy call to put pad into sleep mode when host machine sleeps
// *****************************************************
void suspend_power_down_user(void) {
    //oled_off();
    enter_sleep_mode();
}

void suspend_wakeup_init_user(void) {
    //oled_off();
    sleep_mode_on = false;
    sleep_mode_timer = timer_read();
    inactive_half_minutes = 0;
}

// *****************************************************
// Random choice generator to pick a value between 0 and 3
// *****************************************************
uint8_t one_of_four(void){
    uint8_t choice = 0;
    uint16_t random = rand();

    // Boolean and with 11 to get only bottom two bits
    choice = random & 0x03;
    return choice;
}

uint8_t elastic_bias(uint8_t choice){
    uint8_t new_choice = choice;
    
    // Roll random number
    uint8_t rand_num = rand() % (MAX_JIGGLE_DISTANCE);
    
    // if random number is above stretch, let it stay
    // if below, invert the direction
    // this more or less guarentees that inversion gets more likely as we get further from center
    switch(choice){
        case 0x00: 
            if (rand_num < up_stretch){
                new_choice = 0x01;
            }
            break;
        case 0x01: 
            if (rand_num < down_stretch){
                new_choice = 0x00;
            }
            break;
        case 0x02: 
            if (rand_num < left_stretch){
                new_choice = 0x03;
            }
            break;
        case 0x03: 
            if (rand_num < right_stretch){
                new_choice = 0x02;
            }
            break;
        default:
            break;
    }

    // update stretch after new choice is made -> inversion results in decreased stretch
    switch(new_choice){
        case 0x00:
            up_stretch += 1;
            down_stretch -= 1;
            break;
        case 0x01:
            down_stretch += 1;
            up_stretch -= 1;
            break;
        case 0x02:
            left_stretch += 1;
            right_stretch -= 1;
            break;
        case 0x03:
            right_stretch += 1;
            left_stretch -=1;
            break;
        default:
            break;
    }

    // Enforce bounds
    // yeah yeah i know this is bad and lazy
    if (up_stretch > 254){
        // Back to zero on underflow
        up_stretch = 0;
    }
    else if (up_stretch > MAX_JIGGLE_DISTANCE){
        // hold at max
        up_stretch = MAX_JIGGLE_DISTANCE;
    }

    if (down_stretch > 254){
        // Back to zero on underflow
        down_stretch = 0;
    }
    else if (down_stretch > MAX_JIGGLE_DISTANCE){
        // hold at max
        down_stretch = MAX_JIGGLE_DISTANCE;
    }

    if (left_stretch > 254){
        // Back to zero on underflow
        left_stretch = 0;
    }
    else if (left_stretch > MAX_JIGGLE_DISTANCE){
        // hold at max
        left_stretch = MAX_JIGGLE_DISTANCE;
    }

    if (right_stretch > 254){
        // Back to zero on underflow
        right_stretch = 0;
    }
    else if (right_stretch > MAX_JIGGLE_DISTANCE){
        // hold at max
        right_stretch = MAX_JIGGLE_DISTANCE;
    }

    return new_choice;
}

// *****************************************************
// Callbacks
// *****************************************************

// reset callback
uint32_t reset_callback(uint32_t trigger_time, void *cb_arg) {
    /* reset keyboard */
    reset_keyboard();
    return 0;
}

// *****************************************************
// Macro handling!
// *****************************************************
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    
    // Turn off look busy if any key is pressed, easier to escape since
    // modifier keys are being mashed and that could conflict with other macros
    if (record->event.pressed && (keycode != M_LOOK_BUSY) && look_busy_on) {
        look_busy_on = false;
        return true;
    }

    // reset sleep timers on any press/interaction
    if (record->event.pressed) {
        sleep_mode_timer = timer_read();
        inactive_half_minutes = 0;
        
        // Wake from sleep but DO NOT activate pressed key - this stops accidently running a macro
        //  when waking up lights (to let the user read the layer colors)
        if (sleep_mode_on) {
            sleep_mode_on = false;
            return true;
        }
    }

    // Process almost every other type of interaction
    switch (keycode) {
        case M_WIN_1:
            if (record->event.pressed) {
                win_hold_timer = timer_read();
                if (win_hold_on) {
                    SEND_STRING(SS_TAP(X_1));
                }
                else {
                    win_hold_on = true;
                    SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_TAP(X_1));
                }
                
            } 
            break;
        case M_WIN_2:
            if (record->event.pressed) {
                win_hold_timer = timer_read();
                if (win_hold_on) {
                    SEND_STRING(SS_TAP(X_2));
                }
                else {
                    win_hold_on = true;
                    SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_TAP(X_2));
                }
                
            } 
            break;
        case M_WIN_3:
            if (record->event.pressed) {
                win_hold_timer = timer_read();
                if (win_hold_on) {
                    SEND_STRING(SS_TAP(X_3));
                }
                else {
                    win_hold_on = true;
                    SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_TAP(X_3));
                }
                
            } 
            break;
        case M_WIN_4:
            if (record->event.pressed) {
                win_hold_timer = timer_read();
                if (win_hold_on) {
                    SEND_STRING(SS_TAP(X_4));
                }
                else {
                    win_hold_on = true;
                    SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_TAP(X_4));
                }
                
            } 
            break;
        case M_WIN_5:
            if (record->event.pressed) {
                win_hold_timer = timer_read();
                if (win_hold_on) {
                    SEND_STRING(SS_TAP(X_5));
                }
                else {
                    win_hold_on = true;
                    SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_TAP(X_5));
                }
                
            } 
            break;
        case M_WIN_6:
            if (record->event.pressed) {
                win_hold_timer = timer_read();
                if (win_hold_on) {
                    SEND_STRING(SS_TAP(X_6));
                }
                else {
                    win_hold_on = true;
                    SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_TAP(X_6));
                }
                
            } 
            break;
        case M_WIN_7:
            if (record->event.pressed) {
                win_hold_timer = timer_read();
                if (win_hold_on) {
                    SEND_STRING(SS_TAP(X_7));
                }
                else {
                    win_hold_on = true;
                    SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_TAP(X_7));
                }
                
            } 
            break;
        case M_WIN_8:
            if (record->event.pressed) {
                win_hold_timer = timer_read();
                if (win_hold_on) {
                    SEND_STRING(SS_TAP(X_8));
                }
                else {
                    win_hold_on = true;
                    SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_TAP(X_8));
                }
                
            } 
            break;
        case M_WIN_9:
            if (record->event.pressed) {
                win_hold_timer = timer_read();
                if (win_hold_on) {
                    SEND_STRING(SS_TAP(X_9));
                }
                else {
                    win_hold_on = true;
                    SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_TAP(X_9));
                }
                
            } 
            break;
        case M_WIN_D:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI("d"));
            } 
            break;
        case M_TAB_MENU:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_ALT) SS_TAP(X_TAB));
                SEND_STRING(SS_TAP(X_LEFT));
            } 
            else {
                SEND_STRING(SS_UP(X_LEFT_ALT));
            }
            break;
        case M_ALWAYS_TOP:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_DOWN(X_LEFT_CTRL) SS_DOWN(X_T));
                SEND_STRING(SS_UP(X_LEFT_GUI) SS_UP(X_LEFT_CTRL) SS_UP(X_T));
            } 
            break;
        case M_COLOR_PICKER:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_DOWN(X_LEFT_SHIFT) SS_DOWN(X_C));
                SEND_STRING(SS_UP(X_LEFT_GUI) SS_UP(X_LEFT_SHIFT) SS_UP(X_C));
            } 
            break;
        case M_TEXT_EXTRACT:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_DOWN(X_LEFT_SHIFT) SS_DOWN(X_T));
                SEND_STRING(SS_UP(X_LEFT_GUI) SS_UP(X_LEFT_SHIFT) SS_UP(X_T));
            } 
            break;
        case M_LAUNCHER:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_ALT) SS_TAP(X_SPACE));
                SEND_STRING(SS_UP(X_LEFT_ALT));
            } 
            break;
        case M_LOOK_BUSY:
            if (record->event.pressed) {
                look_busy_on ^= 1;
                look_busy_timer = timer_read();

                // Reset elastic constants
                up_stretch = 0;
                down_stretch = 0;
                left_stretch = 0;
                right_stretch = 0;
            } 
            break;
        case M_TAB_SCROLL_UP:
            if (record->event.pressed) {
                tab_scroll_timer = timer_read();
                if (tab_scroll_on){
                    SEND_STRING(SS_TAP(X_RIGHT));
                }
                else {
                    tab_scroll_on =  true;
                    SEND_STRING(SS_DOWN(X_LEFT_ALT) SS_TAP(X_TAB));
                }
            } 
            break;
        case M_TAB_SCROLL_DOWN:
            if (record->event.pressed) {
                tab_scroll_timer = timer_read();
                if (tab_scroll_on){
                    SEND_STRING(SS_TAP(X_LEFT));
                }
                else {
                    tab_scroll_on =  true;
                    SEND_STRING(SS_DOWN(X_LEFT_ALT) SS_DOWN(X_LEFT_SHIFT) SS_TAP(X_TAB));
                    SEND_STRING(SS_UP(X_LEFT_SHIFT));
                }
            } 
            break;
        case M_CTRL_TAB_SCROLL_UP:
            if (record->event.pressed) {
                ctrl_tab_scroll_timer = timer_read();
                if (ctrl_tab_scroll_on){
                    SEND_STRING(SS_TAP(X_TAB));
                }
                else {
                    ctrl_tab_scroll_on =  true;
                    SEND_STRING(SS_DOWN(X_LEFT_CTRL) SS_TAP(X_TAB));
                }
            } 
            break;
        case M_CTRL_TAB_SCROLL_DOWN:
            if (record->event.pressed) {
                ctrl_tab_scroll_timer = timer_read();
                if (ctrl_tab_scroll_on){
                    SEND_STRING(SS_DOWN(X_LEFT_SHIFT) SS_TAP(X_TAB));
                    SEND_STRING(SS_UP(X_LEFT_SHIFT));
                }
                else {
                    ctrl_tab_scroll_on =  true;
                    SEND_STRING(SS_DOWN(X_LEFT_CTRL) SS_DOWN(X_LEFT_SHIFT) SS_TAP(X_TAB));
                    SEND_STRING(SS_UP(X_LEFT_SHIFT));
                }
            } 
            break;
         case M_DESK_RIGHT:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_DOWN(X_LEFT_CTRL) SS_TAP(X_RIGHT));
                SEND_STRING(SS_UP(X_LEFT_GUI) SS_UP(X_LEFT_CTRL));
            } 
            break;
         case M_DESK_LEFT:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_DOWN(X_LEFT_CTRL) SS_TAP(X_LEFT));
                SEND_STRING(SS_UP(X_LEFT_GUI) SS_UP(X_LEFT_CTRL));
            } 
            break;
        case M_RUN:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_F5));
            } 
            break;
        case M_RUN_DEBUG:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_CTRL) SS_TAP(X_F5));
                SEND_STRING(SS_UP(X_LEFT_CTRL));
            } 
            break;
        case M_RERUN_DEBUG:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_CTRL) SS_DOWN(X_LEFT_SHIFT) SS_TAP(X_F5));
                SEND_STRING(SS_UP(X_LEFT_CTRL) SS_UP(X_LEFT_SHIFT));
            } 
            break;
        case M_STOP_DEBUG:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_SHIFT) SS_TAP(X_F5));
                SEND_STRING(SS_UP(X_LEFT_SHIFT));
            } 
            break;
        case M_STEP_OVER:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_F10));
            } 
            break;
        case M_STEP_INTO:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_F11));
            } 
            break;
        case M_STEP_OUT:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_SHIFT) SS_TAP(X_F11));
                SEND_STRING(SS_UP(X_LEFT_SHIFT));
            } 
            break;
        case M_TOGGLE_BREAKPOINT:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_F9));
            } 
            break;
        case M_EXPLORER:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_CTRL) SS_DOWN(X_LEFT_SHIFT) SS_TAP(X_E));
                SEND_STRING(SS_UP(X_LEFT_CTRL) SS_UP(X_LEFT_SHIFT));
            } 
            break;
        case M_SEARCH:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_CTRL) SS_DOWN(X_LEFT_SHIFT) SS_TAP(X_F));
                SEND_STRING(SS_UP(X_LEFT_CTRL) SS_UP(X_LEFT_SHIFT));
            } 
            break;
        case M_DEBUG_MENU:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_CTRL) SS_DOWN(X_LEFT_SHIFT) SS_TAP(X_D));
                SEND_STRING(SS_UP(X_LEFT_CTRL) SS_UP(X_LEFT_SHIFT));
            } 
            break;
        case M_SHOW_HIDE:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_CTRL) SS_TAP(X_B));
                SEND_STRING(SS_UP(X_LEFT_CTRL));
            }
            break;
        case M_SLEEP_ALL:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LEFT_GUI) SS_TAP(X_X) SS_DELAY(35));
                SEND_STRING(SS_UP(X_LEFT_GUI));
                SEND_STRING(SS_TAP(X_U) SS_DELAY(35) SS_TAP(X_S));
            }
        case M_SLEEP_PAD:
            if (record->event.pressed) {
                enter_sleep_mode();
            }
            break;
        case M_QMK_RESET:
            // This macro kills the board (drops into qmk bootloader ), so be careful with accidentaly pushing it
            if (record->event.pressed) {
                to_be_reset = true;
                defer_exec(100, reset_callback, NULL);
            }
            break;
        case M_HEATER_INC:
            if (record->event.pressed) {
                // Method 1: Use TAB navigation + UP arrow (most reliable)
                // This navigates to heater slider and increments by small amounts
                tap_code(KC_TAB);  // Cycle to next slider
                tap_code(KC_TAB);  // Continue cycling
                tap_code(KC_TAB);  // Should reach heater slider (4th slider)
                tap_code(KC_UP);   // Increment heater power
                
                // Alternative Method 2: Direct keyboard shortcut (uncomment if preferred)
                // if (artisan_heater_value < 100) {
                //     artisan_heater_value++;
                //     tap_code(KC_R);
                //     if (artisan_heater_value < 10) {
                //         tap_code(KC_0);
                //         tap_code(KC_0 + artisan_heater_value);
                //     } else {
                //         tap_code(KC_0 + (artisan_heater_value / 10));
                //         tap_code(KC_0 + (artisan_heater_value % 10));
                //     }
                // }
            }
            break;
        case M_HEATER_DEC:
            if (record->event.pressed) {
                // Method 1: Use TAB navigation + DOWN arrow (most reliable)
                // This navigates to heater slider and decrements by small amounts
                tap_code(KC_TAB);  // Cycle to next slider
                tap_code(KC_TAB);  // Continue cycling
                tap_code(KC_TAB);  // Should reach heater slider (4th slider)
                tap_code(KC_DOWN); // Decrement heater power
                
                // Alternative Method 2: Direct keyboard shortcut (uncomment if preferred)
                // if (artisan_heater_value > 0) {
                //     artisan_heater_value--;
                //     tap_code(KC_R);
                //     if (artisan_heater_value < 10) {
                //         tap_code(KC_0);
                //         tap_code(KC_0 + artisan_heater_value);
                //     } else {
                //         tap_code(KC_0 + (artisan_heater_value / 10));
                //         tap_code(KC_0 + (artisan_heater_value % 10));
                //     }
                // }
            }
            break;
    }
    return true;
}

// *****************************************************
// *****************************************************
// ##################### KEYMAP ########################
// *****************************************************
// *****************************************************

enum layer_names {
    _1,
    _2,
    _3,
    _4,
};

// enum layer_keycodes { };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* LAYER 1 - Windows Navigation
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │ + │ W1│ W4│ W7│   │TO1│ │   │
       ├───┼───┼───┼───┤   └───┘ └───┘
       │ 0 │ W2│ W6│ W8│
       ├───┼───┼───┼───┤
       │ . │ W3│ W6│ W9│      ┌───┐
       ├───┼───┼───┼───┤      │WD │
       │ENT│clr│top│bsy│      └───┘
       └───┴───┴───┴───┘

*/
    /*  Row:    0         1        2        3         4      */
    [_1] = LAYOUT(
                M_DESK_LEFT,     M_WIN_1,    M_WIN_4,    M_WIN_7,     TO(_1),
                M_LAUNCHER,     M_WIN_2,    M_WIN_5,    M_WIN_8,     _______,
                M_DESK_RIGHT,     M_WIN_3,    M_WIN_6,   M_WIN_9,   M_WIN_D,
                M_TEXT_EXTRACT, M_COLOR_PICKER, M_ALWAYS_TOP, M_LOOK_BUSY
            ),

/* LAYER 2 - Numpad

       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │ + │ 1 │ 4 │ 7 │   │TO1│ │   │
       ├───┼───┼───┼───┤   └───┘ └───┘
       │ 0 │ 2 │ 5 │ 8 │
       ├───┼───┼───┼───┤
       │ . │ 3 │ 6 │ 9 │      ┌───┐
       ├───┼───┼───┼───┤      │Mut│
       │ENT│ / │ * │ - │      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4       */
    [_2] = LAYOUT(
                KC_KP_PLUS,     KC_KP_1,    KC_KP_4,    KC_KP_7,     TO(_1),
                KC_KP_0,     KC_KP_2,    KC_KP_5,    KC_KP_8,     _______,
                KC_KP_DOT,     KC_KP_3,    KC_KP_6,   KC_KP_9,   M_WIN_D,
                KC_KP_ENTER, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS
            ),


/* LAYER 3 - ide shortcuts
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │EXP│   │OVR│RUN│   │   │ │   │
       ├───┼───┼───┼───┤   └───┘ └───┘
       │SRC│   │IN │DBG│
       ├───┼───┼───┼───┤
       │DBG│   │OUT│RR │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │SHW│   │TGB│STP│      └───┘
       └───┴───┴───┴───┘

*/
    /*  Row:    0         1        2        3         4      */
    [_3] = LAYOUT(
                M_EXPLORER, _______, M_STEP_OVER, M_RUN, TO(_1),
                M_SEARCH, _______, M_STEP_INTO, M_RUN_DEBUG, _______,
                M_DEBUG_MENU, _______, M_STEP_OUT, M_RERUN_DEBUG, M_WIN_D,
                M_SHOW_HIDE, _______, M_TOGGLE_BREAKPOINT, M_STOP_DEBUG
            ),
    


/* LAYER 4
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │RST│   │   │NLK│   │   │ │TO0│
       ├───┼───┼───┼───┤   └───┘ └───┘
       │   │   │   │   │
       ├───┼───┼───┼───┤
       │   │   │   │   │      ┌───┐
       ├───┼───┼───┼───┤      │   │
       │SLP│   │   │SHT│      └───┘
       └───┴───┴───┴───┘
*/
    /*  Row:    0        1        2        3        4        */
    [_4] = LAYOUT(
                M_SLEEP_PAD, _______, _______, M_QMK_RESET, _______,
                _______, _______, _______, _______, TO(_1),
                _______, _______, _______, _______, _______,
                M_SLEEP_ALL, _______, _______, KC_NUM_LOCK
            ),
};

// *****************************************************
// Write matrix colors
// *****************************************************
//void set_layer_map(const uint8_t color_map[16][3]){
void set_layer_map(const struct RGB_VAL* color_map){
    for (uint8_t i = 0; i < 16; i++){
        rgb_matrix_set_color(i, color_map[i].r, color_map[i].g, color_map[i].b);
    }
}

// *****************************************************
// Update color maps
// Update timers + exit conditions for complex macros
// *****************************************************
bool rgb_matrix_indicators_user(void) {
    // Turn off pad if we are in sleep mode
    if (sleep_mode_on){
        set_layer_map(sleep_map);
        return false;
    }
    // reset light indictor
    else if (to_be_reset){
        set_layer_map(all_red_map);
    }
    // Look busy macro flashinghandling
    else if (look_busy_on) {
        // Flashing animation for active look busy macro
        switch (look_busy_choice)
        {
        case 0:
            set_layer_map(busy_0_map);
            break;
        case 1:
            set_layer_map(busy_1_map);
            break;
        case 2:
            set_layer_map(busy_2_map);
            break;
        case 3:
            set_layer_map(busy_3_map);
            break;
        
        default:
            break;
        }

    }
    // normal layer colormaps
    else {
        // Set layer color map for normal conditions
        uint8_t current_layer = get_highest_layer(layer_state);
        switch (current_layer) {
            case _1: // Navigation + windows stuff
                set_layer_map(layer_1_color_map);
                break;
            case _2: // Numpad
                set_layer_map(layer_2_color_map);
                break;
            case _3:
                set_layer_map(layer_3_color_map);  // RGB blue
                break;
            case _4:
                set_layer_map(layer_4_color_map);  // RGB yellow
                break;
            default:
                break;
        }
    }

    // Held key handling for complex macros

    // These timer handlers are in this function only bc I felt weird putting them in the 
    // Matrix scan function

    // Key inputs for look busy mode
    if (look_busy_on && timer_elapsed(look_busy_timer) >= look_busy_interval) {
        look_busy_timer = timer_read();
        look_busy_state ^= 1;
        
        // Mash control alternatively
        if (look_busy_state) {
            SEND_STRING(SS_TAP(X_LEFT_CTRL));
        }

        // Choose direction for wiggle 
        look_busy_choice = one_of_four();
        // Use elastic rule to keep it in one area
        look_busy_choice = elastic_bias(look_busy_choice);
        
        switch (look_busy_choice)
        {
        case 0:
            SEND_STRING(SS_TAP(X_MS_UP)    SS_DELAY(10) SS_TAP(X_MS_UP)    SS_DELAY(100));
            break;
        case 1:
            SEND_STRING(SS_TAP(X_MS_DOWN)  SS_DELAY(10) SS_TAP(X_MS_DOWN)  SS_DELAY(100));
            break;
        case 2:
            SEND_STRING(SS_TAP(X_MS_LEFT)  SS_DELAY(10) SS_TAP(X_MS_LEFT)  SS_DELAY(100));
            break;
        case 3:
            SEND_STRING(SS_TAP(X_MS_RIGHT) SS_DELAY(10) SS_TAP(X_MS_RIGHT) SS_DELAY(100));
            break;
        
        default:
            break;
        }
    }

    // alt Tab scroll logic to hold down ALT key till timer expires
    if (tab_scroll_on && (timer_elapsed(tab_scroll_timer) >= tab_scroll_interval)) {
        tab_scroll_on = false;
        SEND_STRING(SS_UP(X_LEFT_ALT));
    }

    // CTRL tab scroll logic to hold down ctrl till timer expires
    if (ctrl_tab_scroll_on && (timer_elapsed(ctrl_tab_scroll_timer) >= ctrl_tab_scroll_interval)) {
        ctrl_tab_scroll_on = false;
        SEND_STRING(SS_UP(X_LEFT_CTRL));
    }

    // win hold logic for win + num macros
    if (win_hold_on && (timer_elapsed(win_hold_timer) >= win_hold_interval)) {
        win_hold_on = false;
        SEND_STRING(SS_UP(X_LEFT_GUI));
    }

    // Sleep mode logic
    // Only update timers if sleep mode is NOT on
    if (!sleep_mode_on && !look_busy_on) {
        // Check if the number of half minutes we are expecting has passed, if so, sleep
        if (inactive_half_minutes >= sleep_mode_start_interval){
            enter_sleep_mode();
        }
        // Otherwise, see if another half a minute has passed, update half min counter
        else if (timer_elapsed(sleep_mode_timer) >= half_minute_interval){
            sleep_mode_timer = timer_read();
            inactive_half_minutes = inactive_half_minutes + 1;
        }
    }
    return false;
}

#ifdef OLED_ENABLE
    bool oled_task_user(void) {
        if (sleep_mode_on) {
            if (!animation_done) {
                // Only draw sleep animation for time set by M_SLEEP_PAD macro
                if (timer_elapsed(animation_timer) <= animation_interval) {
                    render_sleep_animation();
                    animation_done = false;
                }
                else{
                    screen_off();
                    animation_done = true;
                }
            }
        }
        else {
            render_layer_status();
        }

        return true;
    }
#endif

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_1] = { ENCODER_CCW_CW(TO(_4), TO(_2)), ENCODER_CCW_CW(M_CTRL_TAB_SCROLL_DOWN, M_CTRL_TAB_SCROLL_UP), ENCODER_CCW_CW(M_TAB_SCROLL_DOWN, M_TAB_SCROLL_UP) },
    [_2] = { ENCODER_CCW_CW(TO(_1), TO(_3)), ENCODER_CCW_CW(M_HEATER_DEC, M_HEATER_INC), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_3] = { ENCODER_CCW_CW(TO(_2), TO(_4)), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_4] = { ENCODER_CCW_CW(TO(_3), TO(_1)), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};
#endif

