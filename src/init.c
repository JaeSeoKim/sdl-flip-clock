#include "FlipClock.h"

int init() {
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

  _G.mode = 1 << MODE_AMPM;

  _G.MARGIN_X = _G.DISPLAY_WIDTH / 20;
  _G.GAP = 16;
  _G.DIVIDER = 4;
  _G.FLIP_SIZE = (_G.DISPLAY_WIDTH - _G.MARGIN_X * 2 - _G.GAP) / 2;
  _G.FLIP_PADDING = 32;
  _G.TIME_SIZE = _G.FLIP_SIZE - _G.FLIP_PADDING * 2;
  _G.AMPM_SIZE = _G.TIME_SIZE / 10;

  const char *FONT_LOCATION = "./fonts/Oswald-Bold.ttf";
  _G.timeFont = TTF_OpenFont(FONT_LOCATION, _G.TIME_SIZE);
  _G.ampmFont = TTF_OpenFont(FONT_LOCATION, _G.AMPM_SIZE);
  if (_G.timeFont == NULL || _G.ampmFont == NULL) {
    printf("\tTTF_OpenFont Fail: %s\n", TTF_GetError());
    return -1;
  }
  _G.timeFontDescent = TTF_FontDescent(_G.timeFont);
  _G.ampmFontDescent = TTF_FontDescent(_G.ampmFont);
  struct tm *time = get_local_time();
  _G.manualTimeEnabled = 0;
  _G.manualHour = time->tm_hour;
  _G.manualMinute = time->tm_min;
  _G.menuOpen = 0;
  _G.menuIndex = 0;
  _G.menuEditMode = 0;
  _G.menuEditField = 0;
  for (int i = 0; i < 2; ++i) {
    _G.cards[i].currentValue = -1;
    _G.cards[i].nextValue = -1;
  }

  _G.window = SDL_CreateWindow("Flip Clock", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, _G.DISPLAY_WIDTH,
                               _G.DISPLAY_HEIGHT, SDL_WINDOW_SHOWN);
  if (_G.window == NULL) {
    printf("\tSDL_CreateWindow Fail: %s\n", SDL_GetError());
    return -1;
  }

  _G.renderer = SDL_CreateRenderer(_G.window, -1, 0);
  if (_G.renderer == NULL) {
    printf("\tSDL_CreateRenderer Fail: %s\n", SDL_GetError());
    return -1;
  }

  _G.frameSurface =
      SDL_CreateRGBSurface(0, _G.DISPLAY_WIDTH, _G.DISPLAY_HEIGHT, 16, 0, 0, 0,
                           0);
  if (_G.frameSurface == NULL) {
    printf("\tSDL_CreateRGBSurface frame Fail: %s\n", SDL_GetError());
    return -1;
  }

  _G.frameTexture =
      SDL_CreateTexture(_G.renderer, SDL_PIXELFORMAT_RGB565,
                        SDL_TEXTUREACCESS_STREAMING, _G.DISPLAY_WIDTH,
                        _G.DISPLAY_HEIGHT);
  if (_G.frameTexture == NULL) {
    printf("\tSDL_CreateTexture frame Fail: %s\n", SDL_GetError());
    return -1;
  }

  return 0;
}

int init_display_Resolution() {
  SDL_DisplayMode mode;
  if (SDL_GetDesktopDisplayMode(0, &mode)) {
    return 0;
  }

  if (mode.w <= 0 || mode.h <= 0) {
    return 0;
  }

  _G.DISPLAY_WIDTH = mode.w;
  _G.DISPLAY_HEIGHT = mode.h;
  return 0;
}
