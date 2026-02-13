target_sources(candlelight
    PRIVATE
        application.cpp
        command_queue.cpp
        renderer.cpp
        render_pipeline.cpp
        shader.cpp
        resource.cpp
        debug.cpp
)

target_include_directories(candlelight
    PRIVATE
        .
)