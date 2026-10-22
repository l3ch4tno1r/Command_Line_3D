set(THIS Scene)

add_library(${THIS}
    src/Scene.cpp)

target_link_libraries(${THIS}
    PUBLIC entt
    PUBLIC Core
    PUBLIC LCNMathLib
    PUBLIC RessourceManagement
    PUBLIC Rendering
    PUBLIC Controller)

target_include_directories(
    ${THIS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)