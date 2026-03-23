target_link_libraries(candlelight
    PUBLIC
        candlelight_core
        candlelight_rendering
)

target_include_directories(candlelight
    PUBLIC
        .
)

target_sources(candlelight
    PUBLIC
        Application.cpp
)