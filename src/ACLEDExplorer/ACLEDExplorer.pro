#-------------------------------------------------
#  Copyright 2016 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------

TEMPLATE = app

QT += core gui opengl network positioning sensors qml quick quickcontrols2
CONFIG += c++11

TARGET = ACLEDExplorer

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 9) { 
        error("$$TARGET requires Qt 5.9.2")
    }
	equals(QT_MINOR_VERSION, 9) : lessThan(QT_PATCH_VERSION, 2) {
		error("$$TARGET requires Qt 5.9.2")
	}
}

# On Windows you can use $$() but not $${} like in the arcgis*.pri project file
ARCGIS_RUNTIME_VERSION = $$(ARCGIS_RUNTIME_VERSION)
include($$PWD/arcgisruntime.pri)

HEADERS += \
    AcledLayerSource.h \
    AppInfo.h \
    ACLEDExplorer.h \
    FeatureTableModel.h

SOURCES += \
    AcledLayerSource.cpp \
    FeatureTableModel.cpp \
    main.cpp \
    ACLEDExplorer.cpp

RESOURCES += \
    qml/qml.qrc \
    Resources/Resources.qrc

#-------------------------------------------------------------------------------

win32 {
    include (Win/Win.pri)
}

macx {
    include (Mac/Mac.pri)
}

ios {
    include (iOS/iOS.pri)
}

android {
    include (Android/Android.pri)
}
