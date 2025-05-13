import numpy as np
import scipy.io.wavfile as wav

SAMPLE_RATE = 44100  # Hz
DURATION = 1.0  # seconds


def generate_sine_wave(frequency, sample_rate, duration):
    t = np.linspace(0, duration, int(sample_rate * duration), endpoint=False)
    return 0.5 * np.sin(
        2 * np.pi * frequency * t
    )  


def generate_white_noise(sample_rate, duration):
    return np.random.uniform(-0.5, 0.5, int(sample_rate * duration))


def generate_chirp(start_freq, end_freq, sample_rate, duration):
    t = np.linspace(0, duration, int(sample_rate * duration), endpoint=False)
    return 0.5 * np.sin(
        2 * np.pi * (start_freq + (end_freq - start_freq) * t / duration) * t
    )


def save_signals():
    signals = {
        "sine_440Hz.wav": generate_sine_wave(440, SAMPLE_RATE, DURATION),
        "sine_1000Hz.wav": generate_sine_wave(1000, SAMPLE_RATE, DURATION),
        "white_noise.wav": generate_white_noise(SAMPLE_RATE, DURATION),
        "chirp_100Hz_5kHz.wav": generate_chirp(100, 5000, SAMPLE_RATE, DURATION),
    }

    for filename, signal in signals.items():
        wav.write(
            filename, SAMPLE_RATE, (signal * 32767).astype(np.int16)
        )  # Convert to 16-bit PCM

    print("Test signals generated!")


# Ensure the script runs only when executed directly
if __name__ == "__main__":
    save_signals()
