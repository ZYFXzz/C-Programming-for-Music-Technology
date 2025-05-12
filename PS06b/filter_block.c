/* FIR filtering of a WAV file in real-time (with playout) using block processing */

#include <stdbool.h> //bool
#include <stdio.h>
#include <stdlib.h> //malloc()
#include <string.h> //memset()
#include <unistd.h> //sleep()
#if __APPLE__
#include <stdatomic.h> //atomic read/write
#endif
#include "filter.h"    //declares struct Filt and State
#include "paUtils.h"   //portaudio utility functions
#include <portaudio.h> //portaudio
#include <sndfile.h>   //sndfile

/* instantiation of FIR filter struct */
struct FIR_Filt filt = {
    /* FIR filter length */
    191,
    /* b coefficients */
    {
#include "fir_filt_coef.h" //FIR filter coefficients
    }};

#define BLK_LEN 1024 // block length for block processing

/* PortAudio callback structure */
struct PABuf {
    float *ifbuf;
    float *ofbuf;
    int frames;
    int channels;
    int next_frame;
#if __APPLE__
    atomic_bool done;
#else
    bool done;
#endif
    struct FIR_Filt *pf;
    float *ps;
};

/* PortAudio callback function protoype */
static int paCallback(const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo *timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData);

int main(int argc, char *argv[]) {
    char *ifile, *ofile;
    float *ifbuf, *ofbuf;
    /* lib snd file */
    SNDFILE *isndfile, *osndfile;
    SF_INFO isfinfo, osfinfo;
    int count;
    /* port audio */
    struct PABuf paBuf;
    PaStream *stream;
    /* state for block processing */
    float state[MAX_COEF * MAX_CHAN];

    /* usage and parse command line */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input_wav output_wav\n", argv[0]);
        return 1;
    }

    ifile = argv[1];
    ofile = argv[2];

    /* zero input file info */
    memset(&isfinfo, 0, sizeof(isfinfo));

    /* Open input and output files
     * Print input file parameters
     */
    isndfile = sf_open(ifile, SFM_READ, &isfinfo);
    if (!isndfile) {
        fprintf(stderr, "Error opening input file: %s\n", ifile);
        return 1;
    }
    printf("Input file: %s\n", ifile);
    printf("Sample rate: %d\n", isfinfo.samplerate);
    printf("Channels: %d\n", isfinfo.channels);
    printf("Frames: %lld\n", isfinfo.frames);

    /* Set output file parameters */
    osfinfo.samplerate = isfinfo.samplerate;
    osfinfo.channels = isfinfo.channels;
    osfinfo.format = isfinfo.format;
    /* Open output file */
    osndfile = sf_open(ofile, SFM_WRITE, &osfinfo);
    if (!osndfile) {
        fprintf(stderr, "Error opening output file: %s\n", ofile);
        return 1;
    }
    /* Allocate input and output buffers and read input signal
     * N is isfinfo.frames
     * C is isfinfo.channels
     * M is filt.num_b
     * Input
     * malloc storage ifbuf[] for N frames of C channels/frame
     * Output
     * malloc storage ofbuf[] for (N+M-1) frames of C channels/frame
     */
    int N = isfinfo.frames;
    int C = isfinfo.channels;
    int M = filt.num_b;
    ofbuf = (float *)malloc(N * C * sizeof(float));
    if (!ofbuf) {
        fprintf(stderr, "Memory allocation failed for output buffer.\n");
        return 1;
    }

    /* Read input WAV file into ifbuf */
    ifbuf = (float *)malloc(N * C * sizeof(float));
    if (!ifbuf) {
        fprintf(stderr, "Memory allocation failed for input buffer.\n");
        return 1;
    }

    sf_readf_float(isndfile, ifbuf, N);
    /* initialize Port Audio data struct */
    paBuf.ifbuf = ifbuf;
    paBuf.ofbuf = ofbuf;
    paBuf.frames = isfinfo.frames;
    paBuf.channels = isfinfo.channels;
    paBuf.next_frame = 0;
    paBuf.done = false;
    paBuf.pf = &filt;
    paBuf.ps = &state[0];
    /* zero state buffer */
    memset(state, 0, sizeof(state));

    /* start up Port Audio */
    printf("Starting PortAudio %d %d\n", 1, isfinfo.channels);
    stream = startupPa(1, isfinfo.channels,
                       isfinfo.samplerate, BLK_LEN, paCallback, &paBuf);

    /*
     * sleep and let callback process audio until done
     */
    while (!paBuf.done) {
        printf("%d\n", paBuf.next_frame);
        fflush(stdout); // Win32 needs this
        sleep(1);
    }
    printf("Done\n");

    /* shut down Port Audio */
    shutdownPa(stream);

    /* Write output */
    sf_writef_float(osndfile, ofbuf, N);

    /* close WAV files
     * free allocated storage
     */
    sf_close(isndfile);
    sf_close(osndfile);
    free(ifbuf);
    free(ofbuf);

    return 0;
}

