#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <portaudio.h>

//use port audio to play sine wave

// Global variables
static float phase = 0.0f;
static float frequency = 440.0f; // 440 Hz
static float sampleRate = 44100.0f; // Sample rate


static int paCallback( const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData )
{

    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */

    for( i=0; i<framesPerBuffer; i++ )
    {
        float sample = sinf(2.0f * M_PI * phase);
        *out++ = sample * 0.2f; // Adjust amplitude
        
        phase += (frequency / sampleRate);
        if (phase >= 1.0f) {
            phase -= 1.0f;
        }
    }
    return 0;
}


int main (int agrc, char**argv){
    PaStream *stream;
    PaError err;

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
        NULL ); /*This is a pointer that will be passed to
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
    Pa_Sleep(5000);

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


// // Global variables
// static float phase = 0.0f;
// static float frequency = 440.0f; // 440 Hz
// static float sampleRate = 44100.0f; // Sample rate

// // static void convolve(
// //     float *Signal,
// //     float *Filter, size_t FilterLength,
// //     float *Output, size_t OutputLength)
// // {
// //     for (size_t i = 0; i < OutputLength; ++i)
// //     {
// //         double sum = 0;
// //         for (size_t j = 0; j < FilterLength; ++j)
// //             sum += Signal[i+j] * Filter[FilterLength - 1 - j];
// //         Output[i] = sum;
// //     }
// // }

// static int pa_callback(const void *inputBuffer, void *outputBuffer,
//                        unsigned long framesPerBuffer,
//                        const PaStreamCallbackTimeInfo *timeInfo,
//                        PaStreamCallbackFlags statusFlags,
//                        void *userData) {

//     float *out = (float *)outputBuffer;
//     for (unsigned long i = 0; i < framesPerBuffer; i++) {
//         // Sine wave generation
//         float sample = sinf(2.0f * M_PI * phase);
//         // double period = 1.0 / frequency;
//         // float sample = (2.0 * (phase / period) - 1.0);
//         *out++ = sample * 0.5f; // Adjust amplitude

//         // Update phase
//         phase += (frequency / sampleRate);
//         if (phase >= 1.0f) {
//             phase -= 1.0f;
//         }
//     }
//     return 0;
// }

// int main() {
//     PaStream *stream;
//     PaError err;

//     // Initialize PortAudio
//     err = Pa_Initialize();
//     if (err != paNoError) {
//         fprintf(stderr, "Error initializing PortAudio: %s\n", Pa_GetErrorText(err));
        
//         return -1;
//     }

//     // Open a stream (using defaults for simplicity)
//     err = Pa_OpenDefaultStream(&stream, 0, /* no input channels */
//                                 1, /* stereo output */
//                                 paFloat32, /* 32 bit floating point output */
//                                 44100, /* sample rate */
//                                 paFramesPerBufferUnspecified, /* frames per buffer */
//                                 pa_callback, /* callback function */
//                                 NULL); /* user data */
//     if (err != paNoError) {
//         fprintf(stderr, "Error opening stream: %s\n", Pa_GetErrorText(err));
//         Pa_Terminate();
//         return -1;
//     }

//     // Start the stream
//     err = Pa_StartStream(stream);
//     if (err != paNoError) {
//         fprintf(stderr, "Error starting stream: %s\n", Pa_GetErrorText(err));
//         Pa_CloseStream(stream);
//         Pa_Terminate();
//         return -1;
//     }

//     // ... Your audio processing logic (or wait for the stream to finish) ...
//     Pa_Sleep(5000); // Wait for 5 seconds (adjust as needed)

//     // Stop the stream
//     err = Pa_StopStream(stream);
//     if (err != paNoError) {
//         fprintf(stderr, "Error stopping stream: %s\n", Pa_GetErrorText(err));
//     }

//     // Close the stream
//     err = Pa_CloseStream(stream);
//     if (err != paNoError) {
//         fprintf(stderr, "Error closing stream: %s\n", Pa_GetErrorText(err));
//     }

//     // Terminate PortAudio
//     err = Pa_Terminate();
//     if (err != paNoError) {
//         fprintf(stderr, "Error terminating PortAudio: %s\n", Pa_GetErrorText(err));
//     }

//     return 0;
// }
