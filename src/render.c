#include "FlipClock.h"

int calculateRadius(int width, int height, int percent) {
  float smallest = width < height ? width : height;
  if (percent > 100) {
    percent = 100;
  } else if (percent < 0) {
    percent = 0;
  }
  return (smallest / 2 * percent) / 100;
}

void SDL_RenderFillRoundedRect(SDL_Renderer *renderer, int x, int y, int w,
                               int h, int percent, Uint32 color) {
  int radius = calculateRadius(w, h, percent);

  int x1 = x + radius;
  int x2 = x + w - radius;
  int y1 = y + radius;
  int y2 = y + h - radius;

  SDL_Rect rect = {x1, y, w - radius * 2, h};
  __SDL_RenderFillRect(renderer, &rect, color);
  rect = (SDL_Rect){x, y1, w, h - radius * 2};
  __SDL_RenderFillRect(renderer, &rect, color);

  SDL_RenderFillCircle(renderer, x1, y1, radius, color);
  SDL_RenderFillCircle(renderer, x2, y1, radius, color);
  SDL_RenderFillCircle(renderer, x1, y2, radius, color);
  SDL_RenderFillCircle(renderer, x2, y2, radius, color);
}

void SDL_RenderFillCircle(SDL_Renderer *renderer, int cx, int cy, int radius,
                          Uint32 color) {
  const int diameter = (radius * 2);

  int x = (radius - 1);
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = (tx - diameter);

  while (x >= y) {
    for (int i = cx - x; i <= cx + x; i++) {
      __SDL_RenderFillRect(renderer, &(SDL_Rect){i, cy + y, 1, 1}, color);
      __SDL_RenderFillRect(renderer, &(SDL_Rect){i, cy - y, 1, 1}, color);
    }
    for (int i = cx - y; i <= cx + y; i++) {
      __SDL_RenderFillRect(renderer, &(SDL_Rect){i, cy + x, 1, 1}, color);
      __SDL_RenderFillRect(renderer, &(SDL_Rect){i, cy - x, 1, 1}, color);
    }

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }
    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

#if defined SDL && SDL == 1
int __SDL_RenderFillRect(SDL_Renderer *dst, SDL_Rect *rect, Uint32 color) {
  return SDL_FillRect(dst, rect, color);
}
#else
int __SDL_RenderFillRect(SDL_Renderer *dst, SDL_Rect *rect,
                         Uint32 color) {
  SDL_SetRenderDrawColor(dst, color >> 24 & 255, color >> 16 & 255,
                         color >> 8 & 255, color & 255);
  return SDL_RenderFillRect(dst, rect);
}
#endif
