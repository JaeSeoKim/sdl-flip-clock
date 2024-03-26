#include "FlipClock.h"

void destroy() {
  printf("##Start destory\n");

  if (access("/tmp/stay_awake", F_OK) != -1) {
    printf("\tStop stay awake\n");
    remove("/tmp/stay_awake");
  }

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

  if (_G.timeFont != NULL) {
    printf("\tClosing font...\n");
    TTF_CloseFont(_G.timeFont);
  }
  if (_G.ampmFont != NULL) {
    printf("\tClosing smFont...\n");
    TTF_CloseFont(_G.ampmFont);
  }

  TTF_Quit();
  SDL_Quit();
  printf("##Finish destory\n\n");
}
