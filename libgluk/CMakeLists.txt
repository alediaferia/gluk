project (libgluk) 

set (glukcore_SRCS
    gluksortfiltermodel.cpp
    gluktreemodel.cpp
    treeitem.cpp
    glukjobs.cpp
)

kde4_add_library(glukcore SHARED ${glukcore_SRCS})

target_link_libraries(glukcore ${KDE4_KDEUI_LIBS} ${KDE4_THREADWEAVER_LIBRARIES})

install(TARGETS glukcore ${INSTALL_TARGETS_DEFAULT_ARGS})

set (glukcore_HDRS
    gluksortfiltermodel.h
    gluktreemodel.h
    treeitem.h
)

install(FILES ${glukcore_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/gluk)

