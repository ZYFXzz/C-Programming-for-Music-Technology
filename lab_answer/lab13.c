#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portaudio.h>

//use port audio to play sine wave

// Global variables
static float phase = 0.0f;
static float frequency = 440.0f; // 440 Hz
static float sampleRate = 44100.0f; // Sample rate

typedef struct {
    int current_key;
    int current_frequency;
} paUserData;


static int paCallback( const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData )
{

    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */
    paUserData *data = (paUserData *)userData;

    for( i=0; i<framesPerBuffer; i++ )
    {
        float sample = sinf(2.0f * M_PI * phase);
        *out++ = sample * 0.2f; // Adjust amplitude
        
        phase += (data->current_frequency / sampleRate);
        if (phase >= 1.0f) {
            phase -= 1.0f;
        }
    }
    return 0;
}


int main (int agrc, char**argv){
    PaStream *stream;
    PaError err;
    paUserData data;
    char key_input;

    data.current_key = 0;
    data.current_frequency = 440;

    err = Pa_Initialize();

    if( err != paNoError ) {
        fprintf(stderr, "Error initializing PortAudio: %s\n", Pa_GetErrorText(err));
        return -1;
    }

    err = Pa_OpenDefaultStream( &stream,
        0,          /* no input channels */
        1,          /* Mono output */
        paFloat32,  /* 32 bit floating point output */
        44100,
        512,        /* frames per buffer, i.e. the number
                           of sample frames that PortAudio will
                           request from the callback. Many apps
                           may want to use
                           paFramesPerBufferUnspecified, which
                           tells PortAudio to pick the best,
                           possibly changing, buffer size.*/
        paCallback, /* this is your callback function */
        &data ); /*This is a pointer that will be passed to
                           your callback*/
    if( err != paNoError ){
        fprintf(stderr, "Error opening PortAudio stream: %s\n", Pa_GetErrorText(err));
        Pa_Terminate();
        return -1;
    }

    err = Pa_StartStream( stream );
    if( err != paNoError ){
        fprintf(stderr, "Error starting PortAudio stream: %s\n", Pa_GetErrorText(err));
        Pa_CloseStream(stream);
        Pa_Terminate();
        return -1;
    }

    //Your audio processing logic (or wait for the stream to finish) ...
    while(1){
        key_input = getchar();

        if (key_input == 'q') break;

            if (key_input >= '1' && key_input <= '9') {
                data.current_key = key_input - '1';
            } else if (key_input == '0') {
                data.current_key = 9;
            } else if (key_input == '-') {
                data.current_key = 10;
            } else if (key_input == '='){
                data.current_key = 11;
            }
        
        data.current_frequency = 440*(pow(2,(data.current_key/12.0)));
    }

    err = Pa_StopStream( stream );
    if( err != paNoError ){
        fprintf(stderr, "Error stopping PortAudio stream: %s\n", Pa_GetErrorText(err));
        Pa_CloseStream(stream);
        Pa_Terminate();
        return -1;
    }

    err = Pa_CloseStream( stream );
    if( err != paNoError ){
        fprintf(stderr, "Error closing PortAudio stream: %s\n", Pa_GetErrorText(err));
        Pa_Terminate();
        return -1;
    }

    err = Pa_Terminate();
    if( err != paNoError ){
        printf(  "Error terminating PortAudio: %s\n", Pa_GetErrorText( err ) );
        return -1;
    }

    return 0;
        

}

// #define SAMPLE_RATE (44100)
// #define FRAMES_PER_BUFFER (64)
// #define NUM_SECONDS (5)
// #define NUM_KEYS 12
// #define A4_FREQUENCY 440.0

// typedef struct {
//     float phase[NUM_KEYS];
//     float frequency[NUM_KEYS];
//     int current_key;
// } paUserData;

// static int patestCallback(const void *inputBuffer, void *outputBuffer,
//                           unsigned long framesPerBuffer,
//                           const PaStreamCallbackTimeInfo *timeInfo,
//                           PaStreamCallbackFlags statusFlags,
//                           void *userData) {


                            
//     paUserData *data = (paUserData *)userData;
//     float *out = (float *)outputBuffer;
//     unsigned long i, j;

//     (void)timeInfo;
//     (void)statusFlags;
//     (void)inputBuffer;
    
//     for (i = 0; i < framesPerBuffer; i++) {
//       float sum = 0.0f;
//     //   printf("%d", data->current_key);
//       for (j = 0; j < NUM_KEYS; ++j) {
//         if(j == data->current_key){
            
//             sum += sinf(2.0f * M_PI * data->phase[j] * data->frequency[j] / SAMPLE_RATE);
//             data->phase[j] += 1.0f;
//             if (data->phase[j] >= SAMPLE_RATE / data->frequency[j]) {
//             data->phase[j] -= SAMPLE_RATE / data->frequency[j];
//             }
//         }
//       }
//       *out++ = sum * 0.1;
//     }
//     return 0;
// }

// int main() {
//     PaStream *stream;
//     PaError err;
//     paUserData data;
//     char key_input;
//     int i;
//     float semitone_ratio = pow(2.0, 1.0 / 12.0);

//     err = Pa_Initialize();
//     if (err != paNoError) goto error;

//     for(i = 0; i < NUM_KEYS; i++){
//         data.phase[i] = 0.0f;
//         data.frequency[i] = A4_FREQUENCY * pow(semitone_ratio, i);
//     }
//     data.current_key = 6;

//     err = Pa_OpenDefaultStream(&stream,
//                                0,          /* no input channels */
//                                1,          /* mono output channels */
//                                paFloat32,  /* 32 bit floating point output */
//                                SAMPLE_RATE,
//                                FRAMES_PER_BUFFER,
//                                patestCallback,
//                                &data);
//     if (err != paNoError) goto error;

//     err = Pa_StartStream(stream);
//     if (err != paNoError) goto error;

//     printf("Playing sound... Press keys (1-9, 0, -, =) to play notes, 'q' to quit.\n");
//     while (1) {
//         key_input = getchar();

//         if (key_input == 'q') break;

//         if (key_input >= '1' && key_input <= '9') {
//             data.current_key = key_input - '1';
//         } else if (key_input == '0') {
//             data.current_key = 9;
//         } else if (key_input == '-') {
//           data.current_key = 10;
//         } else if (key_input == '='){
//           data.current_key = 11;
//         }
//         // } else {
//         //     data.current_key = -1;
//         // }
//         printf("%d", data.current_key);
//     }

//     err = Pa_StopStream(stream);
//     if (err != paNoError) goto error;

//     err = Pa_CloseStream(stream);
//     if (err != paNoError) goto error;

//     Pa_Terminate();
//     printf("Test finished.\n");
//     return 0;

// error:
//     Pa_Terminate();
//     fprintf(stderr, "An error occurred: %s\n", Pa_GetErrorText(err));
//     return 1;
// }