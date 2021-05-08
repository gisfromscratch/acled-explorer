import QtQuick 2.12
import QtQuick.Controls 2.4

import Esri.FeatureTableModel 1.0

/*!
  \qmltype ExplorerFeatureTableView
  \brief Provides a table view representing features from a model.

 */
TableView {
    id: tableView
    property FeatureTableModel featureTableModel
    property var columnWidths: []

    function getColumnWidth(column) {
        var defaultColumnWidth = 200;
        while (columnWidths.length <= column ) {
            columnWidths.push(defaultColumnWidth);
        }

        return columnWidths[column];
    }

    // Updates the new column width if necessary
    function updateColumnWidth(column, width) {
        var defaultColumnMargin = 50;
        var columnMinWidth = Math.floor(width + defaultColumnMargin);
        var columnWidth = getColumnWidth(column);
        if (columnWidth < columnMinWidth) {
            columnWidths[column] = columnMinWidth;
            //console.log(column + "," + columnMinWidth);
            //tableView.forceLayout();
        }
    }

    columnWidthProvider: function (column) {
        return getColumnWidth(column);
    }

    rowHeightProvider: function (column) {
        return 60;
    }

    /*
    leftMargin: rowsHeader.implicitWidth
    topMargin: columnsHeader.implicitHeight
    */

    columnSpacing: 1
    rowSpacing: 1

    model: featureTableModel

    ScrollBar.horizontal: ScrollBar{}
    ScrollBar.vertical: ScrollBar{}
    clip: true
    delegate: Label {
        //width: 60
        //height: 35
        text: display
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        background: Rectangle {
            id: backgroundRectangle
            color: "#434a39"

            state: featureState

            states: [
                State {
                    name: "selected"
                    PropertyChanges { target: backgroundRectangle; color: "#616847" }
                }
            ]

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    featureTableModel.setSelectedFeature(model.row);
                    /*
                    for (var propName in model) {
                        console.log(propName + ":" + model[propName]);
                    }
                    */

                    //backgroundRectangle.state == 'selected' ? backgroundRectangle.state = "" : backgroundRectangle.state = 'selected';
                    //console.log(model.row + "," + model.column);
                }
            }
        }

        Component.onCompleted: {
            updateColumnWidth(model.column, this.implicitWidth);
        }
    }

    /*
    Rectangle { // mask the headers
        z: 3
        color: "#222222"
        y: tableView.contentY
        x: tableView.contentX
        width: tableView.leftMargin
        height: tableView.topMargin
    }
    */

    /*
    Row {
        id: columnsHeader
        y: tableView.contentY
        z: 2
        Repeater {
            model: tableView.columns > 0 ? tableView.columns : 0

            Label {
                width: tableView.columnWidthProvider(modelData)
                height: 35
                text: featureTableModel.headerData(modelData, Qt.Horizontal)
                verticalAlignment: Text.AlignVCenter

                background: Rectangle { color: "#434a39" }
            }
        }
    }
    */

    /*
    Column {
        id: rowsHeader
        x: tableView.contentX
        z: 2
        Repeater {
            model: tableView.rows > 0 ? tableView.rows : 0
            Label {
                width: 60
                height: tableView.rowHeightProvider(modelData)
                text: featureTableModel.headerData(modelData, Qt.Vertical)
                verticalAlignment: Text.AlignVCenter

                background: Rectangle { color: "#434a39" }
            }
        }
    }
    */

    ScrollIndicator.horizontal: ScrollIndicator { }
    ScrollIndicator.vertical: ScrollIndicator { }
}
