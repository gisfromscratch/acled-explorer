// The ACLED explorer is a desktop map application visualizing occurred events worldwide.
// Copyright (C) 2021 Jan Tschada (gisfromscratch@live.de)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Additional permission under GNU GPL version 3 section 7
//
// If you modify this Program, or any covered work, by linking or combining
// it with ArcGIS Runtime for Qt (or a modified version of that library),
// containing parts covered by the terms of ArcGIS Runtime for Qt,
// the licensors of this Program grant you additional permission to convey the resulting work.
// See <https://developers.arcgis.com/qt/> for further information.
//
#ifndef ACLEDLAYERSOURCE_H
#define ACLEDLAYERSOURCE_H

#include <QObject>
#include <QUuid>

#include "Error.h"

namespace Esri
{
namespace ArcGISRuntime
{
class FeatureLayer;
class FeatureQueryResult;
class ServiceFeatureTable;
}
}

class FeatureTableModel;

class AcledLayerSource : public QObject
{
    Q_OBJECT
public:
    explicit AcledLayerSource(QObject *parent = nullptr);

    Esri::ArcGISRuntime::FeatureLayer* featureLayer() const;
    FeatureTableModel* featureTableModel() const;

signals:
    void layerLoaded();

private slots:
    void doneLoading(Esri::ArcGISRuntime::Error loadError);
    void featureTableSelectionChanged();
    void populateFromServiceCompleted(QUuid taskId, Esri::ArcGISRuntime::FeatureQueryResult* featureQueryResult);

private:
    Esri::ArcGISRuntime::FeatureLayer* m_acledFeatureLayer = nullptr;
    Esri::ArcGISRuntime::ServiceFeatureTable* m_acledFeatureTable = nullptr;
    FeatureTableModel* m_acledFeatureTableModel = nullptr;
    int m_acledFeatureOffset = 0;
};

#endif // ACLEDLAYERSOURCE_H
