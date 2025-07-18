TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++14

SOURCES += main.cpp \
    concave_hull.cpp

include(deployment.pri)
qtcAddDeployment()

    LIBS += c:\\opencv_3_1_0\\x86\\mingw\\lib\\libopencv_world310.dll.a
    INCLUDEPATH +=c:\\opencv_3_1_0\\include

HEADERS += \
    concave_hull.h
