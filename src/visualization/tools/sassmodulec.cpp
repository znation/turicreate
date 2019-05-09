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
#include <iostream>

using namespace boost::program_options;

int main(int argc, const char *argv[])
{
  options_description desc{"Options"};
  desc.add_options()
    ("module,m", value<std::string>()->required(), "(S)CSS Module Source");

  try
  {
    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help")) {
      std::cout << desc << '\n';
      return 0;
    }

    std::cout << "Got it!" << '\n';
    std::cout << vm["module"].as<std::string>() << '\n';
    return 0;
  }
  catch (const error &ex)
  {
    std::cerr << "Error: " << ex.what() << "\n\n";
    std::cerr << desc << '\n';
    return 1;
  }
}