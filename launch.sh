#!/bin/bash

# setup 
mkdir -p in
mkdir -p out
mkdir -p data
mkdir -p build

# compile 
make -B 

# convert mp4 to raw ( run it one time and then comment it )
# ./cvt_vid.sh v2r in/input.mp4 in/input.raw


# The program
./sobel in/input.raw out/output.raw



# convert raw to mp4 ( uncomment it when you want to verify your raw file )
./cvt_vid.sh r2v out/output.raw out/output.mp4
mplayer out/output.mp4


