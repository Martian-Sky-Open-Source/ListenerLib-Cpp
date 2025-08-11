# Specify minimum CMake version
cmake_minimum_required(VERSION 3.19)

# Name top-level project
project(ListenerLibCpp)

# Enable testing
enable_testing()

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Allow CMake to build targets with relative paths to shared libraries
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")

# Set variable for target platform and runtime directory royale shared libraries
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(EXT "tar.gz")
	if (EXISTS "/proc/device-tree/model")
		file(READ "/proc/device-tree/model" MODEL)
		if (MODEL MATCHES "Raspberry Pi")
			set(PLATFORM "pi")
		endif()
	else()
		set(PLATFORM "linux_amd64")
	endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(PLATFORM "windows")
	set(EXT "zip")
else()
	message("Cannot determine target operating system.")
endif()

# Set library build directory string based on configuration
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(LIB_BUILD_FOLDER "build-r")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(LIB_BUILD_FOLDER "build-d")
else()
	message("Unknown build configuration")
endif()

# Use FetchContent module to download prebuilt libraries
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# Add subproject directories and set their build directories
add_subdirectory(ListenerLibLite ${CMAKE_BINARY_DIR}/ListenerLibLite)
if (PLATFORM MATCHES "windows")
	add_subdirectory(ListenerLib ${CMAKE_BINARY_DIR}/ListenerLib)
endif()

# Add GTest source subdirectory
add_subdirectory(3rdparty/googletest-1.15.2)

# Add demo executable target
add_executable(demo main.cpp)

# Output demo executable to bin folder in build directory
set_target_properties(demo PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
)

# Link dependency static libraries to demo executable
target_link_libraries(demo
	PRIVATE
		ListenerLib
)