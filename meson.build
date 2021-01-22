project('set-c', 'c',
  version : '1.0.0')

lib = files([
  'src/set.c',
  'src/util.c'
])


inc = include_directories('src')
lib = library('setc',
  lib,
  include_directories : inc,
  install : true)

test1 = executable('initial-test',
    'test/test.c',
    include_directories : inc,
    link_with : lib)

test2 = executable('example',
    'test/example.c',
    include_directories : inc,
    link_with : lib)

test('validate set-c', test1)
test('example set-c', test2)
