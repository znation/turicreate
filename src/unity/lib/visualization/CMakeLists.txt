project(unity)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/tcviz.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/tcviz.pb.h
  COMMAND protoc --cpp_out=${CMAKE_CURRENT_BINARY_DIR} -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src/external ${CMAKE_CURRENT_SOURCE_DIR}/tcviz.proto
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/tcviz.proto
  DEPENDS protoc ${CMAKE_CURRENT_SOURCE_DIR}/vega.3.0.proto
  COMMENT "Generating C++ from tcviz.proto"
  VERBATIM
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/vega.3.0.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/vega.3.0.pb.h
  COMMAND protoc --cpp_out=${CMAKE_CURRENT_BINARY_DIR} -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src/external ${CMAKE_CURRENT_SOURCE_DIR}/vega.3.0.proto
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/vega.3.0.proto
  DEPENDS protoc
  COMMENT "Generating C++ from vega.3.0.proto"
  VERBATIM
)

add_custom_target(
  GenerateVisualizationProtoSources
  DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/vega.3.0.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/vega.3.0.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/tcviz.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/tcviz.pb.h
)

make_library(
  visualization
  SOURCES
    boxes_and_whiskers.cpp
    categorical_heatmap.cpp
    groupby.cpp
    heatmap.cpp
    histogram.cpp
    hyperloglog.cpp
    item_frequency.cpp
    process_wrapper.cpp
    protobuf_generated.cpp
    scatter.cpp
    show.cpp
    thread.cpp
    transformation.cpp
    vega_data.cpp
    vega_spec.cpp
  REQUIRES
    protobuf
    EXTERNAL_VISIBILITY
)

add_dependencies(
  visualization
  GenerateVisualizationProtoSources
)
