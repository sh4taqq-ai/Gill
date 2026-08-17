cmake_minimum_required(VERSION 3.20)
project(GILL C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
# GLFW ships its own examples/tests/docs we don't need — skip building them
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

add_subdirectory(extern/glad)
add_subdirectory(extern/imgui)
add_subdirectory(lib/mathpp)
add_subdirectory(lib/engine)




add_executable(gill source/gill/main.cpp)



file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/asset"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)



target_link_libraries(gill PRIVATE glad engine)

