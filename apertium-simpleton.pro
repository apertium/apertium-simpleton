QT += core gui widgets network

TARGET = apertium-simpleton
TEMPLATE = app

SOURCES += \
	src/main.cpp \
	src/simpleton.cpp \
    src/installer.cpp

HEADERS += \
	src/simpleton.hpp \
    src/installer.hpp

FORMS += \
	src/simpleton.ui \
    src/installer.ui
