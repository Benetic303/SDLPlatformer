It's my first time using GitHub and SDL3, therefore the code might be bad, but I see it as a learning experience.

This project is written in C++ with the libaries SDL3-3.2.10, SDL3_image-3.2.4 and SDL3_ttf-3.1.0. 

Prerequisites:
  * **Git:** To clone the repository.
  * **CMake:** Version **3.15 or newer** is recommended.
  * **A C++ Compiler:**
      * **Windows:** Visual Studio (Community Edition is free) with the "Desktop development with C++" workload.
      * **Linux:** `g++` or `clang++` (often included in a `build-essential` package).
      * **macOS:** Xcode Command Line Tools (`clang++`).

Note:
  The libaries are installed inside third_party, but you can also install them manually.
  CMake is configured to detect them, but make sure your directories for the libaries match the directories in CMakeLists.txt

Building the game:
  1. Create a new directory called build inside the root folder.
  2. Run "cmake .." and specify your specific IDE or build system:
     Example:
       "cmake .. -G "Visual Studio 17 2022" -A x64" for a Visual Studio Solution

Running the game is specifc to your build system.
       
  
