project('Lisp_interpreter', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

inc = include_directories('linenoise')

executable('lisp_interpreter',
           'repl.c',
           'linenoise/linenoise.c',
           include_directories : inc,
           c_args : ['-Wno-unused-parameter'],
           install : true)
