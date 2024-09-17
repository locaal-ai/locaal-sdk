
include(${CMAKE_SOURCE_DIR}/cmake/BuildSDL.cmake)

add_executable(RealtimeTranscription realtime_transcription.cpp audio_capture.cpp)
target_link_libraries(RealtimeTranscription PRIVATE SDL2 Core Transcription)
