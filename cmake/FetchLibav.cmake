include(FetchContent)

if(WIN32)
    include(FetchContent)

    set(FFMPEG_URL "https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-full-shared.7z")
    set(FFMPEG_HASH "SHA256=a0b6e8c7978b95d019a93dcf4b4ab74b17d9e53e0a87cfd463e1376c5927e30b")

    FetchContent_Declare(
      FFmpeg_fetch
      DOWNLOAD_EXTRACT_TIMESTAMP
      URL ${FFMPEG_URL}
      URL_HASH ${FFMPEG_HASH}
    )

    FetchContent_MakeAvailable(FFmpeg_fetch)

    set(FFMPEG_ROOT ${ffmpeg_fetch_SOURCE_DIR})

    find_path(FFMPEG_INCLUDE_DIR libavcodec/avcodec.h
      PATHS ${FFMPEG_ROOT}/include
      NO_DEFAULT_PATH
    )

    find_library(AVCODEC_LIBRARY avcodec
      PATHS ${FFMPEG_ROOT}/lib
      NO_DEFAULT_PATH
    )
    find_library(AVFORMAT_LIBRARY avformat
      PATHS ${FFMPEG_ROOT}/lib
      NO_DEFAULT_PATH
    )
    find_library(AVUTIL_LIBRARY avutil
      PATHS ${FFMPEG_ROOT}/lib
      NO_DEFAULT_PATH
    )
    find_library(SWRESAMPLE_LIBRARY swresample
      PATHS ${FFMPEG_ROOT}/lib
      NO_DEFAULT_PATH
    )

    set(FFMPEG_LIBRARIES
        ${AVCODEC_LIBRARY}
        ${AVFORMAT_LIBRARY}
        ${AVUTIL_LIBRARY}
        ${SWRESAMPLE_LIBRARY}
    )
else()
    # For Linux and macOS, use pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
        libavcodec
        libavformat
        libavutil
        libswresample
    )
    set(FFMPEG_INCLUDE_DIR ${FFMPEG_INCLUDE_DIRS})
    set(FFMPEG_LIBRARIES PkgConfig::FFMPEG)
endif()

if(WIN32)
    # Add FFmpeg bin directory to PATH for runtime
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=${FFMPEG_ROOT}/bin;$ENV{PATH}")

    # Copy DLLs to output directory using install
    install(DIRECTORY ${FFMPEG_ROOT}/bin/ DESTINATION bin)
endif()

# Create FFmpeg interface library
add_library(FFmpeg INTERFACE)
target_include_directories(FFmpeg INTERFACE
    $<BUILD_INTERFACE:${FFMPEG_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(FFmpeg INTERFACE ${FFMPEG_LIBRARIES})

# add exported target
install(TARGETS FFmpeg EXPORT FFmpegTargets)
install(EXPORT FFmpegTargets
    FILE FFmpegTargets.cmake
    NAMESPACE FFmpeg::
    DESTINATION lib/cmake/FFmpeg
)
