#-------------------------------------------------
#
# Project created by QtCreator 2017-04-28 下午07:18
#
#-------------------------------------------------

QT       += network xml

QT       += gui
QT       += widgets


TARGET = RecordMonitor
TEMPLATE = lib

DEFINES += RECORDMONITORGUISHARED_LIBRARY

SOURCES += RecordMonitorGUI.cpp \
    RecordMonitorGUIExport.cpp \
    RecordMonitorGUIDlg.cpp \
    ../../Common/DeviceMap.cpp \
    ../../Common/Log.cpp \
    ../../Common/CommonFunction.cpp \
    RecordDialog.cpp \
    ReplayDialog.cpp

HEADERS += RecordMonitorGUI.h\
    RecordMonitorGUI_global.h \
    RecordMonitorGUIDlg.h \
    ../../Common/DeviceMap.h \
    ../../Common/DataStruct2311.h \
    ../../Common/Dcu_common_DataStruct.h \
    ../../Common/LxTsspCommonDefinition.h \
    ../../Common/TypeDefine.h \
    ../../Common/Log.h \
    ../../Common/CommonFunction.h \
    RecordDialog.h \
    ReplayDialog.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    RecordMonitorGUIDlg.ui \
    recorddialog.ui \
    ReplayDialog.ui

MOC_DIR = RecordMonitor/moc
RCC_DIR = RecordMonitor/rcc
UI_DIR = RecordMonitor/ui
OBJECTS_DIR = RecordMonitor/obj

OTHER_FILES +=

RESOURCES += \
    image.qrc
