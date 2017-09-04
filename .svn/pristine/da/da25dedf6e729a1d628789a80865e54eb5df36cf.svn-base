#-------------------------------------------------
#
# Project created by QtCreator 2017-02-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = TestNet_Switch
TEMPLATE = lib

DEFINES += TestNET_SWITCHGUISHARED_LIBRARY

SOURCES += TestNet_SwitchGUI.cpp \
    TestNet_SwitchGUIDlg.cpp \
    TestNet_SwitchGUIExport.cpp \
    XmlReader.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    ../../Common/GroupSwitch.cpp \
    ../../Common/CommonFunction.cpp

HEADERS +=  \
    TypeDefine.h \
    TestNet_SwitchGUI.h \
    TestNet_SwitchGUI_global.h \
    TestNet_SwitchGUIDlg.h \
    XmlReader.h \
    ../../Common/DeviceMap.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/TypeDefine.h \
    ../../Common/Log.h \
    ../../Common/GroupSwitch.h \
    ../../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += TestNet_SwitchGUIDlg.ui

MOC_DIR = TestNet_Switch/moc
RCC_DIR = TestNet_Switch/rcc
UI_DIR = TestNet_Switch/ui
OBJECTS_DIR = TestNet_Switch/obj
