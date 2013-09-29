#-------------------------------------------------
#
# Project created by QtCreator 2013-09-29T01:08:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OmniInstaller
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/dialogs/WelcomeDialog.cpp \
    src/AdbMonitor.cpp \
    src/AdbDevice.cpp \
    src/util/Utils.cpp \
    src/util/HttpClient.cpp \
    src/BundleManager.cpp \
    src/QJson/serializerrunnable.cpp \
    src/QJson/serializer.cpp \
    src/QJson/qobjecthelper.cpp \
    src/QJson/parserrunnable.cpp \
    src/QJson/parser.cpp \
    src/QJson/json_scanner.cpp \
    src/QJson/json_scanner.cc \
    src/QJson/json_parser.cc \
    src/dialogs/ChooserDialog.cpp

HEADERS  += \
    src/util/HttpClient.h \
    src/dialogs/WelcomeDialog.h \
    src/AdbMonitor.h \
    src/AdbDevice.h \
    src/util/Utils.h \
    src/BundleManager.h \
    src/QJson/stack.hh \
    src/QJson/serializerrunnable.h \
    src/QJson/serializer.h \
    src/QJson/Serializer \
    src/QJson/qobjecthelper.h \
    src/QJson/QObjectHelper \
    src/QJson/qjson_export.h \
    src/QJson/qjson_debug.h \
    src/QJson/position.hh \
    src/QJson/parserrunnable.h \
    src/QJson/parser_p.h \
    src/QJson/parser.h \
    src/QJson/Parser \
    src/QJson/location.hh \
    src/QJson/json_scanner.h \
    src/QJson/json_parser.hh \
    src/QJson/FlexLexer.h \
    src/chooserdialog.h \
    src/dialogs/ChooserDialog.h

FORMS    += \
    ui/welcomedialog.ui \
    ui/chooserdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    src/QJson/json_scanner.yy \
    src/QJson/json_parser.yy
