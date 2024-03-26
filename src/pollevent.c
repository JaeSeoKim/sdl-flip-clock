#include "FlipClock.h"

void handle_keydown(SDLKey sym);
void handle_keyup(SDLKey sym);

void pollevent() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      _G.quit = 1;
      break;
    case SDL_KEYDOWN:
      handle_keydown(event.key.keysym.sym);
      break;
    case SDL_KEYUP:
      handle_keyup(event.key.keysym.sym);
      break;
    default:
      break;
    }
  }
}

void handle_keydown(SDLKey sym) {
  switch (sym) {
  case BTN_UP:
    if (_G.KEY_STATUS[BTN_UP_INDEX] == 0)
      _G.KEY_STATUS[BTN_UP_INDEX] = _G.curTickCount;
    break;
  case BTN_DOWN:
    if (_G.KEY_STATUS[BTN_DOWN_INDEX] == 0)
      _G.KEY_STATUS[BTN_DOWN_INDEX] = _G.curTickCount;
    break;
  case BTN_LEFT:
    if (_G.KEY_STATUS[BTN_LEFT_INDEX] == 0)
      _G.KEY_STATUS[BTN_LEFT_INDEX] = _G.curTickCount;
    break;
  case BTN_RIGHT:
    if (_G.KEY_STATUS[BTN_RIGHT_INDEX] == 0)
      _G.KEY_STATUS[BTN_RIGHT_INDEX] = _G.curTickCount;
    break;
  case BTN_A:
    if (_G.KEY_STATUS[BTN_A_INDEX] == 0)
      _G.KEY_STATUS[BTN_A_INDEX] = _G.curTickCount;
    break;
  case BTN_B:
    if (_G.KEY_STATUS[BTN_B_INDEX] == 0)
      _G.KEY_STATUS[BTN_B_INDEX] = _G.curTickCount;
    break;
  case BTN_X:
    if (_G.KEY_STATUS[BTN_X_INDEX] == 0)
      _G.KEY_STATUS[BTN_X_INDEX] = _G.curTickCount;
    break;
  case BTN_Y:
    if (_G.KEY_STATUS[BTN_Y_INDEX] == 0)
      _G.KEY_STATUS[BTN_Y_INDEX] = _G.curTickCount;
    break;
  case BTN_L1:
    if (_G.KEY_STATUS[BTN_L1_INDEX] == 0)
      _G.KEY_STATUS[BTN_L1_INDEX] = _G.curTickCount;
    break;
  case BTN_R1:
    if (_G.KEY_STATUS[BTN_R1_INDEX] == 0)
      _G.KEY_STATUS[BTN_R1_INDEX] = _G.curTickCount;
    break;
  case BTN_L2:
    if (_G.KEY_STATUS[BTN_L2_INDEX] == 0)
      _G.KEY_STATUS[BTN_L2_INDEX] = _G.curTickCount;
    break;
  case BTN_R2:
    if (_G.KEY_STATUS[BTN_R2_INDEX] == 0)
      _G.KEY_STATUS[BTN_R2_INDEX] = _G.curTickCount;
    break;
  case BTN_SELECT:
    if (_G.KEY_STATUS[BTN_SELECT_INDEX] == 0)
      _G.KEY_STATUS[BTN_SELECT_INDEX] = _G.curTickCount;
    break;
  case BTN_START:
    if (_G.KEY_STATUS[BTN_START_INDEX] == 0)
      _G.KEY_STATUS[BTN_START_INDEX] = _G.curTickCount;
    break;
  case BTN_MENU:
    if (_G.KEY_STATUS[BTN_MENU_INDEX] == 0)
      _G.KEY_STATUS[BTN_MENU_INDEX] = _G.curTickCount;
    break;
  case BTN_POWER:
    if (_G.KEY_STATUS[BTN_POWER_INDEX] == 0)
      _G.KEY_STATUS[BTN_POWER_INDEX] = _G.curTickCount;
    break;
  default:
    break;
  }
  printf("Key pressed: %s\n", SDL_GetKeyName(sym));
}

void handle_keyup(SDLKey sym) {
  switch (sym) {
  case BTN_UP:
    _G.KEY_STATUS[BTN_UP_INDEX] = 0;
    break;
  case BTN_DOWN:
    _G.KEY_STATUS[BTN_DOWN_INDEX] = 0;
    break;
  case BTN_LEFT:
    _G.KEY_STATUS[BTN_LEFT_INDEX] = 0;
    break;
  case BTN_RIGHT:
    _G.KEY_STATUS[BTN_RIGHT_INDEX] = 0;
    break;
  case BTN_A:
    _G.KEY_STATUS[BTN_A_INDEX] = 0;
    break;
  case BTN_B:
    _G.KEY_STATUS[BTN_B_INDEX] = 0;
    break;
  case BTN_X:
    _G.KEY_STATUS[BTN_X_INDEX] = 0;
    break;
  case BTN_Y:
    _G.KEY_STATUS[BTN_Y_INDEX] = 0;
    break;
  case BTN_L1:
    _G.KEY_STATUS[BTN_L1_INDEX] = 0;
    break;
  case BTN_R1:
    _G.KEY_STATUS[BTN_R1_INDEX] = 0;
    break;
  case BTN_L2:
    _G.KEY_STATUS[BTN_L2_INDEX] = 0;
    break;
  case BTN_R2:
    _G.KEY_STATUS[BTN_R2_INDEX] = 0;
    break;
  case BTN_SELECT:
    _G.KEY_STATUS[BTN_SELECT_INDEX] = 0;
    break;
  case BTN_START:
    _G.KEY_STATUS[BTN_START_INDEX] = 0;
    break;
  case BTN_MENU:
    _G.KEY_STATUS[BTN_MENU_INDEX] = 0;
    break;
  case BTN_POWER:
    _G.KEY_STATUS[BTN_POWER_INDEX] = 0;
    break;
  default:
    break;
  }
  printf("Key released: %s\n", SDL_GetKeyName(sym));
}
