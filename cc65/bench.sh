#!/bin/bash

set -e
set -x

# Compil test's main
~/workspace/nes/cc65/bin/ca65 bench.s
~/workspace/nes/cc65/bin/ca65 extra_data.s

# Build variants
variant() {
	local variant_name="$1"
	local options="$2"
	~/workspace/nes/cc65/bin/cc65 $options unzip.c -o unzip_${variant_name}_built.s
	~/workspace/nes/cc65/bin/ca65 unzip_${variant_name}_built.s
	~/workspace/nes/cc65/bin/ld65 --lib-path ~/workspace/nes/cc65/lib -o bench_${variant_name}.6502 -C ld65.cfg --dbgfile bench_${variant_name}.dbg bench.o unzip_${variant_name}_built.o extra_data.o nes.lib
}

variant "nooptim" ""
variant "all" "-O -Oi -Or -Os"

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

extract "nooptim"
extract "all"
