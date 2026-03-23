add_library(candlelight_rendering)

target_link_libraries(candlelight_rendering
    PUBLIC
        candlelight_core
        d3d12.lib
        d3dcompiler.lib
        dxgi.lib
        dxguid.lib
        Microsoft::DirectX-Headers
        Microsoft::DirectXShaderCompiler
        glm::glm
)

get_property(candlelight_INCLUDE_DIR GLOBAL PROPERTY CANDLELIGHT_API_INCLUDE_DIR)

target_include_directories(candlelight_rendering
    PUBLIC
        .
        ${candlelight_INCLUDE_DIR}
)

target_sources(candlelight_rendering
    PRIVATE
        CommandQueue.cpp
        D3D12Debug.cpp
        RenderPipeline.cpp
        Renderer.cpp
        Resource.cpp
        D3D12Debug.cpp
        Shader.cpp
        SwapChainFactory.cpp
)