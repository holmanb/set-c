project('set-c', 'c',
  meson_version: '>=0.49.0',
  license: 'MIT',
  default_options: [
    'optimization=3',
    '-march=native'
  ],
  version : '1.0.0')

add_project_arguments([
  '-pedantic',
  '-Wundef',
  '-Wconversion',
  '-Wshadow',
  '-Wlogical-op',
  '-Winit-self',
  '-Wstrict-prototypes',
  '-Wendif-labels',
  '-Woverflow',
  '-Wno-missing-braces',
  '-Wno-missing-field-initializers',
  '-Wno-unused-parameter',
], language: 'c')

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
  'perf_0',
  'perf_1'
]

inc = include_directories('src')
install_headers('src/set.h')
lib = library('setc',
  lib,
  include_directories : inc,
  install : true)

foreach t : tests


  exe = executable(t,
      'test/' + t + '.c',
      include_directories : inc,
      link_with : lib)

  test(t, exe)

endforeach

foreach b : benchmarks
  exe = executable(b,
      'benchmark/' + b +'.c',
      include_directories : inc,
      link_with : lib)

  benchmark(b, exe)
endforeach
