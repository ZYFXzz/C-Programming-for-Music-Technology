#!/bin/bash
# test.sh

echo "Starting sequential audio test..."

# Array of all playback test files
files=(
    "sine_440Hz.wav"
    "chirp.wav" 
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

# Finally, do real-time recording
echo "===================="
echo "Starting real-time recording to record.wav"
echo "Press 'Q' to stop recording"
echo "===================="
./project.exe r record.wav

echo "Test sequence completed!"