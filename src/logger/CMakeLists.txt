project(Turi)

#/**************************************************************************/
#/*                                                                        */
#/*                             Logger Library                             */
#/*                                                                        */
#/**************************************************************************/

make_library(logger
  SOURCES
    error.cpp
    logger.cpp
    backtrace.cpp
    log_rotate.cpp
    log_level_setter.cpp
    $<TARGET_OBJECTS:parallel>
    $<TARGET_OBJECTS:platform_config>
    $<TARGET_OBJECTS:stack_trace>
    $<TARGET_OBJECTS:timer>
  REQUIRES
    boost
    EXTERNAL_VISIBILITY
  )
add_dependencies(logger
  parallel
  stack_trace
  timer
)