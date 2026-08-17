add_library(imgui STATIC
        imgui.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp
        imgui_demo.cpp
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp


        imgui.h
        imconfig.h
        imgui_internal.h
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        backends/imgui_impl_glfw.h
        backends/imgui_impl_opengl3.h
        backends/imgui_impl_opengl3_loader.h

)

target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_CUSTOM)


target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} backends
)

target_link_libraries(imgui PUBLIC glad glfw)

target_compile_options(imgui PRIVATE
        -include ${CMAKE_CURRENT_SOURCE_DIR}/../glad/include/glad/gl.h
)