add_library(candlelight_rendering)

target_include_directories(candlelight_rendering
    PUBLIC
        .
)

target_sources(candlelight_rendering
    PUBLIC
        CommandQueue.cpp
        renderer.cpp
        RenderPipeline.cpp
        shader.cpp
        resource.cpp
        D3D12Debug.cpp
        SwapChainFactory.cpp

)