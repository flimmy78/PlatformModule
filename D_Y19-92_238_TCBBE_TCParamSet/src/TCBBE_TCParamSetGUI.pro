#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = TCBBE_TCParamSet
TEMPLATE = lib

DEFINES += TCBBE_TCPARASETGUISHARED_LIBRARY

SOURCES += \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    TCBBE_TCParamSetGUI.cpp \
    TCBBE_TCParamSetGUIDlg.cpp \
    TCBBE_TCParamSetGUIExport.cpp \
    ../../Common/CommonFunction.cpp

HEADERS += \
    ../../Common/DeviceMap.h \
    ../../Common/Log.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/MCSDataStruct.h \
    ../../Common/TypeDefine.h \
    TCBBE_TCParamSetGUI.h \
    TCBBE_TCParamSetGUI_global.h \
    TCBBE_TCParamSetGUIDlg.h \
    ../../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    TCBBE_TCParamSetGUIDlg.ui

MOC_DIR = TCBBE_TCParamSet/moc
RCC_DIR = TCBBE_TCParamSet/rcc
UI_DIR = TCBBE_TCParamSet/ui
OBJECTS_DIR = TCBBE_TCParamSet/obj
