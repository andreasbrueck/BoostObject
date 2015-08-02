TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$BOOST_OBJECT_TOP_DIR

SOURCES += dummy.cpp

OTHER_FILES += \
    Jamfile.v2 \
    tests.h \
    generic.cpp \
    network/tcp.cpp \
    bbytearray.cpp \
    connection.cpp \
    detail/callable.cpp

include($$BOOST_OBJECT_TOP_DIR/config.pri)

win32:msvc {
    BJAM_TOOLSET = msvc
} else:linux-g++ {
    BJAM_TOOLSET = gcc
    BJAM_CXXFLAGS = cxxflags=-std=c++11
} else {
    error("Set the toolset for bjam")
}

equals(QT_ARCH,"x86_64") {
    BJAM_ARCH = address-model=64 architecture=x86
} else:equals(QT_ARCH,"i386") {
    BJAM_ARCH = address-model=32 architecture=x86
} else {
    error("Update this to set your architecture for bjam")
}

win32 {
    BJAM_COMMAND = $$(BOOST_ROOT)/b2
} else {
    BJAM_COMMAND = cd $$(BOOST_ROOT) && ./b2
}

CONFIG(release,debug|release){
    BJAM_VARIANT = release
} else {
    BJAM_VARIANT = debug
}

JAMFILE_COMPILE.commands = $$BJAM_COMMAND \
                                --build-dir=$$OUT_PWD/boost/ \
                                $$BJAM_CXXFLAGS \
                                threading=multi \
                                variant=$$BJAM_VARIANT \
                                toolset=$$BJAM_TOOLSET \
                                $$BJAM_ARCH \
                                include=$$BOOST_OBJECT_TOP_DIR \
                                $$_PRO_FILE_PWD_/

QMAKE_EXTRA_TARGETS += JAMFILE_COMPILE
PRE_TARGETDEPS += JAMFILE_COMPILE
