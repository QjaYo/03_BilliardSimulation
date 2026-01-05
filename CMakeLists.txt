cmake_minimum_required(VERSION 3.12)
project(PinballSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add path depending on OS
if (APPLE)
  list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
  set(CMAKE_MACOSX_RPATH 1)
endif()

# Find required packages
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(Eigen3 REQUIRED)

# Add executables
add_executable(${PROJECT_NAME}
  sources/main.cpp
  sources/shader.cpp
  sources/callback.cpp
  sources/mesh.cpp
  sources/shapeGenerator.cpp
  sources/texture.cpp
  sources/material.cpp
  sources/object.cpp
  sources/light.cpp

  externals/glad/src/glad.c
)

# Find headers & include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
  "${CMAKE_CURRENT_SOURCE_DIR}/includes/"
  "${CMAKE_CURRENT_SOURCE_DIR}/externals/glad/include/"
  "${CMAKE_CURRENT_SOURCE_DIR}/externals/"
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
  OpenGL::GL
  glfw
  glm::glm
  Eigen3::Eigen
)

# Link frameworks depending on OS
if (APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE 
    "-framework Cocoa" 
    "-framework IOKit" 
    "-framework CoreVideo"
  )
endif()

# Define paths
target_compile_definitions(${PROJECT_NAME} PRIVATE 
  SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
  TEXTURE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/textures/"
)

# definitions
add_definitions(-DGL_SILENCE_DEPRECATION)