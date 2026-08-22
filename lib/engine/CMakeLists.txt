add_library(engine STATIC
src/core/engine/engine.cpp
        src/core/window/window.cpp
        src/component/transform.cpp
        src/render/mesh/mesh.cpp
        src/render/shader/shader.cpp
        src/scene/scene.cpp
        src/component/entity.cpp
        src/render/core/renderer.cpp
        src/render/mesh/primitives.cpp
        src/core/component/raycast/raycast.cpp
        src/editor/selector/selector.cpp
        src/editor/ui/ui.cpp
        src/core/input/input.cpp
        src/core/component/camera/camera.cpp
        src/editor/grid/grid.cpp
        include/core/engine/engine.hpp
        include/core/window/window.hpp
        include/component/transform.hpp
        include/render/mesh/mesh.hpp
        include/render/shader/shader.hpp
        include/render/data/vertex.hpp
        include/component/mesh.hpp
        include/component/entity.hpp
        include/scene/scene.hpp
        include/render/core/renderer.hpp
        include/render/mesh/primitive.hpp
        include/core/component/raycast/raycast.hpp
        include/editor/selector/selector.hpp
        include/editor/ui/ui.hpp
        include/core/input/input.hpp
        include/core/component/camera/camera.hpp
        include/editor/grid/grid.hpp
        include/core/system/asset/asset.hpp
        src/core/system/asset/asset.cpp
        include/component/sunlight.hpp
        include/editor/gizmo/gizmo.hpp
        src/editor/gizmo/gizmo.cpp

)

target_link_libraries(engine PUBLIC glad glfw mathpp imgui)
target_include_directories(engine PUBLIC include)