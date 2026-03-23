add_library(candlelight)

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