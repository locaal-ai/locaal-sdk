# Locaal SDK

## Overview

Locaal SDK is a comprehensive, modular on-device AI toolkit designed to bring advanced AI capabilities to various platforms including desktop, mobile devices, and web applications. Our focus is on providing powerful, efficient, and privacy-preserving AI features that run locally on the device, eliminating the need for cloud-based processing.

## Key Features

Locaal SDK offers a wide range of AI capabilities through its modular architecture:

- **Transcription & Translation**: Real-time speech-to-text and language translation
- **Optical Character Recognition (OCR)**: Extract text from images and documents
- **Document Analysis**: Understand and extract information from structured documents
- **Speech Synthesis**: Convert text to natural-sounding speech
- **Image Segmentation**: Identify and separate different objects within images
- **Core Module**: Common utilities and shared functionalities

Each feature is implemented as a separate module, allowing developers to include only the capabilities they need, optimizing for performance and resource usage.

## Supported Platforms

- Desktop: Windows, macOS, Linux
- Mobile: iOS, Android
- Web: WebAssembly-compatible browsers

## Prerequisites

Before building the SDK, ensure you have the following installed:
- C++ compiler with C++17 support
- CMake (version 3.12 or higher)
- Git

Additional dependencies may be required for specific modules. Refer to each module's documentation for details.

## Building the SDK

### Desktop (Windows, macOS, Linux)

1. Clone the repository:
   ```
   git clone https://github.com/your-repo/locaal-sdk.git
   cd locaal-sdk
   ```

2. Create a build directory:
   ```
   mkdir build && cd build
   ```

3. Configure and build:
   ```
   cmake ..
   cmake --build . --config Release
   ```

### Mobile and Web Platforms

For mobile and web platforms, please refer to the platform-specific build instructions in the `docs/` directory.


## Usage

### Including Locaal SDK in Your CMake Project

To use Locaal SDK in your CMake project, follow these steps:

1. First, make sure you have the Locaal SDK installed on your system or available as a subdirectory in your project.

2. In your `CMakeLists.txt` file, add the following lines to find and link the Locaal SDK:

```cmake
# Find the Locaal SDK package
find_package(LocaalSDK REQUIRED)

# Create your executable or library
add_executable(your_app main.cpp)

# Link against the Locaal SDK modules you need
target_link_libraries(your_app
    PRIVATE
    LocaalSDK::Core
    LocaalSDK::Transcription
    LocaalSDK::Translation
    # Add other modules as needed
)
```

If you're using Locaal SDK as a subdirectory in your project:

```cmake
# Add the Locaal SDK subdirectory
add_subdirectory(path/to/locaal-sdk)

# Create your executable or library
add_executable(your_app main.cpp)

# Link against the Locaal SDK modules you need
target_link_libraries(your_app
    PRIVATE
    LocaalSDK::Core
    LocaalSDK::Transcription
    LocaalSDK::Translation
    # Add other modules as needed
)
```

### Code Example

Here's a basic example of how to use the Locaal SDK in your C++ project:

```cpp
#include <locaal/core.h>
#include <locaal/transcription.h>
#include <locaal/translation.h>

int main() {
    locaal::Core core;
    
    // Initialize transcription module
    locaal::Transcription transcription(core);
    
    // Start real-time transcription
    transcription.start();
    
    // Initialize translation module
    locaal::Translation translation(core);
    
    // Translate text
    std::string translated = translation.translate("Hello, world!", "en", "fr");
    
    return 0;
}
```

For more detailed usage instructions and API documentation for each module, please refer to the `docs/` folder.


## Modules

- **Core**: Provides common utilities and shared functionalities used across other modules.
- **Transcription**: Enables real-time speech-to-text capabilities.
- **Translation**: Offers text translation between multiple languages.
- **OCR**: Extracts text from images and documents.
- **Document Analysis**: Analyzes and extracts information from structured documents.
- **Speech Synthesis**: Converts text to natural-sounding speech.
- **Image Segmentation**: Identifies and separates different objects within images.

Each module can be used independently or in combination with others, depending on your application's needs.

## Contributing

We welcome contributions to the Locaal SDK! Please feel free to submit issues, feature requests, or pull requests.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

Locaal SDK leverages several amazing open-source projects, including:
- [Whisper.cpp](https://github.com/ggerganov/whisper.cpp) for transcription
- [CTranslate2](https://github.com/OpenNMT/CTranslate2) for translation
- [GGML](https://github.com/ggerganov/ggml) for on-device execution
- [OpenCV](https://opencv.org/) for image processing
- [onnxruntime](https://github.com/microsoft/onnxruntime) for on-device execution
- [cUrl](https://github.com/curl/curl) for networking
- [SDL](https://github.com/libsdl-org/SDL) for media processing and access
- [ICU](https://github.com/unicode-org/icu) for unicode text processing

We are grateful to the developers and contributors of these projects.
