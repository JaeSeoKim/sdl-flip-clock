#include "FlipClock.h"

void destroy() {
  printf("##Start destory\n");

  if (_G.renderer != NULL) {
    printf("\tdestorying renderer...\n");
#if defined SDL && SDL == 1
    SDL_FreeSurface(_G.renderer);
#else
    SDL_DestroyRenderer(_G.renderer);
#endif
  }

  if (_G.window != NULL) {
    printf("\tdestorying window...\n");
#if defined SDL && SDL == 1
    SDL_FreeSurface(_G.window);
#else
    SDL_DestroyWindow(_G.window);
#endif
  }

  if (_G.font != NULL) {
    printf("\tClosing font...\n");
    TTF_CloseFont(_G.font);
  }
  if (_G.smFont != NULL) {
    printf("\tClosing smFont...\n");
    TTF_CloseFont(_G.smFont);
  }

  TTF_Quit();
  SDL_Quit();
  printf("##Finish destory\n\n");
}
