#include "FlipClock.h"

int draw_loop() {
  _G.lastTickCount = SDL_GetTicks();
  _G.curTickCount = _G.lastTickCount;

  float fps = 0.0f;
  int frame = _G.curTickCount;
  int frameCount = 0;

  while (_G.quit == 0) {
    _G.curTickCount = SDL_GetTicks();
    pollevent();
    if (_G.quit != 0) {
      break;
    }

    handle_key();
    if (_G.quit != 0) {
      break;
    }

    int frameDelay = (int)(1000.0f / FRAME_RATE);
    int elapsed = _G.curTickCount - _G.lastTickCount;
    if (elapsed < frameDelay) {
      SDL_Delay(frameDelay - elapsed);
      continue;
    }

    // calculate FPS
    ++frameCount;
    if (_G.curTickCount - frame >= 1000) {
      fps = frameCount / ((_G.curTickCount - frame) / 1000.f);
      frameCount = 0;
      frame = _G.curTickCount;
    }

    if (RenderClear(_G.renderer, BG_COLOR) < 0) {
      printf("RenderClear failed: %s\n", SDL_GetError());
      return -1;
    }

    if (draw_screen(fps) < 0) {
      printf("draw_screen failed: %s\n", SDL_GetError());
      return -1;
    }

    RenderPresent(_G.renderer);
    _G.lastTickCount = _G.curTickCount;
  }

  return 0;
}

#define MENU_ITEM_COUNT 4
#define MENU_ITEM_SET_TIME 0
#define MENU_ITEM_TOGGLE_AMPM 1
#define MENU_ITEM_TOGGLE_FPS 2
#define MENU_ITEM_QUIT 3
#define MENU_EDIT_HOUR 0
#define MENU_EDIT_MINUTE 1

static void draw_time_edit_label();

static int handle_key_press(int keyIndex) {
  if (_G.KEY_STATUS[keyIndex] && _G.KEY_STATUS[keyIndex] <= _G.curTickCount) {
    _G.KEY_STATUS[keyIndex] = _G.curTickCount + INPUT_DELAY;
    return 1;
  }
  return 0;
}

static void toggle_ampm_mode() {
  if (is_active_mode(MODE_AMPM)) {
    _G.mode &= ~(1 << MODE_AMPM);
  } else {
    _G.mode |= 1 << MODE_AMPM;
  }
}

static void toggle_fps_mode() {
  if (is_active_mode(MODE_FPS)) {
    _G.mode &= ~(1 << MODE_FPS);
  } else {
    _G.mode |= 1 << MODE_FPS;
  }
}

static void execute_menu_item() {
  switch (_G.menuIndex) {
  case MENU_ITEM_SET_TIME:
    if (!_G.manualTimeEnabled) {
      struct tm *time = get_local_time();
      _G.manualHour = time->tm_hour;
      _G.manualMinute = time->tm_min;
    }
    _G.menuEditMode = 1;
    _G.menuEditField = MENU_EDIT_HOUR;
    _G.manualTimeEnabled = 1;
    _G.menuOpen = 0;
    break;
  case MENU_ITEM_TOGGLE_AMPM:
    toggle_ampm_mode();
    break;
  case MENU_ITEM_TOGGLE_FPS:
    toggle_fps_mode();
    break;
  case MENU_ITEM_QUIT:
    _G.quit = 1;
    break;
  }
}

static void run_time_set_hook() {
  if (_G.timeSetHook == NULL || _G.timeSetHook[0] == '\0') {
    return;
  }

  if (access(_G.timeSetHook, X_OK) != 0) {
    printf("time set hook is not executable: %s\n", _G.timeSetHook);
    return;
  }

  char hour[3];
  char minute[3];
  snprintf(hour, sizeof(hour), "%02d", _G.manualHour);
  snprintf(minute, sizeof(minute), "%02d", _G.manualMinute);

  pid_t pid = fork();
  if (pid == 0) {
    execl(_G.timeSetHook, _G.timeSetHook, hour, minute, (char *)NULL);
    _exit(127);
  }
  if (pid < 0) {
    printf("time set hook fork failed\n");
    return;
  }

  int status = 0;
  if (waitpid(pid, &status, 0) < 0 || !WIFEXITED(status) ||
      WEXITSTATUS(status) != 0) {
    printf("time set hook failed: %d\n", status);
  }
}

