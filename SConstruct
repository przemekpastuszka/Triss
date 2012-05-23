import fnmatch
import os

AddOption('--release', dest = 'release', action = 'store_true',
    default = False, help ='build with release flags')

simple_env = Environment(CPPPATH = os.getcwd())
if GetOption('release'):
    env = simple_env.Clone(CCFLAGS = ['-O2'])
else:
    env = simple_env.Clone(CCFLAGS = ['-g', '-O0'])

def get_all_files_at(path, ext):
    files = []
    for root, dirnames, filenames in os.walk(path):
      for filename in fnmatch.filter(filenames, ext):
          files.append(os.path.join(root, filename))
    return files

def get_objects_at(paths, build_command):
    return [build_command(get_all_files_at(path, '*.cpp')) for path in paths]

def build_project_with_tests(output, sources):
    main_test_object = Object('triss/tests/test.cpp')
    inputs = get_objects_at(sources, lambda x: env.Object(x)) + [main_test_object]
    prog = env.Program('build/' + output, inputs, LIBS=['gtest', 'pthread', 'boost_serialization', 'boost_system'])
    env.Alias(output, prog)

def build_project_using_command(output, sources, build_command, link_command):
    # if scons does not know where to search for boost libraries
    # you may want to uncomment two following lines and maybe
    # adjust them to your boost installation path
    #env.Append(LIBPATH = ['/usr/local/lib'])
    #env.Append(CPPPATH = ['/usr/local/include/boost'])
    env.Append(LIBS = ['boost_program_options', 'boost_thread', 'boost_serialization', 'boost_system'])
    inputs = get_objects_at(sources, build_command)
    prog = link_command('build/' + output, inputs)
    env.Alias(output, prog)
    return prog

def build_runnable(output, sources):
    return build_project_using_command(output, sources,
        lambda x: env.Object(x),
        lambda x, y: env.Program(x, y))

def build_library(output, sources):
    return build_project_using_command(output, sources,
        lambda x: env.SharedObject(x),
        lambda x, y: env.SharedLibrary(x, y))

def install():
    sources = ['triss/src/utils', 'triss/src/engine', 'triss/src/common', 'triss/src/db']
    lib = build_library('triss', sources)
    env.Alias('make', lib)

    lib_installation = env.Install("/usr/lib", lib)
    env.Alias('install', lib_installation)
    for source in sources:
        for header in get_all_files_at(source, "*.h"):
            env.Alias('install', env.Install(
                os.path.join("/usr/include/", os.path.dirname(header)),
                header))

# prototypes
# build_project_with_tests('bob_test', ['triss/src/utils', 'prototypes/common/src', 'prototypes/bob/src', 'prototypes/bob/tests'])
# build_project_with_tests('alice_test', ['prototypes/common/src', 'prototypes/alice/src', 'prototypes/alice/tests'])
# build_project_with_tests('prototype_common_test', ['prototypes/common/src', 'prototypes/common/tests'])

# tests
build_project_with_tests('engine_test', ['triss/src/utils', 'triss/src/engine', 'triss/src/common', 'triss/tests/engine'])
build_project_with_tests('common_test', ['triss/src/common', 'triss/tests/common', 'triss/src/engine'])
build_project_with_tests('utils_test', ['triss/src/utils', 'triss/tests/utils'])
# build_project_with_tests('client_test', ['triss/src/client', 'triss/tests/client'])

# runnables
# build_runnable('benchmark', ['prototypes/bob/src','prototypes/alice/src','prototypes/common/src', 'prototypes/benchmark/src'])
# build_runnable('database', ['triss/src/db/', 'triss/src/engine', 'triss/src/common', 'triss/src/client'])

install()
