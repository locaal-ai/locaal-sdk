
find_package(LocaalSDK REQUIRED COMPONENTS Core Transcription Translation)

add_executable(RealtimeTranscription realtime_transcription.cpp)
target_link_libraries(MyApp PRIVATE LocaalSDK::Core LocaalSDK::Transcription)
