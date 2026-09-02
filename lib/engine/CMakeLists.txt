add_library(engine STATIC
        src/editor/core/editor.cpp
        src/core/engine/engine.cpp
        src/core/window/window.cpp
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
        src/editor/gizmo/gizmo.cpp
        src/io/obj/obj.cpp
        src/core/component/event/event.cpp
        src/editor/gizmo/gizmoController.cpp
        src/core/system/hierarchy/hierarchy.cpp
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
        include/component/sunlight.hpp
        include/editor/gizmo/gizmo.hpp
        include/io/obj/obj.hpp
        include/editor/core/editor.hpp
        include/core/component/event/event.hpp
        include/editor/gizmo/gizmoData.hpp
        include/editor/gizmo/gizmoController.hpp
        include/component/parent.hpp
        include/core/set/sparseset.hpp
        include/core/system/hierarchy/hierarchy.hpp
        include/component/children.hpp
        include/component/worldTransform.hpp
        include/core/system/transform/transform.hpp
        src/core/system/transform/transform.cpp


)

target_link_libraries(engine PUBLIC glad glfw mathpp imgui gill)
target_include_directories(engine PUBLIC include)