project('Lisp_interpreter', 'c',
  version : '0.2',
  default_options : ['warning_level=3'])

  mpc_lib = library('mpc',
    sources: ['mpc/mpc.c'],
    include_directories: include_directories('mpc')
  )

  linenoise_lib = library('linenoise',
    sources: ['linenoise/linenoise.c'],
    include_directories: include_directories('linenoise')
  )

  executable('lisp_repl',
    sources: ['repl.c'],
    link_with: [linenoise_lib, mpc_lib],
    c_args : ['-Wno-unused-parameter'],
    install : true)

  executable('doge',
    sources: ['doge_example.c'],
    link_with: [mpc_lib],
    c_args : ['-Wno-unused-parameter'],
    install : true)
