#!/bin/sh
set -x

DATA=data


# verify argument $1

# verify dependencies
#[ -e perf ] || [ -e FlameGraph/stackcollapse-perf.pl ] || [ -e FlameGraph/flamegraph.pl ] || echo "must install dependencies (FlameGraph & perf)";exit
mkdir $DATA -p

perf record -F 99 -g "$1"
perf script > "$DATA/$1.perf"
FlameGraph/stackcollapse-perf.pl  "$DATA/$1.perf" > "$DATA/$1.folded"
FlameGraph/flamegraph.pl          "$DATA/$1.folded" > "$1.svg"


