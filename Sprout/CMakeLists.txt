cmake_minimum_required(VERSION 3.15)
project(Sprout)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${PROJECT_NAME})

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "vendor/stb_image/*.cpp" "vendor/stb_image/*.h")

# Precompiled header
set(PCH_HEADER "src/spch.h")
set(PCH_SOURCE "src/spch.cpp")

# Library
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${PCH_HEADER} ${PCH_SOURCE})

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release
    ARCHIVE_OUTPUT_DIRECTORY_DIST ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Dist
)

# Compiler options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /std:c++17)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)  # Enable multi-processor compilation
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -std=c++17)
    target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
endif()

# Definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    SPDLOG_WCHAR_TO_UTF8_SUPPORT
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor)

# Links
target_link_libraries(${PROJECT_NAME} PRIVATE
    vendor
    opengl32.lib
    d3d11.lib
)

# Debug configuration
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/MDd>)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:SPROUT_DEBUG>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/Zi>)

# Release configuration
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/MD>)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:SPROUT_RELEASE>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/O2>)

# Dist configuration
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Dist>:/MD>)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Dist>:SPROUT_DIST>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Dist>:/O2>)

# System-specific settings
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        SPROUT_DX11
    )
endif()