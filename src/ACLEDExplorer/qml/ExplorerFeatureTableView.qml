import QtQuick 2.12

import Esri.FeatureTableModel 1.0

/*!
  \qmltype ExplorerFeatureTableView
  \brief Provides a table view representing features from a model.

 */
TableView {
    property FeatureTableModel featureTableModel

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
