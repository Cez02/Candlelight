add_library(candlelight_core)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(candlelight_core
    PUBLIC
        spdlog::spdlog_header_only
)

target_include_directories(candlelight_core
    PUBLIC
        .
)

add_subdirectory(architecture)
add_subdirectory(debug)
add_subdirectory(dependency_handling)
add_subdirectory(logger)
