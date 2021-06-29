project('tulip', 'c', version : '0.8.0', license : 'LGPL', default_options: 'default_library=static')

tulip_headers = files(['indicators.h'])

cc = meson.get_compiler('c')
libm_dep = cc.find_library('m', required: false)

subdir('indicators')
subdir('utils')

libindicators = library(
    'indicators',
    ['indicators_index.c'],
    dependencies: [libm_dep, tulip_indicators_dep, tulip_util_dep],
    install: true,
    version: '0.8.0'
)
libindicators_dep = declare_dependency(
  link_with : [libindicators],
  include_directories : include_directories('.')
)
install_headers(tulip_headers)

pkg = import('pkgconfig')
pkg.generate(
    libindicators,
    name: 'libindicators',
    description: 'technical analysis indicator library'
)

smoke_exe = executable(
    'smoke',
    ['smoke.c'],
    dependencies: [libindicators_dep]
)
test(
    'TI TEST SUITE',
    smoke_exe,
    workdir : meson.current_source_dir()
)

libtalib_dep = cc.find_library(
    'ta_lib',
    dirs : ['/usr/lib', '/usr/local/lib'],
    static : true
)
benchmark_exe = executable(
    'benchmark_tulip',
    ['benchmark.c'],
    dependencies: [libindicators_dep, libtalib_dep]
)
benchmark(
    'tulip benchmark',
    benchmark_exe,
    workdir : meson.current_source_dir()
)

