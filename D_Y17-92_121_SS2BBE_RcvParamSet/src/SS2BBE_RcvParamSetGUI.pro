#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = SS2BBE_RcvParamSet
TEMPLATE = lib

DEFINES += SS2BBE_RCVPARAMSETGUISHARED_LIBRARY

SOURCES += \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    SS2BBE_RcvParamSetGUI.cpp \
    SS2BBE_RcvParamSetGUIDlg.cpp \
    SS2BBE_RcvParamSetGUIExport.cpp \
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
    SS2BBE_RcvParamSetGUI.h \
    SS2BBE_RcvParamSetGUI_global.h \
    SS2BBE_RcvParamSetGUIDlg.h \
    ../Common/SMCUSSB2_BBE_CDMAStruct.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    SS2BBE_RcvParamSetGUIDlg.ui

MOC_DIR = SS2BBE_RcvParamSet/moc
RCC_DIR = SS2BBE_RcvParamSet/rcc
UI_DIR = SS2BBE_RcvParamSet/ui
OBJECTS_DIR = SS2BBE_RcvParamSet/obj
