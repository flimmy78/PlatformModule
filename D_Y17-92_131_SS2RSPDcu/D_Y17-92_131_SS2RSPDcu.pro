#-------------------------------------------------
#
# Project created by QtCreator 2017-04-07 上午09:30
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = SS2RSPDcu
TEMPLATE = lib

DEFINES += SS2RSPDCUGUI_LIBRARY

SOURCES += src/SS2RSPDcuGUI.cpp \
    src/SS2RSPDcuGUIDlg.cpp \
    src/SS2RSPDcuGUIExport.cpp \
    ../Common/Log.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/CommonFunction.cpp

HEADERS += \
    src/SS2RSPDcuGUI.h \
    src/SS2RSPDcuGUI_global.h \
    src/SS2RSPDcuGUIDlg.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/Log.h \
    ../Common/DeviceMap.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += src/SS2RSPDcuGUIDlg.ui

MOC_DIR = SS2RSPDcu/moc
RCC_DIR = SS2RSPDcu/rcc
UI_DIR = SS2RSPDcu/ui
OBJECTS_DIR = SS2RSPDcu/obj

RESOURCES += \
    src/image.qrc
DESTDIR = ../../
