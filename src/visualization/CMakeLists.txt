project(Turi)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

if(${TC_BUILD_VISUALIZATION_CLIENT})

  set(TC_FRONTEND_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/build")
  set(TC_FRONTEND_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/src")
  file(MAKE_DIRECTORY ${TC_FRONTEND_BUILD_DIR})
  set(CSS_SOURCES
    # CSS files
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationMetrics/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationSettingsSelection/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTable/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationRows/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationRows/TCEvaluationCells/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTooltip/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/Cell/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/Row/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/Table/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Table/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Plot/Chart/index.css"
    "Turi Create Visualization/src/user_interface/src/elements/Plot/Summary/index.css"

    # SCSS files
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/InfiniteScroll/ImageContainer/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/InfiniteScroll/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/StatusBar/ToggleButton/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/StatusBar/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/utils.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/LabelContainer/Labels/ProgressBar/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/LabelContainer/Labels/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/LabelContainer/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/SingleImage/ImageLabeler/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/SingleImage/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/SingleImage/SimilarImage/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/NavigationBar/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/ErrorBar/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/LabelModal/index.module.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationSettings/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationImageCells/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationImageCells/TCEvaluationImageCellsHover/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationHeader/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationHeader/TCEvaluationHeaderCell/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationOverview/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationImageViewerContainer/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionHeader/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionHeader/TCEvaluationConfusionHeaderCells/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionRow/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionRow/TCEvaluationConfusionCell/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationFooter/index.scss"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationImageComponent/index.scss"
  )
  set(JAVASCRIPT_SOURCES
    "Turi Create Visualization/src/user_interface/src/client.js"
    "Turi Create Visualization/src/user_interface/src/tcviz.js"
    "Turi Create Visualization/src/user_interface/src/elements/Plot/Chart/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Plot/Summary/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/InfiniteScroll/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/InfiniteScroll/ImageContainer/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/StatusBar/ToggleButton/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/StatusBar/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/LabelContainer/Labels/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/LabelContainer/Labels/ProgressBar/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/LabelContainer/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/SingleImage/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/SingleImage/ImageLabeler/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/SingleImage/SimilarImage/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/NavigationBar/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/ErrorBar/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/utils.js"
    "Turi Create Visualization/src/user_interface/src/elements/Annotate/LabelModal/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/fontawesome.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/Cell/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/Row/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/Table/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Table/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationSettings/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationImageCells/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationImageCells/TCEvaluationImageCellsHover/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationHeader/TCEvaluationHeaderCell/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationHeader/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTable/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationRows/TCEvaluationCells/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationRows/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationOverview/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationImageViewerContainer/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionHeader/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionHeader/TCEvaluationConfusionHeaderCells/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionRow/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionRow/TCEvaluationConfusionCell/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationSettingsSelection/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationFooter/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationMetrics/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationImageComponent/index.js"
    "Turi Create Visualization/src/user_interface/src/elements/Explore/Evaluation/Components/TCEvaluationTooltip/index.js"
  )

  add_custom_target(tcviz_frontend_assets
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/format"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Annotate/StatusBar/assets"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Annotate/SingleImage/assets"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Annotate/ErrorBar/assets"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionHeader/TCEvaluationConfusionHeaderCells/assets/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationFooter/assets/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationImageComponent/assets/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationImageViewerContainer/assets/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationMetrics/assets/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationSettings/assets/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTable/assets/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationHeader/TCEvaluationHeaderCell/assets/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationRows/TCEvaluationCells/assets/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTooltip/assets/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/format/message.json" "${TC_FRONTEND_BUILD_DIR}/format/message.json"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Annotate/StatusBar/assets/swap.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Annotate/StatusBar/assets/swap.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Annotate/SingleImage/assets/right_arrow.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Annotate/SingleImage/assets/right_arrow.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Annotate/SingleImage/assets/left_arrow.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Annotate/SingleImage/assets/left_arrow.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Annotate/ErrorBar/assets/error.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Annotate/ErrorBar/assets/error.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionHeader/TCEvaluationConfusionHeaderCells/assets/caret-down.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionHeader/TCEvaluationConfusionHeaderCells/assets/caret-down.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionHeader/TCEvaluationConfusionHeaderCells/assets/down.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationConfusionTable/TCEvaluationConfusionHeader/TCEvaluationConfusionHeaderCells/assets/down.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationFooter/assets/cancel.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationFooter/assets/cancel.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationFooter/assets/down.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationFooter/assets/down.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationImageComponent/assets/export.png" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationImageComponent/assets/export.png"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationImageViewerContainer/assets/right.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationImageViewerContainer/assets/right.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationMetrics/assets/info.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationMetrics/assets/info.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationSettings/assets/filter.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationSettings/assets/filter.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationHeader/TCEvaluationHeaderCell/assets/caret-down.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationHeader/TCEvaluationHeaderCell/assets/caret-down.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationHeader/TCEvaluationHeaderCell/assets/down.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationHeader/TCEvaluationHeaderCell/assets/down.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationRows/TCEvaluationCells/assets/warning.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTable/TCEvaluationRows/TCEvaluationCells/assets/warning.svg"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${TC_FRONTEND_SOURCE_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTooltip/assets/carret.svg" "${TC_FRONTEND_BUILD_DIR}/elements/Explore/Evaluation/Components/TCEvaluationTooltip/assets/carret.svg"
    COMMENT "Copying visualization frontend assets from src/"
  )

  # Build JavaScript (client app code) with typescript
  # Includes any external dependencies that are distributed in source form
  # (i.e. need to be converted to AMD module format, and/or compiled down to ES5)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/javascript_build_sentinel
    COMMAND "${CMAKE_SOURCE_DIR}/deps/local/bin/node"
            "${CMAKE_SOURCE_DIR}/src/external/javascript/typescript-3.4.5/tsc.js"
            --module amd
            --target ES5
            --allowJs
            --jsx react
            --sourceMap
            --removeComments
            --outFile "${TC_FRONTEND_BUILD_DIR}/tcviz.js"
            "tcviz.js"
    COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/javascript_build_sentinel
    DEPENDS
      node
      ${JAVASCRIPT_SOURCES}
    WORKING_DIRECTORY "${TC_FRONTEND_SOURCE_DIR}"
    COMMENT "Building ${SOURCE} using TypeScript 3.4.5"
    VERBATIM
  )

  # Build CSS Modules with css_module_compiler.js
  set(CMAKE_CSS_COMPILE_OBJECT "${CMAKE_SOURCE_DIR}/deps/local/bin/node ${CMAKE_SOURCE_DIR}/src/visualization/tools/css_module_compiler.js ${CMAKE_CSS_COMPILER} <SOURCE> <OBJECT>")
  set(CMAKE_CSS_CREATE_STATIC_LIBRARY "${CMAKE_SOURCE_DIR}/deps/local/bin/node ${CMAKE_SOURCE_DIR}/src/visualization/tools/css_module_linker.js <TARGET> \"${TC_FRONTEND_SOURCE_DIR}\" \"${TC_FRONTEND_BUILD_DIR}\" <OBJECTS>")
  add_library(tcviz_css STATIC
    ${CSS_SOURCES}
  )
  set_target_properties(tcviz_css PROPERTIES LINKER_LANGUAGE CSS)
  set_source_files_properties(${CSS_SOURCES} PROPERTIES LANGUAGE CSS)
  add_dependencies(tcviz_css node)

  add_custom_target(tcviz_frontend_copy
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/public/*.*" "${TC_FRONTEND_BUILD_DIR}"
    COMMENT "Copying visualization frontend assets from public/"
  )

  # Includes CSS and JS deps that don't need to be compiled
  add_custom_target(tcviz_frontend_deps
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/src/external/css/font-awesome-4.7.0/font-awesome.min.css"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/create-react-class-15.6.3/create-react-class.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/d3-5.9.2/d3.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/long-4.0.0/long.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/prop-types-15.7.2/prop-types.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/protobufjs-6.8.6/protobuf.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/react-16.8.6/react.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/react-16.8.6/react-dom.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/react-json-pretty-1.7.9/react-json-pretty.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/requirejs-2.3.6/require.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/requirejs-plugins-1.0.3/src/image.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/requirejs-plugins-1.0.3/src/json.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/requirejs-plugins-1.0.3/lib/text.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/vega-4.3.0/vega.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/vega-tooltip-0.6.1/vega-tooltip.js"
        "${CMAKE_SOURCE_DIR}/src/external/javascript/vega-tooltip-0.6.1/vega-tooltip.css"
        "${TC_FRONTEND_BUILD_DIR}"
    COMMENT "Copying visualization frontend dependencies from src/external/"
  )
  add_custom_target(tcviz_frontend_all
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/javascript_build_sentinel
    DEPENDS tcviz_css
    DEPENDS tcviz_frontend_assets
    DEPENDS tcviz_frontend_copy
    DEPENDS tcviz_frontend_deps
  )

  if(APPLE)
    set(VISUALIZATION_XCODE_OUTPUT
      "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/Turi Create Visualization.app/Contents/_CodeSignature/CodeResources"
      # TODO - there are probably more files that should go here, but this one might be sufficient.
      # I'm hoping it reliably changes every build.
    )
    set(VISUALIZATION_XCODE_DEPENDENCIES
      tcviz_frontend_all
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization.xcodeproj/project.pbxproj"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/Info.plist"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/Debugging.swift"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/Pipe.swift"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/AppDelegate.swift"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/JSON.swift"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/CustomWebKitView.swift"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/VegaContainer.swift"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/ViewController.swift"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/NSWindowWorkaround.swift"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/Error.swift"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/AppData.swift"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/public/index.html"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/public/index.js"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/package.json"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/src/tcviz.js"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/src/elements/Plot/Chart/index.js"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/src/elements/Plot/Summary/index.js"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/index.js"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/Cell/index.js"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/Table/index.js"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/src/elements/Explore/Table/sticky-table/Row/index.js"
      "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization/src/user_interface/src/elements/Explore/Table/index.js"
    )
    add_custom_command(
      OUTPUT "${VISUALIZATION_XCODE_OUTPUT}"
      COMMAND xcodebuild -project "${CMAKE_CURRENT_SOURCE_DIR}/Turi Create Visualization.xcodeproj/" -configuration ${CMAKE_BUILD_TYPE} SYMROOT=${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Building visualization client via xcodebuild"
      DEPENDS ${VISUALIZATION_XCODE_DEPENDENCIES}
      VERBATIM
    )
    add_custom_target(visualization_client DEPENDS "${VISUALIZATION_XCODE_OUTPUT}")
  elseif(LINUX)
    include(ExternalProject)

    ExternalProject_Add(
        cef
        PREFIX "cef"
        URL http://opensource.spotify.com/cefbuilds/cef_binary_3.3282.1731.gfc9a4fa_linux64_minimal.tar.bz2
        URL_HASH SHA1=4eeeabee642bfe68f561566e0f249f86afaf6634
        DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/deps
        SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ${CMAKE_COMMAND} . -DCEF_C_COMPILER_FLAGS=-w -DCEF_CXX_COMPILER_FLAGS=-w
        BUILD_COMMAND "make"
        INSTALL_COMMAND ""
        BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/libcef_dll_wrapper/libcef_dll_wrapper.a
                         ${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/Release/libcef.so
    )


    set(CEF_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/Resources)

    set(TC_VIZ_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/TcViz")
    set(TC_VIZ_HTML_DIRECTORY "${TC_VIZ_DIRECTORY}/html")

    set(CEF_RESOURCES_LOCALE "${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/Resources/locales")
    set(TC_VIZ_LOCALES_DIRECTORY "${TC_VIZ_DIRECTORY}/locales")

    set(TCVIZ_SRCS
      src/layer.cpp
      src/handler.cpp
      src/javascript_caller.cpp
      src/pipe.cpp
      src/main.cpp
      src/handler_linux.cpp
    )

    set(LIB_CEF_DLL_WRAPPER "${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/libcef_dll_wrapper/libcef_dll_wrapper.a")
    set(LIB_CEF "${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/Release/libcef.so")
    set(LIB_CEF_SO_DEPS
      ${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/Release/libEGL.so
      ${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/Release/libGLESv2.so)
    set(LIB_CEF_SWIFTSHADER_SO_DEPS
      ${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/Release/swiftshader/libEGL.so
      ${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/Release/swiftshader/libGLESv2.so)
    set(V8_CONTEXT_SNAPSHOT "${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/Release/v8_context_snapshot.bin")
    set(SNAPSHOT_BLOB "${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/Release/snapshot_blob.bin")
    set(NATIVES_BLOB "${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef/Release/natives_blob.bin")

    include_directories(
      ${CMAKE_CURRENT_BINARY_DIR}/deps/cef/src/cef
    )

    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TC_VIZ_DIRECTORY}")

    add_executable(visualization_client "${TCVIZ_SRCS}")

    set_target_properties(visualization_client
      PROPERTIES LINK_FLAGS "-Wl,--unresolved-symbols=ignore-in-shared-libs")

    target_link_libraries(visualization_client
      ${LIB_CEF_DLL_WRAPPER}
      ${LIB_CEF}
      X11
      pthread
    )

    add_dependencies(visualization_client cef)

    add_custom_command(
            TARGET visualization_client POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E make_directory
                    ${TC_VIZ_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E make_directory
                    ${TC_VIZ_HTML_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${TC_FRONTEND_BUILD_DIR}
                    ${TC_VIZ_HTML_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E make_directory
                    ${TC_VIZ_LOCALES_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CEF_RESOURCES}
                    ${TC_VIZ_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CEF_RESOURCES_LOCALE}
                    ${TC_VIZ_LOCALES_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E copy
                    ${LIB_CEF}
                    ${TC_VIZ_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E copy
                    ${V8_CONTEXT_SNAPSHOT}
                    ${TC_VIZ_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E copy
                    ${V8_CONTEXT_SNAPSHOT}
                    ${TC_VIZ_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E copy
                    ${SNAPSHOT_BLOB}
                    ${TC_VIZ_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E copy
                    ${NATIVES_BLOB}
                    ${TC_VIZ_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E make_directory
                    ${TC_VIZ_DIRECTORY}/swiftshader

            COMMAND ${CMAKE_COMMAND} -E copy
                    ${LIB_CEF_SO_DEPS}
                    ${TC_VIZ_DIRECTORY}

            COMMAND ${CMAKE_COMMAND} -E copy
                    ${LIB_CEF_SWIFTSHADER_SO_DEPS}
                    ${TC_VIZ_DIRECTORY}/swiftshader

            VERBATIM
    )
  else()
    make_empty_library(visualization_client)
  endif()
else()
  make_empty_library(visualization_client)
endif()
