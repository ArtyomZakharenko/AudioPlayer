QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    player.cpp

HEADERS += \
    player.h

FORMS += \
    player.ui

RESOURCES += \
    buttons.qrc
