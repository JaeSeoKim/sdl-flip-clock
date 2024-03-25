#ifndef FLIP_CLOCK_H__
#define FLIP_CLOCK_H__

#if defined SDL && SDL == 1
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#endif

#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#include "KeyMap.h"

#if defined SDL && SDL == 1
typedef SDL_Surface SDL_Window;
typedef SDL_Surface SDL_Renderer;
#endif

struct FlipClock {
  int32_t DISPLAY_WIDTH;
  int32_t DISPLAY_HEIGHT;

  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *font;
  TTF_Font *smFont;

  int quit;
  int16_t KEY_STATUS;

  int GAP;
  int DIVIDER;
  int MARGIN_X;

  int CARD_SIZE;
  int CARD_PADDING;

  int TIME_SIZE;
  int AMPM_SIZE;
} typedef FlipClock;

extern FlipClock _G;

int init();
void destroy();
int draw_loop();

void pollevent();

int init_display_Resolution();

void SDL_RenderFillRoundedRect(SDL_Renderer *renderer, int x, int y, int w,
                               int h, int percent, Uint32 color);
void SDL_RenderFillCircle(SDL_Renderer *renderer, int cx, int cy, int radius,
                          Uint32 color);

int __SDL_RenderFillRect(SDL_Renderer *dst, const SDL_Rect *rect, Uint32 color);

#endif // FLIP_CLOCK_H__
