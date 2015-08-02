QT       -= core gui

TARGET = object
TEMPLATE = lib

CONFIG += staticlib
DEFINES += BOOST_OBJECT_LIBRARY BOOST_OBJECT_STATIC_LINK

HEADERS += \
    ../object.hpp \
    detail/config.hpp \
    detail/func_traits.hpp \
    object.hpp \
    detail/object_macros.hpp \
    impl/object.ipp \
    application.hpp \
    impl/application.ipp \
    engine.hpp \
    thread.hpp \
    impl/engine.ipp \
    impl/thread.ipp \
    detail/slot.hpp \
    detail/bind_helper.hpp \
    detail/object_data.hpp \
    detail/object_install.hpp \
    detail/signal.hpp \
    detail/connectable_type.hpp \
    detail/connection_policy.hpp \
    connection.hpp \
    detail/connection_interface.hpp \
    detail/connection_impl.hpp \
    utils/timer.hpp \
    utils/impl/timer.ipp \
    detail/defs/recursive_mutex.hpp \
    detail/defs/thread.hpp \
    detail/defs/mutex.hpp \
    network/tcpserver.hpp \
    network/tcpsocket.hpp \
    network/impl/tcpserver.ipp \
    network/impl/tcpsocket.ipp \
    detail/asio_wrap.hpp \
    utils/bytearray.hpp \
    utils/impl/bytearray.ipp \
    utils/systemsignals.hpp \
    utils/impl/systemsignals.ipp \
    impl/object_p.hpp \
    impl/thread_p.hpp \
    impl/application_p.hpp \
    utils/impl/timer_p.hpp \
    detail/object_access.hpp \
    network/impl/tcpserver_p.hpp \
    network/impl/tcpsocket_p.hpp \
    network/detail/network.hpp \
    detail/defs/global.hpp \
    utils/impl/systemsignals_p.hpp \
    threadpool.hpp \
    impl/threadpool.ipp \
    impl/threadpool_p.hpp \
    impl/engine_p.hpp \
    base.hpp \
    impl/base_p.hpp \
    impl/base.ipp \
    detail/safe_object.hpp \
    detail/object_interface.hpp \
    detail/object_internals.hpp \
    impl/connection.ipp \
    detail/object_primitive.hpp \
    detail/default_log_handler.hpp \
    detail/sender_scope.hpp \
    detail/slot_context.hpp \
    signalcontext.hpp \
    network/tcpclient.hpp \
    network/impl/tcpclient.ipp \
    network/impl/tcpclient_p.hpp \
    detail/log.hpp \
    network/detail/socket_imbue.hpp \
    network/impl/tcpconnection.hpp \
    network/impl/tcpconnection_p.hpp \
    signal_macros.hpp \
    slot_macros.hpp \
    detail/slot_type.hpp \
    detail/callable.hpp \
    detail/signal_type.hpp

SOURCES += \
    ../../libs/object/src/object.cpp \
    ../../libs/object/src/application.cpp \
    ../../libs/object/src/engine.cpp \
    ../../libs/object/src/thread.cpp \
    ../../libs/object/src/base.cpp \
    ../../libs/object/src/connection.cpp \
    ../../libs/object/src/threadpool.cpp \
    ../../libs/object/src/utils/timer.cpp \
    ../../libs/object/src/utils/bytearray.cpp \
    ../../libs/object/src/utils/systemsignals.cpp \
    ../../libs/object/src/network/tcpserver.cpp \
    ../../libs/object/src/network/tcpclient.cpp \
    ../../libs/object/src/network/tcpsocket.cpp


INCLUDEPATH += $$BOOST_OBJECT_TOP_DIR

include($$BOOST_OBJECT_TOP_DIR/config.pri) 
