cmake_minimum_required(VERSION 3.10)
project(projekt2_opencv)

set(CMAKE_CXX_STANDARD 17)

# Path to build folder OpenCV
set(OpenCV_DIR "C:/opencv/build")

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add header paths
include_directories(${OpenCV_INCLUDE_DIRS})

# Add source file
add_executable(projekt2_opencv main.cpp utils.cpp filters.cpp operations.cpp)

# Link libraries OpenCV
target_link_libraries(projekt2_opencv ${OpenCV_LIBS})

# Copy DDLs OpenCV to Release folder
if(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_CONFIGURATION_TYPES)
    add_custom_command(TARGET projekt2_opencv POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${OpenCV_DIR}/bin"
        "$<CONFIG>"
        COMMENT "Copying OpenCV DLLs to Release folder"
    )
endif()