static void apply_time_edit() {
  _G.menuEditMode = 0;
  if (_G.manualTimeEnabled) {
    run_time_set_hook();
  }
}

int handle_key() {
  if (_G.menuEditMode) {
    if (handle_key_press(BTN_MENU_INDEX) || handle_key_press(BTN_START_INDEX)) {
      apply_time_edit();
      return 0;
    }
    if (handle_key_press(BTN_LEFT_INDEX) ||
        handle_key_press(BTN_RIGHT_INDEX)) {
      _G.menuEditField =
          _G.menuEditField == MENU_EDIT_HOUR ? MENU_EDIT_MINUTE
                                              : MENU_EDIT_HOUR;
    }
    if (handle_key_press(BTN_UP_INDEX)) {
      if (_G.menuEditField == MENU_EDIT_HOUR) {
        _G.manualHour = (_G.manualHour + 1) % 24;
      } else {
        _G.manualMinute = (_G.manualMinute + 1) % 60;
      }
    }
    if (handle_key_press(BTN_DOWN_INDEX)) {
      if (_G.menuEditField == MENU_EDIT_HOUR) {
        _G.manualHour = (_G.manualHour + 23) % 24;
      } else {
        _G.manualMinute = (_G.manualMinute + 59) % 60;
      }
    }
    if (handle_key_press(BTN_A_INDEX)) {
      struct tm *time = get_local_time();
      _G.manualHour = time->tm_hour;
      _G.manualMinute = time->tm_min;
      _G.manualTimeEnabled = 1;
    }
    return 0;
  }

  if (handle_key_press(BTN_MENU_INDEX)) {
    _G.menuOpen = !_G.menuOpen;
    return 0;
  }

  if (_G.menuOpen) {
    if (handle_key_press(BTN_UP_INDEX)) {
      _G.menuIndex = (_G.menuIndex + MENU_ITEM_COUNT - 1) % MENU_ITEM_COUNT;
    }
    if (handle_key_press(BTN_DOWN_INDEX)) {
      _G.menuIndex = (_G.menuIndex + 1) % MENU_ITEM_COUNT;
    }
    if (handle_key_press(BTN_A_INDEX) || handle_key_press(BTN_START_INDEX)) {
      execute_menu_item();
    }
    return 0;
  }

  return 0;
}

#define FLIPCLOCK_HOUR 0
#define FLIPCLOCK_MINUTE 1
#define FLIP_PI 3.14159265358979323846f

static float clamp_float(float value, float min, float max) {
  if (value < min) {
    return min;
  }
  if (value > max) {
    return max;
  }
  return value;
}

static float ease_in_cubic(float progress) {
  progress = clamp_float(progress, 0.0f, 1.0f);
  return progress * progress * progress;
}

static float smoothstep(float progress) {
  progress = clamp_float(progress, 0.0f, 1.0f);
  return progress * progress * (3.0f - 2.0f * progress);
}

static float damped_spring(float progress) {
  progress = clamp_float(progress, 0.0f, 1.0f);
  return 1.0f - expf(-7.5f * progress) * cosf(13.0f * progress);
}

