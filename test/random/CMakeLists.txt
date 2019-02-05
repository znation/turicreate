project(random_test)
#make_boost_test(random_test.cxx REQUIRES random logger)
make_executable(alias_benchmark SOURCES test_alias.cpp REQUIRES random)
