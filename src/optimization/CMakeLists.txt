project(optimization)

add_library(optimization OBJECT
  utils.cpp
  lbfgs.cpp
  optimization_interface.cpp
)
