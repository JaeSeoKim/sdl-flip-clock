#include "FlipClock.h"

int init() {
  printf("##Start initialization\n");
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL_Init Fail: %s\n", SDL_GetError());
    return -1;
  }

  if (TTF_Init() < 0) {
    printf("TTF_Init Fail: %s\n", TTF_GetError());
    return -1;
  }

  _G.font = TTF_OpenFont("fonts/Oswald-Bold.ttf", 64);
  if (_G.font == NULL) {
    printf("TTF_OpenFont Fail: %s\n", TTF_GetError());
    return -1;
  }

  if (init_display_Resolution() < 0) {
    return -1;
  }

  printf("\tDisplay Resolution: %dx%d\n", _G.DISPLAY_WIDTH, _G.DISPLAY_HEIGHT);

  if (SDL_CreateWindowAndRenderer(_G.DISPLAY_WIDTH, _G.DISPLAY_HEIGHT,
                                  SDL_WINDOW_SHOWN, &_G.window,
                                  &_G.renderer) < 0) {
    printf("SDL_CreateWindowAndRenderer Fail: %s\n", SDL_GetError());
    return -1;
  }

  printf("##Finish initialization\n\n");
  return 0;
}

int init_display_Resolution() {
  SDL_DisplayMode mode;
  if (SDL_GetDesktopDisplayMode(0, &mode)) {
    printf("SDL_GetDesktopDisplayMode Fail: %s\n", SDL_GetError());
    return -1;
  }
  _G.DISPLAY_WIDTH = mode.w;
  _G.DISPLAY_HEIGHT = mode.h;
  return 0;
}
