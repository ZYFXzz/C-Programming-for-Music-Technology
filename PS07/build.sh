#!/bin/sh

if [[ $(arch) == 'i386' ]]; then
  	echo Intel Mac
	IDIR="/usr/local/include"
	LDIR="/usr/local/lib"
elif [[ $(arch) == 'arm64' ]]; then
  	echo M1 Mac
	IDIR="/opt/homebrew/include"
	LDIR="/opt/homebrew/lib"
else
	echo Win PC
	IDIR="/mingw64/include"
	LDIR="/mingw64/lib"
fi

gcc -Wall -o play_wavfile_sfread play_wavfile_sfread.c paUtils.c \
	-I$IDIR -L$LDIR -lsndfile -lportaudio

gcc -Wall -o play_wavfile_ptr play_wavfile_ptr.c paUtils.c \
	-I$IDIR -L$LDIR -lsndfile -lportaudio


