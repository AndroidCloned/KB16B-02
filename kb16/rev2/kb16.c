#include "kb16.h"

#ifdef RGB_MATRIX_ENABLE

 led_config_t g_led_config = { {
  // Key Matrix to LED Index
  { NO_LED,   0,    1,   2,   3,   NO_LED,   NO_LED},
  { NO_LED,   4,    5,   6,   7,   NO_LED,   NO_LED},
  { NO_LED,   8,    9,   10,  11,   NO_LED,   NO_LED},
  { NO_LED,   12,   13,  14,  15,   NO_LED,   NO_LED},
  { NO_LED,   NO_LED,   NO_LED,     NO_LED,    NO_LED,   NO_LED,   NO_LED},
}, {
{37,0},  {75,0}, {112,0},{150,0},
{37,16}, {75,16}, {112,16},{150,16},
{37,32}, {75,32}, {112,32},{150,32},
{37,48}, {75,48}, {112,48},{150,48},
}, { 
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
} };

void suspend_power_down_kb(void) {
  rgb_matrix_set_suspend_state(true);
}

void suspend_wakeup_init_kb(void) {
  rgb_matrix_set_suspend_state(false);
}

#endif
