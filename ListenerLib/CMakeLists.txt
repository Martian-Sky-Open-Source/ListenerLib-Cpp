# Name subproject to set source and binary directory variables
project(ListenerLib)

# Download and extract Royale content
FetchContent_Declare(
		royale_gitea
		URL http://packagebot:iamthepackagebot@192.168.1.50:3000/api/packages/MSI-Engineering/generic/royale-${PLATFORM}/5.12.0.3089/royale-${PLATFORM}-5.12.0.3089.${EXT}
		SOURCE_DIR "${CMAKE_BINARY_DIR}/../royale_gitea"
		DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(royale_gitea)

# Download and extract RealSense content
FetchContent_Declare(
		librealsense_gitea
		URL http://packagebot:iamthepackagebot@192.168.1.50:3000/api/packages/MSI-Engineering/generic/librealsense-${PLATFORM}/2.56.4/librealsense-${PLATFORM}-2.56.4.${EXT}
		SOURCE_DIR "${CMAKE_BINARY_DIR}/../librealsense_gitea"
		DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(librealsense_gitea)

if(PLATFORM STREQUAL "windows")
	set(SENSOR_SHARED_LIBS
			"${royale_gitea_SOURCE_DIR}/bin/royale.dll"
			"${royale_gitea_SOURCE_DIR}/bin/spectre.dll"
			"${librealsense_gitea_SOURCE_DIR}/${LIB_BUILD_FOLDER}/bin/realsense2$<IF:$<CONFIG:Debug>,d,>.dll"
		CACHE INTERNAL ""
	)
else()
	set(SENSOR_SHARED_LIBS
			"${royale_gitea_SOURCE_DIR}/bin/libroyale.so.5.12.0"
			"${royale_gitea_SOURCE_DIR}/bin/libspectre.so"
			CACHE INTERNAL ""
	)
endif()

list(APPEND CMAKE_PREFIX_PATH
		"${royale_gitea_SOURCE_DIR}/lib"
		"${librealsense_gitea_SOURCE_DIR}/${LIB_BUILD_FOLDER}/lib"
)

# Create physical sensors interface library
add_library(sensors_interface INTERFACE)
find_package(royale REQUIRED)
find_package(realsense2 REQUIRED)
target_include_directories(sensors_interface
	INTERFACE
		${royale_INCLUDE_DIRS}
		${realsense2_INCLUDE_DIRS}
)
target_link_libraries(sensors_interface
	INTERFACE
		royale::royale
		realsense2::realsense2
)
target_precompile_headers(sensors_interface INTERFACE ${CMAKE_SOURCE_DIR}/pch/pchSensors.h)

# Collect ListenerLib source files
file(GLOB SUBPROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Create ListenerLib static library target
add_library(ListenerLib STATIC ${SUBPROJECT_SOURCES})

# Specify include directories for ListenerLib static library
target_include_directories(ListenerLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Link dependency static libraries to ListenerLib static library
target_link_libraries(ListenerLib
	PUBLIC
		sensors_interface
		ListenerLibLite
)