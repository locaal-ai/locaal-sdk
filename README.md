# Real-time Transcription and Translation Library

## Overview

This C++ library provides real-time transcription and translation capabilities using Whisper.cpp and CTranslate2. It's designed to work on-device without relying on cloud services, making it suitable for applications requiring privacy and offline functionality.

Key features:
- Cross-platform support (macOS, Windows, Linux)
- Real-time speech-to-text transcription
- On-device translation
- Built with CMake for easy integration and compilation

## Prerequisites

Before building the library, ensure you have the following installed:
- C++ compiler with C++17 support
- CMake (version 3.12 or higher)
- Git

## Building the Library

### macOS

1. Open Terminal and navigate to the project directory.
2. Run the following commands:

```bash
mkdir build && cd build
cmake ..
make
```

### Windows

1. Open Command Prompt or PowerShell and navigate to the project directory.
2. Run the following commands:

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

Note: Adjust the Visual Studio version as needed.

### Linux

1. Open a terminal and navigate to the project directory.
2. Run the following commands:

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

After building the library, you can include it in your C++ project. Here's a basic example of how to use the library:

```cpp
#include <locaal.h>

int main() {
    // Initialize the library
    locaal::TranscriptionTranslation tt;

    // Start real-time transcription
    tt.startTranscription();

    // Translate text
    std::string translated = tt.translate("Hello, world!", "en", "fr");

    return 0;
}
```

For more detailed usage instructions and API documentation, please refer to the `docs` folder and the `examples` folder.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- [Whisper.cpp](https://github.com/ggerganov/whisper.cpp)
- [CTranslate2](https://github.com/OpenNMT/CTranslate2)
