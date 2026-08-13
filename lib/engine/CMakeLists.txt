add_library(engine STATIC
        src/window.cpp
        src/engine.cpp
        src/shader.cpp
        src/mesh.cpp
        src/object.cpp
        include/window.hpp
        include/engine.hpp
        include/shader.hpp
        include/mesh.hpp
        include/object.hpp
        include/transform.hpp
)
target_include_directories(engine PUBLIC  include)
target_link_libraries(engine PUBLIC glad glfw mathpp)