static void render_translucent_rect(SDL_Rect rect, Uint32 color, Uint8 alpha) {
  if (_G.frameSurface == NULL || rect.w <= 0 || rect.h <= 0) {
    return;
  }

  SDL_Rect clip = {0, 0, _G.frameSurface->w, _G.frameSurface->h};
  if (!SDL_IntersectRect(&rect, &clip, &rect)) {
    return;
  }

  if (alpha == 255) {
    RenderFillRect(_G.renderer, &rect, color);
    return;
  }
  if (alpha == 0) {
    return;
  }

  SDL_Color sdlColor = uint32ToSDLColor(color);
  if (_G.frameSurface->format->format == SDL_PIXELFORMAT_RGB565) {
    Uint16 *pixels = (Uint16 *)_G.frameSurface->pixels;
    int pitch = _G.frameSurface->pitch / 2;
    int invAlpha = 255 - alpha;
    int srcR = sdlColor.r;
    int srcG = sdlColor.g;
    int srcB = sdlColor.b;

    for (int y = rect.y; y < rect.y + rect.h; ++y) {
      Uint16 *row = pixels + y * pitch;
      for (int x = rect.x; x < rect.x + rect.w; ++x) {
        Uint16 pixel = row[x];
        int dstR = ((pixel >> 11) & 0x1F) << 3;
        int dstG = ((pixel >> 5) & 0x3F) << 2;
        int dstB = (pixel & 0x1F) << 3;
        int outR = (srcR * alpha + dstR * invAlpha) / 255;
        int outG = (srcG * alpha + dstG * invAlpha) / 255;
        int outB = (srcB * alpha + dstB * invAlpha) / 255;
        row[x] = (Uint16)(((outR >> 3) << 11) | ((outG >> 2) << 5) |
                          (outB >> 3));
      }
    }
    return;
  }

  SDL_Surface *overlay = SDL_CreateRGBSurface(
      0, rect.w, rect.h, _G.frameSurface->format->BitsPerPixel,
      _G.frameSurface->format->Rmask, _G.frameSurface->format->Gmask,
      _G.frameSurface->format->Bmask, _G.frameSurface->format->Amask);
  if (overlay != NULL) {
    SDL_FillRect(overlay, NULL,
                 SDL_MapRGB(overlay->format, sdlColor.r, sdlColor.g,
                            sdlColor.b));
    SDL_SetSurfaceAlphaMod(overlay, alpha);
    SDL_SetSurfaceBlendMode(overlay, SDL_BLENDMODE_BLEND);
    SDL_BlitSurface(overlay, NULL, _G.frameSurface, &rect);
    SDL_FreeSurface(overlay);
  }
}

static void render_hinge_shadow(int x, int y, Uint8 alpha) {
  int halfHeight = _G.FLIP_SIZE / 2;
  int center = y + halfHeight;
  int radius = _G.DIVIDER;

  for (int offset = -radius; offset <= radius; ++offset) {
    int distance = offset < 0 ? -offset : offset;
    Uint8 lineAlpha = (Uint8)(alpha * (radius + 1 - distance) / (radius + 1));
    render_translucent_rect((SDL_Rect){x, center + offset, _G.FLIP_SIZE, 1},
                            0xFF000000, lineAlpha);
  }
}

static void render_drop_shadow(int x, int y, Uint8 alpha) {
  int bottom = y + _G.FLIP_SIZE;
  int width = _G.FLIP_SIZE - _G.FLIP_PADDING;

  render_translucent_rect((SDL_Rect){x + _G.FLIP_PADDING / 2, bottom, width, 1},
                          0xFF000000, alpha);
  render_translucent_rect((SDL_Rect){x + _G.FLIP_PADDING / 2 + 2, bottom + 1,
                                     width - 4, 1},
                          0xFF000000, alpha / 2);
  render_translucent_rect((SDL_Rect){x + _G.FLIP_PADDING / 2 + 4, bottom + 2,
                                     width - 8, 1},
                          0xFF000000, alpha / 4);
}

static void render_edit_focus(int x, int y) {
  int width = _G.FLIP_SIZE / 3;
  int height = _G.DIVIDER;
  int indicatorX = x + (_G.FLIP_SIZE - width) / 2;
  int indicatorY = y + _G.FLIP_SIZE + _G.DIVIDER * 2;
  RenderFillRect(_G.renderer,
                 &(SDL_Rect){indicatorX, indicatorY, width, height},
                 FONT_COLOR);
}

