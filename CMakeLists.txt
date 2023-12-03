cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)

project(Sprout)

set(CMAKE_CXX_STANDARD 20)

# Set the architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x86_64")
else()
    message(FATAL_ERROR "Unsupported architecture")
endif()

# Configurations
set(CONFIGURATIONS
    Debug
    Release
    Dist
)

# Flags
set(FLAGS
    MultiProcessorCompile
)

# Output directory
set(OUTPUT_DIR "${CMAKE_CFG_INTDIR}-${CMAKE_SYSTEM_NAME}-${ARCHITECTURE}")

# Main projects
add_subdirectory(Sprout)
add_subdirectory(Sandbox)

# Set the start project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)