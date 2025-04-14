/*****************************************************************************
 * play_wavfile.c
 *
 * Plays a WAV file to speaker using PortAudio
 * Uses pointers to Wav buffer in callback
 *
 *****************************************************************************/

#include <ctype.h>     /* lolower() */
#include <math.h>      /* sin() */
#include <portaudio.h> /* portaudio */
#include <sndfile.h>   /* libsndfile */
#include <stdbool.h>   /* true, false */
#include <stdio.h>
#include <stdlib.h> /* malloc() */
#include <string.h> /* memset() */
#include <unistd.h> /* sleep() */
#if (__APPLE__)
#include <stdatomic.h> /* permits write/read of "slection" to be atomic */
#endif
#include "paUtils.h"

#define MAX_CHN 2
#define LINE_LEN 80
#define FRAMES_PER_BUFFER 1024

/* data structure to pass to callback */
typedef struct {
#if (__APPLE__)
    atomic_int play; /* so play variable is thread-safe */
#else
    int play;
#endif
    unsigned int channels;
    float *top;
    float *next;
    float *bottom;
} Buf;

/* PortAudio callback function protoype */
static int paCallback(const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo *timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData);

int main(int argc, char *argv[]) {
    char *ifile;
    /* my data structure */
    Buf buf, *p = &buf;
    /* libsndfile structures */
    SNDFILE *sndfile;
    SF_INFO sfinfo;
    /* PortAudio stream */
    PaStream *stream;

    /* zero libsndfile structures */
    memset(&sfinfo, 0, sizeof(sfinfo));

    /*
     * Parse command line
     */
    // Your code here
    if (argc != 2) {
        fprintf(stderr, "Usage: program name + wavfile.wav\n");
        return 1;
    }
    ifile = argv[1];

    /* Open WAV file */
    /* Print information about WAV file */
    // Your code here
    sndfile = sf_open(ifile, SFM_READ, &sfinfo);
    if (sndfile == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", ifile);
        fprintf(stderr, "%s\n", sf_strerror(NULL));
        return 1;
    }

    printf("File: %s\n", ifile);
    printf("Sample rate: %d Hz\n", sfinfo.samplerate);
    printf("Channels: %d\n", sfinfo.channels);
    printf("Format: 0x%08X\n", sfinfo.format);
    printf("Frames: %ld\n", (long)sfinfo.frames);
    /* If number of channels > MAX_CHN, exit */
    // Your code here
    if (sfinfo.channels > MAX_CHN) {
        fprintf(stderr, "Error: File has %d channels, maximum supported is %d\n",
                sfinfo.channels, MAX_CHN);
        sf_close(sndfile);
        return 1;
    }

    /* malloc storage and read audio data into buffer
     * allocate storage to p->bottom
     */
    // Your code here
    p->bottom = (float *)malloc(sfinfo.frames * sfinfo.channels * sizeof(float));
    if (p->bottom == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for audio data\n");
        sf_close(sndfile);
        return 1;
    }

    sf_count_t numFrames = sf_readf_float(sndfile, p->bottom, sfinfo.frames);
    if (numFrames != sfinfo.frames) {
        fprintf(stderr, "Error: Could not read all audio data from file\n");
        free(p->bottom);
        sf_close(sndfile);
        return 1;
    }

    /* Initialize data structure
     * Set play to 0 so initially only "zeros" are played
     */
    p->play = 0;
    /* Set channels */
    p->channels = sfinfo.channels;
    /* initialize pointers in WAV data struct
     * bottom already points to start of WAV data
     * next points to next frame to play out
     * top points to last frame in buffer
     */
    p->next = p->bottom;
    p->top = &p->bottom[(sfinfo.frames - 1) * sfinfo.channels];

    /* Close WAV file */
    // Your code here
    sf_close(sndfile);

    /* start up Port Audio */
    stream = startupPa(1, sfinfo.channels, sfinfo.samplerate,
                       FRAMES_PER_BUFFER, paCallback, &buf);

    /* User Input */
    while (1) {
        char line[80];
        fgets(line, 80, stdin);
        if (line[0] == 'Q')
            break;
        else
            p->play = (p->play == 0) ? 1 : 0;
    }

    /* shut down Port Audio */
    shutdownPa(stream);

    /* free storage */
    free(p->bottom);

    return 0;
}

/* This routine will be called by the PortAudio engine when audio is needed.
 * It will be called in the "real-time" thread, so don't do anything
 * in the routine that requires significant time or resources.
 */
static int paCallback(const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo *timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData) {
    Buf *p = (Buf *)userData; /* Cast data passed through stream to our structure. */
    // float *input = (float *)inputBuffer; /* input not used in this code */
    float *po = (float *)outputBuffer;
    int samplesPerBuffer = framesPerBuffer * p->channels; /* number or samples in buffer */
    int play = p->play;

    /* if play is 0, then fill output buffer with zeros
     * otherwise, copy from WAV data buffer into callback output buffer
     * if not enough samples (i.e. p->next > p->top), then reset
     * pointer p->next to start of buffer and copy remaining samples into output buffer
     * if using local pointers, update p->next before exiting
     */
    // Your code here
    if (play == 0) {
        for (int i = 0; i < samplesPerBuffer; i++) {
            po[i] = 0.0f;
        }
    } else {
        for (int i = 0; i < samplesPerBuffer; i++) {
            if (p->next > p->top) {
                p->next = p->bottom;
            }
            *po++ = *p->next++;
        }
    }

    return 0;
}
