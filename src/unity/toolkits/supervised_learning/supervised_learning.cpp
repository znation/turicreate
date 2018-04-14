/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#include <unity/lib/unity_sframe.hpp>
#include <unity/lib/gl_sarray.hpp>
#include <unity/lib/gl_sframe.hpp>
#include <unity/lib/unity_global.hpp>
#include <unity/lib/variant_deep_serialize.hpp>
#include <flexible_type/flexible_type.hpp>

#include <sframe/algorithm.hpp>
#include <sframe/sarray_reader.hpp>

// Toolkits
#include <optimization/optimization_interface.hpp>
#include <toolkits/supervised_learning/xgboost.hpp>
#include <toolkits/supervised_learning/supervised_learning.hpp>
#include <toolkits/supervised_learning/supervised_learning_utils-inl.hpp>

// ML Data
#include <ml_data/ml_data_iterator.hpp>

// Evaluation
#include <unity/toolkits/evaluation/metrics.hpp>

namespace turi {
namespace supervised {

constexpr size_t WIDE_DATA = 200;

// TODO: List of todo's for this file
//------------------------------------------------------------------------------


/**
 * Create a supervised learning model.
 */
EXPORT std::shared_ptr<supervised_learning_model_base> create(
               sframe X, sframe y, std::string model_name,
               const variant_map_type& kwargs){

  // Construct an object of the right type.
  // --------------------------------------------------------------------------
  std::shared_ptr<supervised_learning_model_base> model;
   model = std::static_pointer_cast<supervised_learning_model_base>(
               get_unity_global_singleton()->create_toolkit_class(model_name));

  // Error handling.
  // --------------------------------------------------------------------------
  bool support_image_type = false; // used to be neural net
  check_empty_data(X);
  check_feature_column_types(X, support_image_type);
  check_target_column_type(model_name, y);

  // Initialize
  // ----------------------------------------------------------------------
  ml_missing_value_action missing_value_action =
    model->support_missing_value() ? ml_missing_value_action::USE_NAN
                                   : ml_missing_value_action::ERROR;

  if (kwargs.count("features_validation") == 0) {
    sframe valid_X, valid_y;
    model->init(X, y, valid_X, valid_y, missing_value_action);
  } else {

    // Validation data checking and initialization
    sframe valid_X = *(safe_varmap_get<std::shared_ptr<unity_sframe>>(
        kwargs, "features_validation")->get_underlying_sframe());
    sframe valid_y = *(safe_varmap_get<std::shared_ptr<unity_sframe>>(
        kwargs, "target_validation")->get_underlying_sframe());
    check_feature_column_types(valid_X, support_image_type);
    check_target_column_type(model_name, valid_y);
    check_feature_column_types_match(X, valid_X);
    model->init(X, y, valid_X, valid_y, missing_value_action);
  }

  // Training
  // --------------------------------------------------------------------------
  std::map<std::string, flexible_type> opts;
  for (const auto& kvp: kwargs) {
    if (get_variant_which_name(kvp.second.which()) == "flexible_type"){
      opts[kvp.first] = variant_get_value<flexible_type>(kvp.second);
    }
  }

  model->init_options(opts);
  model->train();
  return model;
}

/**
 * Init function common to all supervised_learning inits.
 */
void supervised_learning_model_base::init(const sframe& X, const sframe& y,
                                          const sframe& valid_X,
                                          const sframe& valid_y,
                                          ml_missing_value_action missing_value_action) {
  DASSERT_TRUE(y.num_columns() == 1);

  // Setup the options for ml_data construction.
  std::string target_col = y.column_name(0);
  std::map<std::string, ml_column_mode> mode_overides;
  std::vector<std::string> sorted_columns;
  if(this->is_classifier()) {
    mode_overides[target_col] = ml_column_mode::CATEGORICAL_SORTED;
    sorted_columns = {target_col};
  }

  // Error out if target column has missing values
  auto target_sa = std::make_shared<unity_sarray>();
  target_sa->construct_from_sarray(y.select_column(0));
  bool target_has_na = ((gl_sarray)target_sa).apply(
      [](const flexible_type& x) {
        return x.get_type() == flex_type_enum::UNDEFINED;
      },
      flex_type_enum::INTEGER,
      false /* don't skip nan */
    ).any();

  if (target_has_na) {
    log_and_throw("Target column has missing value. \
                   Please use dropna() to drop rows with missing target values.");
  }

  // Construct the ml_data.
  ml_data data;
  sframe sf_data = X.add_column(y.select_column(0), target_col);
  data.fill(sf_data, target_col, mode_overides, false, missing_value_action);
  this->ml_mdata = data.metadata();

  // Update the model
  std::vector<std::string> feature_names =
                   get_feature_names_from_metadata(this->ml_mdata);
  std::vector<std::string> feature_column_names =
                   get_feature_column_names_from_metadata(this->ml_mdata);

  this->state["target"] =  to_variant((this->ml_mdata)->target_column_name());
  this->state["unpacked_features"] = to_variant(feature_names);
  this->state["features"] = to_variant(feature_column_names);
  this->state["num_examples"] = X.num_rows();
  this->state["num_features"] = feature_column_names.size();
  this->state["num_unpacked_features"] = feature_names.size();

  // Check the number of dimensions in this dataset is small, otherwise warn the
  // user. (see  #3001 for context)
  size_t num_dims = get_number_of_coefficients(this->ml_mdata);
  if(num_dims >= X.num_rows()) {
    std::stringstream ss;
    ss << "WARNING: The number of feature dimensions in this problem is "
       << "very large in comparison with the number of examples. Unless "
       << "an appropriate regularization value is set, this model "
       << "may not provide accurate predictions for a validation/test set."
       << std::endl;
    logprogress_stream << ss.str() << std::endl;
  }

  ml_data valid_data;
  if (valid_X.num_rows() > 0) {
    valid_data = construct_ml_data_using_current_metadata(valid_X,
                      valid_y,
                      missing_value_action);
  }

  // First set which metrics will be computed.
  set_default_evaluation_metric();
  set_default_tracking_metric();

  // Finally call the model-specific init function.
  model_specific_init(data, valid_data);

  // Raise error if mean and variance are not finite.
  check_feature_means_and_variances(this->ml_mdata, show_extra_warnings);

  // One class classification error message.
  if(this->is_classifier()) {
    if (this->ml_mdata->target_index_size() == 1) {
      std::stringstream ss;
      ss << "One-class classification is not currently supported. "
         << "Please check your target column. "
         << "If you used data for validation tracking (by default, a 5\% split is used), "
         << "please make sure the training data contains at least 2 classes."
         << std::endl;
      log_and_throw(ss.str());
    }
    this->state["classes"] = get_class_names(this->ml_mdata);
  }

}

/**
 * Impute missing columns with 'None' values.
 */
sframe
supervised_learning_model_base::impute_missing_columns_using_current_metadata(const
    sframe& X) const{

  sframe _X = X;
  size_t n_rows = X.num_rows();
  if (n_rows == 0)
    return X;

  if (this->ml_mdata) {
    for(auto col : (this->ml_mdata)->column_names()){
      if(!X.contains_column(col)) {
        std::shared_ptr<sarray<flexible_type>> ret (new sarray<flexible_type>);
        ret->open_for_write();
        ret->set_type((this->ml_mdata)->column_type(col));
        auto writer = ret->get_output_iterator(0);
        for(size_t i=0; i < n_rows; ++i){
          *writer = flex_undefined();
          ++writer;
        }
        ret->close();
        _X = _X.add_column(ret, col);
      }
    }
  } else {
    log_and_throw("Model doesn't support missing column imputation");
  }
  return _X;
}

/**
 * Construct ml-data from the predictors and target using the current value of
 * the metadata.
 */
ml_data
supervised_learning_model_base::construct_ml_data_using_current_metadata(
    const sframe& X, const sframe& y,
    ml_missing_value_action missing_value_action) const {

  ml_data data(this->ml_mdata);
  std::string target_col = y.column_name(0);
  sframe sf_data = X.add_column(y.select_column(0), target_col);
  data.fill(sf_data,
            target_col,
            std::map<std::string, ml_column_mode>(),
            true,
            missing_value_action);
  return data;
}

/**
 * Construct ml-data from the predictors ONLY using the current value of the
 * metadata.
 */
ml_data
supervised_learning_model_base::construct_ml_data_using_current_metadata(
    const sframe& X,
    ml_missing_value_action missing_value_action) const {

  ml_data data(this->ml_mdata);
  data.fill(X,
            std::string(""),
            std::map<std::string, ml_column_mode>(),
            true,
            missing_value_action);
  return data;
}


/**
 * Get the number of features in the model.
 */
size_t supervised_learning_model_base::num_features() const{
  DASSERT_TRUE(state.size() > 0);
  DASSERT_TRUE(state.count("num_features") > 0);
  variant_type features = state.at("num_features");
  return variant_get_value<size_t>(features);
}

/**
 * Get the number of feature cols in the model.
 */
size_t supervised_learning_model_base::num_unpacked_features() const{
  DASSERT_TRUE(state.size() > 0);
  DASSERT_TRUE(state.count("num_unpacked_features") > 0);
  variant_type features = state.at("num_unpacked_features");
  return variant_get_value<size_t>(features);
}

/**
 * Get the number of examples in the model.
 */
size_t supervised_learning_model_base::num_examples() const{
  DASSERT_TRUE(state.size() > 0);
  DASSERT_TRUE(state.count("num_examples") > 0);
  variant_type examples = state.at("num_examples");
  return variant_get_value<size_t>(examples);
}

/**
 * Get training stats.
 */
std::map<std::string, flexible_type>
supervised_learning_model_base::get_train_stats() const{
  DASSERT_TRUE(is_trained());
  std::vector<std::string> keys = {"num_examples",
                                   "num_features"};
  std::map<std::string, flexible_type> results;
  for(const auto& k: keys){
    results[k] = variant_get_value<flexible_type>(state.at(k));
  }
  return results;
}

/**
 * Get names of features (as a vector)
 */
std::vector<std::string> supervised_learning_model_base::get_feature_names() const{
  DASSERT_TRUE(state.size() > 0);
  DASSERT_TRUE(state.count("features") > 0);
  return variant_get_value<std::vector<std::string>>(state.at("features"));
}

/**
 * Get name of the target column
 */
std::string supervised_learning_model_base::get_target_name() const{
  DASSERT_TRUE(state.size() > 0);
  DASSERT_TRUE(state.count("target") > 0);
  return variant_get_value<std::string>(state.at("target"));
}


/**
 * Get metrics
 */
std::vector<std::string> supervised_learning_model_base::get_metrics()  const {
  DASSERT_TRUE(metrics.size() > 0);
  return metrics;
}

/**
 * Get tracking metrics
 */
std::vector<std::string> 
      supervised_learning_model_base::get_tracking_metrics()  const {
  return tracking_metrics;
}


/**
 * Classify (with a probability) using a trained model.
 */
sframe supervised_learning_model_base::classify(const ml_data& test_data,
                                            const std::string& output_type){
  DASSERT_TRUE(name() == "classifier_logistic_regression" ||
               name() == "classifier_svm" ||
               name() == "random_forest_classifier" ||
               name() == "decision_tree_classifier" ||
               name() == "boosted_trees_classifier");

  // Class predictions
  sframe sf_class;
  sf_class = sf_class.add_column(predict(test_data, "class"), "class");


  // Binary classification
  if (variant_get_value<size_t>(state.at("num_classes")) == 2){

    // Convert P[X=1] to P[X = predicted_class]
    std::shared_ptr<sarray<flexible_type>> pred_prob =
                                                predict(test_data, "probability");
    std::shared_ptr<sarray<flexible_type>> class_prob(new sarray<flexible_type>);
    class_prob->open_for_write();
    class_prob->set_type(flex_type_enum::FLOAT);
    auto transform_fn = [](const flexible_type& f)->flexible_type{
      if (f <= 0.5){
        return 1 - f;
      } else {
        return f;
      }
    };

    turi::transform(*pred_prob, *class_prob, transform_fn);
    class_prob->close();
    sf_class = sf_class.add_column(class_prob, "probability");

  // Multi-class classification
  } else {
    sf_class = sf_class.add_column(predict(test_data, "max_probability"),
                                              "probability");
  }

  return sf_class;
}



/**
 * Make predictions using a trained model using the predict_single_example
 * interface.
 *
 * \note There are some noted inefficiencies in this function but I think it is
 * of lower priority to optimize. Chose to optimize if this is a known
 * bottleneck.
 *
 *  1. Reads and writes are mixed in this implementation. We can get better
 *  performance if we do batch reads followed by batch writes.
 *
 *  2. Everything is serial right now. Locking an expensive write operation
 *  is pretty pointless unless we do batch mode writing (see 1)
 */
std::shared_ptr<sarray<flexible_type>>
supervised_learning_model_base::predict(const ml_data& test_data,
                                        const std::string& output_type){

  size_t n_threads = turi::thread_pool::get_instance().size();
  size_t variables = 0;
  prediction_type_enum output_type_enum = prediction_type_enum_from_name(output_type);
  if (state.count("num_coefficients") > 0) {
    variables = variant_get_value<size_t>(state.at("num_coefficients"));
  } else {
    variables = variant_get_value<size_t>(state.at("num_features"));
  }

  // Multi-class error
  if (name().find("classifier") != std::string::npos) {
    size_t num_classes = variant_get_value<size_t>(state.at("num_classes"));
    if (((output_type == "margin") || (output_type == "probability"))
                                                  && num_classes > 2){
      std::stringstream ss;
      ss << "Output type '" << output_type
         << "' is only supported for binary classification."
         << " For multi-class classification, use predict_topk() instead."
         << std::endl;
      log_and_throw(ss.str());
    }
    variables = variables / (num_classes - 1);
  }

  // Write to this SArray.
  std::shared_ptr<sarray<flexible_type>> ret (new sarray<flexible_type>);
  ret->open_for_write(n_threads);
  if (output_type == "class"){
    ret->set_type((this->ml_mdata)->target_column_type());
  } else if (output_type == "probability_vector"){
    ret->set_type(flex_type_enum::VECTOR);
  } else {
    ret->set_type(flex_type_enum::FLOAT);
  }

  // Iterate through data.
  in_parallel([&](size_t thread_idx, size_t num_threads){
    DenseVector x(variables);
    SparseVector x_sp(variables);
    flexible_type preds;
    auto writer = ret->get_output_iterator(thread_idx);
    for(auto it = test_data.get_iterator(thread_idx, num_threads);
                                                              !it.done(); ++it) {

      // Dense predict.
      if (this->is_dense()) {
        fill_reference_encoding(*it, x);
        x(variables - 1) = 1;
        preds = predict_single_example(x, output_type_enum);

      // Sparse predict.
      } else {
        fill_reference_encoding(*it, x_sp);
        x_sp.insert(variables - 1, 1);
        preds = predict_single_example(x_sp, output_type_enum);
      }

      *writer = preds;
      ++writer;
    }
  });
  ret->close();
  return ret;
}

gl_sarray supervised_learning_model_base::fast_predict(
    const std::vector<flexible_type>& rows,
    const std::string& missing_value_action,
    const std::string& output_type) {

  // Initialize.
  size_t variables = 0;
  size_t classes = 0;
  if (state.count("num_coefficients") > 0) {
    variables = variant_get_value<size_t>(state.at("num_coefficients"));
  }
  if (state.count("num_classes") > 0) {
    classes = variant_get_value<size_t>(state.at("num_classes"));
    DASSERT_TRUE(classes > 1);
    variables = variables / (classes - 1);
  }

  std::vector<flexible_type> ret(rows.size());
  flex_type_enum ret_type;
  if (output_type == "class"){
    ret_type = (this->ml_mdata)->target_column_type();
  } else if (output_type == "probability_vector"){
    ret_type = flex_type_enum::VECTOR;
  } else {
    ret_type = flex_type_enum::FLOAT;
  }
  gl_sarray_writer writer(ret_type, 1 /* 1 segment */);


  auto na_enum = get_missing_value_enum_from_string(missing_value_action);
  auto pred_type_enum = prediction_type_enum_from_name(output_type);
  for (const auto& row: rows) {
    if (row.get_type() != flex_type_enum::DICT) {
      log_and_throw(
          "TypeError: Expecting dictionary as input type for each example.");
    }

    if (this->is_dense()) {
      DenseVector dense_vec(variables);
      fill_reference_encoding(ml_data_row_reference::from_row(
               this->ml_mdata, row.get<flex_dict>(), na_enum), dense_vec);
      dense_vec(variables - 1) = 1;
      flexible_type pred = predict_single_example(dense_vec, pred_type_enum);
      writer.write(pred, 0);
    } else {
      SparseVector sparse_vec(variables);
      fill_reference_encoding(ml_data_row_reference::from_row(
               this->ml_mdata, row.get<flex_dict>(), na_enum), sparse_vec);
      sparse_vec.insert(variables - 1, 1);
      flexible_type pred = predict_single_example(sparse_vec, pred_type_enum);
      writer.write(pred, 0);
    }
  }
  return writer.close();
}

gl_sframe supervised_learning_model_base::fast_classify(
    const std::vector<flexible_type>& rows,
    const std::string& missing_value_action) {
  DASSERT_TRUE(name() == "classifier_logistic_regression" ||
               name() == "classifier_svm" ||
               name() == "random_forest_classifier" ||
               name() == "decision_tree_classifier" ||
               name() == "boosted_trees_classifier");

  // Class predictions
  gl_sframe sf_class;
  sf_class.add_column(fast_predict(rows, missing_value_action, "class"),
		      "class");

  // Binary classification
  if (variant_get_value<size_t>(state.at("num_classes")) == 2){

    // Convert P[X=1] to P[X = predicted_class]
    gl_sarray pred_prob = fast_predict(rows, "error", "probability");
    auto transform_fn = [](const flexible_type& f)->flexible_type{
      if (f <= 0.5){
        return 1 - f;
      } else {
        return f;
      }
    };
    sf_class["probability"] = pred_prob.apply(transform_fn, flex_type_enum::FLOAT);

  // Multi-class classification
  } else {
    sf_class.add_column(fast_predict(rows, missing_value_action,
				     "max_probability"),
			"probability");
  }
  return sf_class;
}


/**
 * Make predictions using a trained model using the predict_single_example
 * interface.
 *
 * Has the same trouble as predict when it comes to speed.
 */
sframe supervised_learning_model_base::predict_topk(
          const ml_data& test_data, const std::string& output_type,
          const size_t topk){
  DASSERT_TRUE(name().find("classifier") != std::string::npos);
  size_t num_classes = variant_get_value<size_t>(state.at("num_classes"));
  size_t n_threads = turi::thread_pool::get_instance().size();
  size_t variables = 0;
  prediction_type_enum output_type_enum = prediction_type_enum_from_name(output_type);
  if (state.count("num_coefficients") > 0) {
    variables = variant_get_value<size_t>(state.at("num_coefficients"));
  } else {
    variables = variant_get_value<size_t>(state.at("num_features"));
  }

  DASSERT_TRUE(num_classes > 1);
  variables = variables / (num_classes - 1);

  // Error checking.
  if (topk > num_classes) {
    std::stringstream ss;
    ss << "The training data contained " << num_classes << " classes."
       << " The parameter 'k' must be less than or equal to the number of "
       << "classes in the training data." << std::endl;
    log_and_throw(ss.str());
  }

  // Setup the SFrame for output
  sframe sf;
  std::vector<std::string> col_names {"id", "class", output_type};
  std::vector<flex_type_enum> col_types {flex_type_enum::INTEGER,
                                    (this->ml_mdata)->target_column_type()};
  if (output_type == "rank"){
    col_types.push_back(flex_type_enum::INTEGER);
  } else {
    col_types.push_back(flex_type_enum::FLOAT);
  }
  sf.open_for_write(col_names, col_types, "", n_threads);


  // Iterate through data.
  in_parallel([&](size_t thread_idx, size_t num_threads){
    DenseVector x(variables);
    SparseVector x_sp(variables);
    flexible_type preds;
    auto it_sf = sf.get_output_iterator(thread_idx);
    std::vector<std::pair<size_t, double>> out;
    out.resize(num_classes);
    std::vector<flexible_type> write_x;
    write_x.resize(3);
    for(auto it = test_data.get_iterator(thread_idx, num_threads);
                                                              !it.done(); ++it) {

      // Dense predict.
      if (this->is_dense()) {
        fill_reference_encoding(*it, x);
        x(variables-1) = 1;
	      preds = predict_single_example(x, output_type_enum);
      // Sparse predict.
      } else {
        fill_reference_encoding(*it, x_sp);
        x_sp.insert(variables-1, 1);
	      preds = predict_single_example(x_sp, output_type_enum);
      }

      // Multiclass
      if (preds.size() == num_classes) {
        for (size_t k = 0; k < num_classes; ++k) {
          out[k] = std::make_pair(k, preds[k]);
        }
      // Binary
      } else {
        double zero_pred = (output_type_enum == prediction_type_enum::MARGIN) ? 0.0 : 1.0 - (double)preds;
        out[0] = std::make_pair(0, zero_pred);
        out[1] = std::make_pair(1, (double) preds);
      }

      // Sort
      std::nth_element(out.begin(), out.begin() + topk - 1, out.end(),
                    boost::bind(&std::pair<size_t, double>::second, _1) >
                    boost::bind(&std::pair<size_t, double>::second, _2));

      // Write the topk
      for (size_t k = 0; k < topk; ++k) {
        write_x[0] = it.row_index();
        write_x[1] = this->ml_mdata->target_indexer()
                          ->map_index_to_value(out[k].first);
        if (output_type_enum == prediction_type_enum::RANK){
          write_x[2] = k;
        } else {
          write_x[2] = out[k].second;
        }
        *it_sf = write_x;
        ++it_sf;
      }
    }
  });
  sf.close();
  return sf;
}

/**
 * Evaluate (by first making predictions)
 */
std::map<std::string, variant_type> supervised_learning_model_base::evaluate(
            const ml_data& test_data,
            const std::string& evaluation_type){

  // Timers.
  timer t;
  double start_time = t.current_time();
  logstream(LOG_INFO) << "Starting evaluation" << std::endl;

  // Variables needed.
  size_t n_threads = turi::thread_pool::get_instance().size();
  size_t variables = variant_get_value<size_t>(state.at("num_coefficients"));
  bool is_dense = this->is_dense();

  // Classifier specific metrics pre-computations.
  size_t num_classes = 0;
  size_t num_classes_test_and_train = 0;
  std::map<size_t, flexible_type> index_map;
  std::unordered_map<flexible_type, size_t> identity_map;
  bool is_classifier = this->is_classifier();
  if (is_classifier) {
    num_classes = variant_get_value<size_t>(state.at("num_classes"));
    num_classes_test_and_train = test_data.metadata()->target_column_size();
    variables = variables / (num_classes - 1);
    for (size_t i = 0; i < num_classes_test_and_train; i++){
      index_map[i] = this->ml_mdata->target_indexer()->map_index_to_value(i);
      identity_map[i] = i;
    }
  }

  // Setup metric computation.
  std::vector<std::string> evaluator_names;
  typedef std::shared_ptr<evaluation::supervised_evaluation_interface> evalptr;
  std::vector<evalptr> evaluators;
  
  // Compute a specific metric or all metrics ["auto"]
  std::vector<std::string> metrics_computed;
  if (evaluation_type == std::string("auto")) {
    metrics_computed = metrics; 
    DASSERT_TRUE(metrics_computed.size() > 0); 
  } else if (evaluation_type == std::string("train")) {
    metrics_computed = tracking_metrics;
    DASSERT_TRUE(metrics_computed.size() > 0); 
  } else {
    metrics_computed.push_back(evaluation_type);
  }

  // Get the evaluator metrics.
  bool contains_prob_evaluator = false;
  for (const auto& m: metrics_computed){
    std::map<std::string, variant_type> kwargs {
       {"average", to_variant(std::string("default"))}, 
       {"binary", to_variant(false)},
       {"index_map", to_variant(identity_map)},
       {"num_classes", to_variant(num_classes)},
       {"inv_index_map", to_variant(index_map)}};
    
    // Remove metrics that can't be computed. 
    auto e = evaluation::get_evaluator_metric(m, kwargs);
    evaluators.push_back(e);
    evaluator_names.push_back(m);

    // For progress tracking. Make sure the IF in train mode, then 
    // training metrics are table printer compatible.
    DASSERT_TRUE((evaluation_type == std::string("train")) 
                                        <= e->is_table_printer_compatible()); 

    // If a prob-evaluator is needed, then we use prediction probabilities.
    if (! contains_prob_evaluator) {
      contains_prob_evaluator = e->is_prob_evaluator();
    }
  } 
  DASSERT_TRUE(evaluators.size() > 0);
  DASSERT_TRUE(metrics_computed.size() > 0);

  // Init the evaluators
  for(size_t i=0; i < evaluators.size(); i++){
    evaluators[i]->init(n_threads);
  }

  // Go through evaluators that require the predicted class.
  in_parallel([&](size_t thread_idx, size_t num_threads){
    flexible_type true_value, predicted_value, prob_vector;
    DenseVector x(variables);
    SparseVector x_sp(variables);
    for(auto it = test_data.get_iterator(thread_idx, num_threads);
                                                       !it.done(); ++it) {
      // Classifiers.
      if(is_classifier) {

        // Dense predict.
        if (is_dense) {
          fill_reference_encoding(*it, x);
          x(variables-1) = 1;
	        predicted_value = predict_single_example(x,
                                      prediction_type_enum::CLASS_INDEX);
          if (contains_prob_evaluator) {
	          prob_vector = predict_single_example(x,
                                 prediction_type_enum::PROBABILITY_VECTOR);
          }
        // Sparse predict.
        } else {
          fill_reference_encoding(*it, x_sp);
          x_sp.insert(variables-1, 1);
	        predicted_value = predict_single_example(x_sp,
                                      prediction_type_enum::CLASS_INDEX);
          if (contains_prob_evaluator) {
	          prob_vector = predict_single_example(x_sp,
                                 prediction_type_enum::PROBABILITY_VECTOR);
          }
        }
        true_value = it->target_index();
      // Regression.
      } else {

        // Dense predict.
        if (is_dense) {
          fill_reference_encoding(*it, x);
          x(variables-1) = 1;
	        predicted_value = predict_single_example(x);

        // Sparse predict.
        } else {
          fill_reference_encoding(*it, x_sp);
          x_sp.insert(variables-1, 1);
	        predicted_value = predict_single_example(x_sp);
        }
        true_value = it->target_value();
      }

      // Evaluate.
      for(size_t i=0; i < evaluators.size(); i++){
        if (evaluators[i]->is_prob_evaluator()) {
          evaluators[i]->register_example(
          true_value, prob_vector, thread_idx);
        } else {
          if (evaluators[i]->name() == "classifier_accuracy") {
            evaluators[i]->register_unmapped_example(
                     true_value, predicted_value, thread_idx); // Optimized!
          } else {
            evaluators[i]->register_example(
                     true_value, predicted_value, thread_idx);
          }
        }
      }
    }
  });

  // Get results
  std::map<std::string, variant_type> results;
  for(size_t i=0; i < evaluators.size(); i++){
      results[evaluator_names[i]] = evaluators[i]->get_metric();
  }

  logstream(LOG_INFO) << "Evaluation done at "
            << (t.current_time() - start_time) << "s" << std::endl;

  return results;
}

/**
 * Display classifier model training data summary.
 *
 * \param[in] model_display_name   Name to be displayed
 *
 */
void supervised_learning_model_base::display_classifier_training_summary(
                       std::string model_display_name) const {

  size_t examples = num_examples();
  size_t classes =  variant_get_value<size_t>(state.at("num_classes"));
  size_t features = num_features();
  size_t unpacked_features = num_unpacked_features();

  logprogress_stream << model_display_name << ":" << std::endl;
  logprogress_stream << "--------------------------------------------------------" << std::endl;
  logprogress_stream << "Number of examples          : " << examples << std::endl;
  logprogress_stream << "Number of classes           : " << classes << std::endl;
  logprogress_stream << "Number of feature columns   : " << features << std::endl;
  logprogress_stream << "Number of unpacked features : " << unpacked_features << std::endl;

}

/**
 * Display regression model training data summary.
 *
 * \param[in] model_display_name   Name to be displayed
 *
 */
void supervised_learning_model_base::display_regression_training_summary(
                       std::string model_display_name) const {

  size_t examples = num_examples();
  size_t features = num_features();
  size_t unpacked_features = num_unpacked_features();

  logprogress_stream << model_display_name << ":" << std::endl;
  logprogress_stream << "--------------------------------------------------------" << std::endl;
  logprogress_stream << "Number of examples          : " << examples << std::endl;
  logprogress_stream << "Number of features          : " << features << std::endl;
  logprogress_stream << "Number of unpacked features : " << unpacked_features << std::endl;

}

/**
 * Evaluate (by first making predictions)
 */
std::vector<std::vector<flexible_type>>
          supervised_learning_model_base::get_metadata_mapping() {

  auto metadata = this->ml_mdata;
  std::vector<std::vector<flexible_type>> ret(metadata->num_dimensions());
 
  size_t pos = 0; 
  for(size_t col_index = 0; col_index < metadata->num_columns(); ++col_index) {
    std::vector<flexible_type> out(2);
    out[0] = metadata->column_name(col_index);

    switch(metadata->column_mode(col_index)) {

      case ml_column_mode::DICTIONARY: 
      case ml_column_mode::CATEGORICAL: 
      case ml_column_mode::CATEGORICAL_VECTOR:
        {
          for(size_t i = 0; i < metadata->index_size(col_index); ++i) { 
            out[1] = metadata->indexer(
                col_index)->map_index_to_value(i).to<flex_string>();
            ret[pos] = out;
            pos++;
          }
          break; 
        }
      case ml_column_mode::NUMERIC_VECTOR: {
        for(size_t i = 0; i < metadata->index_size(col_index); ++i) { 
          out[1] = i;
          ret[pos] = out;
          pos++;
        }
        break;
      }
      case ml_column_mode::NUMERIC: {
        out[1] = flex_undefined();
        ret[pos] = out;
        pos++;
        break;
      }
      default: ASSERT_TRUE(false); 
    }
  }
  return ret; 
}

void supervised_learning_model_base::api_train(
    gl_sframe data, 
    const std::string& target,
    gl_sframe validation_data,
    const std::map<std::string, flexible_type>& options) {

  // TODO: remove this plumbing now that neural nets has been 
  // moved out. 
  constexpr bool support_image_type = false;

  gl_sframe f_data = data;
  f_data.remove_column(target);
  sframe X = f_data.materialize_to_sframe(); 
    
  sframe y = data.select_columns({target}).materialize_to_sframe();

  ml_missing_value_action missing_value_action =
    this->support_missing_value() ? ml_missing_value_action::USE_NAN
                                  : ml_missing_value_action::ERROR;

  sframe valid_X, valid_y; 

  if(validation_data.num_columns() != 0) {

    gl_sframe f_v_data = validation_data;
    f_v_data.remove_column(target);
    valid_X = f_v_data.materialize_to_sframe(); 
    
    valid_y = validation_data.select_columns({target}).materialize_to_sframe();
    
    check_feature_column_types(valid_X, support_image_type);
    check_target_column_type(this->name(), valid_y);
    check_feature_column_types_match(X, valid_X);
  }

  this->init(X, y, valid_X, valid_y, missing_value_action);

  // Override any default options set by init above.
  this->init_options(options);

  this->train();
}

/**
 * API interface through the unity server.
 *
 * Prediction stuff
 */
gl_sarray supervised_learning_model_base::api_predict(
    gl_sframe data, std::string missing_value_action_str,
    std::string output_type) {

  ml_missing_value_action missing_value_action =
      get_missing_value_enum_from_string(missing_value_action_str);

  sframe X = setup_test_data_sframe(
      data.materialize_to_sframe(),
      std::dynamic_pointer_cast<supervised_learning_model_base>(shared_from_this()),
      missing_value_action);

  ml_data m_data = this->construct_ml_data_using_current_metadata(X, missing_value_action);
  
  return gl_sarray(this->predict(m_data, output_type));
}

/**
 * API interface through the unity server.
 *
 * Multiclass prediction stuff
 */
gl_sframe supervised_learning_model_base::api_predict_topk(
    gl_sframe data, std::string missing_value_action_str,
    std::string output_type, size_t topk) {
  if (topk == 0) log_and_throw("The parameter 'k' must be positive.");

  ml_missing_value_action missing_value_action =
      get_missing_value_enum_from_string(missing_value_action_str);

  sframe X = setup_test_data_sframe(
      data.materialize_to_sframe(),
      std::dynamic_pointer_cast<supervised_learning_model_base>(
          shared_from_this()),
      missing_value_action);

  ml_data m_data =
      this->construct_ml_data_using_current_metadata(X, missing_value_action);

  return gl_sframe(this->predict_topk(m_data, output_type, topk));
}

/**
 * API interface through the unity server.
 *
 *  Classification stuff
 */
gl_sframe supervised_learning_model_base::api_classify(
    gl_sframe data, std::string missing_value_action_str,
    std::string output_type) {

  ml_missing_value_action missing_value_action =
      get_missing_value_enum_from_string(missing_value_action_str);

  sframe X = setup_test_data_sframe(
      data.materialize_to_sframe(),
      std::dynamic_pointer_cast<supervised_learning_model_base>(
          shared_from_this()),
      missing_value_action);

  ml_data m_data = this->construct_ml_data_using_current_metadata(
      X, missing_value_action);

  return gl_sframe(this->classify(m_data, output_type));
}

/**
 *  API interface through the unity server.
 *
 *  Evaluate the model
 */
variant_map_type supervised_learning_model_base::api_evaluate(
    gl_sframe data, std::string missing_value_action_str, std::string metric) {
  auto model = std::dynamic_pointer_cast<supervised_learning_model_base>(
      shared_from_this());
  ml_missing_value_action missing_value_action =
      get_missing_value_enum_from_string(missing_value_action_str);

  sframe test_data = data.materialize_to_sframe();
  sframe X = setup_test_data_sframe(test_data, model, missing_value_action);
  sframe y = test_data.select_columns({get_target_name()});
  ml_data m_data = setup_ml_data_for_evaluation(
      X, y, model, missing_value_action);

  variant_map_type results = evaluate(m_data, metric);
  return results;
}

/**
 *  API interface through the unity server.
 *
 *  Extract features!
 */
gl_sarray supervised_learning_model_base::api_extract_features(
    gl_sframe data, std::string missing_value_action_str) {
  auto model = std::dynamic_pointer_cast<supervised_learning_model_base>(
      shared_from_this());
  ml_missing_value_action missing_value_action =
      get_missing_value_enum_from_string(missing_value_action_str);

  sframe test_data = data.materialize_to_sframe();
  sframe X = setup_test_data_sframe(test_data, model, missing_value_action);

  return extract_features(X, missing_value_action);
}

/**
 * Compute the width of the data.
 *
 * \param[in] X  Input SFrame
 * \returns width
 *
 * The width is the same as the num_coefficients.
 *
 */
size_t compute_data_width(sframe X){
  ml_data data;
  data.fill(X);
  return get_number_of_coefficients(data.metadata());
}

/**
 * Rule based better than stupid model selector.
 */
std::string _regression_model_selector(std::shared_ptr<unity_sframe> _X){

  sframe X = *(_X->get_underlying_sframe());
  size_t data_width = compute_data_width(X);
  if (data_width < WIDE_DATA){
    return "boosted_trees_regression";
  } else {
    return "regression_linear_regression";
  }
}

/**
 * Rule based better than stupid model selector.
 */
std::string _classifier_model_selector(std::shared_ptr<unity_sframe> _X){

  sframe X = *(_X->get_underlying_sframe());

  size_t data_width = compute_data_width(X);
  if (data_width < WIDE_DATA){
    return "boosted_trees_classifier";
  } else {
    return "classifier_logistic_regression";
  }
}

/**
 * Rule based better than stupid model selector.
 */
std::vector<std::string> _classifier_available_models(size_t num_classes,
                                         std::shared_ptr<unity_sframe> _X){
  sframe X = *(_X->get_underlying_sframe());

  // Throw error if only one class.
  // If number of classes more than 2, use boosted trees
  if (num_classes == 1) {
    log_and_throw("One-class classification is not currently supported. Please check your target column.");
  } else if (num_classes > 2) {
    return {"boosted_trees_classifier",
            "random_forest_classifier",
            "decision_tree_classifier",
            "classifier_logistic_regression"};
  } else {
    size_t data_width = compute_data_width(X);
    if (data_width < WIDE_DATA){
      return {"boosted_trees_classifier",
              "random_forest_classifier",
              "decision_tree_classifier",
              "classifier_svm",
              "classifier_logistic_regression"};
    } else {
      return {"classifier_logistic_regression",
              "classifier_svm"};
    }
  }

  return std::vector<std::string>();
}

/**
 * Fast path for in-memory-predictions.
 */
gl_sarray _fast_predict(
    std::shared_ptr<supervised_learning_model_base> model,
    const std::vector<flexible_type>& rows,
    const std::string& missing_value_action,
    const std::string& output_type) {
  return model->fast_predict(rows, missing_value_action, output_type);
}

/**
 * Fast path for in-memory-predictions.
 */
gl_sframe _fast_predict_topk(
    std::shared_ptr<supervised_learning_model_base> model,
    const std::vector<flexible_type>& rows,
    const std::string& missing_value_action,
    const std::string& output_type,
    const size_t topk) {
  return model->fast_predict_topk(rows, missing_value_action, output_type,
				  topk);
}

/**
 * Fast path for in-memory-predictions.
 */
gl_sframe _fast_classify(
    std::shared_ptr<supervised_learning_model_base> model,
    const std::vector<flexible_type>& rows,
    const std::string& missing_value_action) {
  return model->fast_classify(rows, missing_value_action);
}

/**
 * Get the metadata mapping. 
 */
std::vector<std::vector<flexible_type>> _get_metadata_mapping(
    std::shared_ptr<supervised_learning_model_base> model) {
  return model->get_metadata_mapping();
}


} // supervised_learning
} // turicreate
