#!/usr/bin/env bash

for i in {1..31}
do
    if [ "$i" -ne 9 ] && [ "$i" -ne 17 ] && [ "$i" -ne 19 ] && [ "$i" -ne 23 ]; then
        kill "-$i" "$1"
        sleep 1
    fi
done
