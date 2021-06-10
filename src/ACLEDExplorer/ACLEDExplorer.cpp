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
    m_acledLayerSoure(new AcledLayerSource(this)),
    m_navigatingTimer(new QTimer(this))
{
    m_navigatingTimer->setSingleShot(true);

    connect(m_acledLayerSoure, &AcledLayerSource::layerLoaded, this, &ACLEDExplorer::featureTableModelChanged);
    connect(m_acledLayerSoure->featureTableModel(), &FeatureTableModel::featureSelectionChanged, this, &ACLEDExplorer::featureTableSelectionChanged);
    connect(m_navigatingTimer, &QTimer::timeout, this, &ACLEDExplorer::navigatingFinished);
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

    // Register navigate slots
    connect(m_mapView, &MapQuickView::navigatingChanged, this, &ACLEDExplorer::navigatingChanged);
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

void ACLEDExplorer::navigatingChanged()
{
    m_navigating = !m_navigating;
    if (m_navigating)
    {
        // Start navigating
        //m_navigatingTimer->stop();
    }
    else
    {
        // Finished navigating and wait for no changes
        //m_navigatingTimer->start(300);

        // TODO: Filters the feature table like crazy!
        //navigatingFinished();
    }
}

void ACLEDExplorer::navigatingFinished()
{
    // Finished navigating
    Viewpoint currentViewpoint = m_mapView->currentViewpoint(ViewpointType::BoundingGeometry);
    Geometry envelope = currentViewpoint.targetGeometry();
    FeatureTableModel* featureTableModel = m_acledLayerSoure->featureTableModel();
    featureTableModel->setSpatialFilter(envelope);
    //qDebug() << "Spatial filter updated";
}
