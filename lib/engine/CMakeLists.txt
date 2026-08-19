add_library(engine STATIC
src/core/engine/engine.cpp
        src/core/window/window.cpp
        src/component/transform.cpp
        src/render/mesh/mesh.cpp
        src/render/shader/shader.cpp
        src/scene/scene.cpp
        include/core/engine/engine.hpp
        include/core/window/window.hpp
        include/component/transform.hpp
        include/render/mesh/mesh.hpp
        include/render/shader/shader.hpp
        include/render/data/vertex.hpp
        include/component/mesh.hpp
        include/component/entity.hpp
        include/scene/scene.hpp
        src/component/entity.cpp
        include/render/core/renderer.hpp
        src/render/core/renderer.cpp
        include/render/mesh/primitive.hpp
        src/render/mesh/primitives.cpp
        include/core/component/raycast/raycast.hpp
        src/core/component/raycast/raycast.cpp
        include/core/system/selector/selector.hpp
        src/core/system/selector/selector.cpp
        include/ui/ui.hpp
        src/ui/ui.cpp
        include/core/input/input.hpp
        src/core/input/input.cpp
        include/core/component/camera/camera.hpp
        src/core/component/camera/camera.cpp
)

target_link_libraries(engine PUBLIC glad glfw mathpp imgui)
target_include_directories(engine PUBLIC include)