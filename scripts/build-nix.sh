#!/bin/bash

verbose=false
clean=false

# Parse command line arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -v|--verbose) verbose=true ;;
        -c|--clean) clean=true ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

verbose_flag=""
verbose_build_flag=""

if [ "$verbose" = true ] ; then
    verbose_flag="-DCMAKE_VERBOSE_MAKEFILE=ON"
    verbose_build_flag="--verbose"
fi

build_dir="build_x64"

# Clean build directory if requested
if [ "$clean" = true ] ; then
    if [ -d "$build_dir" ] ; then
        echo "Cleaning build directory: $build_dir"
        rm -rf "$build_dir"
    else
        echo "Build directory does not exist. Nothing to clean."
    fi
fi

# Configure step
configure_command="cmake -S . -B $build_dir -DCMAKE_BUILD_TYPE=Release -DLocaalSDK_FIND_COMPONENTS=\"Core;Transcription;Translation\" $verbose_flag"
echo "Executing configure command: $configure_command"
eval $configure_command

# Build step
build_command="cmake --build $build_dir --config Release $verbose_build_flag"
echo "Executing build command: $build_command"
eval $build_command
