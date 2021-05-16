#!/bin/bash
set -x

[ -z "$1" ] && echo "need to pass test executiable" && exit 1

FULL=$(realpath "$0")
ABS=$(dirname "$FULL")
EXE=$(echo "$1" | awk -F/ '{print $NF}')

DATADIR="$ABS/data"
TMPDIR="$DATADIR/tmp"
SVGDIR="$DATADIR/svg"

EXEDATA="$TMPDIR/$EXE.data"
PERFDATA="$TMPDIR/$EXE.perf"

mkdir "$DATADIR" -p
mkdir "$TMPDIR" -p
mkdir "$SVGDIR" -p

perf record -F 99 -g -o "$EXEDATA" "$1"
perf script -i "$EXEDATA" > "$PERFDATA"
"$ABS/FlameGraph/stackcollapse-perf.pl" "$PERFDATA" | "$ABS/FlameGraph/flamegraph.pl"  > "$SVGDIR/$EXE.svg"
