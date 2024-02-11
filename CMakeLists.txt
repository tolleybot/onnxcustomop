cmake_minimum_required(VERSION 3.1)

project(customop)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DONNX_ML)
add_definitions(-DONNX_NAMESPACE=onnx)

# Point CMake to the directory containing onnxruntimeConfig.cmake
set(onnxruntime_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Find ONNXRuntime using the custom config file
find_package(onnxruntime REQUIRED)


# Assuming you're using vcpkg, you should pass -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake to your cmake command.
# This makes the next line unnecessary as vcpkg toolchain file handles it:
# list(APPEND CMAKE_PREFIX_PATH  ${PROJECT_AW} "cmake" "/usr/local/")

# Find Microsoft GSL (Guideline Support Library) using vcpkg
find_package(Microsoft.GSL CONFIG REQUIRED)

# Add executable
add_library(${PROJECT_NAME} SHARED addnumbop.cpp) 

# Link ONNX Runtime and Microsoft GSL
target_link_libraries(${PROJECT_NAME} PRIVATE onnxruntime Microsoft.GSL::GSL)

# If you want to echo out include directories or library paths, uncomment the following lines
# message(STATUS "Include directories: ${onnxruntime_INCLUDE_DIRS}")
# get_target_property(ONNXRUNTIME_LIBRARY onnxruntime LOCATION)
# message(STATUS "ONNX Runtime library: ${ONNXRUNTIME_LIBRARY}")
