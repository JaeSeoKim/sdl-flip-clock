#ifndef FLIP_CLOCK_H__
#define FLIP_CLOCK_H__

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "stdio.h"
#include "stdlib.h"

#include "KeyMap.h"

struct FlipClock {
  int32_t DISPLAY_WIDTH;
  int32_t DISPLAY_HEIGHT;

  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *font;

  int quit;
  int16_t KEY_STATUS;
} typedef FlipClock;

extern FlipClock _G;

int init();
void destroy();
int draw_loop();

void pollevent();

int init_display_Resolution();

void SDL_RenderFillRoundedRect(SDL_Renderer *renderer, int x, int y, int w,
                               int h, int percent);
void SDL_RenderFillCircle(SDL_Renderer *renderer, int cx, int cy, int radius);

#endif // FLIP_CLOCK_H__
