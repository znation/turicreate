project(image)


#/**************************************************************************/
#/*                                                                        */
#/*                             Logger Library                             */
#/*                                                                        */
#/**************************************************************************/
make_library(image_type
  SOURCES
    image_type.cpp
    image_util_impl.cpp
  REQUIRES
    logger
    image_io
    EXTERNAL_VISIBILITY
)

make_executable(io_example
  SOURCES
    io_example.cpp
  REQUIRES
    image_io
)

#set to native if supported
check_cxx_compiler_flag(-Wno-c++11-narrowing HAS_NARROWING)

if(HAS_NARROWING)
  set(EXTRA_FLAGS "-Wno-c++11-narrowing")
endif()

make_library(image_io
        SOURCES
    io.cpp
    jpeg_io.cpp
    png_io.cpp
    REQUIRES
      libpng
      libjpeg
      logger
      fileio
    EXTERNAL_VISIBILITY
    COMPILE_FLAGS_EXTRA
      ${EXTRA_FLAGS}
  )