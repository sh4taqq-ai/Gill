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
)

target_link_libraries(engine PUBLIC glad glfw mathpp)
target_include_directories(engine PUBLIC include)