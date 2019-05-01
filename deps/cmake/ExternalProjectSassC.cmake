ExternalProject_Add(ex_sassc
  PREFIX ${CMAKE_SOURCE_DIR}/deps/build/sassc
  URL ${CMAKE_SOURCE_DIR}/deps/src/sassc-3.5.0/
  INSTALL_DIR ${CMAKE_SOURCE_DIR}/deps/local
  CONFIGURE_COMMAND ""
  BUILD_COMMAND env CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} "CFLAGS=-fPIC ${ARCH_FLAG} ${C_REAL_COMPILER_FLAGS}" PREFIX=${CMAKE_SOURCE_DIR}/deps/local SASS_LIBSASS_PATH=${CMAKE_SOURCE_DIR}/deps/build/libsass/src/ex_libsass make -j4 -C ../ex_sassc
  INSTALL_COMMAND cp ../ex_sassc/bin/sassc ${CMAKE_SOURCE_DIR}/deps/local/bin/
  BUILD_BYPRODUCTS ${CMAKE_SOURCE_DIR}/deps/local/bin/sassc
  BUILD_IN_SOURCE 0
  DEPENDS ex_libsass
)

add_executable(sassc IMPORTED)
set_property(TARGET sassc PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/deps/local/bin/sassc)
add_dependencies(sassc ex_sassc)
