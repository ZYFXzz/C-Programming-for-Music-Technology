/* Port Audio Utilities */
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
PaStream *startupPa(int inputChanCount, int outputChanCount,
                    int samp_rate, int frames_per_buffer, PaStreamCallback *paCallback, void *data);

void shutdownPa(PaStream *stream);
