#include "FlipClock.h"

void destroy() {
  printf("##Start destory\n");

  if (_G.renderer != NULL) {
    printf("\tdestorying renderer...\n");
    SDL_DestroyRenderer(_G.renderer);
  }

  if (_G.window != NULL) {
    printf("\tdestorying window...\n");
    SDL_DestroyWindow(_G.window);
  }

  if (_G.font != NULL) {
    printf("\tClosing font...\n");
    TTF_CloseFont(_G.font);
  }

  TTF_Quit();
  SDL_Quit();
  printf("##Finish destory\n\n");
}
