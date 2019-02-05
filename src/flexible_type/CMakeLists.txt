project(flexible_type)

#/**************************************************************************/
#/*                                                                        */
#/*                             Logger Library                             */
#/*                                                                        */
#/**************************************************************************/

make_library(flexible_type
  SOURCES
    flexible_type.cpp
    string_escape.cpp
    flexible_type_conversion_utilities.cpp
    flexible_type_spirit_parser.cpp
    $<TARGET_OBJECTS:image_type>
  REQUIRES
    logger
    EXTERNAL_VISIBILITY
  )
add_dependencies(flexible_type image_type)
