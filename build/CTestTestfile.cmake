# CMake generated Testfile for 
# Source directory: /home/mnegri/cantiere
# Build directory: /home/mnegri/cantiere/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[boids.t]=] "/home/mnegri/cantiere/build/Debug/boids.t")
  set_tests_properties([=[boids.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/mnegri/cantiere/CMakeLists.txt;53;add_test;/home/mnegri/cantiere/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[boids.t]=] "/home/mnegri/cantiere/build/Release/boids.t")
  set_tests_properties([=[boids.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/mnegri/cantiere/CMakeLists.txt;53;add_test;/home/mnegri/cantiere/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[boids.t]=] "/home/mnegri/cantiere/build/RelWithDebInfo/boids.t")
  set_tests_properties([=[boids.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/mnegri/cantiere/CMakeLists.txt;53;add_test;/home/mnegri/cantiere/CMakeLists.txt;0;")
else()
  add_test([=[boids.t]=] NOT_AVAILABLE)
endif()
