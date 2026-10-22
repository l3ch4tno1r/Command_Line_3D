add_executable(Console3DSandbox
    Source/Main.cpp
    Source/Misc/RTAppTest3D.cpp)

target_link_libraries(Console3DSandbox
PRIVATE
    Core
    Scene
    Controller
    Rendering)