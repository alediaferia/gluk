project (libkportage) 

set (kportagecore_SRCS

)

kde4_add_library(kportagecore SHARED ${kportagecore_SRCS})

target_link_libraries(kportagecore ${KDE4_KDEUI_LIBS})

install(TARGETS kportagecore ${INSTALL_TARGETS_DEFAULT_ARGS})

set (kportagecore_HDRS
)

install(FILES ${_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/kportage)

