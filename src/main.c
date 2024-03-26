#include "FlipClock.h"

FlipClock _G = {
    640,  // DISPLAY_WIDTH
    480,  // DISPLAY_HEIGHT
    NULL, // Fill the remaining fields with NULL
};

void quit(int exitcode) {
  destroy();
  exit(exitcode);
}

int main() {
  int ret = EXIT_SUCCESS;
  signal(SIGTERM, quit);
  signal(SIGSEGV, quit);

  if (init() < 0) {
    printf("init failed\n");
    ret = EXIT_FAILURE;
  }

  if (draw_loop() < 0) {
    printf("draw_lopp failed\n");
    ret = EXIT_FAILURE;
  }

  destroy();
  return ret;
}
