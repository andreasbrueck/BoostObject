TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += $$BOOST_OBJECT_TOP_DIR

include($$BOOST_OBJECT_TOP_DIR/config.pri)
