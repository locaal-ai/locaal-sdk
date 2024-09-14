cmake_minimum_required(VERSION 3.12)
project(LocaalSDK VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 11)

# Option to build shared libraries
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

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

include(cmake/BuildWhispercpp.cmake)
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE Whispercpp)

include(cmake/BuildCTranslate2.cmake)
include(cmake/BuildSentencepiece.cmake)
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE ct2 sentencepiece)

set(USE_SYSTEM_ONNXRUNTIME
    OFF
    CACHE STRING "Use system ONNX Runtime")

set(DISABLE_ONNXRUNTIME_GPU
    OFF
    CACHE STRING "Disables GPU support of ONNX Runtime (Only valid on Linux)")

if(DISABLE_ONNXRUNTIME_GPU)
  target_compile_definitions(${CMAKE_PROJECT_NAME} INTERFACE DISABLE_ONNXRUNTIME_GPU)
endif()

if(USE_SYSTEM_ONNXRUNTIME)
  if(OS_LINUX)
    find_package(Onnxruntime 1.16.3 REQUIRED)
    set(Onnxruntime_INCLUDE_PATH
        ${Onnxruntime_INCLUDE_DIR} ${Onnxruntime_INCLUDE_DIR}/onnxruntime
        ${Onnxruntime_INCLUDE_DIR}/onnxruntime/core/session ${Onnxruntime_INCLUDE_DIR}/onnxruntime/core/providers/cpu)
    target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE "${Onnxruntime_LIBRARIES}")
    target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM INTERFACE "${Onnxruntime_INCLUDE_PATH}")
  else()
    message(FATAL_ERROR "System ONNX Runtime is only supported on Linux!")
  endif()
else()
  include(cmake/FetchOnnxruntime.cmake)
endif()

include(cmake/BuildICU.cmake)
# Add ICU to the target
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE ICU)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM INTERFACE ${ICU_INCLUDE_DIR})

include(cmake/FetchLibav.cmake)
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE FFmpeg)


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
