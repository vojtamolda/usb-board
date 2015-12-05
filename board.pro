TEMPLATE = app
QT = gui core widgets

macx {
    DESTDIR = target/macx/
    BUILDDIR = build/macx/
    INCLUDEPATH += /usr/local/include/libftdi1/
    LIBS += -L/usr/local/lib/ \
            -lftdi1
    ICON = resrc/icon/board.icns
}
unix:!macx {
    DESTDIR = target/linux/
    BUILDDIR = build/linux/
    INCLUDEPATH += /usr/include/
    LIBS += -L/usr/x86_64-linux-gnu/lib/ \
            -lftdi
}
win32 {
    DESTDIR = target/win32/
    BUILDDIR = build/win32/
    INCLUDEPATH += $${PWD}/lib/win32/
    LIBS += -L$${PWD}/lib/win32/ \
             -lftdi1 -lusb-1.0
    RC_FILE = resrc/board.rc
}

TARGET = "USB Board"
MOC_DIR = $${BUILDDIR}/moc
RCC_DIR = $${BUILDDIR}/rcc
UI_DIR = $${BUILDDIR}/ui
OBJECTS_DIR = $${BUILDDIR}/obj

INCLUDEPATH += \
    include \
    firmware
HEADERS += \
    include/ftdidevice.h \
    include/ftditablemodel.h \
    include/boardwidget.h \
    include/boardsettings.h \
    include/boardwindow.h
SOURCES += \
    source/ftdidevice.cpp \
    source/ftditablemodel.cpp \
    source/boardwidget.cpp \
    source/boardsettings.cpp \
    source/boardwindow.cpp \
    source/boardmain.cpp
FORMS += \
    form/boardwidget.ui \
    form/boardsettings.ui \
    form/boardwindow.ui
RESOURCES += \
    resrc/board.qrc
TRANSLATIONS = \
    resrc/language/board-cs.ts \
    resrc/language/board-fr.ts
OTHER_FILES += \
    $${TRANSLATIONS} \
    $${RC_FILE} \
    firmware/firmware.h \
    firmware/firmware.c \
    firmware/makefile \
    board.md