static void draw_menu_text(const char *text, int x, int y, Uint32 color) {
  SDL_Surface *textSurface = RenderText_Blended(_G.ampmFont, text, color);
  if (textSurface == NULL) {
    return;
  }
  BlitSurface(textSurface, NULL, _G.renderer,
              &(SDL_Rect){x, y, textSurface->w, textSurface->h});
  SDL_FreeSurface(textSurface);
}

static void draw_scaled_text(const char *text, int x, int y, int scale,
                             Uint32 color) {
  SDL_Surface *textSurface = RenderText_Blended(_G.ampmFont, text, color);
  if (textSurface == NULL) {
    return;
  }

  BlitSurface(textSurface, NULL, _G.renderer,
              &(SDL_Rect){x, y, textSurface->w * scale / 100,
                          textSurface->h * scale / 100});

  SDL_FreeSurface(textSurface);
}

static const char *get_menu_item_label(int index) {
  switch (index) {
  case MENU_ITEM_SET_TIME:
    return "Set Time";
  case MENU_ITEM_TOGGLE_AMPM:
    return is_active_mode(MODE_AMPM) ? "24-Hour Mode" : "12-Hour Mode";
  case MENU_ITEM_TOGGLE_FPS:
    return is_active_mode(MODE_FPS) ? "Hide FPS" : "Show FPS";
  case MENU_ITEM_QUIT:
    return "Exit";
  }
  return "";
}

static void get_menu_item_text(int index, char *buffer, int bufferSize) {
  if (index == MENU_ITEM_SET_TIME) {
    snprintf(buffer, bufferSize, "%s", get_menu_item_label(index));
    return;
  }
  snprintf(buffer, bufferSize, "%s", get_menu_item_label(index));
}

static void destroy_time_texture(TimeTexture *timeTexture) {
  if (timeTexture->surface != NULL) {
    SDL_FreeSurface(timeTexture->surface);
  }
  timeTexture->surface = NULL;
  timeTexture->value = -1;
  timeTexture->hour = -1;
  timeTexture->showAmpm = 0;
  timeTexture->w = 0;
  timeTexture->h = 0;
}

void destroy_card_textures() {
  for (int i = 0; i < 2; ++i) {
    destroy_time_texture(&_G.cards[i].currentTexture);
    destroy_time_texture(&_G.cards[i].nextTexture);
  }
}

static Uint32 map_surface_color(SDL_Surface *surface, Uint32 color) {
  SDL_Color sdlColor = uint32ToSDLColor(color);
  return SDL_MapRGBA(surface->format, sdlColor.r, sdlColor.g, sdlColor.b,
                     sdlColor.a);
}

static int surface_fill_rect(SDL_Surface *surface, SDL_Rect *rect,
                             Uint32 color) {
  return SDL_FillRect(surface, rect, map_surface_color(surface, color));
}

static void surface_fill_circle(SDL_Surface *surface, int cx, int cy,
                                int radius, Uint32 color) {
  if (radius <= 0) {
    return;
  }

  Uint32 mappedColor = map_surface_color(surface, color);
  const int diameter = radius * 2;
  int x = radius - 1;
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = tx - diameter;

  while (x >= y) {
    SDL_Rect rect = {cx - x, cy + y, x * 2, 1};
    SDL_FillRect(surface, &rect, mappedColor);
    rect = (SDL_Rect){cx - x, cy - y, x * 2, 1};
    SDL_FillRect(surface, &rect, mappedColor);
    rect = (SDL_Rect){cx - y, cy + x, y * 2, 1};
    SDL_FillRect(surface, &rect, mappedColor);
    rect = (SDL_Rect){cx - y, cy - x, y * 2, 1};
    SDL_FillRect(surface, &rect, mappedColor);

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }
    if (error > 0) {
      --x;
      tx += 2;
      error += tx - diameter;
    }
  }
}

