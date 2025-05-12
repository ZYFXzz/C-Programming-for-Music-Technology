#include <stdio.h> 
#include <stdlib.h>
#include <head.h>
#include <lab6.c>

//Hash Search
int h_func(char* item){
    int sum = 0;
    int i = 0;

    while(item[i] != '\0'){
        sum += (int) item[i];
        i ++;
    }

    return sum % 100;

}

int fill_hash_table(int h_value, int* h_table, int index){

        if( h_table[h_value] == -1){
            h_table[h_value] = index;
        }

}

int main(int argc, char ** argv){
    int HASH_SIZE = 100;
    int h_table[HASH_SIZE];

    for(int i = 0; i < HASH_SIZE; i++){
        h_table[i] = -1;
    }

    for(int i = 0; i < argc; i ++){
        fill_hash_table(h_func(argv[i]), h_table, i);
    }

    printf("The index of %s is %d", argv[4], h_table[h_func(argv[4])]);

}

//Write sine wav
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <sndfile.h>

// #define SAMPLE_RATE 44100
// #define DURATION 5
// #define FREQUENCY 440

// int main() {
//     SNDFILE *file;
//     SF_INFO sfinfo;
//     double *buffer;
//     int num_frames = SAMPLE_RATE * DURATION;
//     double amplitude = 0.8;

//     sfinfo.samplerate = SAMPLE_RATE;
//     sfinfo.frames = num_frames;
//     sfinfo.channels = 1;
//     sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

//     file = sf_open("sine_wave.wav", SFM_WRITE, &sfinfo);
//     if (!file) {
//         printf("Error opening file.\n");
//         return 1;
//     }

//     buffer = (double *)malloc(num_frames * sizeof(double));
//     if (!buffer) {
//         printf("Memory allocation error.\n");
//         sf_close(file);
//         return 1;
//     }

//     for (int i = 0; i < num_frames; i++) {
//         double time = (double)i / SAMPLE_RATE;
//         buffer[i] = amplitude * sin(2 * M_PI * FREQUENCY * time);
//     }

//     sf_writef_double(file, buffer, num_frames);

//     free(buffer);
//     sf_close(file);

//     printf("Sine wave written to sine_wave.wav\n");
//     return 0;
// }