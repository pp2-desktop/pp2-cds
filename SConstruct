import os

env = Environment()

env.ParseConfig('pkg-config --cflags --libs openssl')

env.Append(CXXFLAGS=['-std=c++11', '-Wall', '-g'])

env.Append(CPPPATH=['/usr/local/include'])


env.Append(LIBS=['pthread', 'boost_system', 'boost_regex', 'boost_thread', 'boost_filesystem', 'boost_program_options', 'boost_coroutine', 'boost_context'])


env.Append(LIBPATH=['/usr/local/lib'])


env.Program('ws_example', ['ws_examples.cpp', 'vs_room.cpp', 'cd_user.cpp', 'vs_room_md.cpp', 'cd_handler_md.cpp', 'json11.cpp', 'handler/join_room.cpp'])
