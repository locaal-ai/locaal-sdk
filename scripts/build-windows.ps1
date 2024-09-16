
cmake -S . -B build_x64 -DCMAKE_BUILD_TYPE=Release -DLocaalSDK_FIND_COMPONENTS="Core;Transcription;Translation"

cmake --build build_x64 --config Release
