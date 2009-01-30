include_directories(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

set( gluk_SRCS
     mainwindow.cpp
     main.cpp
 )
kde4_add_ui_files(gluk_SRCS main.ui packagedock.ui installdock.ui)

kde4_add_executable(gluk ${gluk_SRCS})

target_link_libraries(gluk glukcore ${KDE4_KDEUI_LIBS})

install(TARGETS gluk DESTINATION ${BIN_INSTALL_DIR})
install(FILES glukui.rc 
        DESTINATION ${DATA_INSTALL_DIR}/gluk)
