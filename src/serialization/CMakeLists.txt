project(serialization)

make_library(serialization
  SOURCES
    dir_archive.cpp
    $<TARGET_OBJECTS:fileio>
  REQUIRES
    cancel_serverside_ops
    logger
    globals
    process
    util
    random
    EXTERNAL_VISIBILITY
  )
add_dependencies(serialization fileio)
