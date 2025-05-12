#!/bin/sh
gcc -Wall -o synth_record main.c synth.c key_tables.c paUtils.c \
    -DNCURSES_STATIC \
    -I/msys64/mingw64/include/ncurses \
    -L/usr/local/lib -lsndfile -lportaudio -lncurses

