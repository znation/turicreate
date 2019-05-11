//  Copyright © 2019 Apple Inc. All rights reserved.
//
//  Use of this source code is governed by a BSD-3-clause license that can
//  be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause

/*
 * SassModuleC
 *
 * Like Sassc (sass/scss compiler), but with support for (S)CSS Modules.
 * 
 */

#include <boost/program_options.hpp>
#include <sass/context.h>

#include <iostream>

using namespace boost::program_options;

static int parse_command_line(int argc, const char *argv[], variables_map& vm_out) {
  options_description desc{"Options"};
  desc.add_options()
    ("module,m", value<std::string>()->required(), "Specifies a path to an input (S)CSS Module")
    ("out_dir,o", value<std::string>()->required(), "Specifies a path to an output directory");

  try
  {
    store(parse_command_line(argc, argv, desc), vm_out);
    notify(vm_out);

    if (vm_out.count("help")) {
      std::cout << desc << '\n';
      return 0;
    }

    std::cout << "Got it!" << '\n';
    std::cout << vm_out["module"].as<std::string>() << '\n';
    return 0;
  }
  catch (const error &ex)
  {
    std::cerr << "Error: " << ex.what() << "\n\n";
    std::cerr << desc << '\n';
    return 1;
  }
}

int main(int argc, const char *argv[])
{
  variables_map vm;
  int ret = parse_command_line(argc, argv, vm);
  if (ret != 0) return ret;

  


}

Sass_File_Context* context = sass_make_file_context(vm["module"].as<std::string>());
Sass_Compiler* compiler = sass_make_file_compiler(sass_context);
sass_compiler_parse(compiler)
sass_compiler_execute(compiler)
std::string output = sass_context_get_output_string(context)
// Retrieve errors during compilation
error_status = sass_context_get_error_status(context)
json_error = sass_context_get_error_json(context)
// Release memory dedicated to the C compiler
sass_delete_compiler(compiler)