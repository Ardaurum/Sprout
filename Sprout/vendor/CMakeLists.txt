add_subdirectory(sdl EXCLUDE_FROM_ALL)
add_subdirectory(spdlog)
add_subdirectory(glm)

# Include stb_image
file(GLOB STB_IMAGE_SOURCES
    stb_image/stb_image.cpp
    stb_image/stb_image.h
)
add_library("stb_image" STATIC ${STB_IMAGE_SOURCES})
target_include_directories("stb_image" PUBLIC stb_image)

# Include imgui
file(GLOB IMGUI_SOURCES
    imgui/imgui.h
    imgui/imgui.cpp
    imgui/imconfig.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp

# SDL bindings
    imgui/backends/imgui_impl_sdl3.h
    imgui/backends/imgui_impl_sdl3.cpp
    imgui/backends/imgui_impl_sdlrenderer3.h
    imgui/backends/imgui_impl_sdlrenderer3.cpp
)
add_library("imgui" STATIC ${IMGUI_SOURCES})
target_include_directories("imgui" PUBLIC imgui)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE
    SDL3::SDL3
    spdlog
    glm
    imgui
    stb_image
)