set(THIS Core)

add_library(${THIS} STATIC
    src/Application.cpp
    src/Console.cpp
    src/ConsoleInput.cpp
    src/Device.cpp
    src/EventApplication.cpp
    src/GraphicEngine.cpp
    src/PaceMaker.cpp
    src/RTApplication.cpp)

target_link_libraries(${THIS} PRIVATE
    UtilitiesLib
    Event)

target_include_directories(
    ${THIS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)