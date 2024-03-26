#ifndef KEYMAP_H__
#define KEYMAP_H__

#include "FlipClock.h"

#if SDL != 1
typedef SDL_Keycode SDLKey;
#endif

#define BTN_UP SDLK_UP
#define BTN_UP_INDEX 0
#define BTN_DOWN SDLK_DOWN
#define BTN_DOWN_INDEX 1
#define BTN_LEFT SDLK_LEFT
#define BTN_LEFT_INDEX 2
#define BTN_RIGHT SDLK_RIGHT
#define BTN_RIGHT_INDEX 3
#define BTN_A SDLK_SPACE
#define BTN_A_INDEX 4
#define BTN_B SDLK_LCTRL
#define BTN_B_INDEX 5
#define BTN_X SDLK_LSHIFT
#define BTN_X_INDEX 6
#define BTN_Y SDLK_LALT
#define BTN_Y_INDEX 7
#define BTN_L1 SDLK_e
#define BTN_L1_INDEX 8
#define BTN_R1 SDLK_t
#define BTN_R1_INDEX 9
#define BTN_L2 SDLK_TAB
#define BTN_L2_INDEX 10
#define BTN_R2 SDLK_BACKSPACE
#define BTN_R2_INDEX 11
#define BTN_SELECT SDLK_RCTRL
#define BTN_SELECT_INDEX 12
#define BTN_START SDLK_RETURN
#define BTN_START_INDEX 13
#define BTN_MENU SDLK_ESCAPE
#define BTN_MENU_INDEX 14
#define BTN_POWER SDLK_UNKNOWN // SDLK_FIRST = 0; SDLK_UNKNOWN = 0;
#define BTN_POWER_INDEX 15

#endif // KEYMAP_H__
