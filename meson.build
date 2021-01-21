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
    'src/test.c',
    link_with : lib)

test2 = executable('example',
    'src/example.c',
    link_with : lib)

test('validate set-c', test1)
test('example set-c', test2)
