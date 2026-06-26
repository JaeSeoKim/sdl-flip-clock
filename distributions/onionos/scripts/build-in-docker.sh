#!/bin/sh
set -eu

if [ -f /opt/onionos/setup-env.sh ]; then
    . /opt/onionos/setup-env.sh
elif [ -f /root/setup-env.sh ]; then
    . /root/setup-env.sh
fi

CROSS_COMPILE="${CROSS_COMPILE:-/opt/miyoomini-toolchain/usr/bin/arm-linux-gnueabihf-}"
APP_NAME="${APP_NAME:-FlipClock}"
VERSION="$(sed -n 's/.*"version"[[:space:]]*:[[:space:]]*"\([^"]*\)".*/\1/p' config.json)"
if [ -z "$VERSION" ]; then
    VERSION=0.0.0
fi
PREFIX="${PREFIX:-/opt/miyoomini-toolchain/usr/arm-linux-gnueabihf/sysroot/usr}"
TOOLCHAIN_RUNTIME_LIB_DIR="${TOOLCHAIN_RUNTIME_LIB_DIR:-/opt/miyoomini-toolchain/arm-linux-gnueabihf/lib}"
TOOLCHAIN_SYSROOT_LIB_DIR="${TOOLCHAIN_SYSROOT_LIB_DIR:-/opt/miyoomini-toolchain/arm-linux-gnueabihf/libc/lib}"
ONIONOS_SDL2_PREFIX="${ONIONOS_SDL2_PREFIX:-/opt/onionos-sdl2}"
SDL2_INCLUDE_DIR="${SDL2_INCLUDE_DIR:-$ONIONOS_SDL2_PREFIX/include/SDL2}"
SDL2_LIB_DIR="${SDL2_LIB_DIR:-$ONIONOS_SDL2_PREFIX/lib}"
PACKAGE_LIB_DIR="output/package/onionos/FlipClock/lib"
TARGET_BINARY="output/build/onionos/binary/flipClock"

make clean-build BUILD_PROFILE=onionos
make -C distributions/onionos clean
make -f Makefile -f distributions/onionos/make/onionos.mk \
    BUILD_PROFILE=onionos \
    CROSS_COMPILE="$CROSS_COMPILE" \
    ONIONOS_SDL2_INCLUDE_DIR="$SDL2_INCLUDE_DIR" \
    ONIONOS_SDL2_LIB_DIR="$SDL2_LIB_DIR"

make -C distributions/onionos stage

mkdir -p "$PACKAGE_LIB_DIR"

find_library() {
    library_name="$1"

    for directory in "$SDL2_LIB_DIR" "$ONIONOS_SDL2_PREFIX/lib" "$PREFIX/lib" "$TOOLCHAIN_RUNTIME_LIB_DIR" "$TOOLCHAIN_SYSROOT_LIB_DIR" /lib/arm-linux-gnueabihf /usr/lib/arm-linux-gnueabihf; do
        if [ -e "$directory/$library_name" ]; then
            printf '%s\n' "$directory/$library_name"
            return 0
        fi
    done

    found_path="$(find "$SDL2_LIB_DIR" "$ONIONOS_SDL2_PREFIX/lib" "$PREFIX/lib" "$TOOLCHAIN_RUNTIME_LIB_DIR" "$TOOLCHAIN_SYSROOT_LIB_DIR" /lib/arm-linux-gnueabihf /usr/lib/arm-linux-gnueabihf \
        -maxdepth 2 -name "$library_name" -print -quit 2>/dev/null || true)"
    if [ -n "$found_path" ]; then
        printf '%s\n' "$found_path"
        return 0
    fi

    return 1
}

copy_needed_libraries() {
    input_file="$1"

    "${CROSS_COMPILE}readelf" -d "$input_file" \
        | sed -n 's/.*Shared library: \[\(.*\)\].*/\1/p' \
        | while IFS= read -r library_name; do
            case "$library_name" in
                ld-linux*.so*|libc.so*|libdl.so*|libgcc_s.so*|libm.so*|libpthread.so*|librt.so*)
                    continue
                    ;;
            esac

            if [ -e "$PACKAGE_LIB_DIR/$library_name" ]; then
                continue
            fi

            if ! library_path="$(find_library "$library_name")"; then
                printf 'Warning: could not find runtime library %s\n' "$library_name" >&2
                continue
            fi

            cp -L "$library_path" "$PACKAGE_LIB_DIR/$library_name"
            copy_needed_libraries "$library_path"
        done
}

copy_needed_libraries "$TARGET_BINARY"

for library_name in libEGL.so libGLESv2.so libjson-c.so.5 libpng16.so.16 libz.so.1 libbz2.so.1.0 libstdc++.so.6; do
    if [ -e "$SDL2_LIB_DIR/$library_name" ] && [ ! -e "$PACKAGE_LIB_DIR/$library_name" ]; then
        cp -L "$SDL2_LIB_DIR/$library_name" "$PACKAGE_LIB_DIR/$library_name"
    fi
done

make -C distributions/onionos zip

printf 'Created %s\n' "output/distributions/onionos/${APP_NAME}-v${VERSION}.zip"
