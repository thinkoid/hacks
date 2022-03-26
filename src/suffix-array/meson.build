libsuffix_array = static_library(
    'suffix_array', [ 'suffix_array.cc'],
    include_directories : includes,
    install : false)

t_example = executable('t', ['t.cc'],
    include_directories : includes,
    link_with : libsuffix_array,
    dependencies : [ boost_deps, benchmark_dep, range_dep ],
    install : false)
