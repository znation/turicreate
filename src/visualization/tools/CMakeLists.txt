project(Turi)

add_executable(sassmodulec
  sassmodulec.cpp
)
add_dependencies(sassmodulec boost)
target_link_libraries(sassmodulec boost)