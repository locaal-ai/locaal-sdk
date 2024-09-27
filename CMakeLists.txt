cmake_minimum_required(VERSION 3.12)
project(LocaalSDK VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to build shared libraries
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_DOCS "Build documentation" OFF)

# Create the LocaalSDK target
add_library(${CMAKE_PROJECT_NAME} INTERFACE)

set(USE_SYSTEM_CURL
    OFF
    CACHE STRING "Use system cURL")

if(USE_SYSTEM_CURL)
  find_package(CURL REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE "${CURL_LIBRARIES}")
  target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PUBLIC "${CURL_INCLUDE_DIRS}")
else()
  include(cmake/BuildMyCurl.cmake)
  target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE libcurl)
endif()

if(WIN32)
  if(DEFINED ENV{ACCELERATION})
    set(ACCELERATION
        $ENV{ACCELERATION}
        CACHE STRING "Acceleration to use" FORCE)
  endif()
  if(NOT DEFINED ACCELERATION)
    set(ACCELERATION
        "cpu"
        CACHE STRING "Acceleration to use")
  endif()
  set_property(CACHE ACCELERATION PROPERTY STRINGS "cpu" "hipblas" "cuda")
endif()

set(DISABLE_ONNXRUNTIME_GPU
    OFF
    CACHE STRING "Disables GPU support of ONNX Runtime (Only valid on Linux)")

if(DISABLE_ONNXRUNTIME_GPU)
  target_compile_definitions(${CMAKE_PROJECT_NAME} INTERFACE DISABLE_ONNXRUNTIME_GPU)
endif()

include(cmake/FetchOnnxruntime.cmake)
include(cmake/BuildWhispercpp.cmake)
include(cmake/BuildCTranslate2.cmake)
include(cmake/BuildSentencepiece.cmake)
include(cmake/BuildICU.cmake)
include(cmake/FetchLibav.cmake)
include(cmake/BuildPlatformdirs.cmake)

# List of all available modules
set(LOCAAL_MODULES
    Core
    Transcription
    Translation
    # OCR
    # DocumentAnalysis
    # SpeechSynthesis
    # ImageSegmentation
)

# Function to add a module
function(add_locaal_module MODULE_NAME)
    add_subdirectory(src/modules/${MODULE_NAME})
    list(APPEND LOCAAL_ENABLED_MODULES ${MODULE_NAME})
    set(LOCAAL_ENABLED_MODULES ${LOCAAL_ENABLED_MODULES} PARENT_SCOPE)
endfunction()


# Add requested modules
foreach(MODULE ${LocaalSDK_FIND_COMPONENTS})
    if(${MODULE} IN_LIST LOCAAL_MODULES)
        add_locaal_module(${MODULE})
    else()
        message(FATAL_ERROR "Unknown module: ${MODULE}")
    endif()
endforeach()

target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE ${LOCAAL_ENABLED_MODULES})

if(BUILD_EXAMPLES)
    add_subdirectory(examples/cpp)
endif()

# Find Doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND)
    # Set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else()
    message("Doxygen need to be installed to generate the doxygen documentation")
endif()

# Generate and install package configuration files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/LocaalSDKConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/LocaalSDKConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/LocaalSDKConfig.cmake"
    INSTALL_DESTINATION lib/cmake/LocaalSDK
)

install(TARGETS ${CMAKE_PROJECT_NAME} EXPORT LocaalSDKTargets)
install(EXPORT LocaalSDKTargets
    FILE LocaalSDKTargets.cmake
    NAMESPACE LocaalSDK::
    DESTINATION lib/cmake/LocaalSDK
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LocaalSDKConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/LocaalSDKConfigVersion.cmake"
    DESTINATION lib/cmake/LocaalSDK
)
