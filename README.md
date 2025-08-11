# MARTIAN SKY INDUSTRIES: Sensor Management Library - Translated to C++

## Clone this repo to build the C++ version of ListenerLib and its dependencies on Windows or ARM64 Linux:
### This project contains and utilizes the following header-only third-party libraries:
- **Eigen**
- **Nlohmann JSON**

### This project contains and utilizes Windows and ARM64 Linux versions of the following pre-built third-party libraries:
- **Open3D** static library
- **OpenCV** static library
- **Royale** dynamic libraries

### This project contains the C++ source and CMake files to build the following targets:
- **ListenerLib** static library
	- Provides a consistent interface for commonly used sensors in the MSI office with listener classes.
- **tests_ListenerLib** executable
	- Unit tests for the **ListenerLib** static library.

### Build targets and dependencies with CMake:
- Project contains the 'CMakeLists.txt' and 'CMakePresets.json' files to allow for a consistent CMake build process.
- Project is designed to be built on Windows or ARM64 Linux, **NOT** x64 Linux.
- Project can be built in *Release* or *Debug* configuration on Windows, but **ONLY** in *Release* configuration on ARM64 Linux.
- It is recommended for simplicity that you use the *Visual Studio* (Windows) or *VSCode* (ARM64 Linux) CMake extension to build this project.
- Built targets and tests will be output in the project's top-level './build/[*configuration*]' directory.

### Build the CMake project for RasPi:
```
cmake -S . -B ./cmake-build-raspi --preset release
cmake --build ./cmake-build-raspi --target demo -j 2