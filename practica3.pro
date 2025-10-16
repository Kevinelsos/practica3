TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        admin.cpp \
        codificar.cpp \
        decodificar.cpp \
        main.cpp \
        usuarios.cpp

HEADERS += \
    admin.h \
    codificar.h \
    decodificar.h \
    usuario.h \
    usuarios.h
