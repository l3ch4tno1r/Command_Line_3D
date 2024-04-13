set(THIS RessourceManagement)

add_library(${THIS}
    src/Model3D.cpp)

target_link_libraries(${THIS} PRIVATE
    LCNMathLib)

target_include_directories(
    ${THIS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)