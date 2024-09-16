add_library(Translation
    src/language_codes.cpp
    src/translation-language-utils.cpp
    src/translation-utils.cpp
    src/translation.cpp
)

target_include_directories(Translation
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(Translation INTERFACE ICU ct2 sentencepiece)

set_target_properties(Translation PROPERTIES
    OUTPUT_NAME locaal_translation
    EXPORT_NAME Translation
)

# Install the target and create export
install(TARGETS Translation
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
