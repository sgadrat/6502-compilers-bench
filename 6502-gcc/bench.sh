#!/bin/bash

# Build variants
6502-gcc -Os unzip.c -S -o unzip_os_built.s
~/workspace/nes/cc65/bin/ca65 unzip_os_built.s

6502-gcc -O3 unzip.c -S -o unzip_o3_built.s
~/workspace/nes/cc65/bin/ca65 unzip_o3_built.s

#
# Build bench program
#

~/workspace/nes/cc65/bin/ca65 bench.s
~/workspace/nes/cc65/bin/ld65 -o bench.nes -C ld65.cfg bench.o
