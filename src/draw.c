#include "FlipClock.h"

#define FLIPCLOCK_HOUR 0
#define FLIPCLOCK_MINIUTE 1

int draw_screen() {
  struct tm *local;
  time_t _time;
  char timeString[3] = "\0\0\0";

  _time = time(&_time);
  local = localtime(&_time);

  int x = _G.MARGIN_X;
  int y = (_G.DISPLAY_HEIGHT - _G.CARD_SIZE) / 2;

  for (int layout = 0; layout < 2; ++layout) {
    int time_tmp = local->tm_min;
    if (layout == FLIPCLOCK_HOUR) {
      time_tmp = (local->tm_hour % 12);
      if (time_tmp == 0)
        time_tmp = 12;
    }
    timeString[1] = time_tmp % 10 + '0';
    timeString[0] = time_tmp / 10 + '0';

    Uint32 rectColor = 0x222222FF;
    SDL_RenderFillRoundedRect(_G.renderer, x, y, _G.CARD_SIZE, _G.CARD_SIZE, 10,
                              rectColor);

    SDL_Color textColor = {186, 186, 186, 255};

    // Draw Time
    int fontDescentSize = TTF_FontDescent(_G.font);
    SDL_Surface *textSurface =
        TTF_RenderText_Blended(_G.font, timeString, textColor);

#if defined SDL && SDL == 1
    SDL_BlitSurface(
        textSurface, NULL, _G.renderer,
        &(SDL_Rect){x + _G.CARD_PADDING + (_G.TIME_SIZE - textSurface->w) / 2,
                    y + _G.CARD_PADDING + fontDescentSize, textSurface->w,
                    textSurface->h});

#else
    SDL_Texture *mTexture =
        SDL_CreateTextureFromSurface(_G.renderer, textSurface);

    SDL_RenderCopy(
        _G.renderer, mTexture, NULL,
        &(SDL_Rect){x + _G.CARD_PADDING + (_G.TIME_SIZE - textSurface->w) / 2,
                    y + _G.CARD_PADDING + fontDescentSize, textSurface->w,
                    textSurface->h});
    // free allocated memories
    SDL_DestroyTexture(mTexture);
#endif

    // free allocated memories
    SDL_FreeSurface(textSurface);

    if (layout != FLIPCLOCK_HOUR) {
      continue;
    }

    // Draw AM/PM
    fontDescentSize = TTF_FontDescent(_G.smFont);
    textSurface = TTF_RenderText_Blended(
        _G.smFont, local->tm_hour < 11 ? "AM" : "PM", textColor);

#if defined SDL && SDL == 1
    SDL_BlitSurface(textSurface, NULL, _G.renderer,
                    &(SDL_Rect){x + _G.CARD_PADDING / 2,
                                y + _G.CARD_SIZE - _G.CARD_PADDING / 2 -
                                    _G.AMPM_SIZE + fontDescentSize,
                                textSurface->w, textSurface->h});

#else
    mTexture = SDL_CreateTextureFromSurface(_G.renderer, textSurface);
    SDL_RenderCopy(_G.renderer, mTexture, NULL,
                   &(SDL_Rect){x + _G.CARD_PADDING / 2,
                               y + _G.CARD_SIZE - _G.CARD_PADDING / 2 -
                                   _G.AMPM_SIZE + fontDescentSize,
                               textSurface->w, textSurface->h});
    // free allocated memories
    SDL_DestroyTexture(mTexture);
#endif
    // free allocated memories
    SDL_FreeSurface(textSurface);

    // Update Position
    x += _G.GAP + _G.CARD_SIZE;
  }

  // Draw Divider
  __SDL_RenderFillRect(_G.renderer,
                       &(SDL_Rect){0, _G.DISPLAY_HEIGHT / 2 - _G.DIVIDER,
                                   _G.DISPLAY_WIDTH, _G.DIVIDER},
                       0x000000FF);
  return 0;
}

int handle_key() {
  // MENU + SELECT -> Exit
  if (_G.KEY_STATUS & (1 << BTN_MENU_BIT) &&
      _G.KEY_STATUS & (1 << BTN_SELECT_BIT)) {
    _G.quit = 1;
  }

  return 0;
}

int draw_loop() {
  printf("##Start rendering loop\n");

  int lastTickCount = SDL_GetTicks();
  int curTickCount = lastTickCount;

  while (_G.quit == 0) {
    // pollEvent
    pollevent();
    // handle key
    handle_key();

    // before draw
#if defined SDL && SDL == 1
    curTickCount = SDL_GetTicks();
    Uint32 black = SDL_MapRGB(_G.renderer->format, 0, 0, 0);
    __SDL_RenderFillRect(_G.renderer, NULL, black);
#else
    SDL_SetRenderDrawColor(_G.renderer, 0, 0, 0, 255);
    SDL_RenderClear(_G.renderer);
#endif

    // draw
    draw_screen();

    // after draw
#if defined SDL && SDL == 1
    SDL_BlitSurface(_G.renderer, NULL, _G.window, NULL);
#else
    SDL_RenderPresent(_G.renderer);
#endif
    lastTickCount = curTickCount;
  }

  printf("##Finish rendering loop\n");
  return 0;
}
