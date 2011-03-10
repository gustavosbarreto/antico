QT += network

CONFIG += link_pkgconfig debug

INCLUDEPATH += ../lib/

HEADERS = \
  ../lib/paneltaskwatcher.h \
  ../lib/taskwindow.h

SOURCES = \
  main.cpp \
  panel.cpp \
  paneldecoration.cpp \
  panelsettings.cpp \
  paneltaskwatcher.cpp \
  taskwindow.cpp
