project(vega_renderer_test)

if(APPLE)

    find_library( WEBKIT_LIBRARY WebKit )
    message( STATUS "WebKit found at ${WEBKIT_LIBRARY}")

    find_library( XCTEST_LIBRARY XCTest )
    message( STATUS "XCTest found at ${XCTEST_LIBRARY}")

    add_definitions(-DPROJECT_DIR=${CMAKE_CURRENT_SOURCE_DIR})
    make_library(
        vega_renderer_test_utils
        SOURCES
            base_fixture.mm
            vega_webkit_renderer.mm
        REQUIRES
            vega_renderer
            ${WEBKIT_LIBRARY}
            ${XCTEST_LIBRARY}
    )
    target_compile_options(vega_renderer_test_utils PUBLIC "-fobjc-arc")
    target_compile_options(vega_renderer_test_utils PUBLIC "-fsanitize=address")
    target_link_options(vega_renderer_test_utils PUBLIC "-fsanitize=address")
    target_compile_options(vega_renderer_test_utils PUBLIC "-g")

    make_boost_test(vega_example_tests.cxx REQUIRES vega_renderer_test_utils)
    make_boost_test(vega_lite_example_tests.cxx REQUIRES vega_renderer_test_utils)
    make_boost_test(turicreate_example_tests.cxx REQUIRES vega_renderer_test_utils)
    target_compile_options(turicreate_example_tests.cxxtest PUBLIC "-fobjc-arc")
    target_compile_options(turicreate_example_tests.cxxtest PUBLIC "-fsanitize=address")
    target_link_options(turicreate_example_tests.cxxtest PUBLIC "-fsanitize=address")
    target_compile_options(turicreate_example_tests.cxxtest PUBLIC "-g")

endif()
