cmake_minimum_required(VERSION 3.12)
project(locaal)

set(CMAKE_CXX_STANDARD 11)

set(USE_SYSTEM_CURL
    OFF
    CACHE STRING "Use system cURL")

if(USE_SYSTEM_CURL)
  find_package(CURL REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE "${CURL_LIBRARIES}")
  target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PUBLIC "${CURL_INCLUDE_DIRS}")
else()
  include(cmake/BuildMyCurl.cmake)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE libcurl)
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
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Whispercpp)

include(cmake/BuildCTranslate2.cmake)
include(cmake/BuildSentencepiece.cmake)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ct2 sentencepiece)

set(USE_SYSTEM_ONNXRUNTIME
    OFF
    CACHE STRING "Use system ONNX Runtime")

set(DISABLE_ONNXRUNTIME_GPU
    OFF
    CACHE STRING "Disables GPU support of ONNX Runtime (Only valid on Linux)")

if(DISABLE_ONNXRUNTIME_GPU)
  target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE DISABLE_ONNXRUNTIME_GPU)
endif()

if(USE_SYSTEM_ONNXRUNTIME)
  if(OS_LINUX)
    find_package(Onnxruntime 1.16.3 REQUIRED)
    set(Onnxruntime_INCLUDE_PATH
        ${Onnxruntime_INCLUDE_DIR} ${Onnxruntime_INCLUDE_DIR}/onnxruntime
        ${Onnxruntime_INCLUDE_DIR}/onnxruntime/core/session ${Onnxruntime_INCLUDE_DIR}/onnxruntime/core/providers/cpu)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE "${Onnxruntime_LIBRARIES}")
    target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PUBLIC "${Onnxruntime_INCLUDE_PATH}")
  else()
    message(FATAL_ERROR "System ONNX Runtime is only supported on Linux!")
  endif()
else()
  include(cmake/FetchOnnxruntime.cmake)
endif()

include(cmake/BuildICU.cmake)
# Add ICU to the target
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ICU)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PUBLIC ${ICU_INCLUDE_DIR})

# Add your source files here
set(SOURCES
    src/locaal.cpp
)

# Add your header files here
set(HEADERS
    include/locaal.h
)

# Create the shared library
add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

# Set the include directories
target_include_directories(locaal PUBLIC include)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install the library
install(TARGETS ${CMAKE_PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install the headers
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME})

# Install the cmake config files
install(EXPORT ${CMAKE_PROJECT_NAME}Targets
        FILE ${CMAKE_PROJECT_NAME}Targets.cmake
        NAMESPACE ${CMAKE_PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME})

configure_package_config_file(cmake/${CMAKE_PROJECT_NAME}Config.cmake.in
                              ${CMAKE_PROJECT_NAME}Config.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME})

write_basic_package_version_file(${CMAKE_PROJECT_NAME}ConfigVersion.cmake
                                 VERSION 1.0
                                 COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME})
