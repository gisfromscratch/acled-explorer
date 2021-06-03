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
#include "ACLEDExplorer.h"
#include "AcledLayerSource.h"
#include "FeatureTableModel.h"

#include "Map.h"
#include "MapQuickView.h"
#include "Basemap.h"
#include "FeatureLayer.h"
#include "GeometryEngine.h"

using namespace Esri::ArcGISRuntime;

ACLEDExplorer::ACLEDExplorer(QQuickItem* parent /* = nullptr */):
    QQuickItem(parent),
    m_acledLayerSoure(new AcledLayerSource(this))
{
    connect(m_acledLayerSoure, &AcledLayerSource::layerLoaded, this, &ACLEDExplorer::featureTableModelChanged);
    connect(m_acledLayerSoure->featureTableModel(), &FeatureTableModel::featureSelectionChanged, this, &ACLEDExplorer::featureTableSelectionChanged);
}

ACLEDExplorer::~ACLEDExplorer()
{
}

FeatureTableModel* ACLEDExplorer::featureTableModel() const
{
    return m_acledLayerSoure->featureTableModel();
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

void ACLEDExplorer::featureTableSelectionChanged()
{
    const QList<Feature*> selectedFeatures = m_acledLayerSoure->featureTableModel()->selectedFeatures();
    if (!selectedFeatures.empty())
    {
        QList<Geometry> centers;
        for (Feature const* selectedFeature : selectedFeatures)
        {
            Geometry geometry = selectedFeature->geometry();
            if (!geometry.isEmpty())
            {
                Point center = geometry.extent().center();
                centers.append(center);
            }
        }

        if (!centers.empty())
        {
            Geometry unionOfCenters = GeometryEngine::unionOf(centers);
            if (!unionOfCenters.isEmpty())
            {
                Point center = unionOfCenters.extent().center();
                m_mapView->setViewpointCenter(center);
            }
        }
    }
}
