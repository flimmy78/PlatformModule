#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = TCPNCodeSet
TEMPLATE = lib

DEFINES += TCPNCODESETGUISHARED_LIBRARY

SOURCES += \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    src/TCPNCodeSetGUI.cpp \
    src/TCPNCodeSetGUIExport.cpp \
    src/TCPNCodeSetGUIDlg.cpp \
    ../Common/CommonFunction.cpp

HEADERS += \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/SMCUHeadFile.h \
    src/TCPNCodeSetGUI.h \
    src/TCPNCodeSetGUI_global.h \
    src/TCPNCodeSetGUIDlg.h \
    ../Common/SMCUSSB2_BBE_CDMAStruct.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/TCPNCodeSetGUIDlg.ui

MOC_DIR = TCPNCodeSet/moc
RCC_DIR = TCPNCodeSet/rcc
UI_DIR = TCPNCodeSet/ui
OBJECTS_DIR = TCPNCodeSet/obj

DESTDIR += ../../
