include(ExternalProject)
include(FetchContent)

set(PREBUILT_WHISPERCPP_VERSION "0.0.6")
set(PREBUILT_WHISPERCPP_URL_BASE
    "https://github.com/occ-ai/occ-ai-dep-whispercpp/releases/download/${PREBUILT_WHISPERCPP_VERSION}")

if(APPLE)
  # check the "MACOS_ARCH" env var to figure out if this is x86 or arm64
  if($ENV{MACOS_ARCH} STREQUAL "x86_64")
    set(WHISPER_CPP_HASH "454abee900a96a0a10a91f631ff797bdbdf2df0d2a819479a409634c9be1e12c")
  elseif($ENV{MACOS_ARCH} STREQUAL "arm64")
    set(WHISPER_CPP_HASH "f726388cc494f6fca864c860af6c1bc2932c3dc823ef92197b1e29f088425668")
  else()
    message(
      FATAL_ERROR
        "The MACOS_ARCH environment variable is not set to a valid value. Please set it to either `x86_64` or `arm64`")
  endif()
  set(WHISPER_CPP_URL
      "${PREBUILT_WHISPERCPP_URL_BASE}/whispercpp-macos-$ENV{MACOS_ARCH}-${PREBUILT_WHISPERCPP_VERSION}.tar.gz")

  FetchContent_Declare(
    whispercpp_fetch
    URL ${WHISPER_CPP_URL}
    URL_HASH SHA256=${WHISPER_CPP_HASH})
  FetchContent_MakeAvailable(whispercpp_fetch)

  add_library(Whispercpp::Whisper STATIC IMPORTED)
  set_target_properties(
    Whispercpp::Whisper
    PROPERTIES IMPORTED_LOCATION
               ${whispercpp_fetch_SOURCE_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}whisper${CMAKE_STATIC_LIBRARY_SUFFIX})
  set_target_properties(Whispercpp::Whisper PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                                       ${whispercpp_fetch_SOURCE_DIR}/include)
  add_library(Whispercpp::GGML STATIC IMPORTED)
  set_target_properties(
    Whispercpp::GGML
    PROPERTIES IMPORTED_LOCATION
               ${whispercpp_fetch_SOURCE_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ggml${CMAKE_STATIC_LIBRARY_SUFFIX})

  add_library(Whispercpp::CoreML STATIC IMPORTED)
  set_target_properties(
    Whispercpp::CoreML
    PROPERTIES
      IMPORTED_LOCATION
      ${whispercpp_fetch_SOURCE_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}whisper.coreml${CMAKE_STATIC_LIBRARY_SUFFIX})

elseif(WIN32)
  if(NOT DEFINED ACCELERATION)
    message(FATAL_ERROR "ACCELERATION is not set. Please set it to either `cpu`, `cuda` or `hipblas`")
  endif()

  set(ARCH_PREFIX ${ACCELERATION})
  set(WHISPER_CPP_URL
      "${PREBUILT_WHISPERCPP_URL_BASE}/whispercpp-windows-${ARCH_PREFIX}-${PREBUILT_WHISPERCPP_VERSION}.zip")
  if(${ACCELERATION} STREQUAL "cpu")
    set(WHISPER_CPP_HASH "126c5d859e902b4cd0f2cd09304a68750f1dbc6a7aa62e280cfd56c51a6a1c95")
    add_compile_definitions("LOCALVOCAL_WITH_CPU")
  elseif(${ACCELERATION} STREQUAL "cuda")
    set(WHISPER_CPP_HASH "5b9592c311a7f1612894ca0b36f6bd4effb6a46acd03d33924df56c52f566779")
    add_compile_definitions("LOCALVOCAL_WITH_CUDA")
  elseif(${ACCELERATION} STREQUAL "hipblas")
    set(WHISPER_CPP_HASH "c306ecce16cd10f377fdefbf7bb252abac8e6638a2637f82b1f1f32dd2cb4e39")
    add_compile_definitions("LOCALVOCAL_WITH_HIPBLAS")
  else()
    message(
      FATAL_ERROR
        "The ACCELERATION environment variable is not set to a valid value. Please set it to either `cpu` or `cuda` or `hipblas`"
    )
  endif()

  FetchContent_Declare(
    whispercpp_fetch
    URL ${WHISPER_CPP_URL}
    URL_HASH SHA256=${WHISPER_CPP_HASH}
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
  FetchContent_MakeAvailable(whispercpp_fetch)

  add_library(Whispercpp::Whisper SHARED IMPORTED)
  set_target_properties(
    Whispercpp::Whisper
    PROPERTIES IMPORTED_LOCATION
               ${whispercpp_fetch_SOURCE_DIR}/bin/${CMAKE_SHARED_LIBRARY_PREFIX}whisper${CMAKE_SHARED_LIBRARY_SUFFIX})
  set_target_properties(
    Whispercpp::Whisper
    PROPERTIES IMPORTED_IMPLIB
               ${whispercpp_fetch_SOURCE_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}whisper${CMAKE_STATIC_LIBRARY_SUFFIX})
  set_target_properties(Whispercpp::Whisper PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                                       ${whispercpp_fetch_SOURCE_DIR}/include)

  if(${ACCELERATION} STREQUAL "cpu")
    # add openblas to the link line
    add_library(Whispercpp::OpenBLAS STATIC IMPORTED)
    set_target_properties(Whispercpp::OpenBLAS PROPERTIES IMPORTED_LOCATION
                                                          ${whispercpp_fetch_SOURCE_DIR}/lib/libopenblas.dll.a)
  endif()

  # glob all dlls in the bin directory and install them
  file(GLOB WHISPER_DLLS ${whispercpp_fetch_SOURCE_DIR}/bin/*.dll)
  install(FILES ${WHISPER_DLLS} DESTINATION "bin")
else()
  if(${CMAKE_BUILD_TYPE} STREQUAL Release OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
    set(Whispercpp_BUILD_TYPE Release)
  else()
    set(Whispercpp_BUILD_TYPE Debug)
  endif()
  set(Whispercpp_Build_GIT_TAG "v1.6.2")
  set(WHISPER_EXTRA_CXX_FLAGS "-fPIC")
  set(WHISPER_ADDITIONAL_CMAKE_ARGS -DWHISPER_BLAS=OFF -DWHISPER_CUBLAS=OFF -DWHISPER_OPENBLAS=OFF)

  # On Linux build a static Whisper library
  ExternalProject_Add(
    Whispercpp_Build
    DOWNLOAD_EXTRACT_TIMESTAMP true
    GIT_REPOSITORY https://github.com/ggerganov/whisper.cpp.git
    GIT_TAG ${Whispercpp_Build_GIT_TAG}
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config ${Whispercpp_BUILD_TYPE}
    BUILD_BYPRODUCTS <INSTALL_DIR>/lib/static/${CMAKE_STATIC_LIBRARY_PREFIX}whisper${CMAKE_STATIC_LIBRARY_SUFFIX}
    CMAKE_GENERATOR ${CMAKE_GENERATOR}
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config ${Whispercpp_BUILD_TYPE} && ${CMAKE_COMMAND} -E
                    copy <SOURCE_DIR>/ggml.h <INSTALL_DIR>/include
    CONFIGURE_COMMAND
      ${CMAKE_COMMAND} -E env ${WHISPER_ADDITIONAL_ENV} ${CMAKE_COMMAND} <SOURCE_DIR> -B <BINARY_DIR> -G
      ${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DCMAKE_BUILD_TYPE=${Whispercpp_BUILD_TYPE}
      -DCMAKE_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM} -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13
      -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES_} -DCMAKE_CXX_FLAGS=${WHISPER_EXTRA_CXX_FLAGS}
      -DCMAKE_C_FLAGS=${WHISPER_EXTRA_CXX_FLAGS} -DBUILD_SHARED_LIBS=OFF -DWHISPER_BUILD_TESTS=OFF
      -DWHISPER_BUILD_EXAMPLES=OFF ${WHISPER_ADDITIONAL_CMAKE_ARGS})

  ExternalProject_Get_Property(Whispercpp_Build INSTALL_DIR)

  # add the static Whisper library to the link line
  add_library(Whispercpp::Whisper STATIC IMPORTED)
  set_target_properties(
    Whispercpp::Whisper
    PROPERTIES IMPORTED_LOCATION
               ${INSTALL_DIR}/lib/static/${CMAKE_STATIC_LIBRARY_PREFIX}whisper${CMAKE_STATIC_LIBRARY_SUFFIX})
  set_target_properties(Whispercpp::Whisper PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)
endif()

add_library(Whispercpp INTERFACE)
add_dependencies(Whispercpp Whispercpp_Build)
target_link_libraries(Whispercpp INTERFACE Whispercpp::Whisper)
if(WIN32 AND "${ACCELERATION}" STREQUAL "cpu")
  target_link_libraries(Whispercpp INTERFACE Whispercpp::OpenBLAS)
endif()
if(APPLE)
  target_link_libraries(Whispercpp INTERFACE "-framework Accelerate -framework CoreML -framework Metal")
  target_link_libraries(Whispercpp INTERFACE Whispercpp::GGML Whispercpp::CoreML)
endif(APPLE)

# add exported target install
install(TARGETS Whispercpp EXPORT WhispercppTargets)
install(EXPORT WhispercppTargets NAMESPACE Whispercpp:: DESTINATION lib/cmake/Whispercpp)
install(FILES ${whispercpp_fetch_SOURCE_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}whisper${CMAKE_STATIC_LIBRARY_SUFFIX}
        DESTINATION "bin")
install(FILES ${whispercpp_fetch_SOURCE_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ggml${CMAKE_STATIC_LIBRARY_SUFFIX}
        DESTINATION "bin")
if(APPLE)
    install(FILES ${whispercpp_fetch_SOURCE_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}whisper.coreml${CMAKE_STATIC_LIBRARY_SUFFIX}
            DESTINATION "bin")
endif(APPLE)
