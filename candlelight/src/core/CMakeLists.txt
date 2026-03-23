add_library(candlelight_core)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(candlelight_core
    PUBLIC
        spdlog::spdlog_header_only
)

get_property(candlelight_INCLUDE_DIR GLOBAL PROPERTY CANDLELIGHT_API_INCLUDE_DIR)

target_include_directories(candlelight_core
    PUBLIC
        .
        ${candlelight_INCLUDE_DIR}
)

add_subdirectory(algorithm)
add_subdirectory(architecture)
add_subdirectory(debug)
add_subdirectory(dependency_handling)
add_subdirectory(logger)
