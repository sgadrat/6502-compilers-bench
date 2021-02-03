#!/bin/bash

#
# Script to run tests in a simplified command line.
#
# Sets environment variables, and outputs human redable report.
#
# This is a personal script, it should be improved to meet minimal quality.
# In the meantime, you can replace environment variables to fit your setup
# and play with it.
#
# Official way to start tests is described in README.rst
#

#TODO Get environment variables from a non-versioned config file
#TODO Show a human readable error if the needed compiler is not available
#TODO Handle build errors gracefuly
#TODO Handle bench scripts outputing garbage on stdout gracefuly
#TODO Help message when arguments don't match expectation

if [ "$1" == "regular" ]; then
	export CA65_BIN=~/workspace/nes/cc65/bin/ca65
	export LD65_BIN=~/workspace/nes/cc65/bin/ld65
	export CC65_BIN=~/workspace/nes/cc65/bin/cc65
	export CC65_LIB=~/workspace/nes/cc65/lib
elif [ "$1" == "master" ]; then
	export CA65_BIN=~/tmp/tmp_de_ouf/cc65/bin/ca65
	export LD65_BIN=~/tmp/tmp_de_ouf/cc65/bin/ld65
	export CC65_BIN=~/tmp/tmp_de_ouf/cc65/bin/cc65
	export CC65_LIB=~/tmp/tmp_de_ouf/cc65/lib
else
	echo unknown cc65 version
	exit 1
fi

export VBCC=~/workspace/nes/6502_compil/c/vbcc6502/vbcc6502_linux/vbcc
export PATH=$PATH:~/workspace/nes/6502_compil/c/vbcc6502/vbcc6502_linux/vbcc/bin
export KICKC_BIN=~/workspace/nes/6502_compil/c/kickc/bin/kickc.sh

shift
compiler=$1
shift
options=$1
shift

pyformat=`cat <<EOF
import json, sys, textwrap
info = json.load(sys.stdin)

print("summary")
print("=======")
print("")
print("compilation: \`\`{} {}\`\`".format("$compiler", "$options"))
print("")

print("perf")
print("====")
print("")
print("code size: {} bytes".format(info["code_size"]))
print("cycles count: {} cycles".format(info["cycle_count"]))
print("")

print("generated asm")
print("=============")
for source in info["asm"]:
	print("")
	print(source["source"])
	print("=" * len(source["source"]))
	print("")
	print("::")
	print("")
	print("{}".format(textwrap.indent(source["asm"], "\t")))
	print("")

print("memory dump")
print("===========")
print("")
for i in range(len(info["ram"])):
	if i % 16 == 0:
		print("\n\t{:03x}  ".format(i), end="")
	print(" {:02x}".format(info["ram"][i]), end="")
print("")
EOF
`

rm -rf /tmp/c65_bench/
if [ -z "$RAW" ]; then
	compilers/$compiler/bench --options="$options" -v $@ | python -c "$pyformat"
else
	compilers/$compiler/bench --options="$options" -v $@
fi
