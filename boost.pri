# Set this variable to where you unpacked boost.
# If this variable is empty, qmake will attempt to retrieve it from the environment variable BOOST_DIRECTORY.
# If still empty, compilers default paths are searched.
BOOST_DIRECTORY=

# Boost /libs directory !!relative!! to BOOST_DIRECTORY.
# Doesn't need to be changed if BOOST_DIRECTORY is the location of your unpacked boost.
BOOST_SRC_DIRECTORY=/libs

####################################################################################################################
# Only change below if you know what you're doing
####################################################################################################################

isEmpty(BOOST_DIRECTORY){
    BOOST_DIRECTORY=$$(BOOST_DIRECTORY)
}

INCLUDEPATH += $$BOOST_DIRECTORY




#contains(DEFINES,BOOST_ALL_NO_LIB){
#
#    equals(BOOST_OBJECT_NEED_COROUTINE, "1"){
#        BOOST_OBJECT_NEED_CONTEXT=1
#
#        SOURCES += \
#            $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/coroutine/src/exceptions.cpp \
#            $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/coroutine/src/detail/coroutine_context.cpp
#
#
#
#        win32 {
#
#            SOURCES += \
#                $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/coroutine/src/windows/stack_traits.cpp
#
#        } else {
#
#            SOURCES += \
#                $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/coroutine/src/posix/stack_traits.cpp
#
#        }
#    }
#
#    equals(BOOST_OBJECT_NEED_CONTEXT, "1"){
#
#        windows {
#
#            *-msvc* {
#
#                SOURCES += \
#                    $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/context/src/asm/make_x86_64_ms_pe_masm.asm \
#                    $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/context/src/asm/jump_x86_64_ms_pe_masm.asm
#
#            # } *-g++* {
#
#            } else {
#
#                error(Your compiler is not implemented in boost.pri for boost::context)
#            }
#        }
#
#    }
#
#    equals(BOOST_OBJECT_NEED_THREAD, "1"){
#
#        BOOST_OBJECT_NEED_SYSTEM=1
#
#        SOURCES += \
#            $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/thread/src/future.cpp \
#            $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/thread/src/tss_null.cpp
#
#        win32 {
#
#            SOURCES += \
#                $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/thread/src/win32/thread.cpp
#        } else {
#
#            SOURCES += \
#                $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/thread/src/posix/once.cpp \
#                $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/thread/src/posix/once_atomic.cpp \
#                $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/thread/src/posix/thread.cpp
#
#        }
#    }
#
#    equals(BOOST_OBJECT_NEED_SYSTEM, "1"){
#
#        SOURCES += \
#            $$BOOST_DIRECTORY$$BOOST_SRC_DIRECTORY/system/src/error_code.cpp
#    }
#
#}
