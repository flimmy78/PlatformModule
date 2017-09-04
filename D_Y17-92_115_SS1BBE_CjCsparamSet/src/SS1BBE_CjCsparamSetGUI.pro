#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml
QT       += gui
QT       += widgets


TARGET = SS1BBE_CjCsParamSet
TEMPLATE = lib

DEFINES += SS1BBE_CJCSPARAMSETGUISHARED_LIBRARY

SOURCES += \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    SS1BBE_CjCsParamSetGUI.cpp \
    SS1BBE_CjCsParamSetGUIDlg.cpp \
    SS1BBE_CjCsParamSetGUIExport.cpp \
    ../../Common/CommonFunction.cpp

HEADERS += \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/MCSDataStruct.h \
    ../../Common/TypeDefine.h \
    ../../Common/DeviceMap.h \
    ../../Common/Log.h \
    SS1BBE_CjCsParamSetGUI.h \
    SS1BBE_CjCsParamSetGUI_global.h \
    SS1BBE_CjCsParamSetGUIDlg.h \
    ../../Common/SMCU_BBE_CDMAStruct.h \
    ../../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    SS1BBE_CjCsParamSetGUIDlg.ui

MOC_DIR = SS1BBE_CjCsParamSet/moc
RCC_DIR = SS1BBE_CjCsParamSet/rcc
UI_DIR = SS1BBE_CjCsParamSet/ui
OBJECTS_DIR = SS1BBE_CjCsParamSet/obj
