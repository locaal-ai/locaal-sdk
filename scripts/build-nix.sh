#!/bin/bash

verbose=false
clean=false
examples=false
install=false

# Parse command line arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -v|--verbose) verbose=true ;;
        -c|--clean) clean=true ;;
        -e|--examples) examples=true ;;
        -i|--install) install=true ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

generate_flags=""
build_flags=""

if [ "$verbose" = true ] ; then
    generate_flags="-DCMAKE_VERBOSE_MAKEFILE=ON"
    build_flags="--verbose"
fi

if [ "$examples" = true ] ; then
    generate_flags="$generate_flags -DBUILD_EXAMPLES=ON"
fi

build_dir="build"

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

if [ "$install" = true ] ; then
    install_command="cmake --install $build_dir --config Release $verbose_build_flag"
    echo "Executing install command: $install_command"
    eval $install_command
fi
