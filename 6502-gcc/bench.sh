#!/bin/bash

set -e
set -x

# Build variants
6502-gcc -Os unzip.c -S -o unzip_os_built.s
~/workspace/nes/cc65/bin/ca65 unzip_os_built.s

6502-gcc -O3 unzip.c -S -o unzip_o3_built.s
~/workspace/nes/cc65/bin/ca65 unzip_o3_built.s

#
# Build bench program
#

~/workspace/nes/cc65/bin/ca65 bench.s
~/workspace/nes/cc65/bin/ld65 -o bench_os.6502 -C ld65.cfg bench.o unzip_os_built.o
~/workspace/nes/cc65/bin/ld65 -o bench_o3.6502 -C ld65.cfg bench.o unzip_o3_built.o
