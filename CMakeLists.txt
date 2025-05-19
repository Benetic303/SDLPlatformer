# Set the minimum required CMake version
# Use a version that supports features like target_link_directories visibility (PRIVATE)
cmake_minimum_required(VERSION 3.15)

# Define your project
project(SDLPlatformer VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard to C++20
# This ensures your code is compiled using the specified standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False) # Optional: Disallow compiler extensions

# --- Source Files ---
# Define the source files for your executable
# Add all your .cpp files here
set(GAME_SOURCES
    src/camera.cpp
    src/camera.hpp
    src/Chunk.cpp
    src/Chunk.hpp
    src/enemy.cpp
    src/enemy.hpp
    src/entity.cpp
    src/entity.hpp
    src/main.cpp
    src/math.cpp
    src/math.hpp
    src/noise.cpp
    src/noise.hpp
    src/Player.cpp
    src/Player.hpp
    src/renderwindow.cpp
    src/RenderWindow.hpp
    src/Utils.cpp
    src/Utils.hpp
    src/World.cpp
    src/World.hpp
    # Add other source files here
)

# Add the executable target
# Use the variable defined above to pass the list of source files
add_executable(SDLPlatformer ${GAME_SOURCES})

# --- Include your own project's headers (optional but good practice) ---
# This ensures CMake knows where to find your .hpp files if they're not in the same dir as .cpp
target_include_directories(SDLPlatformer
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src # Adds the src directory to include paths for your own headers
)

# --- External Libraries: Configure SDL3, SDL3_image, SDL3_ttf for Static Linking ---

# Define paths to the roots of your extracted SDL development packages
# Use CMAKE_SOURCE_DIR which points to the directory containing this CMakeLists.txt
set(SDL3_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL3-3.2.12")
set(SDL3_IMAGE_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL3_image-3.2.4")
set(SDL3_TTF_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL3_ttf-3.2.2")

# Add Include Directories for SDL
# These are PUBLIC because your game code needs the headers
target_include_directories(SDLPlatformer
    PUBLIC
        ${SDL3_DIR}/include
        ${SDL3_IMAGE_DIR}/include
        ${SDL3_TTF_DIR}/include
)

# Add Library Directories for SDL
# These are PRIVATE because only the linker needs to know where the .lib files are
# We use generator expressions to handle the configuration (Debug/Release) and platform (x64/x86) specific paths
target_link_directories(SDLPlatformer
    PRIVATE
        # Use CMAKE_LIBRARY_ARCHITECTURE to automatically select x64 or x86
        ${SDL3_DIR}/lib/x64
        ${SDL3_IMAGE_DIR}/lib/x64
        ${SDL3_TTF_DIR}/lib/x64
)
# NOTE: Adjust the paths and configurations (Debug/Release, x64/x86) above
# based on the exact structure of your extracted library folders.
# For simpler structures like lib/x64/ only, you might just use:
# ${SDL3_DIR}/lib/x64
# ${SDL3_IMAGE_DIR}/lib/x64
# ${SDL3_TTF_DIR}/lib/x64


# Link the Libraries
# List the specific .lib files to link.
# Order sometimes matters for static libs: dependents *before* dependencies.
# Add system libraries needed for static linking on Windows with SDL.
target_link_libraries(SDLPlatformer
    PRIVATE
        # SDL3_image and SDL3_ttf depend on SDL3
        SDL3_image.lib
        SDL3_ttf.lib
        SDL3.lib

        # Windows system libraries required for static SDL linking
        user32.lib
        gdi32.lib
        winmm.lib
        imm32.lib
        version.lib
        setupapi.lib
        shell32.lib
        dinput8.lib  # Often needed for input handling
        dxguid.lib   # Often needed with dinput8 or other DX components
        # Add any other system libraries your specific libraries might need
)

# --- CRITICAL for MSVC (Visual Studio) - Set Runtime Library ---
# This MUST match the Runtime Library setting used to compile the static SDL libraries.
# For static linking, this is typically Multi-threaded (/MT) for Release and Multi-threaded Debug (/MTd) for Debug.
set_property(TARGET SDLPlatformer PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# If your libraries were built with a different runtime (unlikely for static VC libs), adjust this.

# --- Optional: Add Preprocessor Definitions ---
# SDL_MAIN_HANDLED is often needed if you provide your own main() function
# _CRT_SECURE_NO_WARNINGS is common to suppress warnings from C-style functions in MSVC
target_compile_definitions(SDLPlatformer
    PRIVATE
        # SDL_MAIN_HANDLED
        _CRT_SECURE_NO_WARNINGS
    # Add other definitions your project might need
)

# --- Optional: Set Output Directory ---
# This places the final executable in a specific location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



# --- Copy required DLLs to the output directory using add_custom_command ---
# This ensures they are alongside your executable for runtime.
# add_custom_command runs *during the build process*, allowing $<CONFIG> to be resolved correctly.

# Define a list of all DLLs you need to copy
# IMPORTANT: Adjust the paths for your DLLs and add ALL other dependency DLLs
# that SDL_image and SDL_ttf might require (e.g., zlib1.dll, libpng16.dll, freetype.dll).
# You must find these in your SDL_image/SDL_ttf downloaded packages (often in 'lib' or 'bin' folders).
set(SDL_RUNTIME_DLL_PATHS
    "${SDL3_DIR}/lib/x64/SDL3.dll"
    "${SDL3_IMAGE_DIR}/lib/x64/SDL3_image.dll"
    "${SDL3_TTF_DIR}/lib/x64/SDL3_ttf.dll"
    # Example for other common dependencies - CHECK YOUR DOWNLOADED PACKAGES FOR THESE:
    # "${SDL3_IMAGE_DIR}/lib/${CMAKE_LIBRARY_ARCHITECTURE}/zlib1.dll"
    # "${SDL3_IMAGE_DIR}/lib/${CMAKE_LIBRARY_ARCHITECTURE}/libpng16.dll"
    # "${SDL3_IMAGE_DIR}/lib/${CMAKE_LIBRARY_ARCHITECTURE}/jpeg.dll"
    # "${SDL3_IMAGE_DIR}/lib/${CMAKE_LIBRARY_ARCHITECTURE}/webp.dll"
    # "${SDL3_TTF_DIR}/lib/${CMAKE_LIBRARY_ARCHITECTURE}/freetype.dll"
    # ... add any other .dlls you find in your SDL/SDL_image/SDL_ttf packages
)

# Loop through each DLL path and create a custom command to copy it
foreach(DLL_SRC_PATH IN LISTS SDL_RUNTIME_DLL_PATHS)
    # Extract just the filename (e.g., SDL3.dll from C:/path/to/SDL3.dll)
    get_filename_component(DLL_FILENAME "${DLL_SRC_PATH}" NAME)

    # Add a custom command that runs POST_BUILD (after SDLPlatformer is built)
    # to copy the DLL to the executable's output directory.
    add_custom_command(
        TARGET SDLPlatformer
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL_SRC_PATH}"
            "$<TARGET_FILE_DIR:SDLPlatformer>/${DLL_FILENAME}"
        COMMENT "Copying runtime DLL: ${DLL_FILENAME}"
    )
endforeach()





# --- Copy Assets to the output directory ---
# This ensures your game can find its textures, sounds, etc. at runtime.
# It will copy the entire 'assets' folder from your source directory into the output.
add_custom_command(
    TARGET SDLPlatformer
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"  # Source assets directory (adjust if your assets aren't directly in src/assets)
        "$<TARGET_FILE_DIR:SDLPlatformer>/assets" # Destination: 'assets' subfolder in exe's output dir
    COMMENT "Copying game assets to output directory"
)