#!/bin/sh

set -eu

if [ "$#" -lt 2 ]; then
    exit 0
fi

hour="$1"
minute="$2"

case "$hour" in
    "" | *[!0-9]*)
        exit 1
        ;;
esac

case "$minute" in
    "" | *[!0-9]*)
        exit 1
        ;;
esac

case "$hour" in
    ?)
        hour="0$hour"
        ;;
esac

case "$minute" in
    ?)
        minute="0$minute"
        ;;
esac

if [ "$hour" -gt 23 ] || [ "$minute" -gt 59 ]; then
    exit 1
fi

date -s "$hour:$minute:00" > /dev/null 2>&1 || exit 1

if command -v hwclock > /dev/null 2>&1; then
    hwclock -w > /dev/null 2>&1 || true
fi
