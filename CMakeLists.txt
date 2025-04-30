cmake_minimum_required(VERSION 3.20)
project(PlatformEscape)
# Project Version 
set(PROJECT_VERSION "0.0.1")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Setting up Flags
set(CMAKE_EXPORT_COMPLE_COMMANDS ON)

# Setting compile definitions
add_compile_definitions(VERSION="${PROJECT_VERSION}")
add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

# Add raylib
find_package(raylib CONFIG REQUIRED)

# Adding ImGui
find_package(imgui CONFIG REQUIRED)

# Adding rlImGui
set(RLIMGUI_DIR "${CMAKE_SOURCE_DIR}/external/rlimgui")
add_library(rlImGui STATIC
  ${RLIMGUI_DIR}/rlImGui.cpp
)
target_include_directories(rlImGui PUBLIC ${RLIMGUI_DIR})
target_link_libraries(rlImGui PUBLIC raylib imgui::imgui)

# File organization (for visual studio)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.c) 
file(GLOB_RECURSE HEADER_FILES include/*.hpp include/*.h)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})

# Building target 
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib imgui::imgui rlImGui)

# Check if Generator is Visual studio
if (CMAKE_GENERATOR MATCHES "Visual Studio")
  message(STATUS "Setting up for Visual Studio...")
  # Set the working Directory for each CONFIG
  set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
elseif(CMAKE_GENERATOR MATCHES "Ninja")
  if(WIN32)
    # Copy compile_commands.json
    add_custom_target(
      copy_compile_commands ALL
      COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
      COMMENT "Copying compile_commands.json to source directory (windows)"
      VERBATIM
    ) 
  else()
    # symlink compile_commands.json
    add_custom_target(
      copy_compile_commands ALL
      COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
      COMMENT "Symlinking compile_commands.json to source directory (Unix)"
      VERBATIM
    )
  endif()
endif()

# set the source directory of resources
set(RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res)

# Copy the contents of the res folder to the build directory if there is one
if(EXISTS ${RES_DIR})
  message(STATUS "Resource folder fond: ${RES_DIR}")
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RES_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying resources to the build directory..."
  )
else()
  # Skipping copying resources
  message(STATUS "Resource folder not found: ${RES_DIR}. Skipping sesource copy.")
endif()


