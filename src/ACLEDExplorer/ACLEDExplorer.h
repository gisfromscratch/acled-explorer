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

#ifndef ACLEDEXPLORER_H
#define ACLEDEXPLORER_H

#include <QQuickItem>

namespace Esri
{
namespace ArcGISRuntime
{
class Map;
class MapQuickView;
}
}

class AcledLayerSource;
class FeatureTableModel;

class ACLEDExplorer : public QQuickItem
{
    Q_OBJECT

public:
    ACLEDExplorer(QQuickItem* parent = nullptr);
    ~ACLEDExplorer();

    Q_PROPERTY(FeatureTableModel* featureTableModel READ featureTableModel NOTIFY featureTableModelChanged)

    FeatureTableModel* featureTableModel() const;

    void componentComplete() override;

signals:
    void featureTableModelChanged();

private slots:
    void featureTableSelectionChanged();
    void navigatingChanged();

private:
    Esri::ArcGISRuntime::Map *m_map = nullptr;
    Esri::ArcGISRuntime::MapQuickView *m_mapView = nullptr;
    AcledLayerSource *m_acledLayerSoure = nullptr;
    bool m_navigating = false;
};

#endif // ACLEDEXPLORER_H
