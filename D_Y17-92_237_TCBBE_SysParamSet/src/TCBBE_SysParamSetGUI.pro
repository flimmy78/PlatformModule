#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = TCBBE_SysParamSet
TEMPLATE = lib

DEFINES += SS2BBE_SYSPARAMSETGUISHARED_LIBRARY

SOURCES += \
    ../../Common/CommonFunction.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/GroupSwitch.cpp \
    ../../Common/Log.cpp \
    TCBBE_SysParamSetGUI.cpp \
    TCBBE_SysParamSetGUIExport.cpp \
    TCBBE_SysParamSetGUIDlg.cpp

HEADERS += \
    ../../Common/CommonFunction.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/ddtTTCCtrlResultReport.h \
    ../../Common/DeviceMap.h \
    ../../Common/GroupSwitch.h \
    ../../Common/Log.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/MCSDataStruct.h \
    ../../Common/MCSUSBDataStruct.h \
    ../../Common/SMCU_BBE_CDMAStruct.h \
    ../../Common/SMCUHeadFile.h \
    ../../Common/SMCUSSB2_BBE_CDMAStruct.h \
    ../../Common/ss1trlResultReport.h \
    ../../Common/ss2trlResultReport.h \
    ../../Common/TypeDefine.h \
    TCBBE_SysParamSetGUI.h \
    TCBBE_SysParamSetGUIDlg.h \
    TCBBE_SysParamSetGUI_global.h \
    ../../Common/struct_header/Dcu_2311_AK_DataStruct.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    TCBBE_SysParamSetGUIDlg.ui

MOC_DIR = TCBBE_SysParamSet/moc
RCC_DIR = TCBBE_SysParamSet/rcc
UI_DIR = TCBBE_SysParamSet/ui
OBJECTS_DIR = TCBBE_SysParamSet/obj
