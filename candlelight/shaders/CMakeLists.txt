add_custom_command(
        OUTPUT default_shader.ps.hlsl
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/default_shader.ps.hlsl ${CMAKE_CURRENT_BINARY_DIR}/default_shader.ps.hlsl
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/default_shader.ps.hlsl
)

add_custom_command(
        OUTPUT default_shader.vs.hlsl
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/default_shader.vs.hlsl ${CMAKE_CURRENT_BINARY_DIR}/default_shader.vs.hlsl
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/default_shader.vs.hlsl
)

add_custom_target(
        candlelight_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Copying shaders..."
        DEPENDS default_shader.ps.hlsl default_shader.vs.hlsl
)

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/default_shader.ps.hlsl DESTINATION bin/shaders)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/default_shader.vs.hlsl DESTINATION bin/shaders)