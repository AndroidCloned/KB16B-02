# Caffeine Coffee Roaster Firmware Overview

This firmware for the Doio 16 Key + 3 Knob Macro Pad (Megalodon), is designed around adding some nice navigation features that are useful for day to day Windows use + development work, with a special focus on coffee roasting control. It was designed with the idea of rotating the pad 90 degrees to have the knobs on the top so that the pad can be used as a num pad if desired, but with the knobs in a more reasonable place.

The firmware features a **coffee-themed aesthetic** with warm browns, creams, and coffee-inspired colors throughout all layers. The menu and animations use a coffee/caffeine theme that perfectly complements the Artisan Coffee Roaster integration.

Follow the normal installation procedure for any other keyboard using QMK (install QMK MSYS + QMK Tool kit for Windows). There are some other tweaks that were made to the base board config/drivers (deeper in the QMK install directory than this repo goes) that were made to change the screen brightness + timeout. Those are discussed in the "Special Considerations" section.

Since this repo **does not fork the base QMK repo** please make sure that you use its contents to replace: `qmk_firmware\keyboards\doio\kb16`

# Keymaps

This firmware utilizes four layers/keymaps, selected by the bottom left knob (when in "numpad orientation"). Since the firmware makes an assumption on the orientation of the device that isn't really true once you rotate it, I suggest looking at the diagrams in this readme rather than what is in the source. Of course the actual keymap array in the source has the final say.

The function of the knobs varies by layer:
* Bottom Left - Layer Selection: scroll through the four layers available. A click (press) goes back to layer 1.
* Top Left - Ctrl Tab Scrolling: scroll through *tabs* of active program using "CTRL + TAB" wizardry. A click (press) does nothing.
* Right (Big Knob) - Layer dependent:
  - **Layer 1**: Alt Tab Scrolling: scroll through *windows* as if pressing "ALT + TAB". A click (press) activates the "WIN + D" shortcut to show the desktop.
  - **Layer 2**: Artisan Coffee Roaster heater control (Kaleido M2s compatible). Clockwise increases heater power, counter-clockwise decreases.
  - **Layer 3**: Alt Tab Scrolling (same as Layer 1)
  - **Layer 4**: Alt Tab Scrolling (same as Layer 1)

I tried to put some helpful colorful backlighting on most things to give a quick idea of where things are. The firmware uses a **coffee-themed color palette** with warm browns, creams, and coffee-inspired colors that create a cohesive aesthetic perfect for coffee roasting sessions. You may want to tweak the coffee color constants if you want to adjust the theme.

## Layer 1 - Windows Fast Navigation
The first layer is focused on letting the user quickly navigate around in Windows and access some utilities through (PowerToys)[https://learn.microsoft.com/en-us/windows/powertoys/]. Of course, you need PowerToys installed for some of these features to work.

```
       ┌───┬───┬───┬───┐
       │ W7│ W8│ W9│JGL│
       ├───┼───┼───┼───┤
       │ W4│ W5│ W6│AOT│
       ├───┼───┼───┼───┤
       │ W1│ W2│ W3│CLR│
       ├───┼───┼───┼───┤
       │WR │LCH│ WL│TXT│
       └───┴───┴───┴───┘

```

The "W\<NUMBER\>" macros emulate the "WIN + \<NUM\>" command, but hold down the WIN key for a moment so that the user can rapidly tap the same key to scroll through instances of the same app. For example, if you have 4 file explorer windows open, you can tap the macro key twice to get to window two. There is also a quick key for the PowerToys "Launcher", as well as keys to switch windows desktops (a la "WIN+ \<ArrowKey\>").

The right most column has several "utilities". The first is a custom mouse jiggler (which is a toggle and will flash the lights so you know its on), then below are the PowerToys applets for "Always On Top", "Color Picker", and "Text Extractor".

## Layer 2 - Numpad + Artisan Coffee Roaster Control

Its a num pad, congratulations TKL users! That said, the function keys are in weird spots since we dont have the normal key layout.

**Special Feature**: The large top-middle knob controls Artisan Coffee Roaster heater power for Kaleido M2s roasters:
- **Clockwise rotation**: Increases heater power by 1% (max 100%)
- **Counter-clockwise rotation**: Decreases heater power by 1% (min 0%)
- **Commands sent**: Proper Kaleido format `kaleido(HP,{value})` where {value} is the percentage

This feature requires Artisan Coffee Roaster software with keyboard shortcuts enabled and a Kaleido M2s roaster connected.

```
       ┌───┬───┬───┬───┐
       │ 7 │ 8 │ 9 │SUB│
       ├───┼───┼───┼───┤
       │ 4 │ 5 │ 6 │MUL│
       ├───┼───┼───┼───┤
       │ 1 │ 2 │ 3 │DIV│
       ├───┼───┼───┼───┤
       │ADD│ 0 │ . │ENT│
       └───┴───┴───┴───┘

```

## Layer 3 - VS/VSCode Run + Debug

The second layer is mostly for dealing with VS Code and Visual Studio's run and debugger shortcuts. These assume that you have the the "Run" and "Run w/ Debug" shortcuts rebound **in your IDE** such that "Run" is F5, and "Run w/ Debug" is CTRL+5. This is a preference of mine, so switch them in the firmware if you prefer it the other way around.

```
       ┌───┬───┬───┬───┐
       │RUN│DBG│RDB│STP│
       ├───┼───┼───┼───┤
       │OVR│IN │OUT│TOG│
       ├───┼───┼───┼───┤
       │   │   │   │   │
       ├───┼───┼───┼───┤
       │EXP│SRC│DBM│HID│
       └───┴───┴───┴───┘

```

In row column order:
1) Run current configuration, Run current configuration w/ Debug, Rerun current configuration w/ Debug, Stop Run
2) Step over, Step into, Step out of, Toggle breakpoint
3) A whole lot of nothing
4) (Mostly VS Code) Open File Explorer Sidepane, Open search Sidepane, open Debug Sidepane, Hide Sidepanes



