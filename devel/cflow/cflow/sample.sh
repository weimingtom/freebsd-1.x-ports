#!/bin/sh
# This script prints the function call hierarchy of the 386BSD kernel source.
# It creates three files in the working directory:
#	386BSD-cflow.sh	- a script to parse the kernel source
#	386BSD.t - table generated by 386BSD-cflow.sh
#	386BSD.g - graph of the kernel's function call hierarchy
# To run it, create a kernel Makefile, i.e.,
# 	$ cd /sys/i386/conf
#	$ config SYSTEM_NAME
#	$ cd /sys/compile/SYSTEM_NAME
#	$ make clean
#	$ make depend
# Then copy this script to /sys/compile/SYSTEM_NAME and run it:
#	$ cp ~public/cflow/sample.sh .
#	$ sh sample.sh

trap 'rm -f 386BSD.t 386BSD.g 386BSD-cflow.sh; exit 2' 1 2 3 15

# make a script, 386BSD-cflow.sh, to parse the 386BSD kernel source
echo ':' >386BSD-cflow.sh
echo '# This script parses the 386BSD kernel source to 386BSD.t' >>386BSD-cflow.sh
echo '>386BSD.t' >>386BSD-cflow.sh
#echo '>err.tmp' >>386BSD-cflow.sh
make -n |
sed '1,2d;/^echo/,$d;s/^cc[ 	]\(.*$\)/cc -E \1 | prcc -gv >>386BSD.t 2>\/dev\/null/' \
>>386BSD-cflow.sh
#sed '1,2d;/^echo/,$d;s/^cc[ 	]\(.*$\)/cc -E \1 | prcc -gv >>386BSD.t 2>>err.tmp/' \
#>>386BSD-cflow.sh

# run the parse script
sh 386BSD-cflow.sh

# print the parse graph
prcg <386BSD.t >386BSD.g
expand -4 386BSD.g | more

# or, print an inverted parse graph
# sed 's/\(.*	\)\(.*	\)/\2\1/' 386BSD.t |
# sort +0 -1 +2 |
# uniq |
# prcg -i >386BSD.ig
