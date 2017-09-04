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

SOURCES += src/RecordMonitorGUI.cpp \
    src/RecordMonitorGUIExport.cpp \
    src/RecordMonitorGUIDlg.cpp \
    ../Common/DeviceMap.cpp \
    ../Common/Log.cpp \
    ../Common/CommonFunction.cpp \
    src/RecordDialog.cpp \
    src/ReplayDialog.cpp

HEADERS += RecordMonitorGUI.h\
    src/RecordMonitorGUI_global.h \
    src/RecordMonitorGUIDlg.h \
    ../Common/DeviceMap.h \
    ../Common/DataStruct2311.h \
    ../Common/Dcu_common_DataStruct.h \
    ../Common/LxTsspCommonDefinition.h \
    ../Common/TypeDefine.h \
    ../Common/Log.h \
    ../Common/CommonFunction.h \
    src/RecordDialog.h \
    src/ReplayDialog.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    src/RecordMonitorGUIDlg.ui \
    src/recorddialog.ui \
    src/ReplayDialog.ui

MOC_DIR = RecordMonitor/moc
RCC_DIR = RecordMonitor/rcc
UI_DIR = RecordMonitor/ui
OBJECTS_DIR = RecordMonitor/obj

OTHER_FILES +=

RESOURCES += \
    src/image.qrc

DESTDIR = ../../
