cmake_minimum_required(VERSION 3.13)  # CMake version check
project(bitset_example)              # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard

include(FetchContent)

FetchContent_Declare(
  etl
  GIT_REPOSITORY https://github.com/ETLCPP/etl.git
  GIT_TAG        20.39.4  # 20.39.4
)

FetchContent_MakeAvailable(etl)

# Add example.cpp file of the project root directory as a source file
set(SOURCE_FILES example.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} PRIVATE etl::etl pthread)