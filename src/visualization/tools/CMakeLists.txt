project(Turi)

add_executable(sassmodulec
  sassmodulec.cpp
)
add_dependencies(sassmodulec boost libsass)
target_link_libraries(sassmodulec boost libsass)