static void surface_fill_rounded_rect(SDL_Surface *surface, int x, int y,
                                      int w, int h, int percent,
                                      Uint32 color) {
  int radius = calculateRadius(w, h, percent);
  SDL_Rect rect = {x + radius, y, w - radius * 2, h};
  surface_fill_rect(surface, &rect, color);
  rect = (SDL_Rect){x, y + radius, w, h - radius * 2};
  surface_fill_rect(surface, &rect, color);

  surface_fill_circle(surface, x + radius, y + radius, radius, color);
  surface_fill_circle(surface, x + w - radius, y + radius, radius, color);
  surface_fill_circle(surface, x + radius, y + h - radius, radius, color);
  surface_fill_circle(surface, x + w - radius, y + h - radius, radius, color);
}

static int surface_blit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst,
                        SDL_Rect *dstrect) {
  SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_BLEND);
  return SDL_BlitSurface(src, srcrect, dst, dstrect);
}

static int create_card_texture(TimeTexture *timeTexture, int value, int hour,
                               int showAmpm) {
  if (timeTexture->surface != NULL && timeTexture->value == value &&
      timeTexture->hour == hour && timeTexture->showAmpm == showAmpm) {
    return 0;
  }

  destroy_time_texture(timeTexture);

  SDL_Surface *cardSurface =
      SDL_CreateRGBSurface(0, _G.FLIP_SIZE, _G.FLIP_SIZE, 32, 0x000000FF,
                           0x0000FF00, 0x00FF0000, 0xFF000000);
  if (cardSurface == NULL) {
    printf("SDL_CreateRGBSurface Fail: %s\n", SDL_GetError());
    return -1;
  }

  surface_fill_rect(cardSurface, NULL, 0x00000000);
  surface_fill_rounded_rect(cardSurface, 0, 0, _G.FLIP_SIZE, _G.FLIP_SIZE, 10,
                            FLIP_COLOR);

  char timeString[3] = {
      value / 10 + '0',
      value % 10 + '0',
      '\0',
  };
  SDL_Surface *textSurface =
      RenderText_Blended(_G.timeFont, timeString, FONT_COLOR);
  if (textSurface == NULL) {
    printf("TTF_RenderText Fail: %s\n", TTF_GetError());
    SDL_FreeSurface(cardSurface);
    return -1;
  }
  SDL_Rect textDestination = {_G.FLIP_PADDING +
                                  (_G.TIME_SIZE - textSurface->w) / 2,
                              _G.FLIP_PADDING + _G.timeFontDescent,
                              textSurface->w, textSurface->h};
  surface_blit(textSurface, NULL, cardSurface, &textDestination);
  SDL_FreeSurface(textSurface);

  if (showAmpm) {
    int isAm = hour < 12;
    SDL_Surface *ampmSurface =
        RenderText_Blended(_G.ampmFont, isAm ? "AM" : "PM", FONT_COLOR);
    if (ampmSurface != NULL) {
      int ampmY = isAm ? _G.FLIP_PADDING / 2
                       : _G.FLIP_SIZE - _G.FLIP_PADDING / 2 - _G.AMPM_SIZE +
                             _G.ampmFontDescent;
      SDL_Rect ampmDestination = {_G.FLIP_PADDING / 2, ampmY, ampmSurface->w,
                                  ampmSurface->h};
      surface_blit(ampmSurface, NULL, cardSurface, &ampmDestination);
      SDL_FreeSurface(ampmSurface);
    }
  }

  SDL_Rect divider = {0, _G.FLIP_SIZE / 2 - _G.DIVIDER / 2, _G.FLIP_SIZE,
                      _G.DIVIDER};
  surface_fill_rect(cardSurface, &divider, BG_COLOR);

  timeTexture->surface = cardSurface;
  timeTexture->value = value;
  timeTexture->hour = hour;
  timeTexture->showAmpm = showAmpm;
  timeTexture->w = _G.FLIP_SIZE;
  timeTexture->h = _G.FLIP_SIZE;
  return 0;
}

