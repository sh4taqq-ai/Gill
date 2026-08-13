add_library(engine STATIC
        src/window.cpp
        src/engine.cpp
        src/shader.cpp
        include/window.hpp
        include/engine.hpp
        include/shader.hpp
)
target_include_directories(engine PUBLIC  include)
target_link_libraries(engine PUBLIC glad glfw mathpp)