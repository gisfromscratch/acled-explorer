
// Copyright 2016 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: appWindow
    visible: true
    width: 800
    height: 600

    Material.theme: Material.Dark
    Material.accent: "#a7ad6d"      // BW Hellgrün
    //Material.accent: "#616847"      // BW Helloliv
    Material.background: "#312d2a"  // BW Schwarz
    Material.foreground: "#d3c2a6"  // BW Beige
    Material.primary: "#434a39"     // BW Dunkelgrün

    ColumnLayout {
        anchors.fill: parent

        ExplorerMapView {
            id: explorerView
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        ExplorerFeatureTableView {
            height: 300
            Layout.fillWidth: true
            featureTableModel: explorerView.featureTableModel
        }

    }
}
