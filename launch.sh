#!/bin/bash

# setup 
mkdir -p in
mkdir -p out
mkdir -p data
mkdir -p build

rm out/*.raw

# compile 

# convert mp4 to raw ( run it one time and then comment it )
# ./cvt_vid.sh v2r in/input.mp4 in/input.raw


# The program
 # make -B DEFINE=BASELINE
 # ./sobel in/input.raw out/output.raw
 #
 # make -B DEFINE=OPT1
 # ./sobel in/input.raw out/output.raw
 #
 # make -B DEFINE=OPT2
 # ./sobel in/input.raw out/output.raw


 make -B DEFINE=AVX2
 ./sobel in/input.raw out/output.raw

  # make -B DEFINE=AVX2_2
  # ./sobel in/input.raw out/output.raw
  #
  # make -B DEFINE=AVX2_3
  # ./sobel in/input.raw out/output.raw


 make -B DEFINE=CL
 ./sobel in/input.raw out/output.raw

 make -B DEFINE=CL_AVX
 ./sobel in/input.raw out/output.raw


 # make -B DEFINE=ONED
 # ./sobel in/input.raw out/output.raw




# 15 opt1
# 19 avx2
# 4.4 baseline
# 11 opt2
# 22 avxv2
# 17 avxV3


# convert raw to mp4 ( uncomment it when you want to verify your raw file )
./cvt_vid.sh r2v out/output.raw out/output.mp4

mplayer out/output.mp4

sha1sum out/output.raw
# v0 ??

# v1, smallerize 
# 141be9138b6e1c4635c5a8c41a2d3306dc6b2d1f  out/output.raw



