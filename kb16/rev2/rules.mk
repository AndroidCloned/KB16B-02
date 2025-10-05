SRC += ./lib/layer_status/layer_status.c
SRC += ./lib/logo.c

# Platform
PLATFORM = STM32

# MCU Configuration for APM32F103CBT6 (STM32F103 clone)
MCU = STM32F103
BOOTLOADER = stm32duino

# Configure for 128K flash
MCU_LDSCRIPT = STM32F103xB

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output

# OLED enabled
OLED_ENABLE = yes

# RGB Matrix enabled
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = ws2812

# Encoder enabled
ENCODER_ENABLE = yes
