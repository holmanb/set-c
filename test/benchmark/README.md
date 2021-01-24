# Benchmarking
perf.sh will generate a flamegraph for the executable passed to it and store the results in data/svg/

## Initialization
if FlameGraph wasn't recursively initialized (`git submodule update --init --recursive`) on the initial clone of this repo, make sure to initialize it
```
cd test/benchmark/FlameGraph
git submodule update --recursive
```

## Usage
this is integrated into meson such that the following command will execute all benchmarks using `perf` and create a flamegraph using Brendan Gregg's FlameGraph tooling
```
meson test --benchmark --setup=perf
```
