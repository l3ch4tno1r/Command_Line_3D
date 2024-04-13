set(THIS Rendering)

add_library(${THIS}
    src/Camera.cpp
    src/ConsoleRenderer.cpp)

target_link_libraries(${THIS}
    PRIVATE LCNMathLib
    PUBLIC  Event
    PRIVATE Scene
    PRIVATE RessourceManagement
    PRIVATE Core
    PRIVATE Controller)

target_include_directories(
    ${THIS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)