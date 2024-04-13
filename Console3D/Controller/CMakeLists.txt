set(THIS Controller)

add_library(${THIS} STATIC
    src/BaseController.cpp)

target_link_libraries(${THIS}
    PUBLIC  UtilitiesLib
    PRIVATE Event
    PRIVATE Scene
    PRIVATE Core)

target_include_directories(
    ${THIS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)