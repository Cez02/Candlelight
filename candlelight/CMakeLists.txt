add_library(candlelight src/resource.cpp src/resource.h src/debug.cpp src/debug.h)

add_subdirectory(shaders)

add_dependencies(candlelight
        candlelight_shaders
)

target_include_directories(candlelight
    PUBLIC
        include
)

target_link_libraries(candlelight
    PUBLIC 
        d3d12.lib
        d3dcompiler.lib
        Microsoft::DirectX-Headers
        Microsoft::DirectXShaderCompiler
        glm::glm
)

add_subdirectory(src)

install(TARGETS candlelight DESTINATION lib)