include(../common/common.pri)
include(lib/lib.pri)
include(decorations/decorations.pri)

HEADERS += \
  client.hpp \
  windowmanager.hpp \
  decoration.hpp
SOURCES += \
  client.cpp \
  main.cpp \
  windowmanager.cpp \
  decoration.cpp

TARGET = antico-wm