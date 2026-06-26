# OnionOS / Miyoo Mini series SDL2 build settings.
#
# Usage:
#   make -f Makefile -f distributions/onionos/make/onionos.mk

CROSS_COMPILE ?= ~/buildroot/output/host/bin/arm-linux-gnueabihf-
CC=$(CROSS_COMPILE)gcc

SYSROOT ?= $(shell command -v $(CC) >/dev/null 2>&1 && $(CC) --print-sysroot || true)
ARCH ?= -marm -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7ve+simd
ONIONOS_SDL2_PREFIX ?= $(SYSROOT)/usr
ONIONOS_SDL2_INCLUDE_DIR ?= $(ONIONOS_SDL2_PREFIX)/include/SDL2
ONIONOS_SDL2_LIB_DIR ?= $(ONIONOS_SDL2_PREFIX)/lib

override SDL2_CFLAGS := -I$(ONIONOS_SDL2_INCLUDE_DIR) -D_REENTRANT
override SDL2_LIBS := -L$(ONIONOS_SDL2_LIB_DIR) -lSDL2 -lSDL2_ttf -lEGL -lGLESv2 -ljson-c -lfreetype -lpng16 -lz -lbz2

CFLAGS += -Os $(ARCH) -ffunction-sections -fdata-sections -Wall
LDFLAGS += $(ARCH) -Wl,-rpath,'$$ORIGIN/lib' -Wl,-rpath-link,$(ONIONOS_SDL2_LIB_DIR) -Wl,-rpath-link,$(ONIONOS_SDL2_PREFIX)/lib -Wl,-rpath-link,$(SYSROOT)/usr/lib
