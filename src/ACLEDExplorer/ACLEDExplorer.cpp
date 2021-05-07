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

#include "Map.h"
#include "MapQuickView.h"
#include "Basemap.h"
#include "FeatureLayer.h"

#include "ACLEDExplorer.h"
#include "AcledLayerSource.h"

using namespace Esri::ArcGISRuntime;

ACLEDExplorer::ACLEDExplorer(QQuickItem* parent /* = nullptr */):
    QQuickItem(parent),
    m_acledLayerSoure(new AcledLayerSource(this))
{
}

ACLEDExplorer::~ACLEDExplorer()
{
}

void ACLEDExplorer::componentComplete()
{
    QQuickItem::componentComplete();

    // find QML MapView component
    m_mapView = findChild<MapQuickView*>("mapView");

    // Create a map using the openStreetMap Basemap
    m_map = new Map(Basemap::openStreetMap(this), this);

    // Set map to map view
    m_mapView->setMap(m_map);

    // Add the ACLED feature layer
    m_map->operationalLayers()->append(m_acledLayerSoure->featureLayer());
}
