#include "FlipClock.h"

void handle_keydown(SDLKey sym);
void handle_keyup(SDLKey sym);

static int key_index_for(SDLKey sym) {
  switch (sym) {
  case BTN_UP:
    return BTN_UP_INDEX;
  case BTN_DOWN:
    return BTN_DOWN_INDEX;
  case BTN_LEFT:
    return BTN_LEFT_INDEX;
  case BTN_RIGHT:
    return BTN_RIGHT_INDEX;
  case BTN_A:
    return BTN_A_INDEX;
  case BTN_B:
    return BTN_B_INDEX;
  case BTN_X:
    return BTN_X_INDEX;
  case BTN_Y:
    return BTN_Y_INDEX;
  case BTN_L1:
    return BTN_L1_INDEX;
  case BTN_R1:
    return BTN_R1_INDEX;
  case BTN_L2:
    return BTN_L2_INDEX;
  case BTN_R2:
    return BTN_R2_INDEX;
  case BTN_SELECT:
    return BTN_SELECT_INDEX;
  case BTN_START:
    return BTN_START_INDEX;
  case BTN_MENU:
    return BTN_MENU_INDEX;
  case BTN_POWER:
    return BTN_POWER_INDEX;
  default:
    return -1;
  }
}

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
  int keyIndex = key_index_for(sym);
  if (keyIndex >= 0 && _G.KEY_STATUS[keyIndex] == 0) {
    _G.KEY_STATUS[keyIndex] = _G.curTickCount;
  }
}

void handle_keyup(SDLKey sym) {
  int keyIndex = key_index_for(sym);
  if (keyIndex >= 0) {
    _G.KEY_STATUS[keyIndex] = 0;
  }
}