static void start_card_animation(FlipCard *card, int value, int hour,
                                 int showAmpm) {
  if (card->currentValue < 0) {
    card->currentValue = value;
    create_card_texture(&card->currentTexture, value, hour, showAmpm);
    return;
  }

  if (card->animating && card->nextValue == value) {
    return;
  }

  if (card->currentValue == value && !card->animating) {
    create_card_texture(&card->currentTexture, value, hour, showAmpm);
    return;
  }

  if (card->animating) {
    destroy_time_texture(&card->currentTexture);
    card->currentTexture = card->nextTexture;
    card->nextTexture = (TimeTexture){NULL, -1, -1, 0, 0, 0};
    card->currentValue = card->nextValue;
  }

  card->nextValue = value;
  card->animationStartTick = _G.curTickCount;
  card->animating = 1;
  create_card_texture(&card->nextTexture, value, hour, showAmpm);
}

static void finish_card_animation(FlipCard *card) {
  destroy_time_texture(&card->currentTexture);
  card->currentTexture = card->nextTexture;
  card->nextTexture = (TimeTexture){NULL, -1, -1, 0, 0, 0};
  card->currentValue = card->nextValue;
  card->nextValue = -1;
  card->animating = 0;
}

static void render_card_half_shaded(TimeTexture *timeTexture, int x, int half,
                                    int destinationY,
                                    int destinationHeight, Uint8 shade) {
  int halfHeight = _G.FLIP_SIZE / 2;
  SDL_Rect source = {0, half == 0 ? 0 : halfHeight, _G.FLIP_SIZE, halfHeight};
  SDL_Rect destination = {x, destinationY, _G.FLIP_SIZE, destinationHeight};

  if (destination.h <= 0 || timeTexture->surface == NULL) {
    return;
  }

  if (shade == 0) {
    BlitSurfaceToSurface(timeTexture->surface, &source, _G.frameSurface,
                         &destination);
    return;
  }

  Uint8 color = 255 - shade;
  SDL_SetSurfaceColorMod(timeTexture->surface, color, color, color);
  BlitSurfaceToSurface(timeTexture->surface, &source, _G.frameSurface,
                       &destination);
  SDL_SetSurfaceColorMod(timeTexture->surface, 255, 255, 255);
}

static void render_card_half(TimeTexture *timeTexture, int x, int half,
                             int destinationY, int destinationHeight) {
  render_card_half_shaded(timeTexture, x, half, destinationY, destinationHeight,
                          0);
}

static void render_static_card(TimeTexture *timeTexture, int x, int y) {
  if (timeTexture->surface == NULL) {
    return;
  }
  render_drop_shadow(x, y, 42);
  BlitSurfaceToSurface(timeTexture->surface, NULL, _G.frameSurface,
                       &(SDL_Rect){x, y, _G.FLIP_SIZE, _G.FLIP_SIZE});
}

static void render_animated_card(FlipCard *card, int x, int y) {
  int halfHeight = _G.FLIP_SIZE / 2;
  float progress =
      (float)(_G.curTickCount - card->animationStartTick) / FLIP_ANIMATION_MS;
  progress = clamp_float(progress, 0.0f, 1.0f);

  if (progress >= 1.0f) {
    finish_card_animation(card);
    render_static_card(&card->currentTexture, x, y);
    return;
  }

  if (progress < 0.46f) {
    float phase = progress / 0.46f;
    float angle = (FLIP_PI / 2.0f) * smoothstep(phase);
    float projection = cosf(angle);
    int foldHeight = (int)(halfHeight * projection);
    Uint8 shade = (Uint8)(32 + 96 * ease_in_cubic(phase));
    Uint8 hingeAlpha = (Uint8)(40 + 90 * phase);

    render_card_half(&card->nextTexture, x, 0, y, halfHeight);
    render_card_half(&card->currentTexture, x, 1, y + halfHeight, halfHeight);
    render_card_half_shaded(&card->currentTexture, x, 0,
                            y + halfHeight - foldHeight, foldHeight, shade);

    render_hinge_shadow(x, y, hingeAlpha);
  } else {
    float phase = (progress - 0.46f) / 0.54f;
    float spring = damped_spring(phase);
    float projection = clamp_float(spring, 0.0f, 1.0f);
    int foldHeight = (int)(halfHeight * projection);
    Uint8 shade = (Uint8)(88 * (1.0f - projection));
    Uint8 hingeAlpha = (Uint8)(95 * (1.0f - phase));

    render_card_half(&card->nextTexture, x, 0, y, halfHeight);
    render_card_half(&card->currentTexture, x, 1, y + halfHeight, halfHeight);
    render_card_half_shaded(&card->nextTexture, x, 1, y + halfHeight,
                            foldHeight, shade);

    render_hinge_shadow(x, y, hingeAlpha);
  }

  render_drop_shadow(x, y, 54);
  RenderFillRect(_G.renderer,
                 &(SDL_Rect){x, y + halfHeight - _G.DIVIDER / 2, _G.FLIP_SIZE,
                             _G.DIVIDER},
                 BG_COLOR);
}

