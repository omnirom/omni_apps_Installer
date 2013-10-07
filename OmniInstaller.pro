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
    src/dialogs/ChooserDialog.cpp \
    src/dialogs/RomListDialog.cpp \
    src/FlashScenario.cpp \
    src/flasher/AbstractStep.cpp \
    src/flasher/FastbootStep.cpp \
    src/dialogs/FlashingDialog.cpp \
    src/FastbootMonitor.cpp \
    src/ui/AnimatedLabel.cpp \
    src/flasher/NullStep.cpp

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
    src/dialogs/ChooserDialog.h \
    src/dialogs/RomListDialog.h \
    src/FlashScenario.h \
    src/flasher/AbstractStep.h \
    src/flasher/FastbootStep.h \
    src/dialogs/FlashingDialog.h \
    src/FastbootMonitor.h \
    src/ui/AnimatedLabel.h \
    src/flasher/NullStep.h

FORMS    += \
    ui/welcomedialog.ui \
    ui/chooserdialog.ui \
    ui/romlistdialog.ui \
    ui/flashingdialog.ui \
    ui/finishdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    src/QJson/json_scanner.yy \
    src/QJson/json_parser.yy
