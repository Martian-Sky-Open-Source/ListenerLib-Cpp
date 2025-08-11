# Name subproject to set source and binary directory variables
project(ListenerLibLite)

# Create NaturalSort interface library
add_library(naturalsort_interface INTERFACE)
target_include_directories(naturalsort_interface INTERFACE ${CMAKE_SOURCE_DIR}/3rdparty/NaturalSort/include)
target_precompile_headers(naturalsort_interface INTERFACE ${CMAKE_SOURCE_DIR}/pch/pchNaturalSort.h)

# Create nlohmann interface library
add_library(nlohmann_interface INTERFACE)
target_include_directories(nlohmann_interface INTERFACE ${CMAKE_SOURCE_DIR}/3rdparty/nlohmann)
target_precompile_headers(nlohmann_interface INTERFACE ${CMAKE_SOURCE_DIR}/pch/pchNlohmann.h)

# Create libnpy interface library
add_library(libnpy_interface INTERFACE)
target_include_directories(libnpy_interface INTERFACE ${CMAKE_SOURCE_DIR}/3rdparty/libnpy-1.0.1/include)
target_precompile_headers(libnpy_interface INTERFACE ${CMAKE_SOURCE_DIR}/pch/pchLibnpy.h)

# Create Eigen interface library
add_library(eigen_interface INTERFACE)
target_include_directories(eigen_interface INTERFACE ${CMAKE_SOURCE_DIR}/3rdparty/eigen-3.4.0)
target_precompile_headers(eigen_interface INTERFACE ${CMAKE_SOURCE_DIR}/pch/pchEigen.h)

# Download and extract Open3D content
FetchContent_Declare(
		open3d_gitea
		URL http://packagebot:iamthepackagebot@192.168.1.50:3000/api/packages/MSI-Engineering/generic/open3d-${PLATFORM}/0.19.0/open3d-${PLATFORM}-0.19.0.${EXT}
		SOURCE_DIR "${CMAKE_BINARY_DIR}/../open3d_gitea"
		DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(open3d_gitea)

# Download and extract OpenCV content
FetchContent_Declare(
		opencv_gitea
		URL http://packagebot:iamthepackagebot@192.168.1.50:3000/api/packages/MSI-Engineering/generic/opencv-${PLATFORM}/4.11.0/opencv-${PLATFORM}-4.11.0.${EXT}
		SOURCE_DIR "${CMAKE_BINARY_DIR}/../opencv_gitea"
		DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(opencv_gitea)

# Set CMAKE_PREFIX_PATH based on operating system to find third-party dependency static libraries
list(APPEND CMAKE_PREFIX_PATH
		"${opencv_gitea_SOURCE_DIR}/${LIB_BUILD_FOLDER}"
		"${open3d_gitea_SOURCE_DIR}/${LIB_BUILD_FOLDER}"
)

# Create Open3D interface library
find_package(Open3D REQUIRED)
add_library(open3d_interface INTERFACE)
target_include_directories(open3d_interface INTERFACE ${Open3D_INCLUDE_DIRS})
target_link_libraries(open3d_interface INTERFACE Open3D::Open3D)
target_precompile_headers(open3d_interface INTERFACE ${CMAKE_SOURCE_DIR}/pch/pchOpen3D.h)

# Create OpenCV interface library
find_package(OpenCV REQUIRED)
add_library(opencv_interface INTERFACE)
target_include_directories(opencv_interface INTERFACE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv_interface INTERFACE ${OpenCV_LIBRARIES})
target_precompile_headers(opencv_interface INTERFACE ${CMAKE_SOURCE_DIR}/pch/pchOpenCV.h)

# Collect ListenerLibLite source files
file(GLOB SUBPROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src/*/*.cpp)

# Create ListenerLibLite static library target
add_library(ListenerLibLite STATIC ${SUBPROJECT_SOURCES}
        src/sensor_interfaces/ScanningLidarInterface.cpp
		include/sensor_interfaces/ScanningLidarInterface.h)

# Specify include directories for ListenerLibLite static library
target_include_directories(ListenerLibLite PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Link dependency static libraries to ListenerLibLite static library
target_link_libraries(ListenerLibLite
	PUBLIC
		naturalsort_interface
		nlohmann_interface
		libnpy_interface
		eigen_interface
		open3d_interface
		opencv_interface
)

# Add test project for subproject and set its build directory
add_subdirectory(tests ${PROJECT_BINARY_DIR}/tests)