ExternalProject_Add(ex_libsass
  PREFIX ${CMAKE_SOURCE_DIR}/deps/build/libsass
  URL ${CMAKE_SOURCE_DIR}/deps/src/libsass-3.5.5/
  INSTALL_DIR ${CMAKE_SOURCE_DIR}/deps/local
  CONFIGURE_COMMAND ""
  BUILD_COMMAND env CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} "CFLAGS=-fPIC ${ARCH_FLAG} ${C_REAL_COMPILER_FLAGS}" SASS_LIBSASS_PATH=${CMAKE_SOURCE_DIR}/deps/src/libsass-3.5.5/ PREFIX=${CMAKE_SOURCE_DIR}/deps/local make -j4 ../ex_libsass
  INSTALL_COMMAND ""
  BUILD_BYPRODUCTS ${CMAKE_SOURCE_DIR}/deps/local/lib/libsass.a
  BUILD_IN_SOURCE 0
)

add_library(sass STATIC IMPORTED)
set_property(TARGET sass PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/deps/local/lib/libsass.a)
add_dependencies(sass ex_libsass)
