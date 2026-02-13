add_executable(candlelight_app WIN32)

target_sources(candlelight_app
    PRIVATE
        main.cpp
)

find_package(glm REQUIRED)

target_link_libraries(candlelight_app
    PRIVATE
        candlelight
        glm::glm
)

install(TARGETS candlelight_app DESTINATION bin)