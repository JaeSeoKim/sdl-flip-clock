#include "FlipClock.h"

FlipClock _G = {
    .DISPLAY_WIDTH = 640,
    .DISPLAY_HEIGHT = 480,
};

void quit(int exitcode) {
  destroy();
  exit(exitcode);
}

static void print_usage(const char *program) {
  printf("Usage: %s [--set-time-hook PATH]\n", program);
}

static int parse_args(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--set-time-hook") == 0) {
      if (i + 1 >= argc) {
        print_usage(argv[0]);
        return -1;
      }
      _G.timeSetHook = argv[++i];
      continue;
    }

    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      print_usage(argv[0]);
      return 1;
    }

    printf("Unknown option: %s\n", argv[i]);
    print_usage(argv[0]);
    return -1;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  int ret = EXIT_SUCCESS;
  signal(SIGTERM, quit);
  signal(SIGINT, quit);

  int parseResult = parse_args(argc, argv);
  if (parseResult != 0) {
    return parseResult > 0 ? EXIT_SUCCESS : EXIT_FAILURE;
  }

  if (init() < 0) {
    printf("init failed\n");
    destroy();
    return EXIT_FAILURE;
  }

  if (draw_loop() < 0) {
    printf("draw_loop failed\n");
    ret = EXIT_FAILURE;
  }

  destroy();
  return ret;
}
