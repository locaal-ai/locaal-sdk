include(ExternalProject)

set(SAGO_INSTALL_DIR ${CMAKE_BINARY_DIR}/external/sago_platform_folders)

# Define the sago::platform_folders external project
ExternalProject_Add(
    sago_platform_folders
    GIT_REPOSITORY https://github.com/sago007/PlatformFolders.git
    GIT_TAG master  # You might want to use a specific tag or commit hash for stability
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${SAGO_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --config ${CMAKE_BUILD_TYPE}
    INSTALL_COMMAND ${CMAKE_COMMAND} --install . --config ${CMAKE_BUILD_TYPE}
)

# Create an interface library for sago::platform_folders
add_library(sago_platform_folders_lib INTERFACE)
add_dependencies(sago_platform_folders_lib sago_platform_folders)

# Set include directories for the interface library
target_include_directories(sago_platform_folders_lib
    INTERFACE
        $<BUILD_INTERFACE:${SAGO_INSTALL_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# add exported target install
install(TARGETS sago_platform_folders_lib
    EXPORT sago_platform_folders_lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export the targets file
install(EXPORT sago_platform_folders_lib
    NAMESPACE sago::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sago_platform_folders_lib
)
