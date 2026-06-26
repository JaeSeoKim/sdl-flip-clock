#! /bin/sh
set -eu

SDL_PREFIX="${ONIONOS_SDL2_PREFIX:-/opt/onionos-sdl2}"
BUILD_ROOT=/tmp/onionos-sdl2-build
STEWARD_SDL2_REF="${STEWARD_SDL2_REF:-master}"
STEWARD_SDL2_URL="https://github.com/steward-fu/sdl2/archive/refs/heads/${STEWARD_SDL2_REF}.tar.gz"
STEWARD_SDL2_TAR="steward-fu-sdl2-${STEWARD_SDL2_REF}.tar.gz"
STEWARD_SDL2_DIR="$BUILD_ROOT/sdl2-${STEWARD_SDL2_REF}"

mkdir -p "$BUILD_ROOT" "$SDL_PREFIX/include/SDL2" "$SDL_PREFIX/lib"
cd "$BUILD_ROOT"

if [ ! -d "$STEWARD_SDL2_DIR" ]; then
    wget -O "$STEWARD_SDL2_TAR" "$STEWARD_SDL2_URL"
    tar xf "$STEWARD_SDL2_TAR"
fi

cp "$STEWARD_SDL2_DIR"/sdl2/include/*.h "$SDL_PREFIX/include/SDL2/"
cp "$STEWARD_SDL2_DIR"/android/app/jni/SDL2_ttf/SDL_ttf.h "$SDL_PREFIX/include/SDL2/"

cp "$STEWARD_SDL2_DIR"/prebuilt/640x480/libSDL2-2.0.so.0 "$SDL_PREFIX/lib/"
cp "$STEWARD_SDL2_DIR"/prebuilt/640x480/libEGL.so "$SDL_PREFIX/lib/"
cp "$STEWARD_SDL2_DIR"/prebuilt/640x480/libGLESv2.so "$SDL_PREFIX/lib/"
cp "$STEWARD_SDL2_DIR"/examples/libSDL2_ttf-2.0.so.0 "$SDL_PREFIX/lib/"
cp "$STEWARD_SDL2_DIR"/examples/libjson-c.so.5 "$SDL_PREFIX/lib/"
cp "$STEWARD_SDL2_DIR"/examples/libpng16.so.16 "$SDL_PREFIX/lib/"
cp "$STEWARD_SDL2_DIR"/examples/libz.so.1 "$SDL_PREFIX/lib/"

if [ -f /opt/miyoomini-toolchain/arm-linux-gnueabihf/libc/usr/lib/libbz2.so.1.0.6 ]; then
    cp /opt/miyoomini-toolchain/arm-linux-gnueabihf/libc/usr/lib/libbz2.so.1.0.6 "$SDL_PREFIX/lib/"
fi

ln -sf libSDL2-2.0.so.0 "$SDL_PREFIX/lib/libSDL2.so"
ln -sf libSDL2_ttf-2.0.so.0 "$SDL_PREFIX/lib/libSDL2_ttf.so"
ln -sf libjson-c.so.5 "$SDL_PREFIX/lib/libjson-c.so"
ln -sf libpng16.so.16 "$SDL_PREFIX/lib/libpng16.so"
ln -sf libz.so.1 "$SDL_PREFIX/lib/libz.so"

if [ -f "$SDL_PREFIX/lib/libbz2.so.1.0.6" ]; then
    ln -sf libbz2.so.1.0.6 "$SDL_PREFIX/lib/libbz2.so.1.0"
    ln -sf libbz2.so.1.0.6 "$SDL_PREFIX/lib/libbz2.so"
fi
