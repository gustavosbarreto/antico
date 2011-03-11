QT += network

CONFIG += link_pkgconfig debug

INCLUDEPATH += ../lib/ ../

include(../applets/startmenu/startmenu.pri)
include(../applets/tasklist/tasklist.pri)

RESOURCES += resources.qrc

HEADERS += \
  ../lib/paneltaskwatcher.h \
  ../lib/taskwindow.h

SOURCES += \
  main.cpp \
  panel.cpp \
  paneldecoration.cpp \
  panelapplet.cpp \
  panelsettings.cpp \
  paneltaskwatcher.cpp \
  taskwindow.cpp
