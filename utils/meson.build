tulip_util_sources = (['buffer.c'])

tulip_util = static_library(
    'tulip_util',
    tulip_util_sources
)

tulip_util_dep = declare_dependency(
  link_with : tulip_util,
  include_directories : include_directories('.')
)
