#!/bin/bash

set -e
set -x

# Setup environment
if [ -z "$VBCC" ]; then
	export VBCC=~/workspace/nes/6502_compil/c/vbcc6502/vbcc6502_linux/vbcc
	export PATH+=:$VBCC/bin
fi
script_dir="`dirname $0`"

# Compil test's main
~/workspace/nes/cc65/bin/ca65 bench.s

# Build variants
variant() {
	local variant_name="$1"
	local options="$2"
	vc $options unzip.c -S -o unzip_${variant_name}_built.s
	"${script_dir}/asm_convert" unzip_${variant_name}_built.s
	~/workspace/nes/cc65/bin/ca65 unzip_${variant_name}_built.s
	~/workspace/nes/cc65/bin/ld65 -o bench_${variant_name}.6502 -C ld65.cfg --dbgfile bench_${variant_name}.dbg bench.o unzip_${variant_name}_built.o
}

variant "o3" "-O3"
variant "o4" "-O4"

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

extract "o3"
extract "o4"
