
include(${CMAKE_SOURCE_DIR}/cmake/BuildSDL.cmake)

add_executable(RealtimeTranscription realtime_transcription.cpp audio_capture.cpp)
target_link_libraries(RealtimeTranscription PRIVATE SDL2 Core Transcription)

# add target "examples" to the global target list
add_custom_target(examples)
add_dependencies(examples RealtimeTranscription)

# install the target to the bin directory
install(TARGETS RealtimeTranscription DESTINATION bin)
