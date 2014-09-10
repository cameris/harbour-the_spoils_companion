TEMPLATE = lib
TARGET = keepaliveplugin
TARGET = $$qtLibraryTarget($$TARGET)
QT += dbus qml
CONFIG += debug plugin
INCLUDEPATH += nemo-keepalive/lib
 
PROJECT = the_spoils_companion
TARGET_PATH = /usr/share/harbour-$$PROJECT/harbour/$$PROJECT/keepalive

system(qdbusxml2cpp -p nemo-keepalive/lib/mceiface.h:nemo-keepalive/lib/mceiface.cpp nemo-keepalive/lib/mceiface.xml)
 
# workaround to allow custom import paths
system(sed -e s/org\.nemomobile\.keepalive/harbour.the_spoils_companion.keepalive/ -e s/plugin\.moc/plugin-fixed.moc/ nemo-keepalive/plugin/plugin.cpp > nemo-keepalive/plugin/plugin-fixed.cpp)
 
SOURCES += \
    libiphb/src/libiphb.c \
    nemo-keepalive/lib/displayblanking.cpp \
    nemo-keepalive/lib/displayblanking_p.cpp \
    nemo-keepalive/lib/backgroundactivity.cpp \
    nemo-keepalive/lib/backgroundactivity_p.cpp \
    nemo-keepalive/lib/mceiface.cpp \
    nemo-keepalive/lib/heartbeat.cpp \
    nemo-keepalive/plugin/declarativebackgroundactivity.cpp \
    nemo-keepalive/plugin/plugin-fixed.cpp
 
HEADERS += \
    libiphb/src/libiphb.h \
    libiphb/src/iphb_internal.h \
    nemo-keepalive/lib/displayblanking.h \
    nemo-keepalive/lib/displayblanking_p.h \
    nemo-keepalive/lib/backgroundactivity.h \
    nemo-keepalive/lib/backgroundactivity_p.h \
    nemo-keepalive/lib/mceiface.h \
    nemo-keepalive/lib/heartbeat.h \
    nemo-keepalive/lib/common.h \
    nemo-keepalive/plugin/declarativebackgroundactivity.h
 
target.path = $$TARGET_PATH
qmldir.path = $$TARGET_PATH
qmldir.files = qmldir
INSTALLS += target qmldir
