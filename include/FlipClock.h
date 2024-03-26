#ifndef FLIP_CLOCK_H__
#define FLIP_CLOCK_H__

#if defined SDL && SDL == 1
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#endif

#include "fcntl.h"
#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "unistd.h"

#include "KeyMap.h"

#if defined SDL && SDL == 1
typedef SDL_Surface SDL_Window;
typedef SDL_Surface SDL_Renderer;
#endif

#define FRAME_RATE 60.0f

#define FLIP_COLOR 0xFF222222
#define FONT_COLOR 0xFFbababa
#define BG_COLOR 0xFF000000

struct FlipClock {
  int32_t DISPLAY_WIDTH;
  int32_t DISPLAY_HEIGHT;
  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *timeFont;
  int timeFontDescent;
  TTF_Font *ampmFont;
  int ampmFontDescent;
  int KEY_STATUS[16];
  int quit;
  int mode;
  int lastTickCount;
  int curTickCount;
  int GAP;
  int DIVIDER;
  int MARGIN_X;
  int FLIP_SIZE;
  int FLIP_PADDING;
  int TIME_SIZE;
  int AMPM_SIZE;
} typedef FlipClock;

#define is_active_mode(MODE) _G.mode & (1 << (MODE))
#define MODE_AMPM 0
#define MODE_FPS 1

#define INPUT_DELAY 300;

extern FlipClock _G;

// init.c
int init();
int init_display_Resolution();

// destroy.c
void destroy();

// draw.c
int draw_loop();
int handle_key();
int draw_screen(float fps);
void draw_flip(int x, int y);
void draw_time(int x, int y, int time);
void draw_ampm(int x, int y, int hour);
void draw_divider();
void draw_fps(float fps);
struct tm *get_local_time();

// pollevent.c
void pollevent();

// render.c
void RenderFillRoundedRect(SDL_Renderer *renderer, int x, int y, int w, int h,
                           int percent, Uint32 color);
void RenderFillCircle(SDL_Renderer *renderer, int cx, int cy, int radius,
                      Uint32 color);
int RenderFillRect(SDL_Renderer *dst, SDL_Rect *rect, Uint32 color);
int RenderClear(SDL_Renderer *dst, Uint32 color);
int RenderPresent(SDL_Renderer *renderer, SDL_Window *window);
int BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Renderer *dst,
                SDL_Rect *dstrect);
SDL_Surface *RenderText_Blended(TTF_Font *font, const char *text, Uint32 color);
SDL_Color uint32ToSDLColor(Uint32 color);

#endif // FLIP_CLOCK_H__
