gcc wav_io.c -o wav_io -lm # include -lm for linking with math.h
./wav_io 2500 audio.wav output.wav # 32767 for scale of 1