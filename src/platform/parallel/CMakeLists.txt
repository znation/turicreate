project(parallel)

if(APPLE)
  set(_parallel_platform_sources "$<TARGET_OBJECTS:platform_config>")
endif()

add_library(parallel OBJECT
  pthread_tools.cpp
  thread_pool.cpp
  execute_task_in_native_thread.cpp
)
add_dependencies(parallel boost)

if(APPLE)
  add_dependencies(parallel platform_config)
endif()