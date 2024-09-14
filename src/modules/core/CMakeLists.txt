add_library(Core
    src/model-downloader.cpp
    src/model-infos.cpp
    src/model-find-utils.cpp
)

target_include_directories(Core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# If you have any dependencies for the Core module, link them here
# target_link_libraries(Core PUBLIC SomeDependency)

set_target_properties(Core PROPERTIES
    OUTPUT_NAME locaal_core
    EXPORT_NAME Core
)

# Install the target and create export
install(TARGETS Core
    EXPORT LocaalSDKTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install public headers
install(DIRECTORY include/locaal
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
