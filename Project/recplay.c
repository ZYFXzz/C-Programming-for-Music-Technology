#include <fftw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <ncurses/ncurses.h>
#include <portaudio.h>
#include <sndfile.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// hyper-params
#define CHANNELS 1
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256
// fft params
#define FFT_SIZE 8192
#define SPECTRUM_BANDS 96
#define SPECTRUM_HEIGHT 18
// Recording timeout
#define RECORD_TIMEOUT_SECONDS 16

typedef struct {
    SNDFILE *file;
    SF_INFO sfinfo;
    // fft info
    double *fft_input;
    fftw_complex *fft_output;
    fftw_plan fft_plan;
    float *spectrum;
    int spectrum_ready;
    // Recording timeout
    time_t start_time;
} UserData;

// Function declarations
double hz_to_mel(double hz);
double mel_to_hz(double mel);
double get_equal_loudness_weight(double freq);
void calculate_spectrum(double *fft_input, fftw_complex *fft_output, float *spectrum,
                        fftw_plan plan, int fft_size, int num_bands, int sample_rate);
void draw_spectrum(float *spectrum, int num_bands, int height, bool is_playback);
void process_audio_for_spectrum(UserData *data, const short *audio_data, unsigned long frameCount);
void print_device_info(char mode, const PaDeviceInfo *deviceInfo);

// Mel-scale conversion
double hz_to_mel(double hz) {
    return 2595.0 * log10(1.0 + hz / 700.0);
}

double mel_to_hz(double mel) {
    return 700.0 * (pow(10.0, mel / 2595.0) - 1.0);
}

// Equal loudness weighting
double get_equal_loudness_weight(double freq) {
    // Simplified A-weighting approximation
    double f2 = freq * freq;
    double f4 = f2 * f2;

    // A-weighting formula
    double num = 12194.0 * 12194.0 * f4;
    double den = (f2 + 20.6 * 20.6) * sqrt((f2 + 107.7 * 107.7) * (f2 + 737.9 * 737.9)) * (f2 + 12194.0 * 12194.0);

    if (den == 0)
        return 1.0;

    double weight = num / den;

    // Normalize at 1kHz
    return weight * 2.5;
}

// Calculate spectrum with mel-scale
void calculate_spectrum(double *fft_input, fftw_complex *fft_output, float *spectrum,
                        fftw_plan plan, int fft_size, int num_bands, int sample_rate) {
    // Apply Hamming window
    for (int i = 0; i < fft_size; i++) {
        double window = 0.54 - 0.46 * cos(2 * M_PI * i / (fft_size - 1));
        fft_input[i] *= window;
    }

    // Execute FFT
    fftw_execute_dft_r2c(plan, fft_input, fft_output);

    // Use mel-scale frequency scaling
    double freq_min = 20.0;
    double freq_max = 20000.0;
    double mel_min = hz_to_mel(freq_min);
    double mel_max = hz_to_mel(freq_max);

    for (int i = 0; i < num_bands; i++) {
        // Calculate mel-scale frequency range for this band
        double mel_start = mel_min + (mel_max - mel_min) * i / num_bands;
        double mel_end = mel_min + (mel_max - mel_min) * (i + 1) / num_bands;

        double freq_start = mel_to_hz(mel_start);
        double freq_end = mel_to_hz(mel_end);

        // Convert to FFT bin indices
        int bin_start = (int)(freq_start * fft_size / sample_rate);
        int bin_end = (int)(freq_end * fft_size / sample_rate);

        if (bin_start >= fft_size / 2)
            bin_start = fft_size / 2 - 1;
        if (bin_end >= fft_size / 2)
            bin_end = fft_size / 2 - 1;
        if (bin_start < 0)
            bin_start = 0;
        if (bin_end < bin_start)
            bin_end = bin_start;

        // Average the FFT bins in this frequency range
        double sum = 0;
        int count = 0;
        for (int j = bin_start; j <= bin_end && j < fft_size / 2; j++) {
            double real = fft_output[j][0];
            double imag = fft_output[j][1];
            double magnitude = sqrt(real * real + imag * imag);
            sum += magnitude;
            count++;
        }

        if (count > 0) {
            spectrum[i] = sum / count;
        } else {
            spectrum[i] = 0;
        }

        // Get center frequency of this band
        double center_freq = (freq_start + freq_end) / 2.0;

        // Apply equal loudness weighting
        double loudness_weight = get_equal_loudness_weight(center_freq);
        spectrum[i] *= loudness_weight;

        // Better scaling for music
        spectrum[i] = 20 * log10(spectrum[i] + 1e-6); // Convert to dB
        spectrum[i] = (spectrum[i] + 30) / 30;        // Adjust range (-80dB to 0dB)

        if (spectrum[i] < 0)
            spectrum[i] = 0;
        if (spectrum[i] > 1)
            spectrum[i] = 1;

        // Apply a slight boost to make the visualization more visible
        spectrum[i] = pow(spectrum[i], 0.5);
    }
}

