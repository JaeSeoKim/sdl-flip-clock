#!/bin/sh
cd /mnt/SDCARD/App/FlipClock/

if pgrep flipClock > /dev/null; then
    killall -9 flipClock
fi

/mnt/SDCARD/App/FlipClock/flipClock
