project(gluk)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES} libgluk/)

add_subdirectory(src)
# add_subdirectory(libgluk)