// Draw spectrum
void draw_spectrum(float *spectrum, int num_bands, int height, bool is_playback) {
    clear();
    if (is_playback) {
        mvprintw(0, 0, "Mel-Scale Spectrum Analyzer - Press 'Q' to quit");
    } else {
        mvprintw(0, 0, "Mel-Scale Spectrum Analyzer (Recording) - Press 'Q' to quit");
    }

    // Draw spectrum bars
    for (int i = 0; i < num_bands; i++) {
        int bar_height = (int)(spectrum[i] * height);

        for (int j = 0; j < height; j++) {
            int y = height - j + 2;
            int x = i + 5;

            if (j < bar_height) {
                if (j < height * 0.33)
                    attron(COLOR_PAIR(1));
                else if (j < height * 0.66)
                    attron(COLOR_PAIR(2));
                else
                    attron(COLOR_PAIR(3));

                mvaddch(y, x, '|');
                attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3));
            }
        }
    }

    // Draw mel-scale frequency labels
    double mel_min = hz_to_mel(20);
    double mel_max = hz_to_mel(20000);

    // Select frequencies to label
    double frequencies[] = {50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000};
    for (int i = 0; i < 9; i++) {
        double mel_pos = (hz_to_mel(frequencies[i]) - mel_min) / (mel_max - mel_min);
        int x_pos = (int)(mel_pos * num_bands) + 5;

        // Only show labels that fit properly
        if (x_pos >= 5 && x_pos < num_bands + 5) {
            if (frequencies[i] >= 1000) {
                mvprintw(height + 3, x_pos - 1, "%.0fk", frequencies[i] / 1000.0);
            } else {
                mvprintw(height + 3, x_pos - 1, "%.0f", frequencies[i]);
            }
        }
    }

    // Add "Hz" label at the end
    mvprintw(height + 3, num_bands + 6, "Hz");

    refresh();
}

// Shared function to process audio for spectrum analysis
void process_audio_for_spectrum(UserData *data, const short *audio_data, unsigned long frameCount) {
    // Convert stereo to mono if needed and fill FFT input buffer
    for (unsigned long i = 0; i < frameCount && i < FFT_SIZE; i++) {
        if (data->sfinfo.channels == 2) {
            // Mix stereo to mono: average left and right channels
            double left = (double)audio_data[i * 2] / 32768.0;
            double right = (double)audio_data[i * 2 + 1] / 32768.0;
            data->fft_input[i] = (left + right) / 2.0;
        } else {
            // Mono: just copy the single channel
            data->fft_input[i] = (double)audio_data[i] / 32768.0;
        }
    }

    // Zero-pad if necessary
    for (int i = frameCount; i < FFT_SIZE; i++) {
        data->fft_input[i] = 0.0;
    }

    calculate_spectrum(data->fft_input, data->fft_output, data->spectrum,
                       data->fft_plan, FFT_SIZE, SPECTRUM_BANDS, data->sfinfo.samplerate);
    data->spectrum_ready = 1;
}

// Print device info
void print_device_info(char mode, const PaDeviceInfo *deviceInfo) {
    printf("Using %s device: %s\n", (mode == 'r') ? "input" : "output", deviceInfo->name);
    printf("  Channels: %d\n", (mode == 'r') ? deviceInfo->maxInputChannels : deviceInfo->maxOutputChannels);
    printf("  Sample Rate: %.0f Hz\n", deviceInfo->defaultSampleRate);
    printf("  Latency: %.3f seconds\n", (mode == 'r') ? deviceInfo->defaultLowInputLatency : deviceInfo->defaultLowOutputLatency);
    printf("\n");
}

