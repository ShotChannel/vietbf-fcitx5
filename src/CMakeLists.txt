add_library(vietbf SHARED vietbf.cpp)
target_link_libraries(vietbf PRIVATE Fcitx5::Core)
target_include_directories(vietbf PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(vietbf PROPERTIES PREFIX "")

configure_file(vietbf-addon.conf.in.in vietbf-addon.conf @ONLY)

install(TARGETS vietbf DESTINATION "${FCITX_INSTALL_LIBDIR}/fcitx5")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/vietbf-addon.conf"
        DESTINATION "${FCITX_INSTALL_PKGDATADIR}/addon")
install(FILES vietbf-im.conf RENAME vietbf.conf
        DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
