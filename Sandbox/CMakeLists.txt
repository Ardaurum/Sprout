cmake_minimum_required(VERSION 3.15)
project(Sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${PROJECT_NAME})

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "vendor/glm/glm/*.hpp" "vendor/glm/glm/*.inl")

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release
    RUNTIME_OUTPUT_DIRECTORY_DIST ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Dist
)

# Included directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../Sprout/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../Sprout/vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/../Sprout/vendor/spdlog/include
    ${IncludeDir.glm}
)

# Links
target_link_libraries(${PROJECT_NAME} PRIVATE Sprout)

# Debug configuration
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:SPROUT_DEBUG>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/MDd>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/Zi>)

# Release configuration
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:SPROUT_RELEASE>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/MD>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/O2>)

# Dist configuration
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Dist>:SPROUT_DIST>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Dist>:/MD>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Dist>:/O2>)

# System-specific settings
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        SPROUT_DX11
    )
endif()