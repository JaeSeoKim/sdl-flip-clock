#!/bin/sh
set -eu

IMAGE_NAME="${IMAGE_NAME:-sdl-flip-clock-onionos-builder}"
PROJECT_ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../../.." && pwd)"

docker build \
    -f "$PROJECT_ROOT/distributions/onionos/Dockerfile" \
    -t "$IMAGE_NAME" \
    "$PROJECT_ROOT"

docker run --rm \
    --user "$(id -u):$(id -g)" \
    -v "$PROJECT_ROOT:/workspace" \
    -w /workspace \
    "$IMAGE_NAME" \
    ./distributions/onionos/scripts/build-in-docker.sh
