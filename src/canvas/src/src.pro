QT += webkit

CONFIG += link_pkgconfig
PKGCONFIG += xdamage xtst xcomposite
TARGET = antico

SOURCES = main.cpp windowmanager.cpp window.cpp
HEADERS = windowmanager.hpp window.hpp
