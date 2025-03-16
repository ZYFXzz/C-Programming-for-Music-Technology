#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // check input argc
    if (argc != 4) { /* 打印用法并退出 */
        fprintf(stderr, "need 4 arguments: %s, max_amplitude, input_audio(wav), output_audio(wav)\n", argv[0]);
        return 1;
    }

    // read file
    FILE *ifp = fopen(argv[2], "rb"); // rb for binary
    if (ifp == NULL) {                /* 错误处理 */
        printf("no such file, please enter currect file dir\n");
        return -1;
    }

    unsigned char header[44];
    if (fread(header, 1, 44, ifp) != 44) {
        /*
        read x:read from ifp, each elements in 1 bytes, read 44 elements, store x in header through pointer
        */
        fprintf(stderr, "Error: Input file is not a valid\n");
        fclose(ifp); // close file and free resources
        return -1;
    }
    // print header numbers
    printf("WAV Header info map:\n");
    for (int i = 0; i < 44; i++) {
        char c = isprint(header[i]) ? header[i] : ' ';
        printf("%c %02x ", c, header[i]);
        if ((i + 1) % 11 == 0)
            printf("\n"); // new line after 11 chars
    }
    printf("\n"); // end printing raw wave header

    // read header info for processing
    //  channels
    int NumChannels = header[22] | (header[23] << 8);

    // sample rate
    int SampleRate =
        (header[24]) |
        (header[25] << 8) |
        (header[26] << 16) |
        (header[27] << 24);

    // bit rate
    int BitsPerSample = header[34] | (header[35] << 8);

    // subchunk2size
    int Subchunk2Size =
        (header[40]) |
        (header[41] << 8) |
        (header[42] << 16) |
        (header[43] << 24);

    // num_samples
    int num_samp = Subchunk2Size / (BitsPerSample / 8);

    // print header info
    printf("Channels: %d\n", NumChannels);
    printf("Sample Rate: %d\n", SampleRate);
    printf("Bits per Sample: %d\n", BitsPerSample);
    printf("Number of samples: %d\n", num_samp);

    // read audio samples
    short *x = malloc(num_samp * sizeof(short));
    if (x == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(ifp);
        return -1;
    }
    size_t count = fread(x, sizeof(short), num_samp, ifp);
    if (count != num_samp) {
        /*
        included in here, as there might be situation that wave file is corrupted,
        so number of sample is in fact, smaller than the stated value
         */
        fprintf(stderr, "Error: Failed to read audio x, file is broken\n");
        free(x);
        fclose(ifp);
        return -1;
    }

    // traverse through x, to find absolute max val
    int max_x = 0;
    for (int i = 0; i < num_samp; i++) {
        int abs_val = abs(x[i]);
        if (abs_val > max_x) {
            max_x = abs_val;
        }
    }
    printf("max value of input .wav file is %d\n", max_x);

    // adjust amplitude based on given max_amplitude
    int max_amplitude = atoi(argv[1]);
    float scale = (float)max_amplitude / (float)max_x;
    printf("based on provided info, the adjustment scale is %f\n", scale);

    for (int i = 0; i < num_samp; i++) {
        float scaled_value = x[i] * scale;
        x[i] = (short)round(scaled_value); // note: round return a double, need to cast into short

        // check amplitude max in x
        if (x[i] < -32768 || x[i] > 32767) {
            printf("Sample %d overflow (value=%d), cutoff at 32767\n", i, x[i]);
            // cutoff at 32767, the max value allowed
            x[i] = (x[i] < -32768) ? -32768 : 32767;
        }
    }

    // write output
    FILE *ofp = fopen(argv[3], "wb");
    if (ofp == NULL) {
        perror("Error opening output file\n");
        free(x);
        fclose(ifp);
        return -1;
    }

    // write header - copying the original header
    if (fwrite(header, 1, 44, ofp) != 44) { // fwrite will return the number of elements written
        fprintf(stderr, "Error writing WAV header\n");
        free(x);
        fclose(ifp);
        fclose(ofp);
        return -1;
    }

    // write the adjusted val
    size_t written = fwrite(x, sizeof(short), num_samp, ofp);
    if (written != num_samp) {
        fprintf(stderr, "Error writing audio data\n");
        free(x);
        fclose(ifp);
        fclose(ofp);
        return -1;
    }

    // close and free all resources
    free(x);
    fclose(ifp);
    fclose(ofp);

    printf("Finished: Output file saved as %s\n", argv[3]);
    return 0;
}