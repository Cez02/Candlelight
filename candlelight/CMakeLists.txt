add_library(candlelight)

add_subdirectory(shaders)
add_subdirectory(include)

add_dependencies(candlelight
    candlelight_shaders
)

target_include_directories(candlelight
    PUBLIC
        include
)

add_subdirectory(src)

install(TARGETS candlelight DESTINATION lib)