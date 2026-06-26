#! /bin/sh
set -eu

TOOLCHAIN_VERSION=v0.0.3
TOOLCHAIN_TAR="miyoomini-toolchain.tar.xz"

TOOLCHAIN_ARCH="$(uname -m)"
if [ "$TOOLCHAIN_ARCH" = "aarch64" ]; then
    TOOLCHAIN_REPO=miyoomini-toolchain-buildroot-aarch64
else
    TOOLCHAIN_REPO=miyoomini-toolchain-buildroot
fi

TOOLCHAIN_URL="https://github.com/shauninman/$TOOLCHAIN_REPO/releases/download/$TOOLCHAIN_VERSION/$TOOLCHAIN_TAR"

cd /opt
if [ -f "/root/$TOOLCHAIN_TAR" ]; then
    cp "/root/$TOOLCHAIN_TAR" /opt
    echo "extracting local toolchain ($TOOLCHAIN_ARCH)"
else
    wget "$TOOLCHAIN_URL"
    echo "extracting remote toolchain $TOOLCHAIN_VERSION ($TOOLCHAIN_ARCH)"
fi

tar xf "./$TOOLCHAIN_TAR"
rm -f "./$TOOLCHAIN_TAR"
