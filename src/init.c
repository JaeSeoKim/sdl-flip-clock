#include "FlipClock.h"

int init() {
  printf("##Start initialization\n");

  if (access("/tmp/stay_awake", F_OK) == -1) {
    printf("\tSet stay awake\n");
    close(creat("/tmp/stay_awake", 666));
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("\tSDL_Init Fail: %s\n", SDL_GetError());
    return -1;
  }

  if (TTF_Init() < 0) {
    printf("\tTTF_Init Fail: %s\n", TTF_GetError());
    return -1;
  }

  if (init_display_Resolution() < 0) {
    return -1;
  }
  printf("\tDisplay Resolution: %dx%d\n", _G.DISPLAY_WIDTH, _G.DISPLAY_HEIGHT);

  _G.MARGIN_X = _G.DISPLAY_WIDTH / 20;
  _G.GAP = 16;
  _G.DIVIDER = 4;
  _G.FLIP_SIZE = (_G.DISPLAY_WIDTH - _G.MARGIN_X * 2 - _G.GAP) / 2;
  _G.FLIP_PADDING = 32;
  _G.TIME_SIZE = _G.FLIP_SIZE - _G.FLIP_PADDING * 2;
  _G.AMPM_SIZE = _G.TIME_SIZE / 10;

#if defined SDL && SDL == 1
  const char *FONT_LOCATION = "/mnt/SDCARD/App/FlipClock/fonts/Oswald-Bold.ttf";
#else
  const char *FONT_LOCATION = "./fonts/Oswald-Bold.ttf";
#endif
  _G.font = TTF_OpenFont(FONT_LOCATION, _G.TIME_SIZE);
  _G.smFont = TTF_OpenFont(FONT_LOCATION, _G.AMPM_SIZE);
  if (_G.font == NULL || _G.smFont == NULL) {
    printf("\tTTF_OpenFont Fail: %s\n", TTF_GetError());
    return -1;
  }

#if defined SDL && SDL == 1
  _G.window =
      SDL_SetVideoMode(_G.DISPLAY_WIDTH, _G.DISPLAY_HEIGHT, 32, SDL_HWSURFACE);
#else
  _G.window = SDL_CreateWindow("Flip Clock", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, _G.DISPLAY_WIDTH,
                               _G.DISPLAY_HEIGHT, SDL_WINDOW_SHOWN);
#endif
  if (_G.window == NULL) {
    printf("\tSDL_SetVideoMode Fail: %s\n", SDL_GetError());
    return -1;
  }

#if defined SDL && SDL == 1
  _G.renderer = SDL_CreateRGBSurface(SDL_HWSURFACE, _G.DISPLAY_WIDTH,
                                     _G.DISPLAY_HEIGHT, 32, 0, 0, 0, 0);
#else
  _G.renderer = SDL_CreateRenderer(_G.window, -1, 0);
#endif

  if (_G.renderer == NULL) {
    printf("\tSDL_CreateRGBSurface Fail: %s\n", SDL_GetError());
    return -1;
  }

  printf("##Finish initialization\n\n");
  return 0;
}

int init_display_Resolution() {
#if defined SDL && SDL == 1
  const SDL_VideoInfo *info = SDL_GetVideoInfo();
  _G.DISPLAY_WIDTH = info->current_w;
  _G.DISPLAY_HEIGHT = info->current_h;
#else
  SDL_DisplayMode mode;
  if (SDL_GetDesktopDisplayMode(0, &mode)) {
    printf("SDL_GetDesktopDisplayMode Fail: %s\n", SDL_GetError());
    return -1;
  }
  _G.DISPLAY_WIDTH = mode.w;
  _G.DISPLAY_HEIGHT = mode.h;
#endif
  return 0;
}
