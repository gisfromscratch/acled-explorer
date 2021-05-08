import Esri.ACLEDExplorer 1.0

/*!
  \qmltype ExplorerMapView
  \brief Provides a map view visualizing the ACLED events.

 */
ACLEDExplorer {
    // Create MapQuickView here, and create its Map etc. in C++ code
    MapView {
        anchors.fill: parent
        objectName: "mapView"
        // set focus to enable keyboard navigation
        focus: true
    }
}
