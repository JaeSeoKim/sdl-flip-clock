#!/bin/sh
cd "$(dirname "$0")"
export LD_LIBRARY_PATH="$PWD/lib:$PWD:/config/lib:/customer/lib:${LD_LIBRARY_PATH:-}"

STAY_AWAKE_FILE=/tmp/stay_awake

cleanup() {
    rm -f "$STAY_AWAKE_FILE"
}

trap cleanup EXIT INT TERM

if pgrep flipClock > /dev/null; then
    killall -9 flipClock
fi

touch "$STAY_AWAKE_FILE"
./flipClock --set-time-hook ./set-time.sh
