TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += $$BOOST_OBJECT_TOP_DIR

win32:msvc {
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

include($$BOOST_OBJECT_TOP_DIR/config.pri)


