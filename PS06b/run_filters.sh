#!/bin/bash

# Run filter_file on the test signals as described in the PDF
echo "Running filter_file on test signals..."

# Filter chirp.wav
echo "Filtering chirp.wav..."
./filter_file signals/chirp.wav chirp_out.wav

# Filter noise.wav
echo "Filtering noise.wav..."
./filter_file signals/noise.wav noise_out.wav

echo "Filtering complete!"
echo "Output files created:"
echo "  - chirp_out.wav"
echo "  - noise_out.wav"

# Optional: Run filter_block as well (should produce same results)
# echo "Running filter_block for comparison..."
# ./filter_block signals/chirp.wav chirp_block_out.wav
# ./filter_block signals/noise.wav noise_block_out.wav