static int paCallback(
    const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {

    /* Cast data passed via paCallback to our struct */
    struct PABuf *p = (struct PABuf *)userData;
    float *output = (float *)outputBuffer;
    int N = framesPerBuffer;
    int C = p->channels;
    int M = p->pf->num_b; // number of FIR coefficients
    double *b = &(p->pf->b[0]);

    /* return if done */
    if (p->done == true) {
        /* Zero the output buffer */
        memset(output, 0, N * C * sizeof(float));
        return 0;
    }

    /* adjust N if last frame is partial frame */
    if (p->next_frame + N > p->frames) {
        N = p->frames - p->next_frame;
    }

    /* local pointers to ifbuf[] and ofbuf[] */
    float *ifbuf = p->ifbuf + p->next_frame * C;
    float *ofbuf = p->ofbuf + p->next_frame * C;

    /* Filter the input block using state buffer for history */
    for (int ch = 0; ch < C; ch++) {
        for (int n = 0; n < N; n++) {
            float sum = 0.0;

            /* Filter using current block samples */
            for (int m = 0; m <= n && m < M; m++) {
                sum += b[m] * ifbuf[(n - m) * C + ch];
            }

            /* Filter using state buffer (past samples) */
            for (int m = n + 1; m < M; m++) {
                // State buffer contains past samples in order:
                // state[0] = sample at t-1
                // state[1] = sample at t-2
                // ...
                // state[M-2] = sample at t-(M-1)
                int state_idx = m - n - 1;
                sum += b[m] * p->ps[state_idx * C + ch];
            }

            /* Store filtered output */
            ofbuf[n * C + ch] = sum;
            output[n * C + ch] = sum;
        }
    }

    /* Update state buffer with last (M-1) samples of current block */
    if (N >= M - 1) {
        /* Copy last (M-1) samples from current block */
        for (int i = 0; i < M - 1; i++) {
            for (int ch = 0; ch < C; ch++) {
                p->ps[i * C + ch] = ifbuf[(N - (M - 1) + i) * C + ch];
            }
        }
    } else {
        /* Current block is smaller than (M-1), need to keep some old state */
        int old_samples = (M - 1) - N;

        /* Shift old state forward */
        for (int i = 0; i < old_samples; i++) {
            for (int ch = 0; ch < C; ch++) {
                p->ps[i * C + ch] = p->ps[(i + N) * C + ch];
            }
        }

        /* Copy all samples from current block to end of state */
        for (int i = 0; i < N; i++) {
            for (int ch = 0; ch < C; ch++) {
                p->ps[(old_samples + i) * C + ch] = ifbuf[i * C + ch];
            }
        }
    }

    /* Zero remaining output if N < framesPerBuffer */
    if (N < framesPerBuffer) {
        memset(output + N * C, 0, (framesPerBuffer - N) * C * sizeof(float));
    }

    /* increment next_frame counter */
    p->next_frame += N;

    /* check if done */
    if (p->next_frame >= p->frames) {
        p->done = true;
    }

    return 0;
}