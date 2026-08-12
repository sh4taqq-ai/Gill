add_library(mathlib STATIC
        source/vec2.c
        source/vec3.c
        source/vec4.c
        source/mat3.c
        source/mat4.c

)

target_include_directories(mathlib PUBLIC include)


set_target_properties(mathlib PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
)
