#include "FlipClock.h"

FlipClock _G = {640, 480, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0};

void quit(int exitcode) {
  destroy();
  exit(exitcode);
}

int main() {
  int ret = EXIT_SUCCESS;
  signal(SIGTERM, quit);
  signal(SIGSEGV, quit);

  if (init() < 0) {
    ret = EXIT_FAILURE;
  }

  if (draw_loop() < 0) {
    ret = EXIT_FAILURE;
  }

  destroy();
  return ret;
}
