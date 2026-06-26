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
  if (surface == NULL) {
    return;
  }
  SDL_FillRect(surface, NULL, 0);

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
  (void)dst;
  if (_G.frameSurface == NULL) {
    return -1;
  }
  SDL_Color sdlColor = uint32ToSDLColor(color);
  Uint32 mapped = SDL_MapRGBA(_G.frameSurface->format, sdlColor.r, sdlColor.g,
                              sdlColor.b, sdlColor.a);
  return SDL_FillRect(_G.frameSurface, rect, mapped);
}

int RenderPresent(SDL_Renderer *renderer) {
  if (_G.frameSurface != NULL && _G.frameTexture != NULL) {
    SDL_UpdateTexture(_G.frameTexture, NULL, _G.frameSurface->pixels,
                      _G.frameSurface->pitch);
    SDL_RenderCopy(renderer, _G.frameTexture, NULL, NULL);
  }
  SDL_RenderPresent(renderer);
  return 0;
}

int RenderClear(SDL_Renderer *dst, Uint32 color) {
  (void)dst;
  return RenderFillRect(dst, NULL, color);
}

int BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Renderer *dst,
                SDL_Rect *dstrect) {
  (void)dst;
  if (_G.frameSurface == NULL) {
    return -1;
  }
  return BlitSurfaceToSurface(src, srcrect, _G.frameSurface, dstrect);
};

int BlitSurfaceToSurface(SDL_Surface *src, SDL_Rect *srcrect,
                         SDL_Surface *dst, SDL_Rect *dstrect) {
  if (src == NULL || dst == NULL) {
    return -1;
  }
  SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_BLEND);
  if (dstrect != NULL) {
    int sourceW = srcrect != NULL ? srcrect->w : src->w;
    int sourceH = srcrect != NULL ? srcrect->h : src->h;
    if (dstrect->w == sourceW && dstrect->h == sourceH) {
      return SDL_BlitSurface(src, srcrect, dst, dstrect);
    }
    return SDL_BlitScaled(src, srcrect, dst, dstrect);
  }
  return SDL_BlitSurface(src, srcrect, dst, dstrect);
}

SDL_Surface *RenderText_Blended(TTF_Font *font, const char *text,
                                Uint32 color) {
  return TTF_RenderText_Blended(font, text, uint32ToSDLColor(color));
}

SDL_Color uint32ToSDLColor(Uint32 color) {
  return (SDL_Color){color >> 16 & 255, color >> 8 & 255, color & 255,
                     color >> 24 & 255};
}
