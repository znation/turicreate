project(unity_toolkits)

make_library(supervised_learning
  SOURCES
    supervised_learning.cpp
    linear_regression.cpp
    linear_regression_opt_interface.cpp
    logistic_regression.cpp
    logistic_regression_opt_interface.cpp
    linear_svm.cpp
    linear_svm_opt_interface.cpp
    xgboost.cpp
    xgboost_iterator.cpp
    boosted_trees.cpp
    random_forest.cpp
    decision_tree.cpp
    xgboost_error.cpp
    automatic_model_creation.cpp
    classifier_evaluations.cpp
    class_registrations.cpp
  REQUIRES
    unity_core
    unity_util
    unity_ml_model
    unity_coreml_model_export
    minipsutil_static
    ml_data
    optimization
    xgboost
    numerics
  COMPILE_FLAGS_EXTRA
    -DXGBOOST_CUSTOMIZE_MSG_
  COMPILE_FLAGS_EXTRA_GCC
    -Wno-unknown-pragmas  # NOTE: used for auto-generated protobuf source files
    -Wno-unused-function  # NOTE: used for auto-generated protobuf source files
  )