#include "FlipClock.h"

#define FLIPCLOCK_HOUR 0
#define FLIPCLOCK_MINIUTE 1

struct FlipCLockStyle {
  int GAP;
  int DIVIDER;
  int MARGIN_X;

  int CARD_SIZE;
  int CARD_PADDING;

  int TIME_SIZE;
  int AMPM_SIZE;
} typedef FlipCLockStyle;

int draw(SDL_Renderer *renderer, const FlipCLockStyle *style) {
  struct tm *local;
  time_t _time;
  char timeString[3] = "\0\0\0";

  _time = time(&_time);
  local = localtime(&_time);

  int x = style->MARGIN_X;
  int y = (_G.DISPLAY_HEIGHT - style->CARD_SIZE) / 2;

  for (int layout = 0; layout < 2; ++layout) {
    int time_tmp = local->tm_min;
    if (layout == FLIPCLOCK_HOUR) {
      time_tmp = (local->tm_hour % 12);
      if (time_tmp == 0)
        time_tmp = 12;
    }
    timeString[1] = time_tmp % 10 + '0';
    timeString[0] = time_tmp / 10 + '0';

    // Draw Rect
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
    SDL_RenderFillRoundedRect(renderer, x, y, style->CARD_SIZE,
                              style->CARD_SIZE, 10);

    // Draw Time
    TTF_SetFontSize(_G.font, style->TIME_SIZE);

    int fontDescentSize = TTF_FontDescent(_G.font);
    SDL_Color textColor = {186, 186, 186, 255};
    SDL_Surface *textSurface =
        TTF_RenderText_Blended(_G.font, timeString, textColor);
    SDL_Texture *mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_RenderCopy(renderer, mTexture, NULL,
                   &(SDL_Rect){x + style->CARD_PADDING +
                                   (style->TIME_SIZE - textSurface->w) / 2,
                               y + style->CARD_PADDING + fontDescentSize,
                               textSurface->w, textSurface->h});
    // free allocated memories
    SDL_DestroyTexture(mTexture);
    SDL_FreeSurface(textSurface);

    if (layout != FLIPCLOCK_HOUR) {
      continue;
    }

    // Draw AM/PM
    TTF_SetFontSize(_G.font, style->AMPM_SIZE);
    fontDescentSize = TTF_FontDescent(_G.font);

    textSurface = TTF_RenderText_Blended(
        _G.font, local->tm_hour < 11 ? "AM" : "PM", textColor);
    mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_RenderCopy(renderer, mTexture, NULL,
                   &(SDL_Rect){x + style->CARD_PADDING / 2,
                               y + style->CARD_SIZE - style->CARD_PADDING / 2 -
                                   style->AMPM_SIZE + fontDescentSize,
                               textSurface->w, textSurface->h});
    // free allocated memories
    SDL_DestroyTexture(mTexture);
    SDL_FreeSurface(textSurface);

    // Update Position
    x += style->GAP + style->CARD_SIZE;
  }

  // Draw Divider
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer,
                     &(SDL_Rect){0, _G.DISPLAY_HEIGHT / 2 - style->DIVIDER,
                                 _G.DISPLAY_WIDTH, style->DIVIDER});

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

  FlipCLockStyle style;

  style.MARGIN_X = _G.DISPLAY_WIDTH / 20;
  style.GAP = 16;
  style.DIVIDER = 4;
  style.CARD_SIZE = (_G.DISPLAY_WIDTH - style.MARGIN_X * 2 - style.GAP) / 2;
  style.CARD_PADDING = 32;
  style.TIME_SIZE = style.CARD_SIZE - style.CARD_PADDING * 2;
  style.AMPM_SIZE = style.TIME_SIZE / 10;

  int lastTickCount = SDL_GetTicks();
  int curTickCount = lastTickCount;

  while (_G.quit == 0) {
    // pollEvent
    pollevent();
    // handle key
    handle_key();

    // before draw
    curTickCount = SDL_GetTicks();
    SDL_SetRenderDrawColor(_G.renderer, 0, 0, 0, 255);
    SDL_RenderClear(_G.renderer);

    // draw
    draw(_G.renderer, &style);

    // after draw
    SDL_RenderPresent(_G.renderer);
    lastTickCount = curTickCount;
  }

  printf("##Finish rendering loop\n");
  return 0;
}
