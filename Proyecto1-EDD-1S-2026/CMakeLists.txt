cmake_minimum_required(VERSION 3.19)
project(Proyecto1-EDD-1S-2026 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Proyecto1-EDD-1S-2026
    WIN32 MACOSX_BUNDLE
    main.cpp
    view/pantallas/mainwindow.cpp
    view/pantallas/mainwindow.h
    view/pantallas/mainwindow.ui

    view/pantallas/pantallasistema.h
    view/pantallas/pantallasistema.cpp
    view/pantallas/pantallasistema.ui
    view/widgets/customgraphics.h
    view/widgets/customgraphics.cpp
    model/estructuras/listaenlazadanoordenada.h model/estructuras/listaenlazadanoordenada.cpp
    model/estructuras/listaenlazadaordenada.h model/estructuras/listaenlazadaordenada.cpp
    model/estructuras/arbolb.h model/estructuras/arbolb.cpp
    model/estructuras/arbolbplus.h model/estructuras/arbolbplus.cpp
    model/entidades/nodo.h model/entidades/nodo.cpp
    model/entidades/product.h model/entidades/product.cpp
    controller/viewcontroller.h controller/viewcontroller.cpp
    controller/filecontroller.h controller/filecontroller.cpp
    controller/estructurascontroller.h controller/estructurascontroller.cpp
    controller/appcontroller.h controller/appcontroller.cpp
    model/estructuras/arbolavl.h model/estructuras/arbolavl.cpp
     view/pantallas/pantallaagregarproducto.h  view/pantallas/pantallaagregarproducto.cpp  view/pantallas/pantallaagregarproducto.ui
     view/pantallas/pantallamostrarcsv.h view/pantallas/pantallamostrarcsv.cpp view/pantallas/pantallamostrarcsv.ui
     view/pantallas/pantallaeliminar.h view/pantallas/pantallaeliminar.cpp view/pantallas/pantallaeliminar.ui
     view/pantallas/pantallabuscarpornombre.h view/pantallas/pantallabuscarpornombre.cpp view/pantallas/pantallabuscarpornombre.ui
     view/pantallas/pantallabuscarporcategoria.h view/pantallas/pantallabuscarporcategoria.cpp view/pantallas/pantallabuscarporcategoria.ui
     view/pantallas/pantallabuscarporrangocaducidad.h view/pantallas/pantallabuscarporrangocaducidad.cpp view/pantallas/pantallabuscarporrangocaducidad.ui
     view/pantallas/pantallalistarpornombre.h view/pantallas/pantallalistarpornombre.cpp view/pantallas/pantallalistarpornombre.ui


)

target_include_directories(Proyecto1-EDD-1S-2026
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/model
        ${CMAKE_CURRENT_SOURCE_DIR}/controller
        ${CMAKE_CURRENT_SOURCE_DIR}/view
)

target_link_libraries(Proyecto1-EDD-1S-2026
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS Proyecto1-EDD-1S-2026
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Proyecto1-EDD-1S-2026
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
