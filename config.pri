## BOOST_DIRECTORY is an environment variable,
## having the path to the directory in which your "boost(/*)" folder lives in.
##
## You can change it to a hardcoded path too.

BOOST_DIRECTORY=$$(BOOST_ROOT)
#BOOST_DIRECTORY=OR.YOUR.OWN.HARDCODED.PATH

## BOOST_LIB_DIRECTORY is the path that contains the *.lib files, relative to
## BOOST_DIRECTORY. If empty, defaults to $$BOOST_DIRECTORY/$$ARCH/lib
BOOST_LIB_DIRECTORY=

####################################################################################################
####################################################################################################

CONFIG += no_keywords c++11
DEFINES += BOOST_ALL_STATIC_LINK

!isEmpty(BOOST_DIRECTORY){
    INCLUDEPATH += $$BOOST_DIRECTORY
    isEmpty(BOOST_LIB_DIRECTORY){
        LIBS += -L$$BOOST_DIRECTORY/$$QMAKE_TARGET.arch/lib
    }else{
        LIBS += -L$$BOOST_DIRECTORY/$$BOOST_LIB_DIRECTORY
    }
}

################################################################################

CONFIG(release,debug|release){
    DEFINES += NDEBUG
}

################################################################################

win32 {
    DEFINES += _WIN32_WINNT=0x0501
}

unix {
    LIBS += -lboost_system

    !contains(DEFINES,BOOST_OBJECT_USE_STD) {
        LIBS += -lpthread -lboost_thread
    }
}
