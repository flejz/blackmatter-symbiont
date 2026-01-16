cmake_minimum_required(VERSION 3.16)
project(blackmatter-symbiont LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)
pkg_check_modules(GLEW REQUIRED glew)
pkg_check_modules(ASSIMP REQUIRED assimp)

# GLM is header-only, find it
find_package(glm REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# Source files
set(SOURCES
    main.cpp
    include/stb_image.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${GLFW3_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GLFW3_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    OpenGL::GL
    glm::glm
)

# Add library directories (needed for pkg-config libraries)
target_link_directories(${PROJECT_NAME} PRIVATE
    ${GLFW3_LIBRARY_DIRS}
    ${GLEW_LIBRARY_DIRS}
    ${ASSIMP_LIBRARY_DIRS}
)

# Compiler flags from pkg-config
target_compile_options(${PROJECT_NAME} PRIVATE
    ${GLFW3_CFLAGS_OTHER}
    ${GLEW_CFLAGS_OTHER}
    ${ASSIMP_CFLAGS_OTHER}
)
