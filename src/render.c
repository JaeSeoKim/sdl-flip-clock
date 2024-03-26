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

void RenderFillRoundedRect(SDL_Renderer *renderer, int x, int y, int w, int h,
                           int percent, Uint32 color) {
  int radius = calculateRadius(w, h, percent);

  int x1 = x + radius;
  int x2 = x + w - radius;
  int y1 = y + radius;
  int y2 = y + h - radius;

  SDL_Rect rect = {x1, y, w - radius * 2, h};
  RenderFillRect(renderer, &rect, color);
  rect = (SDL_Rect){x, y1, w, h - radius * 2};
  RenderFillRect(renderer, &rect, color);

  RenderFillCircle(renderer, x1, y1, radius, color);
  RenderFillCircle(renderer, x2, y1, radius, color);
  RenderFillCircle(renderer, x1, y2, radius, color);
  RenderFillCircle(renderer, x2, y2, radius, color);
}

void RenderFillCircle(SDL_Renderer *renderer, int cx, int cy, int radius,
                      Uint32 color) {
  const int diameter = (radius * 2);

  int x = (radius - 1);
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = (tx - diameter);

  SDL_Surface *surface =
      SDL_CreateRGBSurface(0, diameter, diameter, 32, 0x000000FF, 0x0000FF00,
                           0x00FF0000, 0xFF000000);

  while (x >= y) {
    for (int i = radius - x; i <= radius + x; i++) {
      *(Uint32 *)(surface->pixels + surface->pitch * (radius + y) +
                  (i * surface->format->BytesPerPixel)) = color;
      *(Uint32 *)(surface->pixels + surface->pitch * (radius - y) +
                  (i * surface->format->BytesPerPixel)) = color;
    }
    for (int i = radius - y; i <= radius + y; i++) {
      *(Uint32 *)(surface->pixels + surface->pitch * (radius + x) +
                  (i * surface->format->BytesPerPixel)) = color;
      *(Uint32 *)(surface->pixels + surface->pitch * (radius - x) +
                  (i * surface->format->BytesPerPixel)) = color;
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
  BlitSurface(surface, NULL, renderer,
              &(SDL_Rect){cx - radius, cy - radius, diameter, diameter});

  SDL_FreeSurface(surface);
}

int RenderFillRect(SDL_Renderer *dst, SDL_Rect *rect, Uint32 color) {
#if defined SDL && SDL == 1
  return SDL_FillRect(dst, rect, color);
#else
  SDL_SetRenderDrawColor(dst, color >> 16 & 255, color >> 8 & 255, color & 255,
                         color >> 24 & 255);
  return SDL_RenderFillRect(dst, rect);
#endif
}

int RenderPresent(SDL_Renderer *renderer, SDL_Window *window) {
#if defined SDL && SDL == 1
  return SDL_BlitSurface(renderer, NULL, window, NULL);
#else
  SDL_RenderPresent(renderer);
  return 0;
#endif
}

int RenderClear(SDL_Renderer *dst, Uint32 color) {
#if defined SDL && SDL == 1
  return RenderFillRect(dst, NULL, color);
#else
  SDL_SetRenderDrawColor(dst, color >> 16 & 255, color >> 8 & 255, color & 255,
                         color >> 24 & 255);
  return SDL_RenderClear(dst);
#endif
}

int BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Renderer *dst,
                SDL_Rect *dstrect) {
#if defined SDL && SDL == 1
  return SDL_BlitSurface(src, srcrect, dst, dstrect);
#else
  SDL_Texture *mTexture = SDL_CreateTextureFromSurface(dst, src);
  if (mTexture == NULL) {
    return -1;
  }
  int ret = SDL_RenderCopy(dst, mTexture, NULL, dstrect);
  SDL_DestroyTexture(mTexture);
  return ret;
#endif
};

SDL_Surface *RenderText_Blended(TTF_Font *font, const char *text,
                                Uint32 color) {
  return TTF_RenderText_Blended(font, text, uint32ToSDLColor(color));
}

SDL_Color uint32ToSDLColor(Uint32 color) {
  return (SDL_Color){color >> 16 & 255, color >> 8 & 255, color & 255,
                     color >> 24 & 255};
}
