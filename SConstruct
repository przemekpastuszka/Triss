import fnmatch
import os

AddOption('--release', dest = 'release', action = 'store_true',
    default = False, help ='build with release flags')

simple_env = Environment(CPPPATH = os.getcwd())
if GetOption('release'):
    env = simple_env.Clone(CCFLAGS = ['-O2'])
else:
    env = simple_env.Clone(CCFLAGS = ['-g', '-O0'])

def get_all_files_at(path):
    files = []
    for root, dirnames, filenames in os.walk(path):
      for filename in fnmatch.filter(filenames, '*.cpp'):
          files.append(os.path.join(root, filename))
    return files

def get_objects_at(paths):
    return [env.Object(get_all_files_at(path)) for path in paths]

def build_project_with_tests(output, sources):
    main_test_object = Object('tests/test.cpp')
    inputs = get_objects_at(sources) + [main_test_object]
    prog = env.Program('build/' + output, inputs, LIBS=['gtest', 'pthread'])
    env.Alias(output, prog)

def build_project(output, sources):
    # if scons does not know where to search for boost libraries
    # you may want to uncomment two following lines and maybe
    # adjust them to your boost installation path
    #env.Append(LIBPATH = ['/usr/local/lib'])
    #env.Append(CPPPATH = ['/usr/local/include/boost'])
    env.Append(LIBS = ['boost_program_options', 'boost_thread'])
    inputs = get_objects_at(sources)
    prog = env.Program('build/' + output, inputs)
    env.Alias(output, prog)

build_project_with_tests('engine_test', ['utils/src', 'src/engine', 'src/common', 'tests/engine'])
build_project_with_tests('common_test', ['src/common', 'tests/common'])

# build_project_with_tests('bob_test', ['utils/src', 'prototypes/common/src', 'prototypes/bob/src', 'prototypes/bob/tests'])
# build_project_with_tests('alice_test', ['prototypes/common/src', 'prototypes/alice/src', 'prototypes/alice/tests'])
# build_project_with_tests('prototype_common_test', ['prototypes/common/src', 'prototypes/common/tests'])
build_project_with_tests('utils_test', ['utils/src', 'utils/tests'])

build_project('benchmark', ['prototypes/bob/src','prototypes/alice/src','prototypes/common/src', 'prototypes/benchmark/src'])
