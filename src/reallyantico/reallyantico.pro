QT += dbus

CONFIG += warn_on
DEFINES = QT_FATAL_WARNINGS QT_NO_DEBUG_OUTPUT

devel {
  DEFINES -= QT_NO_DEBUG_OUTPUT
}

OBJECTS_DIR += build
MOC_DIR += build

isEmpty(PREFIX):PREFIX=/usr/local

TARGET = antico

TRANSLATIONS = \
 ../../language/it_IT.ts \
 ../../language/cs_CZ.ts \
 ../../language/ru_RU.ts \
 ../../language/pl_PL.ts \
 ../../language/de_DE.ts \
 ../../language/es_ES.ts \
 ../../language/pt_BR.ts

translations.name = Translations ${QMAKE_FILE_BASE}
translations.CONFIG += no_link target_predeps
unix:translations.commands = @$$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT} &> /dev/null
translations.input = TRANSLATIONS
translations.output = ${QMAKE_FILE_BASE}.qm
translations.clean = ${QMAKE_FILE_BASE}.qm
QMAKE_EXTRA_COMPILERS += translations

include(../../xsessions/xsessions.pri)

SOURCES = \
 main.cpp \
 antico.cpp \
 frame.cpp \
 border.cpp \
 dockbar.cpp \
 launcher.cpp \
 dockicon.cpp \
 desk.cpp \
 titlebar.cpp \
 systray.cpp \
 dateclock.cpp \
 sysicon.cpp \
 deskapp.cpp \
 utils.cpp \
 deskdev.cpp \
 deskfile.cpp \
 deskfolder.cpp \
 dockapp.cpp \
 deskicon.cpp \
 dockmenu.cpp \
 debug.cpp \
 atoms.cpp \
 client.cpp

HEADERS = \
 antico.h \
 frame.h \
 border.h \
 dockbar.h \
 launcher.h \
 dockicon.h \
 desk.h \
 titlebar.hpp \
 systray.h \
 dateclock.h \
 sysicon.h \
 deskapp.h \
 utils.h \
 deskdev.h \
 deskfile.h \
 deskfolder.h \
 dockapp.h \
 deskicon.h \
 dockmenu.h \
 debug.hpp \
 atoms.hpp \
 client.hpp

target.path = $$PREFIX/bin/

xsession_files.CONFIG = no_check_exist
xsession_files.files = *.desktop
xsession_files.path = /usr/share/xsessions/

qm.files = *.qm
qm.path = $$PREFIX/share/antico/language
qm.CONFIG += no_check_exist

INSTALLS += target qm xsession_files