## Layer 4 - Sleep + Power

This layer is mostly open for now, it just has some power related things that I find useful. I might add more in the future.

```
       ┌───┬───┬───┬───┐
       │RST│   │   │   │
       ├───┼───┼───┼───┤
       │   │   │   │   │
       ├───┼───┼───┼───┤
       │   │   │   │   │
       ├───┼───┼───┼───┤
       │PSL│   │   │CSL│
       └───┴───┴───┴───┘

```

RST - This pulls the QMK Reset function and drops into the boot loader so you can reflash. That said, you need to do a full power cycle after this if you ever use it. The LEDs should all turn red when this happens to let you know that the RST macro ran and your board is in a weird state now.

PSL - This forces the pad into sleep mode. See the special considerations section for what all that means.

CSL - This uses the windows sleep keystrokes to put the host machine to sleep and THEN puts the macro pad into sleep mode.

# Special Considerations

## Coffee Theme
This firmware features a comprehensive coffee-themed aesthetic that complements the Artisan Coffee Roaster integration:

### Color Palette:
- **Coffee Bean**: Dark coffee bean brown (`#4A2C1A`)
- **Cream**: Light cream color (`#F5F0E6`) 
- **Espresso**: Dark espresso (`#2F1B14`)
- **Caramel**: Warm caramel (`#D2691E`)
- **Mocha**: Rich mocha brown (`#8B4513`)
- **Heater On**: Hot orange for active heater (`#FF4400`)
- **Heater Off**: Grey for inactive heater (`#666666`)
- **Steam**: Light steam blue (`#E6E6FA`)

### Layer-Specific Themes:
- **Layer 1**: Cream and caramel tones for general navigation
- **Layer 2**: Coffee bean and cream for numpad + roaster control
- **Layer 3**: Steam blue and heater orange for development tools
- **Layer 4**: Espresso and mocha for power management

## Layer Menu
There is a coffee-themed layer selection menu that will be used in this firmware. The animations and menu system use coffee-inspired imagery and colors that create a cohesive experience throughout the device.

You can find the image versions of all the menus + animations in the `frogimancer\images` directory.

## Oh So Eeepy Sleepy
This firmware comes equiped with a custom sleep mode that will activate if the pad is inactive for 30 minutes OR the user puts it into sleep using the sleep macro (see layer four). Any user interaction should keep the pad awake for another 30 minutes.

There is also a nice little easter egg to let you know when its going to sleep. When asleep, all the LEDs will turn off and the screen should be blank.

While asleep, the keymatrix is still "on" and can be woken up using any interaction (knob turn, keypress), however the action tied to that interation **will not execute** on first press. This means that you would need one tap to wake the pad up and then one tap on the key you want to use. This is done so that you know what layer you are on when you wake up the pad and don't accidentally hit the computer sleep macro or something because you didn have the layer's backlighting on.