int draw_screen(float fps) {
  struct tm *time = get_local_time();
  int hour = _G.manualTimeEnabled ? _G.manualHour : time->tm_hour;
  int minute = _G.manualTimeEnabled ? _G.manualMinute : time->tm_min;

  int x = _G.MARGIN_X;
  int y = (_G.DISPLAY_HEIGHT - _G.FLIP_SIZE) / 2;
  int cardX[2] = {0, 0};

  for (int layout = 0; layout < 2; ++layout) {
    cardX[layout] = x;
    int display_time_number = layout == FLIPCLOCK_HOUR ? hour : minute;

    if (layout == FLIPCLOCK_HOUR && is_active_mode(MODE_AMPM)) {
      display_time_number %= 12;
      if (display_time_number == 0) {
        display_time_number = 12;
      }
    }

    draw_card(x, y, layout, display_time_number, hour,
              layout == FLIPCLOCK_HOUR && is_active_mode(MODE_AMPM));

    // Update Position
    x += _G.GAP + _G.FLIP_SIZE;
  }

  if (_G.menuEditMode) {
    render_edit_focus(cardX[_G.menuEditField == MENU_EDIT_HOUR
                                ? FLIPCLOCK_HOUR
                                : FLIPCLOCK_MINUTE],
                      y);
  }

  draw_divider();

  if (is_active_mode(MODE_FPS)) {
    draw_fps(fps);
  }

  if (_G.menuEditMode) {
    draw_time_edit_label();
  }

  if (_G.menuOpen) {
    draw_menu();
  }

  return 0;
}

struct tm *get_local_time() {
  time_t ltime;
  ltime = time(&ltime);
  return localtime(&ltime);
}

void draw_flip(int x, int y) {
  RenderFillRoundedRect(_G.renderer, x, y, _G.FLIP_SIZE, _G.FLIP_SIZE, 10,
                        FLIP_COLOR);
}

void draw_card(int x, int y, int cardIndex, int time, int hour, int showAmpm) {
  FlipCard *card = &_G.cards[cardIndex];
  start_card_animation(card, time, hour, showAmpm);

  if (card->animating) {
    render_animated_card(card, x, y);
    return;
  }
  render_static_card(&card->currentTexture, x, y);
}

void draw_divider() {
  RenderFillRect(_G.renderer,
                 &(SDL_Rect){0, _G.DISPLAY_HEIGHT / 2 - _G.DIVIDER,
                             _G.DISPLAY_WIDTH, _G.DIVIDER},
                 BG_COLOR);
}

