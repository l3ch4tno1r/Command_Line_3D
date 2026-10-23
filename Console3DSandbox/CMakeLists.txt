add_executable(Console3DSandbox
    Source/Main.cpp
    Source/Misc/AppTestDisplayImage.cpp)

target_link_libraries(Console3DSandbox
PRIVATE
    Core
    Scene
    Controller
    Rendering
    LCNMathLib
    UtilitiesLib)