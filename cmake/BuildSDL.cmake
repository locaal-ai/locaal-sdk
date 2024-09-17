# Include ExternalProject module
include(ExternalProject)

# Set SDL version
set(SDL_VERSION "2.28.2")

# Define SDL installation directory
set(SDL_INSTALL_DIR "${CMAKE_BINARY_DIR}/sdl_install")

# ExternalProject for SDL2
ExternalProject_Add(
    SDL2_external
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-${SDL_VERSION}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${SDL_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DSDL_STATIC=ON
        -DSDL_SHARED=OFF
    BUILD_BYPRODUCTS
        "${SDL_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2${CMAKE_STATIC_LIBRARY_SUFFIX}"
        "${SDL_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX}"
)

# Create interface library for SDL2
add_library(SDL2 INTERFACE)
add_dependencies(SDL2 SDL2_external)

# Set include directories for the interface library
target_include_directories(SDL2 INTERFACE
    $<BUILD_INTERFACE:${SDL_INSTALL_DIR}/include/SDL2>
    $<INSTALL_INTERFACE:include/SDL2>
)

# Link SDL2 and SDL2main libraries
target_link_libraries(SDL2 INTERFACE
    $<BUILD_INTERFACE:${SDL_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2-static${CMAKE_STATIC_LIBRARY_SUFFIX}>
    $<BUILD_INTERFACE:${SDL_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX}>
)

# Platform-specific configurations
if(WIN32)
    target_link_libraries(SDL2 INTERFACE imm32 version winmm setupapi)
elseif(APPLE)
    target_link_libraries(SDL2 INTERFACE "-framework Cocoa" "-framework IOKit" "-framework CoreAudio" "-framework CoreVideo")
else()
    # Linux
    find_package(Threads REQUIRED)
    target_link_libraries(SDL2 INTERFACE Threads::Threads dl)
endif()

# Export the targets
install(TARGETS SDL2
    EXPORT SDL2Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Export the targets file
install(EXPORT SDL2Targets
    FILE SDL2Targets.cmake
    NAMESPACE SDL2::
    DESTINATION lib/cmake/SDL2
)
