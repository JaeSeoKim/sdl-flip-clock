#ifndef KEYMAP_H__
#define KEYMAP_H__

#include "FlipClock.h"

#if SDL != 1
typedef SDL_Keycode SDLKey;
#endif

#define BTN_UP SDLK_UP
#define BTN_UP_BIT 0
#define BTN_DOWN SDLK_DOWN
#define BTN_DOWN_BIT 1
#define BTN_LEFT SDLK_LEFT
#define BTN_LEFT_BIT 2
#define BTN_RIGHT SDLK_RIGHT
#define BTN_RIGHT_BIT 3
#define BTN_A SDLK_SPACE
#define BTN_A_BIT 4
#define BTN_B SDLK_LCTRL
#define BTN_B_BIT 5
#define BTN_X SDLK_LSHIFT
#define BTN_X_BIT 6
#define BTN_Y SDLK_LALT
#define BTN_Y_BIT 7
#define BTN_L1 SDLK_e
#define BTN_L1_BIT 8
#define BTN_R1 SDLK_t
#define BTN_R1_BIT 9
#define BTN_L2 SDLK_TAB
#define BTN_L2_BIT 10
#define BTN_R2 SDLK_BACKSPACE
#define BTN_R2_BIT 11
#define BTN_SELECT SDLK_RCTRL
#define BTN_SELECT_BIT 12
#define BTN_START SDLK_RETURN
#define BTN_START_BIT 13
#define BTN_MENU SDLK_ESCAPE
#define BTN_MENU_BIT 14
#define BTN_POWER SDLK_UNKNOWN // SDLK_FIRST = 0; SDLK_UNKNOWN = 0;
#define BTN_POWER_BIT 15

#endif // KEYMAP_H__
