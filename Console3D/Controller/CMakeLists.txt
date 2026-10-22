set(THIS Controller)

add_library(${THIS} STATIC
    src/BaseController.cpp
    src/Camera2DController.cpp
    src/FPSCameraController.cpp)

target_link_libraries(${THIS}
    PUBLIC UtilitiesLib
    PUBLIC Event
    PUBLIC Scene
    PUBLIC Core)

target_include_directories(
    ${THIS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)