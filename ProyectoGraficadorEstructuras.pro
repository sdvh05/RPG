QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AreaDibujo.cpp \
    DoubleLL.cpp \
    GraficadorEstruDat.cpp \
    LinkedList.cpp \
    Queue.cpp \
    Stack.cpp \
    avl.cpp \
    bst.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AreaDibujo.h \
    DoubleLL.h \
    GraficadorEstruDat.h \
    LinkedList.h \
    Nodo.h \
    NodoArbol.h \
    NodoDLL.h \
    Queue.h \
    Stack.h \
    avl.h \
    bst.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS +=
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
