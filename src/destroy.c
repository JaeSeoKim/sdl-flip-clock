#include "FlipClock.h"

void destroy() {
  destroy_card_textures();

  if (_G.frameTexture != NULL) {
    SDL_DestroyTexture(_G.frameTexture);
  }

  if (_G.frameSurface != NULL) {
    SDL_FreeSurface(_G.frameSurface);
  }

  if (_G.renderer != NULL) {
    SDL_DestroyRenderer(_G.renderer);
  }

  if (_G.window != NULL) {
    SDL_DestroyWindow(_G.window);
  }

  if (_G.timeFont != NULL) {
    TTF_CloseFont(_G.timeFont);
  }
  if (_G.ampmFont != NULL) {
    TTF_CloseFont(_G.ampmFont);
  }

  if (_G.fpsSurface != NULL) {
    SDL_FreeSurface(_G.fpsSurface);
  }

  TTF_Quit();
  SDL_Quit();
}
