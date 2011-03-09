QT += network

CONFIG += link_pkgconfig debug

INCLUDEPATH += ../lib/

HEADERS = ../lib/paneltaskwatcher.h
SOURCES = main.cpp \
  panel.cpp \
  panelwidgetplugin.cpp \
  panelsettings.cpp \
  paneltaskwatcher.cpp
