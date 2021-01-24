project('set-c', 'c',
  meson_version: '>=0.49.0',
  license: 'MIT',
  version : '1.0.0')

# validate memory issues
add_test_setup('valgrind',
  exe_wrapper : ['valgrind', '--error-exitcode=1', '--leak-check=full', '--track-origins=yes', '--exit-on-first-error=yes'],
  is_default : false)

# generate flamegraphs w/perf
add_test_setup('perf',
  exe_wrapper : ['../benchmark/perf.sh'],
  is_default : false)

lib = files([
  'src/set.c',
  'src/util.c'
])

tests = [
  'example',
  'member',
  'init',
  'add',
  'length_delete',
  'adt1',
  'adt2',
  'adt3',
  'set-ops',
]
benchmarks = [
  'perf_0'
]

inc = include_directories('src')
install_headers('src/set.h')
lib = library('setc',
  lib,
  include_directories : inc,
  install : true)

foreach t : tests

  path = 'test/' + t +'.c'

  exe = executable(t,
      path,
      include_directories : inc,
      link_with : lib)

  test(t, exe)

endforeach

foreach b : benchmarks 
  path = 'benchmark/' + b +'.c'
  exe = executable(b,
      path,
      include_directories : inc,
      link_with : lib)

  benchmark(b, exe)
endforeach