// Recording callback
static int recordCallback(const void *input, void *output, unsigned long frameCount,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {
    UserData *data = (UserData *)userData;
    const short *in = (const short *)input;

    // Check timeout
    time_t current_time = time(NULL);
    if (difftime(current_time, data->start_time) >= RECORD_TIMEOUT_SECONDS) {
        return paComplete;
    }

    // Write to file
    sf_count_t numWritten = sf_writef_short(data->file, in, frameCount);
    if (numWritten != frameCount) {
        fprintf(stderr, "Error writing to file\n");
        return paComplete;
    }

    // Process audio for spectrum
    process_audio_for_spectrum(data, in, frameCount);

    return paContinue;
}

// Playback callback
static int playbackCallback(const void *input, void *output, unsigned long frameCount,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData) {
    UserData *data = (UserData *)userData;
    short *out = (short *)output;

    // Read from file
    sf_count_t numRead = sf_readf_short(data->file, out, frameCount);
    if (numRead != frameCount) {
        memset(out + numRead * data->sfinfo.channels, 0,
               (frameCount - numRead) * data->sfinfo.channels * sizeof(short));
    }

    // Process audio for spectrum
    process_audio_for_spectrum(data, out, frameCount);

    if (numRead != frameCount) {
        return paComplete;
    }

    return paContinue;
}

int main(int argc, char *argv[]) {
    // Parse command line
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <r/p> <filename.wav>\n", argv[0]);
        return 1;
    }

    if (Pa_Initialize() != paNoError) {
        fprintf(stderr, "Failed to initialize PortAudio\n");
        return 1;
    }

    // Initialize data structure
    UserData data;
    memset(&data, 0, sizeof(UserData));
    memset(&data.sfinfo, 0, sizeof(SF_INFO));
    data.sfinfo.samplerate = SAMPLE_RATE;
    data.sfinfo.channels = 1;
    data.sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    data.start_time = time(NULL);

    PaStream *stream;
    const PaDeviceInfo *inputDeviceInfo = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice());
    const PaDeviceInfo *outputDeviceInfo = Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice());

    // Allocate FFT memory
    data.fft_input = (double *)fftw_malloc(sizeof(double) * FFT_SIZE);
    data.fft_output = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * (FFT_SIZE / 2 + 1));
    data.fft_plan = fftw_plan_dft_r2c_1d(FFT_SIZE, data.fft_input, data.fft_output, FFTW_ESTIMATE);
    data.spectrum = (float *)calloc(SPECTRUM_BANDS, sizeof(float));
    data.spectrum_ready = 0;

    // Mode selection
    if (strcmp(argv[1], "r") == 0) {
        // Recording mode
        // Check if file exists
        data.file = sf_open(argv[2], SFM_READ, &data.sfinfo);
        if (data.file) {
            sf_close(data.file);
            fprintf(stderr, "File %s already exists.\n", argv[2]);
            return 1;
        }

        data.file = sf_open(argv[2], SFM_WRITE, &data.sfinfo);
        if (!data.file) {
            fprintf(stderr, "Failed to open file for writing: %s\n", argv[2]);
            return 1;
        }

        if (Pa_OpenDefaultStream(&stream, 1, 0, paInt16, SAMPLE_RATE,
                                 FRAMES_PER_BUFFER, recordCallback, &data) != paNoError) {
            fprintf(stderr, "Failed to open default stream for recording\n");
            return 1;
        }

        print_device_info('r', inputDeviceInfo);

    } else if (strcmp(argv[1], "p") == 0) {
        // Playback mode
        data.file = sf_open(argv[2], SFM_READ, &data.sfinfo);
        if (!data.file) {
            fprintf(stderr, "Failed to open file for reading: %s\n", argv[2]);
            return 1;
        }

        if (Pa_OpenDefaultStream(&stream, 0, data.sfinfo.channels, paInt16,
                                 data.sfinfo.samplerate, FRAMES_PER_BUFFER,
                                 playbackCallback, &data) != paNoError) {
            fprintf(stderr, "Failed to open default stream for playback\n");
            return 1;
        }

        print_device_info('p', outputDeviceInfo);

    } else {
        fprintf(stderr, "Invalid option: %s\n", argv[1]);
        return 1;
    }

    if (Pa_StartStream(stream) != paNoError) {
        fprintf(stderr, "Failed to start stream\n");
        return 1;
    }

    // Initialize ncurses
    initscr();
    start_color();
    timeout(20); // Non-blocking getch with 20ms timeout
    noecho();
    curs_set(0); // Hide cursor

    // Define color pairs
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

    // Main loop
    bool is_playback = (strcmp(argv[1], "p") == 0);
    while (Pa_IsStreamActive(stream) == 1) {
        // Draw spectrum
        if (data.spectrum_ready) {
            draw_spectrum(data.spectrum, SPECTRUM_BANDS, SPECTRUM_HEIGHT, is_playback);
            data.spectrum_ready = 0;
        }

        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            break;
        }
    }

    // Cleanup
    if (Pa_StopStream(stream) != paNoError) {
        fprintf(stderr, "Failed to stop stream\n");
        return 1;
    }

    if (Pa_CloseStream(stream) != paNoError) {
        fprintf(stderr, "Failed to close stream\n");
        return 1;
    }

    sf_close(data.file);

    // Clean up FFT resources
    fftw_destroy_plan(data.fft_plan);
    fftw_free(data.fft_input);
    fftw_free(data.fft_output);
    free(data.spectrum);
    fftw_cleanup();

    if (Pa_Terminate() != paNoError) {
        fprintf(stderr, "Failed to terminate PortAudio\n");
        return 1;
    }

    endwin();

    printf("Operation completed successfully\n");
    return 0;
}