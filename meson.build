project('hacks', 'cpp', version : '0.0.1', license : 'GPL',
        default_options : [ 'cpp_std=c++20' ])

includes = include_directories('include')

boost_deps = dependency('boost', modules : [
    'filesystem', 'iostreams', 'program_options', 'system', 'thread', 'timer' ])

range_dep = dependency('range-v3')
benchmark_dep = dependency('benchmark')

subdir('src')
