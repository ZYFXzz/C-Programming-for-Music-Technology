#!/bin/bash
# build_test.sh

echo "========================================="
echo "Build and Test Script"
echo "========================================="


# Step 1: Build the project
echo ""
echo "Step 1: Building project..."
gcc recplay.c -o project.exe -DNCURSES_STATIC -I/msys64/mingw64/include/ncurses -L/usr/local/lib -lsndfile -lportaudio -lncurses -lfftw3 -lm

if [ $? -eq 0 ]; then
    echo "Build successful!"
else
    echo "Build failed!"
    exit 1
fi

# Step 2: Run tests
echo ""
echo "Step 2: Running sequential audio test..."

# Array of all playback test files (updated with correct chirp filename)
files=(
    "sine_440Hz.wav"
    "chirp_100Hz_5kHz.wav"
    "sine_1000Hz.wav"
    "white_noise.wav"
    "sig1.wav"
    "sig2.wav"
    "sig3.wav"
)

# Play each file sequentially
for file in "${files[@]}"; do
    if [ -f "$file" ]; then
        echo "===================="
        echo "Playing: $file"
        echo "Press 'Q' to skip to next file"
        echo "===================="
        ./project.exe p "$file"
        # Small pause between files
        sleep 1
    else
        echo "File $file not found, skipping..."
    fi
done

# Last, real-time recording
echo "===================="
echo "Starting real-time recording to record.wav"
echo "The program will record for 10 seconds"
echo "Press 'Q' to stop recording early"
echo "===================="
timeout 10s ./project.exe r record.wav

echo ""
echo "========================================="
echo "Build and test sequence completed!"