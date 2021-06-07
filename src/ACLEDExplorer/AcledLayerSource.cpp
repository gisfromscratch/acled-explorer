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
#include "AcledLayerSource.h"
#include "FeatureTableModel.h"

#include "FeatureLayer.h"
#include "ServiceFeatureTable.h"

#include <limits>

using namespace Esri::ArcGISRuntime;

static const bool useManualCaching = true;

AcledLayerSource::AcledLayerSource(QObject *parent) : QObject(parent)
{
    m_acledFeatureTable = new ServiceFeatureTable(QUrl("https://services.arcgis.com/LG9Yn2oFqZi5PnO5/arcgis/rest/services/Armed_Conflict_Location_Event_Data_ACLED/FeatureServer/0"), this);
    connect(m_acledFeatureTable, &ServiceFeatureTable::doneLoading, this, &AcledLayerSource::doneLoading);
    if (useManualCaching)
    {
        m_acledFeatureTable->setFeatureRequestMode(FeatureRequestMode::ManualCache);
        QueryParameters queryAll;
        queryAll.setWhereClause("1=1");
        queryAll.setMaxFeatures(std::numeric_limits<int>().max());
        m_acledFeatureTable->populateFromService(queryAll, true, QStringList { "*" });
    }
    m_acledFeatureLayer = new FeatureLayer(m_acledFeatureTable, this);

    m_acledFeatureTableModel = new FeatureTableModel(m_acledFeatureTable, this);
    connect(m_acledFeatureTableModel, &FeatureTableModel::featureSelectionChanged, this, &AcledLayerSource::featureTableSelectionChanged);
}

FeatureLayer* AcledLayerSource::featureLayer() const
{
    return m_acledFeatureLayer;
}

FeatureTableModel* AcledLayerSource::featureTableModel() const
{
    return m_acledFeatureTableModel;
}

void AcledLayerSource::doneLoading(Esri::ArcGISRuntime::Error loadError)
{
    if (!loadError.isEmpty())
    {
        qCritical() << loadError.message();
        return;
    }

    qDebug() << "ACLED feature layer loaded";
    emit layerLoaded();
}

void AcledLayerSource::featureTableSelectionChanged()
{
    m_acledFeatureLayer->clearSelection();
    QList<Feature*> selectedFeatures = m_acledFeatureTableModel->selectedFeatures();
    if (!selectedFeatures.empty())
    {
        m_acledFeatureLayer->selectFeatures(selectedFeatures);
    }
}
