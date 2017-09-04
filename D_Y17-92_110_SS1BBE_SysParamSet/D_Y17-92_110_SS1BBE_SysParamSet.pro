#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = SSBBE_SysParamSet
TEMPLATE = lib

DEFINES += SSBBE_SYSPARAMSETGUISHARED_LIBRARY

SOURCES += \
    src/SSBBE_SysParamSetGUI.cpp \
    src/SSBBE_SysParamSetGUIExport.cpp \
    src/SSBBE_SysParamSetGUIDlg.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    ../Common/CommonFunction.cpp

HEADERS += \
    src/SSBBE_SysParamSetGUI.h \
    src/SSBBE_SysParamSetGUI_global.h \
    src/SSBBE_SysParamSetGUIDlg.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/SMCU_BBE_CDMAStruct.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/SSBBE_SysParamSetGUIDlg.ui

MOC_DIR = SSBBE_SysParamSet/moc
RCC_DIR = SSBBE_SysParamSet/rcc
UI_DIR = SSBBE_SysParamSet/ui
OBJECTS_DIR = SSBBE_SysParamSet/obj
DESTDIR += ../../
