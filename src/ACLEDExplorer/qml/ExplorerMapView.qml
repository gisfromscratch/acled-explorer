import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

import Esri.ACLEDExplorer 1.0
import Esri.FeatureTableModel 1.0

ACLEDExplorer {
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
