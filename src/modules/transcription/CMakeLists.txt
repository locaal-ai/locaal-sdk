add_library(Transcription
    src/silero-vad-onnx.cpp
    src/token-buffer-thread.cpp
    src/transcription-utils.cpp
    src/vad-processing.cpp
    src/whisper-model-utils.cpp
    src/whisper-processing.cpp
    src/whisper-utils.cpp
)

target_include_directories(Transcription
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# If you have any dependencies for the Core module, link them here
# target_link_libraries(Core PUBLIC SomeDependency)

set_target_properties(Transcription PROPERTIES
    OUTPUT_NAME locaal_transcription
    EXPORT_NAME Transcription
)

# Install the target and create export
install(TARGETS Transcription
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
