#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = SS2BBE_SysParamSet
TEMPLATE = lib

DEFINES += SS2BBE_SYSPARAMSETGUISHARED_LIBRARY

SOURCES += \
    src/SS2BBE_SysParamSetGUI.cpp \
    src/SS2BBE_SysParamSetGUIDlg.cpp \
    src/SS2BBE_SysParamSetGUIExport.cpp \
    ../Common/CommonFunction.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/GroupSwitch.cpp \
    ../Common/Log.cpp

HEADERS += \
    src/SS2BBE_SysParamSetGUI.h \
    src/SS2BBE_SysParamSetGUI_global.h \
    src/SS2BBE_SysParamSetGUIDlg.h \
    ../Common/CommonFunction.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/ddtTTCCtrlResultReport.h \
    ../Common/DeviceMap.h \
    ../Common/GroupSwitch.h \
    ../Common/Log.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/MCSUSBDataStruct.h \
    ../Common/SMCU_BBE_CDMAStruct.h \
    ../Common/SMCUHeadFile.h \
    ../Common/SMCUSSB2_BBE_CDMAStruct.h \
    ../Common/ss1trlResultReport.h \
    ../Common/ss2trlResultReport.h \
    ../Common/TypeDefine.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/SS2BBE_SysParamSetGUIDlg.ui

MOC_DIR = SS2BBE_SysParamSet/moc
RCC_DIR = SS2BBE_SysParamSet/rcc
UI_DIR = SS2BBE_SysParamSet/ui
OBJECTS_DIR = SS2BBE_SysParamSet/obj

DESTDIR += ../../

