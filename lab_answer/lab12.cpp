#include <iostream>
#include <portaudio.h>
#include <cmath>


// //experiment with classes
// class Dog{
//     public:
//         Dog(int a, char* n){
//             age = a;
//             name = n;
//         }

//         void bark(){
//             std::cout << "BARK!!! MY NAME IS " << name << "!!\n";
//         }

//         void how_old(){
//             std::cout << "My age is " << age << "!!\n";
//         }

//         ~Dog(){
//             std::cout << name << " was destroid\n";
//         }

//     private:
//         int age;
//         char* name;

// };

// int main(){
//     Dog d1 = Dog(7, "Clifford");
//     Dog d2 = Dog(65, "Spot");

//     d1.bark();
//     d1.how_old();
//     d2.bark();
//     d2.how_old();

//     return 0;
// }





//make audio processor class with port audio

class audio_processor{
    public:
        audio_processor(float f, float a){
            freq = f;
            amp = a;

            Pa_Initialize();

            Pa_OpenDefaultStream( &stream,
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
                this ); /*This is a pointer that will be passed to
                                your callback*/
        }

        ~audio_processor(){
            Pa_Terminate();
        }

        void start(){
            Pa_StartStream( stream );
        }

        void stop(){
            Pa_StopStream( stream );
            Pa_CloseStream( stream );
        }

    private:
        float freq;
        float amp;
        PaStream *stream;
        float sampleRate = 44100;
        float phase = 0;

        float generateSine() {
                phase += freq * 2.0f * M_PI / sampleRate;
                if (phase > 2.0f * M_PI) {
                    phase -= 2.0f * M_PI;
                }
                return amp * sinf(phase);
            }
            
            static int paCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
                audio_processor* processor = static_cast<audio_processor*>(userData);
                float* out = static_cast<float*>(outputBuffer);
        
                for (unsigned int i = 0; i < framesPerBuffer; ++i) {
                    *out++ = processor->generateSine() * 0.5f; 
                }
                return paContinue;
            }

};

int main(int argc, char**argv) {

            audio_processor processor(440, 0.1);
            audio_processor processor2(440*(pow(2,(6.0/12.0))), 0.1);
            audio_processor processor3(440*(pow(2,(3.0/12.0))), 0.1);
            processor.start();
            processor2.start(); 
            processor3.start();
            std::cout << "Playing sine wave... Press any key to stop!" << std::endl;
            std::cin.get();
            processor.stop();
            processor2.stop();
            processor3.stop();
        return 0;
    }








// class dog {
//     public:
//         dog(int n){
//             num = n;
//         }
//         void bark(){
//             std::cout << num;
//         }
//     private:
//         int num;

// };

// int main(int agrc, char**argv) {
//     dog d = dog(5);
//     d.bark();
//     return 0;
// }

// class AudioProcessor {
// public:
//     AudioProcessor(int sampleRate = 44100, int frameSize = 64) : sampleRate_(sampleRate), frameSize_(frameSize), phase_(0.0) {
//         PaError err = Pa_Initialize();
//         if (err != paNoError) {
//             std::cerr << "PortAudio initialization error: " << Pa_GetErrorText(err) << std::endl;
//             throw std::runtime_error("PortAudio initialization failed");
//         }

//         PaStreamParameters outputParameters;
//         outputParameters.device = Pa_GetDefaultOutputDevice();
//         outputParameters.channelCount = 1;
//         outputParameters.sampleFormat = paFloat32;
//         outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
//         outputParameters.hostApiSpecificStreamInfo = nullptr;

//         err = Pa_OpenStream(&stream_, nullptr, &outputParameters, sampleRate_, frameSize_, paClipOff, &AudioProcessor::paCallback, this);
//         if (err != paNoError) {
//             std::cerr << "PortAudio stream opening error: " << Pa_GetErrorText(err) << std::endl;
//             Pa_Terminate();
//             throw std::runtime_error("PortAudio stream opening failed");
//         }
//     }

//     ~AudioProcessor() {
//         PaError err = Pa_Terminate();
//         if (err != paNoError) {
//             std::cerr << "PortAudio termination error: " << Pa_GetErrorText(err) << std::endl;
//         }
//     }

//     void start() {
//         PaError err = Pa_StartStream(stream_);
//         if (err != paNoError) {
//             std::cerr << "PortAudio start stream error: " << Pa_GetErrorText(err) << std::endl;
//             Pa_CloseStream(stream_);
//             Pa_Terminate();
//             throw std::runtime_error("PortAudio start stream failed");
//         }
//     }

//     void stop() {
//         PaError err = Pa_StopStream(stream_);
//         if (err != paNoError) {
//             std::cerr << "PortAudio stop stream error: " << Pa_GetErrorText(err) << std::endl;
//         }

//         err = Pa_CloseStream(stream_);
//         if (err != paNoError) {
//             std::cerr << "PortAudio close stream error: " << Pa_GetErrorText(err) << std::endl;
//         }
//     }

// private:
//     int sampleRate_;
//     int frameSize_;
//     PaStream* stream_;
//     float phase_;

//     float generateSine(float frequency) {
//         phase_ += frequency * 2.0f * M_PI / sampleRate_;
//         if (phase_ > 2.0f * M_PI) {
//             phase_ -= 2.0f * M_PI;
//         }
//         return sinf(phase_);
//     }

//     static int paCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
//         AudioProcessor* processor = static_cast<AudioProcessor*>(userData);
//         float* out = static_cast<float*>(outputBuffer);

//         for (unsigned int i = 0; i < framesPerBuffer; ++i) {
//             *out++ = processor->generateSine(440.0f) * 0.5f; 
//         }
//         return paContinue;
//     }
// };

// int main(int agrc, char**argv) {
//     try {
//         AudioProcessor processor;
//         std::cout << argv[1] << std::endl;
//         processor.start();
//         std::cout << "Playing sine wave... Press Enter to stop." << std::endl;
//         std::cin.get();
//         processor.stop();
//     } catch (const std::runtime_error& error) {
//         std::cerr << "Exception caught: " << error.what() << std::endl;
//         return 1;
//     }
//     return 0;
// }