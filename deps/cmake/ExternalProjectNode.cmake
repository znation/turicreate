set(EXTRA_CONFIGURE_FLAGS "")
if(WIN32 AND ${MSYS_MAKEFILES})
  set(EXTRA_CONFIGURE_FLAGS --build=x86_64-w64-mingw32)
endif()
if(APPLE AND TC_BUILD_IOS)
  set(EXTRA_CONFIGURE_FLAGS --host=arm-apple-darwin)
endif()

add_custom_target(ex_node_copy_source
  COMMAND rsync -a ${CMAKE_SOURCE_DIR}/deps/src/node-v10.15.3/ ${CMAKE_SOURCE_DIR}/deps/build/node-v10.15.3/
  COMMENT "Copying node.js source from deps/src to deps/build"
  VERBATIM
)

add_custom_target(ex_node
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps/build/node-v10.15.3/
  DEPENDS ex_node_copy_source
  COMMAND env CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} "CFLAGS=-fPIC ${ARCH_FLAG} ${C_REAL_COMPILER_FLAGS}" ./configure --prefix=${CMAKE_SOURCE_DIR}/deps/local ${EXTRA_CONFIGURE_FLAGS}
  COMMAND make -j4
  COMMAND make install
  COMMENT "Building node.js from source"
  VERBATIM
)

add_executable(node IMPORTED)
set_property(TARGET node PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/deps/local/bin/node)
add_dependencies(node ex_node)
