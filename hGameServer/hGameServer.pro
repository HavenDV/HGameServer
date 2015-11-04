TARGET = server
QT += core network gui
QMAKE_CXXFLAGS += -std=c++1y
HEADERS += ./hserver.h \
    ./hconnection.h \
    ./hgameserver.h \
    ./hworld.h \
    ./hobject.h \
    ./hthread.h \
    ./hthreadparent.h
SOURCES += ./hconnection.cpp \
    ./hgameserver.cpp \
    ./hobject.cpp \
    ./hserver.cpp \
    ./hthread.cpp \
    ./hthreadparent.cpp \
    ./hworld.cpp \
    ./main.cpp
