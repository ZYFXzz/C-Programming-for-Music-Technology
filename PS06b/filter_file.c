/* FIR filtering of a entire WAV file */

#include "filter.h"  //declares struct Filt
#include <sndfile.h> //sndfile
#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h> //memset

/* instantiation of FIR filter struct */
struct FIR_Filt filt = {
    /* FIR filter length */
    191,
    /* b coefficients */
    {
#include "fir_filt_coef.h" //FIR filter coefficients
    }};

int main(int argc, char *argv[]) {
    char *ifile, *ofile;
    SNDFILE *isndfile, *osndfile;
    SF_INFO isfinfo, osfinfo;
    float *ifbuf, *ofbuf;
    int count;

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

    /* Read input WAV file into ifbuf */
    ifbuf = (float *)malloc(N * C * sizeof(float));
    if (!ifbuf) {
        fprintf(stderr, "Memory allocation failed for input buffer.\n");
        return 1;
    }

    sf_readf_float(isndfile, ifbuf, N);

    /*
     * filter
     */
    double *b = filt.b; // pointer to FIR filter coefficients

    /* Write output buffer to file */
    ofbuf = (float *)calloc((N + M - 1) * C, sizeof(float));
    if (!ofbuf) {
        fprintf(stderr, "Memory allocation failed for output buffer.\n");
        return 1;
    }

    // Apply FIR filter
    for (int ch = 0; ch < C; ch++) {
        for (int n = 0; n < N + M - 1; n++) {
            ofbuf[n * C + ch] = 0.0;
            for (int m = 0; m < M; m++) {
                if (n - m >= 0 && n - m < N) {
                    ofbuf[n * C + ch] += b[m] * ifbuf[(n - m) * C + ch];
                }
            }
        }
    }
    sf_writef_float(osndfile, ofbuf, N + M - 1);

    /* close WAV files
     * free allocated storage
     */
    sf_close(isndfile);
    sf_close(osndfile);
    free(ifbuf);
    free(ofbuf);

    return 0;
}
