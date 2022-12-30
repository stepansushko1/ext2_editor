QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    datablockbitmap.cpp \
    datablocks.cpp \
    datablocksingle.cpp \
    groupblock.cpp \
    groupdescriptors.cpp \
    inodebitmap.cpp \
    inodetable.cpp \
    inodetableoneinode.cpp \
    main.cpp \
    mainwindow.cpp \
    superblockwindow.cpp

HEADERS += \
    datablockbitmap.h \
    datablocks.h \
    datablocksingle.h \
    groupblock.h \
    groupdescriptors.h \
    inodebitmap.h \
    inodetable.h \
    inodetableoneinode.h \
    mainwindow.h \
    superblockwindow.h

FORMS += \
    datablockbitmap.ui \
    datablocks.ui \
    datablocksingle.ui \
    groupblock.ui \
    groupdescriptors.ui \
    inodebitmap.ui \
    inodetable.ui \
    inodetableoneinode.ui \
    mainwindow.ui \
    superblockwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
