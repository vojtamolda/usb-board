TEMPLATE = app
QT = gui core widgets

macx {
    DESTDIR = target/macx/
    BUILDDIR = build/macx/
    INCLUDEPATH += /usr/local/include/libftdi1/
    LIBS += -L/usr/local/lib/ \
            -lftdi1 -lusb
    ICON = resrc/icon/attenuator.icns
}
unix:!macx {
    DESTDIR = target/linux/
    BUILDDIR = build/linux/
    INCLUDEPATH += /usr/include/
    LIBS += -L/usr/lib/x86_64-linux-gnu/ \
            -lftdi
}
win32 {
    DESTDIR = target/win32/
    BUILDDIR = build/win32/
    INCLUDEPATH += $${PWD}/lib/win32/
    LIBS += -L$${PWD}/lib/win32/ \
            -lftdi1 -lusb-1.0
    RC_FILE = resrc/attenuator.rc
}

TARGET = "USB Attenuator"
MOC_DIR = $${BUILDDIR}/moc
RCC_DIR = $${BUILDDIR}/rcc
UI_DIR = $${BUILDDIR}/ui
OBJECTS_DIR = $${BUILDDIR}/obj

INCLUDEPATH += include
HEADERS += \
    include/ftdidevice.h \
    include/ftditablemodel.h \
    include/attenuatorwidget.h \
    include/attenuatorwindow.h
SOURCES += \
    source/ftdidevice.cpp \
    source/ftditablemodel.cpp \
    source/attenuatorwidget.cpp \
    source/attenuatorwindow.cpp \
    source/attenuatormain.cpp
FORMS += \
    form/attenuatorwidget.ui \
    form/attenuatorwindow.ui
RESOURCES += \
    resrc/attenuator.qrc
TRANSLATIONS += \
    resrc/language/attenuator-cs.ts \
    resrc/language/attenuator-fr.ts \
    resrc/language/attenuator-de.ts
OTHER_FILES += \
    $${TRANSLATIONS} \
    $${RC_FILE} \
    attenuator.md
