SRC += ./lib/layer_status/layer_status.c
SRC += ./lib/logo.c

# Platform
PLATFORM = STM32

# MCU Configuration for KB16 Wireless (APM32F103CBT6)
MCU = STM32F103
BOARD = STM32_F103_STM32DUINO
BOOTLOADER = custom

# Configure for 128K flash
MCU_LDSCRIPT = STM32F103xB

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no       # Enable Bootmagic Lite (disabled for wireless)
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
AUDIO_ENABLE = no           # Audio output

# OLED enabled
OLED_ENABLE = yes
OLED_DRIVER = SSD1306

# RGB Matrix enabled
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812

# Encoder enabled
ENCODER_ENABLE = yes
ENCODER_MAP_ENABLE = no

# Wireless/Bluetooth support
BLUETOOTH_ENABLE = yes
BLUETOOTH_DRIVER = RN42

# VIA support
VIA_ENABLE = yes

# EEPROM driver
EEPROM_DRIVER = vendor

# Additional optimizations
LTO_ENABLE = yes

# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE

# Additional source files
SRC += analog.c
