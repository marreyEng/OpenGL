cmake_minimum_required(VERSION 4.2)

project(qwe)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(PROJECT_IS_TOP_LEVEL)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink 
        "${CMAKE_BINARY_DIR}/compile_commands.json" 
        "${CMAKE_SOURCE_DIR}/compile_commands.json"
    )
endif()

find_package(glfw QUIET)
find_package(OpenGL REQUIRED)

if(glfw_FOUND)
    message(STATUS "Found system glfw library.")
else()
    message(STATUS "glfw library not found. Fetching from GitHub...")
    
    include(FetchContent)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        master
    )

    set(GLFW_BUILD_DOCS     OFF CACHE INTERNAL "")
    set(GLFW_BUILD_TESTS    OFF CACHE INTERNAL "")
    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
    set(GLFW_BUILD_WAYLAND  OFF CACHE BOOL     "" FORCE)
    FetchContent_MakeAvailable(glfw)
endif()

add_executable(exec)

target_sources(exec
PRIVATE
    src/main.cpp
    src/glad.c
PRIVATE
    FILE_SET headers
    TYPE HEADERS
    BASE_DIRS include
    FILES
        include/header.h
        include/glad/glad.h
)

target_link_libraries(exec
PRIVATE
    pthread
    glfw
    OpenGL::GL
)