void draw_fps(float fps) {
  char fpsText[16];
  snprintf(fpsText, sizeof(fpsText), "%.1f FPS", fps);

  if (_G.fpsSurface == NULL || strcmp(_G.fpsText, fpsText) != 0) {
    if (_G.fpsSurface != NULL) {
      SDL_FreeSurface(_G.fpsSurface);
      _G.fpsSurface = NULL;
    }
    snprintf(_G.fpsText, sizeof(_G.fpsText), "%s", fpsText);
    _G.fpsSurface = RenderText_Blended(_G.ampmFont, _G.fpsText, FONT_COLOR);
  }

  if (_G.fpsSurface == NULL) {
    return;
  }

  BlitSurface(_G.fpsSurface, NULL, _G.renderer,
              &(SDL_Rect){_G.DISPLAY_WIDTH - _G.fpsSurface->w * 7 / 10 - 6, 4,
                          _G.fpsSurface->w * 7 / 10,
                          _G.fpsSurface->h * 7 / 10});
}

static void draw_time_edit_label() {
  draw_scaled_text("Set Time", 6, 4, 60, FONT_COLOR);
}

void draw_menu() {
  int maxTextWidth = 0;
  int textHeight = 0;
  int titleWidth = 0;
  int titleHeight = 0;

  TTF_SizeText(_G.ampmFont, "Menu", &titleWidth, &titleHeight);
  maxTextWidth = titleWidth;
  textHeight = titleHeight;
  for (int i = 0; i < MENU_ITEM_COUNT; ++i) {
    char itemText[64];
    int textWidth = 0;
    int itemTextHeight = 0;
    get_menu_item_text(i, itemText, sizeof(itemText));
    TTF_SizeText(_G.ampmFont, itemText, &textWidth, &itemTextHeight);
    if (textWidth > maxTextWidth) {
      maxTextWidth = textWidth;
    }
    if (itemTextHeight > textHeight) {
      textHeight = itemTextHeight;
    }
  }

  int horizontalPadding = 36;
  int menuWidth = maxTextWidth + horizontalPadding * 2;
  int headerHeight = textHeight + 20;
  int maxMenuWidth = _G.DISPLAY_WIDTH - 24;
  if (menuWidth > maxMenuWidth) {
    menuWidth = maxMenuWidth;
  }
  if (menuWidth < _G.DISPLAY_WIDTH / 3) {
    menuWidth = _G.DISPLAY_WIDTH / 3;
  }

  int itemHeight = textHeight + 12;
  int menuHeight = headerHeight + itemHeight * MENU_ITEM_COUNT + 14;
  int maxMenuHeight = _G.DISPLAY_HEIGHT - 24;
  if (menuHeight > maxMenuHeight) {
    itemHeight = (maxMenuHeight - headerHeight - 14) / MENU_ITEM_COUNT;
    menuHeight = headerHeight + itemHeight * MENU_ITEM_COUNT + 14;
  }

  int x = (_G.DISPLAY_WIDTH - menuWidth) / 2;
  int y = (_G.DISPLAY_HEIGHT - menuHeight) / 2;

  render_translucent_rect((SDL_Rect){x, y, menuWidth, menuHeight}, 0xFF000000,
                          220);
  RenderFillRect(_G.renderer,
                 &(SDL_Rect){x, y + headerHeight - 1, menuWidth, 1},
                 FLIP_COLOR);

  draw_menu_text("Menu", x + horizontalPadding, y + 10, 0xFFFFFFFF);

  for (int i = 0; i < MENU_ITEM_COUNT; ++i) {
    char itemText[64];
    int itemY = y + headerHeight + itemHeight * i + 8;
    Uint32 textColor = FONT_COLOR;
    get_menu_item_text(i, itemText, sizeof(itemText));

    if (i == _G.menuIndex) {
      RenderFillRect(_G.renderer,
                     &(SDL_Rect){x + 8, itemY - 5, menuWidth - 16,
                                 itemHeight - 3},
                     FLIP_COLOR);
      RenderFillRect(_G.renderer,
                     &(SDL_Rect){x + 14, itemY - 1, 4, itemHeight - 11},
                     FONT_COLOR);
      textColor = 0xFFFFFFFF;
    }

    draw_menu_text(itemText, x + horizontalPadding, itemY, textColor);
  }
}
