set(ENTT_LIB      entt)
set(STB_IMAGE_LIB stb_image)

############
#-- entt --#
############

add_library(${ENTT_LIB} INTERFACE)

target_include_directories(
    ${ENTT_LIB}
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/entt/include>)

#################
#-- stb_image --#
#################

add_library(${STB_IMAGE_LIB} stb_image/stb_image.cpp)

target_include_directories(
    ${STB_IMAGE_LIB}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/stb_image/include>)