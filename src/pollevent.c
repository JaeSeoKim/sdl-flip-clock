#include "FlipClock.h"

void handle_keydown(SDL_Keycode sym);
void handle_keyup(SDL_Keycode sym);

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

void handle_keydown(SDL_Keycode sym) {
  switch (sym) {
  case BTN_UP:
    _G.KEY_STATUS |= 1 << BTN_UP_BIT;
    break;
  case BTN_DOWN:
    _G.KEY_STATUS |= 1 << BTN_DOWN_BIT;
    break;
  case BTN_LEFT:
    _G.KEY_STATUS |= 1 << BTN_LEFT_BIT;
    break;
  case BTN_RIGHT:
    _G.KEY_STATUS |= 1 << BTN_RIGHT_BIT;
    break;
  case BTN_A:
    _G.KEY_STATUS |= 1 << BTN_A_BIT;
    break;
  case BTN_B:
    _G.KEY_STATUS |= 1 << BTN_B_BIT;
    break;
  case BTN_X:
    _G.KEY_STATUS |= 1 << BTN_X_BIT;
    break;
  case BTN_Y:
    _G.KEY_STATUS |= 1 << BTN_Y_BIT;
    break;
  case BTN_L1:
    _G.KEY_STATUS |= 1 << BTN_L1_BIT;
    break;
  case BTN_R1:
    _G.KEY_STATUS |= 1 << BTN_R1_BIT;
    break;
  case BTN_L2:
    _G.KEY_STATUS |= 1 << BTN_L2_BIT;
    break;
  case BTN_R2:
    _G.KEY_STATUS |= 1 << BTN_R2_BIT;
    break;
  case BTN_SELECT:
    _G.KEY_STATUS |= 1 << BTN_SELECT_BIT;
    break;
  case BTN_START:
    _G.KEY_STATUS |= 1 << BTN_START_BIT;
    break;
  case BTN_MENU:
    _G.KEY_STATUS |= 1 << BTN_MENU_BIT;
    break;
  case BTN_POWER:
    _G.KEY_STATUS |= 1 << BTN_POWER_BIT;
    break;
  default:
    break;
  }
  printf("Key pressed: %s\n", SDL_GetKeyName(sym));
}

void handle_keyup(SDL_Keycode sym) {
  switch (sym) {
  case BTN_UP:
    _G.KEY_STATUS &= ~(1 << BTN_UP_BIT);
    break;
  case BTN_DOWN:
    _G.KEY_STATUS &= ~(1 << BTN_DOWN_BIT);
    break;
  case BTN_LEFT:
    _G.KEY_STATUS &= ~(1 << BTN_LEFT_BIT);
    break;
  case BTN_RIGHT:
    _G.KEY_STATUS &= ~(1 << BTN_RIGHT_BIT);
    break;
  case BTN_A:
    _G.KEY_STATUS &= ~(1 << BTN_A_BIT);
    break;
  case BTN_B:
    _G.KEY_STATUS &= ~(1 << BTN_B_BIT);
    break;
  case BTN_X:
    _G.KEY_STATUS &= ~(1 << BTN_X_BIT);
    break;
  case BTN_Y:
    _G.KEY_STATUS &= ~(1 << BTN_Y_BIT);
    break;
  case BTN_L1:
    _G.KEY_STATUS &= ~(1 << BTN_L1_BIT);
    break;
  case BTN_R1:
    _G.KEY_STATUS &= ~(1 << BTN_R1_BIT);
    break;
  case BTN_L2:
    _G.KEY_STATUS &= ~(1 << BTN_L2_BIT);
    break;
  case BTN_R2:
    _G.KEY_STATUS &= ~(1 << BTN_R2_BIT);
    break;
  case BTN_SELECT:
    _G.KEY_STATUS &= ~(1 << BTN_SELECT_BIT);
    break;
  case BTN_START:
    _G.KEY_STATUS &= ~(1 << BTN_START_BIT);
    break;
  case BTN_MENU:
    _G.KEY_STATUS &= ~(1 << BTN_MENU_BIT);
    break;
  case BTN_POWER:
    _G.KEY_STATUS &= ~(1 << BTN_POWER_BIT);
    break;
  default:
    break;
  }
  printf("Key released: %s\n", SDL_GetKeyName(sym));
}
