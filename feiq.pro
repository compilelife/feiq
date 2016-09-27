#-------------------------------------------------
#
# Project created by QtCreator 2016-07-30T10:47:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = feiq
TEMPLATE = app

LIBS += -liconv -lsqlite3

mac{
    QT += macextras

    ICON = icon.icns

    LIBS += -framework Foundation

    OBJECTIVE_SOURCES += osx/notification.mm

    SOURCES += osx/osxplatform.cpp

    HEADERS += osx/notification.h\
                osx/osxplatform.h
}

SOURCES += main.cpp\
        mainwindow.cpp \
    feiqlib/udpcommu.cpp \
    feiqlib/feiqcommu.cpp \
    feiqlib/feiqengine.cpp \
    feiqlib/feiqmodel.cpp \
    feiqlib/encoding.cpp \
    feiqlib/tcpserver.cpp \
    feiqlib/tcpsocket.cpp \
    feiqlib/utils.cpp \
    feiqlib/uniqueid.cpp \
    feiqlib/filetask.cpp \
    feiqlib/defer.cpp \
    feiqlib/asynwait.cpp \
    feiqlib/history.cpp \
    fellowlistwidget.cpp \
    searchfellowdlg.cpp \
    recvtextedit.cpp \
    filemanagerdlg.cpp \
    addfellowdialog.cpp \
    emoji.cpp \
    chooseemojidlg.cpp \
    platformdepend.cpp \
    chooseemojiwidget.cpp \
    sendtextedit.cpp \
    feiqwin.cpp \
    plugin/unreadchecker.cpp \
    plugin/iplugin.cpp \
    plugin/rankuser.cpp \
    settings.cpp


HEADERS  += mainwindow.h \
    feiqlib/ipmsg.h \
    feiqlib/udpcommu.h \
    feiqlib/feiqcommu.h \
    feiqlib/protocol.h \
    feiqlib/post.h \
    feiqlib/content.h \
    feiqlib/feiqengine.h \
    feiqlib/feiqmodel.h \
    feiqlib/fellow.h \
    feiqlib/ifeiqview.h \
    feiqlib/msgqueuethread.h \
    feiqlib/encoding.h \
    feiqlib/tcpserver.h \
    feiqlib/tcpsocket.h \
    feiqlib/utils.h \
    feiqlib/uniqueid.h \
    feiqlib/filetask.h \
    feiqlib/defer.h \
    feiqlib/asynwait.h \
    feiqlib/history.h \
    feiqlib/parcelable.h \
    fellowlistwidget.h \
    searchfellowdlg.h \
    recvtextedit.h \
    filemanagerdlg.h \
    addfellowdialog.h \
    emoji.h \
    chooseemojidlg.h \
    platformdepend.h \
    chooseemojiwidget.h \
    sendtextedit.h \
    plugin/iplugin.h \
    feiqwin.h \
    plugin/unreadchecker.h \
    plugin/rankuser.h \
    settings.h

FORMS    += mainwindow.ui \
    searchfellowdlg.ui \
    downloadfiledlg.ui \
    addfellowdialog.ui \
    chooseemojidlg.ui

RESOURCES += \
    default.qrc
