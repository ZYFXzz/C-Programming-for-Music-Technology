#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <portaudio.h>
#include <sndfile.h>

//use string.h to split a large string and use atof from stdlib.h to convert to numbers then add
// ./l9 8->12->34->34 -> "sum is 88"

// int main(int argc, char** argv){

//     char *myPtr = strtok(argv[1], "-");
//     int sum = 0;
//     while (myPtr != NULL) {
//         sum += atoi(myPtr); 
//         myPtr = strtok(NULL, "-");
//     }

//     printf("%d\n", sum);

//     return 0;
// }


//use port audio to print all of the available audio devices on your computer

// int main() {
//     PaError err;

//     err = Pa_Initialize();
//     if (err != paNoError) {
//         fprintf(stderr, "PortAudio initialization error: %s\n", Pa_GetErrorText(err));
//         return 1;
//     }

//     int numDevices = Pa_GetDeviceCount();
//     if (numDevices < 0) {
//         fprintf(stderr, "PortAudio error getting device count: %s\n", Pa_GetErrorText(err));
//         Pa_Terminate();
//         return 1;
//     }

//     printf("Available audio devices:\n");
//     for (int i = 0; i < numDevices; i++) {
//         const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
//         printf("  Device %d: %s\n", i, deviceInfo->name);
//         printf("    Max input channels: %d\n", deviceInfo->maxInputChannels);
//         printf("    Max output channels: %d\n", deviceInfo->maxOutputChannels);
//         printf("    Default sample rate: %.2f Hz\n", deviceInfo->defaultSampleRate);
//     }

//     Pa_Terminate();
//     return 0;
// }

//use port audio to play sound file

//write sine wave to file with libsndfile




// #include <portaudio.h>
// #include <math.h>

// // Global variables
// static float phase = 0.0f;
// static float frequency = 440.0f; // 440 Hz
// static float sampleRate = 44100.0f; // Sample rate

// static int pa_callback(const void *inputBuffer, void *outputBuffer,
//                        unsigned long framesPerBuffer,
//                        const PaStreamCallbackTimeInfo *timeInfo,
//                        PaStreamCallbackFlags statusFlags,
//                        void *userData) {

//     float *out = (float *)outputBuffer;
//     for (unsigned long i = 0; i < framesPerBuffer; i++) {
//         // Sine wave generation
//         float sample = sinf(2.0f * M_PI * phase);
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
//         // fprintf(stderr, "Error initializing PortAudio: %s\n", Pa_GetErrorString(err));
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
//         // fprintf(stderr, "Error opening stream: %s\n", Pa_GetErrorString(err));
//         Pa_Terminate();
//         return -1;
//     }

//     // Start the stream
//     err = Pa_StartStream(stream);
//     if (err != paNoError) {
//         // fprintf(stderr, "Error starting stream: %s\n", Pa_GetErrorString(err));
//         Pa_CloseStream(stream);
//         Pa_Terminate();
//         return -1;
//     }

//     // ... Your audio processing logic (or wait for the stream to finish) ...
//     Pa_Sleep(5000); // Wait for 5 seconds (adjust as needed)

//     // Stop the stream
//     err = Pa_StopStream(stream);
//     if (err != paNoError) {
//         // fprintf(stderr, "Error stopping stream: %s\n", Pa_GetErrorString(err));
//     }

//     // Close the stream
//     err = Pa_CloseStream(stream);
//     if (err != paNoError) {
//         // fprintf(stderr, "Error closing stream: %s\n", Pa_GetErrorString(err));
//     }

//     // Terminate PortAudio
//     err = Pa_Terminate();
//     if (err != paNoError) {
//         // fprintf(stderr, "Error terminating PortAudio: %s\n", Pa_GetErrorString(err));
//     }

//     return 0;
// }


//write sine wave to file with libsndfile


#define SAMPLE_RATE 44100
#define DURATION 5
#define FREQUENCY 440
#define AMPLITUDE 0.8

int main() {
    SF_INFO sfinfo;
    SNDFILE *outfile;
    double *buffer;
    int num_frames = SAMPLE_RATE * DURATION;
    double angle_increment = 2.0 * M_PI * FREQUENCY / SAMPLE_RATE;
    double current_angle = 0.0;

    sfinfo.samplerate = SAMPLE_RATE;
    sfinfo.frames = num_frames;
    sfinfo.channels = 1;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    outfile = sf_open("sine_wave2.wav", SFM_WRITE, &sfinfo);
    if (!outfile) {
        fprintf(stderr, "Error opening file: %s\n", sf_strerror(NULL));
        return 1;
    }

    buffer = (double *)malloc(num_frames * sizeof(double));
    if (!buffer) {
      fprintf(stderr, "Memory allocation error\n");
      sf_close(outfile);
      return 1;
    }
    
    for (int i = 0; i < num_frames; i++) {
        buffer[i] = AMPLITUDE * sin(current_angle);
        current_angle += angle_increment;
    }

    sf_write_double(outfile, buffer, num_frames);

    free(buffer);
    sf_close(outfile);

    printf("Sine wave written to sine_wave.wav\n");

    return 0;
}