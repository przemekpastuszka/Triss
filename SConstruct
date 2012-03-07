import fnmatch
import os

env = Environment(CPPPATH = os.getcwd())

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
    inputs = get_objects_at(sources)
    prog = env.Program('build/' + output, inputs, LIBS=['pthread'])
    env.Alias(output, prog)

build_project_with_tests('bob_test', ['prototypes/common/src', 'prototypes/bob/src', 'prototypes/bob/tests'])
build_project_with_tests('alice_test', ['prototypes/common/src', 'prototypes/alice/src', 'prototypes/alice/tests'])
build_project_with_tests('common_test', ['prototypes/common/src', 'prototypes/common/tests'])

build_project('benchmark', ['prototypes/bob/src','prototypes/common/src', 'prototypes/benchmark/src'])
