set(THIS Rendering)

add_library(${THIS}
    src/Camera.cpp
    src/ConsoleRenderer.cpp
    src/Renderer2D.cpp)

target_link_libraries(${THIS}
    PUBLIC LCNMathLib
    PUBLIC LCNCollisionLib
    PUBLIC Event
    PUBLIC Scene
    PUBLIC RessourceManagement
    PUBLIC Core
    PUBLIC Controller
    PUBLIC UtilitiesLib)

target_include_directories(
    ${THIS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)