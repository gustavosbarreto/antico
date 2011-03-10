TEMPLATE = lib
CONFIG += create_pc create_prl
TARGET = workspacepanel

HEADERS = paneldecoration.h
SOURCES = dummy.cpp

target.path = $$PREFIX/lib/

headers.path = $$PREFIX/include/workspace/
headers.files = paneldecoration.h

QMAKE_PKGCONFIG_NAME = workspacepanel
QMAKE_PKGCONFIG_DESCRIPTION = workspacepanel
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

#INSTALLS += headers target
