# Frogimancer Firmware Overview

This firmware for the Doio 16 Key + 3 Knob Macro Pad (Megalodon) is designed around adding some nice navigation features that are useful for day to day Windows use + development work. It was developed with the idea of rotating the pad 90 degrees to have the knobs on the top so that the pad can be used as a num pad if desired.

It also replaces the default menu + splash screen with something more fun.

Follow the normal installation procedure for any other keyboard using QMK (install QMK MSYS + QMK Tool kit for Windows) There are some other tweaks that were made to the base board config/drivers (deeper in the QMK install directory than this repo goes) that were made to change the screen brightness + timeout. Those are discussed in the "Special Considerations" section.

Since this repo **does not fork the base QMK repo** please make sure that you use its contents to replace: `qmk_firmware\keyboards\doio\kb16`. **This will clobber your default kb16 configuration (since some of its customization exists outside of the keymap files themselves), so please proceed with caution**

**Note:** This firmware is specifically for the REV2 version of the board, so the you should be building out of the `kb16\rev2\keymaps\frogimancer` directory

# Keymaps

This firmware utilizes four layers/keymaps, selected by the bottom left knob (when in "numpad orientation"). Since the firmware makes an assumption on the orientation of the device that isn't really true once you rotate it, I suggest looking at the diagrams in this readme rather than what is in the source. Of course the actual keymap array in the source has the final say.

The function of the knobs *does not change from layer to layer*, they are as follows:
* Bottom Left - Layer Selection: scroll through the four layers available. A click (press) goes back to layer 1.
* Top Left - Ctrl Tab Scrolling: scroll through *tabs* of active program using "CTRL + TAB" wizardry. A click (press) does nothing.
* Right (Big Knob) - Alt Tab Scrolling: scroll through *windows* as if pressing "ALT + TAB". A click (press) activatates the "WIN + D" shortcut to show the desktop.

I tried to put some helpful colorful backlighting on most things to give a quick idea of where things are. That said, it is mostly optimized for my light green + magenta color scheme, so you may want to tweak the DEFAULT_\<COLORNAME\> structs if you want to quickly change these.

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

The "W\<NUMBER\>" macros emulate the "WIN + \<NUM\>" command, but hold down the WIN key for a moment so that the user can rapidly tap the same key to scroll through instances of the same app. For example, if you have 4 explorer windows open, you can tap the macro twice to get to window two. There is also a quick key for the PowerToys "Launcher", as well as keys to switch windows desktops (a la "WIN+ \<ArrowKey\>").

The right most column has several "utilities". The first is a mouse jiggler (which is a toggle and will flash the light so you know its on), then below are the PowerToys applets for "Always On Top", "Color Picker", and "Text Extractor".

## Layer 2 - VS/VSCode Run + Debug

Second layer is mostly for making dealing with VS Code and Visual Studio's run and debugger shortcuts easier. These assume that you have the the "Run" and "Run w/ Debug" shortcuts rebound **in your IDE** such that "Run" is F5, and "Run w/ Debug" is CTRL+5. This is a preference of mine, so switch them in the firmware if you prefer it the other way around.

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

## Layer 3 - Numpad

Its a num pad, congratulations TKL users! That said, the function keys are in weird spots since we dont have the normal keys.

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

## Layer Menu
There is a lily pad themed layer selection menu that will be used in this firmware. The old default firmware animation frames are still available in the `layer_status.c` file if you want them back, although they aren't rotated 90 degrees to match everything else.

You can find the image versions of all the menus + animations in the `frogimancer\images` directory.

## Oh So Eeepy Sleepy
This firmware comes equiped with a custom sleep mode that will activate if the pad is inactive for 30 minutes OR the user puts it into sleep using the sleep macro (see layer four). Any user interaction should keep the pad awake for another 30 minutes.

There is also a nice little easter egg to let you know when its going to sleep. When asleep, all the LEDs will turn off and the screen should be blank.

While asleep, the keymatrix is still "on" and can be woken up using any interaction (knob turn, keypress), however the action tied to that interation **will not execute** on first press. This means that you would need one tap to wake the pad up and then one tap on the key you want to use. This is done so that you know what layer you are on when you wake up the pad and don't accidentally hit the computer sleep macro or something because you didn have the layer's backlighting on.

## Mouse Jiggler
The mouse jiggler is mostly intended to keep your machine from going to sleep, without needing to use something like "Awake" which I always forget to turn off. It does three things: 
1) it will move your mouse in a little square 
2) it will tap the ALT and CTRL keys
3) it will flash the keypad backlighting in a two frame pattern while this macro is active so you always know when its on

This means that the host computer is getting two kinds of inputs, which should cover all your bases.

To break out of the macro, tap any key (similar to waking from sleep).

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
The logo doesnt need to be shown for nearly as long as it is shown stock, so I turned that down to 2 (2000 ms) seconds in `kb16.c` at the root of this repo.
