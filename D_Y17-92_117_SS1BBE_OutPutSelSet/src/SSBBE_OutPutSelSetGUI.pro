#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = SSBBE_OutPutSelSet
TEMPLATE = lib

DEFINES += SSBBE_OUTPUTSELSETGUISHARED_LIBRARY

SOURCES += \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    SSBBE_OutPutSelSetGUI.cpp \
    SSBBE_OutPutSelSetGUIDlg.cpp \
    SSBBE_OutPutSelSetGUIExport.cpp \
    ../../Common/CommonFunction.cpp

HEADERS += \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/MCSDataStruct.h \
    ../../Common/TypeDefine.h \
    ../../Common/DeviceMap.h \
    ../../Common/Log.h \
    ../../Common/SMCU_BBE_CDMAStruct.h \
    SSBBE_OutPutSelSetGUI.h \
    SSBBE_OutPutSelSetGUI_global.h \
    SSBBE_OutPutSelSetGUIDlg.h \
    ../../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    SSBBE_OutPutSelSetGUIDlg.ui

MOC_DIR = SSBBE_OutPutSelSet/moc
RCC_DIR = SSBBE_OutPutSelSet/rcc
UI_DIR = SSBBE_OutPutSelSet/ui
OBJECTS_DIR = SSBBE_OutPutSelSet/obj
