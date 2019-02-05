project(image)


#/**************************************************************************/
#/*                                                                        */
#/*                             Logger Library                             */
#/*                                                                        */
#/**************************************************************************/
add_library(image_type OBJECT
  image_type.cpp
  image_util_impl.cpp
)
add_dependencies(image_type
  logger
  image_io
)

make_executable(io_example
  SOURCES
    io_example.cpp
    $<TARGET_OBJECTS:image_io>
    $<TARGET_OBJECTS:fileio>
  REQUIRES
    # deps & external
    aws-sdk-cpp
    curl
    libpng
    libjpeg

    # in-project static libraries
    # TODO - migrate these to object libs
    cancel_serverside_ops
    logger
    globals
    process
    random
    util
)
add_dependencies(io_example image_io)

add_library(image_io OBJECT
  io.cpp
  jpeg_io.cpp
  png_io.cpp
)

add_dependencies(image_io
  libpng
  libjpeg
)

#set to native if supported
check_cxx_compiler_flag(-Wno-c++11-narrowing HAS_NARROWING)

if(HAS_NARROWING)
  target_compile_options(image_io PRIVATE "-Wno-c++11-narrowing")
endif()
