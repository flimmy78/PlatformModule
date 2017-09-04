#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = SS2BBE_TCParamSet
TEMPLATE = lib

DEFINES += SS2BBE_TCPARASETGUISHARED_LIBRARY

SOURCES += \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    src/SS2BBE_TCParamSetGUI.cpp \
    src/SS2BBE_TCParamSetGUIDlg.cpp \
    src/SS2BBE_TCParamSetGUIExport.cpp \
    ../Common/CommonFunction.cpp

HEADERS += \
    ../Common/DeviceMap.h \
    ../Common/Log.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/MCSDataStruct.h \
    ../Common/TypeDefine.h \
    src/SS2BBE_TCParamSetGUI.h \
    src/SS2BBE_TCParamSetGUI_global.h \
    src/SS2BBE_TCParamSetGUIDlg.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/SS2BBE_TCParamSetGUIDlg.ui

MOC_DIR = SS2BBE_TCParamSet/moc
RCC_DIR = SS2BBE_TCParamSet/rcc
UI_DIR = SS2BBE_TCParamSet/ui
OBJECTS_DIR = SS2BBE_TCParamSet/obj
DESTDIR += ../../
