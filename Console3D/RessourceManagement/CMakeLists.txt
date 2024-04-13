set(THIS RessourceManagement)

add_library(${THIS}
    src/Model3D.cpp
    src/OBJReader.cpp
    src/Texture.cpp)

target_link_libraries(${THIS} PRIVATE
    LCNMathLib
    UtilitiesLib)

target_link_libraries(${THIS} PUBLIC
    stb_image)

target_include_directories(
    ${THIS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)