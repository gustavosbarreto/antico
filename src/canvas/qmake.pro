QT += webkit

CONFIG += link_pkgconfig
PKGCONFIG += xdamage xtst xcomposite
TARGET = antico

SOURCES = main.cpp windowmanager.cpp
HEADERS = windowmanager.hpp
