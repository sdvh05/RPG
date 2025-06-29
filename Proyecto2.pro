QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Aliados.cpp \
    Enemigos.cpp \
    arboldecision.cpp \
    arbolhabilidad.cpp \
    battlewidget.cpp \
    caballero.cpp \
    curandera.cpp \
    esqueletos.cpp \
    files.cpp \
    grafomapa.cpp \
    grafomapavisual.cpp \
    hashlogros.cpp \
    inventario.cpp \
    inventariowidget.cpp \
    main.cpp \
    mainwindow.cpp \
    ogro.cpp \
    personaje.cpp \
    princesa.cpp \
    skilltreeviewer.cpp \
    slime.cpp \
    gamewindow.cpp \
    tiendawidget.cpp \
    were.cpp

HEADERS += \
    Aliados.h \
    EfectoVisual.h \
    NodoSkill.h \
    Objeto.h \
    arboldecision.h \
    arbolhabilidad.h \
    battlewidget.h \
    caballero.h \
    curandera.h \
    esqueletos.h \
    files.h \
    grafomapa.h \
    grafomapavisual.h \
    hashlogros.h \
    inventario.h \
    inventariowidget.h \
    mainwindow.h \
    ogro.h \
    personaje.h \
    princesa.h \
    skilltreeviewer.h \
    slime.h \
    CustomGraphicsView.h \
    gamewindow.h \
    tiendawidget.h \
    were.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
