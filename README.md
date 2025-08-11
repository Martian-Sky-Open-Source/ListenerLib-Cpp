# MARTIAN SKY INDUSTRIES: Real-time COVERT Program - Translated to C++

## Clone this repo to build the C++ version of COVERT and its dependencies on Windows or ARM64 Linux:
### This project contains and utilizes the following header-only thrid-party libraries:
- **Eigen**
- **Nlohmann JSON**

### This project contains and utilizes Windows and ARM64 Linux versions of the following pre-built third-party libraries:
- **Open3D** static library
- **OpenCV** static library
- **Royale** dynamic libraries

### This project contains the C++ source and CMake files to build the following targets:
- **FrameLib** static library
	- Provides classes to contain and provide utilities for data commonly acquired from sensors in the MSI office.
- **tests_FrameLib** executable 
	- Unit tests for the **FrameLib** static library.
- **ListenerLib** static library
	- Provides a consistent interface for commonly used sensors in the MSI office with listener classes.
- **tests_ListenerLib** executable
	- Unit tests for the **ListenerLib** static library.
- **CovertICP** static library
	- Provides the core point cloud stitching and pose estimation functionality of the COVERT process.
- **tests_CovertICP** executable
	- Unit tests for the **CovertICP** static library.
- **COVERT** executable
	- Main COVERT program executable that links to all aforementioned libraries when being built.

### Build COVERT and its dependencies with CMake:
- Project contains the 'CMakeLists.txt' and 'CMakePresets.json' files to allow for a consistent CMake build process.
- Project is designed to be built on Windows or ARM64 Linux, **NOT** x64 Linux.
- Project can be built in *Release* or *Debug* configuration on Windows, but **ONLY** in *Release* configuration on ARM64 Linux.
- It is recommended for simplicity that you use the *Visual Studio* (Windows) or *VSCode* (ARM64 Linux) CMake extension to build this project.
- Built targets and tests will be output in the project's top-level './build/[*configuration*]' directory.

### Run the built COVERT executable as a standalone process:
- Copy the 'build/[*configuration*]/bin' folder to the necessary location, renaming if desired.
- The COVERT executable, two **Royale** dynamic library files, and a 'resources' folder are contained within.
	- The COVERT executable looks for the other objects in its own directory, so do not move the files relative to each other.
- Run the COVERT executable ('COVERT.exe' on Windows, 'COVERT' on ARM64 Linux) to start the main COVERT process.

### Build the CMake project for RasPi:
```
cmake -S . -B ./cmake-build-raspi --preset release
cmake --build ./cmake-build-raspi --target COVERT_flight -j 2
docker compose up
bash push_docker_image.sh
```

### Upload Docker Image:
```
bash push_docker_image.sh
```

### Save image as tar archive:
```
docker pull 192.168.1.50:3000/msi-engineering/covert:latest
docker tag 192.168.1.50:3000/msi-engineering/covert:latest covert:latest
docker save -o covertRevB.tar covert:latest
```