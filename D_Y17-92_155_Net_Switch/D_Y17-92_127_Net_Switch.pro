#-------------------------------------------------
#
# Project created by QtCreator 2017-02-24 上午11:12
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = DOWNNet_Switch
TEMPLATE = lib

DEFINES += NET_SWITCHGUISHARED_LIBRARY

SOURCES +=  src/Net_SwitchGUI.cpp \
    src/Net_SwitchGUIDlg.cpp \
    src/Net_SwitchGUIExport.cpp \
    src/XmlReader.cpp \
    src/shape/arrowlabel.cpp \
    src/shape/dotlabel.cpp \
    src/shape/drawlabel.cpp \
    src/shape/linelabel.cpp \
    src/shape/Matrix.cpp \
    src/shape/signalitem.cpp \
    src/shape/menubutton.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    ../Common/CommonFunction.cpp

HEADERS +=  \
    src/TypeDefine.h \
    src/Net_SwitchGUI.h \
    src/Net_SwitchGUI_global.h \
    src/Net_SwitchGUIDlg.h \
    src/XmlReader.h \
    src/shape/arrowlabel.h \
    src/shape/dotlabel.h \
    src/shape/drawlabel.h \
    src/shape/linelabel.h \
    src/shape/Matrix.h \
    src/shape/signalitem.h \
    src/shape/menubutton.h \
    ../Common/DeviceMap.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/TypeDefine.h \
    ../Common/Log.h \
    ../Common/CommonFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += src/Net_SwitchGUIDlg.ui

MOC_DIR = Net_Switch/moc
RCC_DIR = Net_Switch/rcc
UI_DIR = Net_Switch/ui
OBJECTS_DIR = Net_Switch/obj

DESTDIR = ../../
