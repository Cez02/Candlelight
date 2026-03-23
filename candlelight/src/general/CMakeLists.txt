target_link_libraries(candlelight
    PUBLIC
        candlelight_core
        candlelight_rendering
)

get_property(candlelight_INCLUDE_DIR GLOBAL PROPERTY CANDLELIGHT_API_INCLUDE_DIR)

target_include_directories(candlelight
    PUBLIC
        .
        ${candlelight_INCLUDE_DIR}
)

target_sources(candlelight
    PRIVATE
        Application.cpp
)