#!/bin/bash

rm -rf /tmp/bench_results 
mkdir -p /tmp/bench_results 

for compiler in `ls compilers`; do
	echo 
	echo ============
	echo $compiler 
	echo ============
	echo

	cd compilers/$compiler 

		./bench.sh 

		for variant in *.6502; do
			../../emulator/emulator $variant > /tmp/bench_results/${compiler}_${variant}.txt 
		done 

	cd -
done
