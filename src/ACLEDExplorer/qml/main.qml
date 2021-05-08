
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

import QtQuick 2.12 as Quick
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.12

import Esri.ACLEDExplorer 1.0
import Esri.FeatureTableModel 1.0

import Qt.labs.qmlmodels 1.0

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

        ACLEDExplorer {
            id: explorerView
            Layout.fillHeight: true
            Layout.fillWidth: true

            // Create MapQuickView here, and create its Map etc. in C++ code
            MapView {
                anchors.fill: parent
                objectName: "mapView"
                // set focus to enable keyboard navigation
                focus: true
            }
        }

        Quick.TableView {
            id: tableView
            Layout.fillHeight: true
            Layout.fillWidth: true
            columnSpacing: 1
            rowSpacing: 1
            clip: true

            model: explorerView.featureTableModel

            /*
            model: TableModel {
                TableModelColumn { display: "name" }
                TableModelColumn { display: "color" }

                rows: [
                    {
                        "name": "cat",
                        "color": "black"
                    },
                    {
                        "name": "dog",
                        "color": "brown"
                    },
                    {
                        "name": "bird",
                        "color": "white"
                    }
                ]
            }
            */

            delegate: Quick.Rectangle {
                implicitWidth: 100
                implicitHeight: 50
                border.width: 1
                color: "#d3c2a6"

                Quick.Text {
                    text: display
                    anchors.centerIn: parent
                }
            }
        }

    }
}
