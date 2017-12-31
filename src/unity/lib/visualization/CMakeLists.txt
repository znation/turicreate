project(unity)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tcviz.pb.cc
  COMMAND protoc --cpp_out=${CMAKE_CURRENT_BINARY_DIR} -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src/external ${CMAKE_CURRENT_SOURCE_DIR}/tcviz.proto
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/tcviz.proto
  DEPENDS protoc
  COMMENT "Generating C++ Vega spec sources from .proto"
  VERBATIM
)

set(VISUALIZATION_SOURCES
  boxes_and_whiskers.cpp
  categorical_heatmap.cpp
  process_wrapper.cpp
  groupby.cpp
  heatmap.cpp
  histogram.cpp
  hyperloglog.cpp
  item_frequency.cpp
  scatter.cpp
  show.cpp
  thread.cpp
  transformation.cpp
  vega_data.cpp
  vega_spec.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/vega.3.0.pb.cc
  ${CMAKE_CURRENT_BINARY_DIR}/tcviz.pb.cc
)

make_library(visualization
  SOURCES
    ${VISUALIZATION_SOURCES}
  REQUIRES
    protobuf
    EXTERNAL_VISIBILITY
)