## Mouse Jiggler
The mouse jiggler is mostly intended to keep your machine from going to sleep, without needing to use something like "Awake" or "Caffine", which I always forget to turn off. It does three things: 
1) it will move your mouse around 
2) it will tap the CTRL keys
3) it will flash the keypad backlighting to indicate its on

This means that the host computer is getting two kinds of inputs (keys and mouse movement), which should cover all your bases.

To break out of the macro, tap any key (similar to waking from sleep). You may want to tap the jiggler key to make sure you dont accidentally trigger something else

There is also a limit imposed on how far the random wandering can stray from the starting point. You can increase or decrease this limit by changing the constant MAX_JIGGLE_DISTANCE, or disable this entirely by commenting out the call to `elastic_bias`.

## Driver Tweaks

There are a few recommended tweaks that help make the I2C driven screen a bit less harsh and hopefully extend its lifespan.

#### 1) Turning down the brightness
By default, the backlight on the OLED is set to max power (255) so that it is VERY bright. I didn't really like this (its pretty eye searing) so I turned it down quite a bit.

This is done in the file `\qmk_firmware\drivers\oled\oled_driver.h`

```
// Default brightness level
#if !defined(OLED_BRIGHTNESS)
#    define OLED_BRIGHTNESS 100
#endif
```

Aside: Im not really sure if this is the "best practice" for changing these settings, but just changing defines in the keymap files didn't seem to have any effect. Note that changes in these driver files will require a much longer+deeper rebuild next time you compile.

#### 2) Turning down the timeout + enabling fade

The screen timeout (auto shut off), is WAY to long, so I turned it down.

This is done in the file `\qmk_firmware\drivers\oled\oled_driver.h`

By default, I think it is 60 seconds, but since we have LED coloring, that wasnt necessary. I set it to 15 seconds.

```
#if !defined(OLED_TIMEOUT)
#    if defined(OLED_DISABLE_TIMEOUT)
#        define OLED_TIMEOUT 0
#    else
#        define OLED_TIMEOUT 15000
#    endif
#endif
```

I also enabled the OLED fade out, though this has a pretty minimal effect in practice. The minimum brightness for "ON" is still quite bright, so this fade effect still ends in a rather abrupt powering off of the screen. I would like to revisit this another time and see if there is a way to make the effect less obvious and eye catching (turning off pixles incramentally to "fizzle" out etc.).

```
# define OLED_FADE_OUT 1 // Added this line to turn fade on
#if !defined(OLED_FADE_OUT_INTERVAL)
#    define OLED_FADE_OUT_INTERVAL 0x05
#endif
```

Aside: Im not really sure if this is the "best practice" for changing these settings, but just changing defines in the keymap files didnt seem to have any effect. Note that changes in these driver files will require a much longer+deeper rebuild next time you compile.

#### 3) Turning down the logo timout
The logo doesnt need to be shown for nearly as long as it is shown in the stock firmware, so I turned that down to 2 (2000 ms) seconds in `kb16.c` at the root of this repo.

# Artisan Coffee Roaster Integration

This firmware includes specialized support for controlling Kaleido M2s coffee roasters through Artisan Coffee Roaster software.

## Setup Requirements

1. **Artisan Coffee Roaster** software installed and running
2. **Kaleido M2s** roaster connected and configured in Artisan
3. **Keyboard shortcuts enabled** in Artisan settings
4. **Rev2 hardware** (this firmware is designed specifically for Rev2)

## Usage

1. Switch to **Layer 2** on the macropad
2. Rotate the **large top-middle knob** to control heater power:
   - Clockwise: Increase heater power by 1%
   - Counter-clockwise: Decrease heater power by 1%
3. The macropad sends proper Kaleido commands in the format `kaleido(HP,{value})`
4. Heater power range: 0% to 100% with automatic bounds checking

## Technical Details

- **Default heater value**: 50%
- **Command format**: Uses Kaleido's native command structure
- **State tracking**: Maintains current heater value in firmware memory
- **Layer-specific**: Only active on Layer 2 to avoid accidental activation
- **Encoder mapping**: Uses Encoder 2 (pins A1/A2) - the large top-middle knob

This integration allows precise, hands-free control of your Kaleido M2s roaster during coffee roasting sessions without needing to interact with the computer interface.