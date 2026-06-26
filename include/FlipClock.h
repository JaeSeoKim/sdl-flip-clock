#ifndef FLIP_CLOCK_H__
#define FLIP_CLOCK_H__

#include "SDL.h"
#include "SDL_ttf.h"

#include "fcntl.h"
#include "math.h"
#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/wait.h"
#include "time.h"
#include "unistd.h"

#include "KeyMap.h"

#define FRAME_RATE 60.0f

#define FLIP_COLOR 0xFF222222
#define FONT_COLOR 0xFFbababa
#define BG_COLOR 0xFF000000
#define FLIP_ANIMATION_MS 760

typedef struct TimeTexture {
  SDL_Surface *surface;
  int value;
  int hour;
  int showAmpm;
  int w;
  int h;
} TimeTexture;

typedef struct FlipCard {
  int currentValue;
  int nextValue;
  int animationStartTick;
  int animating;
  TimeTexture currentTexture;
  TimeTexture nextTexture;
} FlipCard;

struct FlipClock {
  int32_t DISPLAY_WIDTH;
  int32_t DISPLAY_HEIGHT;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Surface *frameSurface;
  SDL_Texture *frameTexture;
  TTF_Font *timeFont;
  int timeFontDescent;
  TTF_Font *ampmFont;
  int ampmFontDescent;
  int KEY_STATUS[KEY_STATUS_COUNT];
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
  int manualTimeEnabled;
  int manualHour;
  int manualMinute;
  int menuOpen;
  int menuIndex;
  int menuEditMode;
  int menuEditField;
  char *timeSetHook;
  char fpsText[16];
  SDL_Surface *fpsSurface;
  FlipCard cards[2];
} typedef FlipClock;

#define is_active_mode(MODE) _G.mode & (1 << (MODE))
#define MODE_AMPM 0
#define MODE_FPS 1

#define INPUT_DELAY 300

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
void draw_card(int x, int y, int cardIndex, int time, int hour, int showAmpm);
void draw_divider();
void draw_fps(float fps);
void draw_menu();
struct tm *get_local_time();
void destroy_card_textures();

// pollevent.c
void pollevent();

// render.c
int calculateRadius(int width, int height, int percent);
void RenderFillRoundedRect(SDL_Renderer *renderer, int x, int y, int w, int h,
                           int percent, Uint32 color);
void RenderFillCircle(SDL_Renderer *renderer, int cx, int cy, int radius,
                      Uint32 color);
int RenderFillRect(SDL_Renderer *dst, SDL_Rect *rect, Uint32 color);
int RenderClear(SDL_Renderer *dst, Uint32 color);
int RenderPresent(SDL_Renderer *renderer);
int BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Renderer *dst,
                SDL_Rect *dstrect);
int BlitSurfaceToSurface(SDL_Surface *src, SDL_Rect *srcrect,
                         SDL_Surface *dst, SDL_Rect *dstrect);
SDL_Surface *RenderText_Blended(TTF_Font *font, const char *text, Uint32 color);
SDL_Color uint32ToSDLColor(Uint32 color);

#endif // FLIP_CLOCK_H__
