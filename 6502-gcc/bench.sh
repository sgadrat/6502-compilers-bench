#!/bin/bash

# Build variants
6502-gcc -Os unzip.c -S -o unzip_os.s
6502-gcc -O3 unzip.c -S -o unzip_o3.s
