#-------------------------------------------------
#
# Project created by QtCreator 2017-04-07 上午09:30
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = RCLOCKDcu
TEMPLATE = lib

DEFINES += RCLOCKDCUGUI_LIBRARY

SOURCES += src/RCLOCKDcuGUI.cpp \
    src/RCLOCKDcuGUIDlg.cpp \
    src/RCLOCKDcuGUIExport.cpp \
    ../Common/Log.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/CommonFunction.cpp

HEADERS += \
    src/RCLOCKDcuGUI.h \
    src/RCLOCKDcuGUI_global.h \
    src/RCLOCKDcuGUIDlg.h \
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

FORMS += src/RCLOCKDcuGUIDlg.ui

MOC_DIR = RCLOCKDcu/moc
RCC_DIR = RCLOCKDcu/rcc
UI_DIR = RCLOCKDcu/ui
OBJECTS_DIR = RCLOCKDcu/obj

OTHER_FILES +=

RESOURCES += \
    src/image.qrc

DESTDIR = ../../
