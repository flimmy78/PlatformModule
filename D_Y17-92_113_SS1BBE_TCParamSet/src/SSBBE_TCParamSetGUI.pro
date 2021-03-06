#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = SSBBE_TCParamSet
TEMPLATE = lib

DEFINES += SSBBE_TCPARASETGUISHARED_LIBRARY

SOURCES += \
    SSBBE_TCParamSetGUIExport.cpp \
    SSBBE_TCParamSetGUIDlg.cpp \
    SSBBE_TCParamSetGUI.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    ../../Common/CommonFunction.cpp

HEADERS += \
    SSBBE_TCParamSetGUIDlg.h \
    SSBBE_TCParamSetGUI_global.h \
    SSBBE_TCParamSetGUI.h \
    ../../Common/DeviceMap.h \
    ../../Common/Log.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/MCSDataStruct.h \
    ../../Common/TypeDefine.h \
    ../../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    SSBBE_TCParamSetGUIDlg.ui

MOC_DIR = SSBBE_TCParamSet/moc
RCC_DIR = SSBBE_TCParamSet/rcc
UI_DIR = SSBBE_TCParamSet/ui
OBJECTS_DIR = SSBBE_TCParamSet/obj
