add_library(Core
    src/model-downloader.cpp
    src/model-infos.cpp
    src/model-find-utils.cpp
    src/logger.cpp
)

target_link_libraries(Core PUBLIC sago_platform_folders_lib libcurl)
target_include_directories(Core
    PRIVATE
    ${SAGO_INSTALL_DIR}/include
)

target_include_directories(Core
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

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
