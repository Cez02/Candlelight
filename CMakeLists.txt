cmake_minimum_required(VERSION 3.24)

project(candlelight LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 20)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)

include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

find_package(directx-headers CONFIG REQUIRED)
find_package(directx-dxc CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(candlelight)
add_subdirectory(app)
