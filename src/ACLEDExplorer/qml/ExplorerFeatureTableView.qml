import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

import Esri.FeatureTableModel 1.0

TableView {
    property FeatureTableModel featureTableModel

    Layout.fillHeight: true
    Layout.fillWidth: true
    columnSpacing: 1
    rowSpacing: 1
    clip: true

    model: featureTableModel

    delegate: Rectangle {
        implicitWidth: 100
        implicitHeight: 50
        border.width: 1
        color: "#d3c2a6"

        Text {
            text: display
            anchors.centerIn: parent
        }
    }
}
