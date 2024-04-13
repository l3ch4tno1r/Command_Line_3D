set(THIS Scene)

add_library(${THIS}
    src/Scene.cpp)

target_link_libraries(${THIS}
    PUBLIC  entt
    PRIVATE Core
    PRIVATE LCNMathLib
    PRIVATE RessourceManagement
    PRIVATE Rendering
    PRIVATE Controller)

target_include_directories(
    ${THIS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)