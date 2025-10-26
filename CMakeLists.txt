cmake_minimum_required(VERSION 3.15)
project(ReflectionLibrary)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add MSVC-specific flags
if(MSVC)
    add_compile_options(/Zc:preprocessor /MP)
endif()

# Include FetchContent module for downloading dependencies
include(FetchContent)

# Fetch RapidJSON
FetchContent_Declare(
    rapidjson
    GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
    GIT_TAG master
)

# Disable RapidJSON tests, examples, and docs
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(rapidjson)

# Fetch Catch2 testing framework
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
)

FetchContent_MakeAvailable(Catch2)

# Collect all source files from tests directory
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${rapidjson_SOURCE_DIR}/include
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})