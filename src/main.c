#include "FlipClock.h"

FlipClock _G = {640, 480, NULL, NULL, NULL, 0, 0};

int main() {
  int ret = EXIT_SUCCESS;

  if (init() < 0 || draw_loop() < 0) {
    ret = EXIT_FAILURE;
  }

  destroy();
  return ret;
}
