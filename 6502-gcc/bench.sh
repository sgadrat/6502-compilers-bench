#!/bin/bash

set -e
#set -x

# Compil test's main
~/workspace/nes/cc65/bin/ca65 bench.s

# Build variants
variant() {
	local variant_name="$1"
	local options="$2"
	6502-gcc $options unzip.c -S -o unzip_${variant_name}_built.s
	~/workspace/nes/cc65/bin/ca65 unzip_${variant_name}_built.s
	~/workspace/nes/cc65/bin/ld65 -o bench_${variant_name}.6502 -C ld65.cfg --dbgfile bench_${variant_name}.dbg bench.o unzip_${variant_name}_built.o
}

variant "os" "-Os"
variant "o3" "-O3"

#
# Extract metrics
#

set +x

extract() {
	local variant_name="$1"

	echo
	echo unzip_${variant_name}
	echo ========
	echo

	#seg id=0,name="CODE",start=0x008013,size=0x00DB,addrsize=absolute,type=ro,oname="bench_o3.6502",ooffs=19
	local size_hex=`grep 'name="CODE",start=0x008' bench_${variant_name}.dbg | grep -Eo 'size=0x[^,]+' | grep -Eo '[0-9A-F]+$'`
	local size=$((16#$size_hex))

	echo "code size: $size"
}

extract "os"
extract "o3"
