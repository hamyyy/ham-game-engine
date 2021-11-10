cmake_minimum_required(VERSION 3.4)
project(magnum-gui-template)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

file(GLOB SOURCE "./src/*.cpp" "./src/*/*.cpp" "./src/*.c" "./src/*/*.c")
file(GLOB INCLUDE "./src/*.hpp" "./src/*/*.hpp" "./src/*.h" "./src/*/*.h")

file(GLOB IMGUIZMO_SOURCE "./lib/ImGuizmo/*.cpp" "./lib/ImGuizmo/*.h")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(${PROJECT_NAME} WIN32 ${SOURCE} ${INCLUDE} ${IMGUIZMO_SOURCE})

target_precompile_headers(${PROJECT_NAME} PRIVATE "include/PCH.hpp")
target_include_directories(${PROJECT_NAME} PRIVATE "./include")

include(CTest)
enable_testing()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/assets
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

## vcpkg linked libraries

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

find_package(Catch2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2)

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)

# find_package(imguizmo CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE imguizmo::imguizmo)

target_include_directories(${PROJECT_NAME} PRIVATE "./lib/ImGuizmo")

## add packages

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui)

## set Magnum variables

set(WITH_IMGUI ON CACHE BOOL "" FORCE)
set(WITH_GLFWAPPLICATION ON CACHE BOOL "" FORCE)

## add Magnum

add_subdirectory(lib/corrade EXCLUDE_FROM_ALL)
add_subdirectory(lib/magnum EXCLUDE_FROM_ALL)
add_subdirectory(lib/magnum-integration EXCLUDE_FROM_ALL)

## set Magnum plugins

find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED 
    GL
    MeshTools
    Primitives
    Shaders
    GlfwApplication
    ShaderTools
    Trade
    # Vk
    SceneGraph
)
find_package(MagnumIntegration REQUIRED 
    ImGui
)

## link Magnum libraries

target_link_libraries(${PROJECT_NAME} PRIVATE
    Corrade::Main
    
    Magnum::Application
    Magnum::GlfwApplication
    Magnum::GL
    # Magnum::Vk
    Magnum::Trade
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders
    Magnum::ShaderTools
    Magnum::SceneGraph
    
    MagnumIntegration::ImGui
)