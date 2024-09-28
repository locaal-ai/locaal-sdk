param(
    [switch]$Verbose,
    [switch]$Clean,
    [switch]$Examples,
    [switch]$Install
)

# Set build directory
$buildDir = "build_x64"

$generateFlags = ""
$buildFlags = ""

if ($Verbose) {
    $generateFlags += "-DCMAKE_VERBOSE_MAKEFILE=ON"
    $buildFlags += "--verbose"
}

if ($Examples) {
    $generateFlags += " -DBUILD_EXAMPLES=ON"
}

# Clean build directory if requested
if ($Clean) {
    if (Test-Path $buildDir) {
        Write-Host "Cleaning build directory: $buildDir"
        Remove-Item -Recurse -Force $buildDir
    }
    else {
        Write-Host "Build directory does not exist. Nothing to clean."
    }
}

# Configure step
$configureCommand = "cmake -S . -B $buildDir -DCMAKE_BUILD_TYPE=Release ``
    -DLocaalSDK_FIND_COMPONENTS=`"Core;Transcription;Translation`" $generateFlags"
Write-Host "Executing configure command: $configureCommand"
Invoke-Expression $configureCommand

# Build step
$buildCommand = "cmake --build $buildDir --config Release $buildFlags"
Write-Host "Executing build command: $buildCommand"
Invoke-Expression $buildCommand

# Install step
if ($Install) {
    $installCommand = "cmake --install $buildDir --config Release --prefix ./installed/"
    Write-Host "Executing install command: $installCommand"
    Invoke-Expression $installCommand
}

Write-Host "Done."
