project(startup_teardown)

make_library(startup_teardown
  SOURCES
    startup_teardown.cpp
  REQUIRES
    logger serialization random crash_handler pylambda minipsutil_static
  )