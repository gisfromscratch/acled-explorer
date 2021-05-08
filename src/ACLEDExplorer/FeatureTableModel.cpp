// The ACLED explorer is a desktop map application visualizing occurred events worldwide.
// Copyright (C) 2021 Jan Tschada (gisfromscratch@live.de)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Additional permission under GNU LGPL version 3 section 4 and 5
// If you modify this Program, or any covered work, by linking or combining
// it with ArcGIS Runtime for Qt (or a modified version of that library),
// containing parts covered by the terms of ArcGIS Runtime for Qt,
// the licensors of this Program grant you additional permission to convey the resulting work.
// See <https://developers.arcgis.com/qt/> for further information.
//
#include "FeatureTableModel.h"

#include "FeatureTable.h"

using namespace Esri::ArcGISRuntime;

FeatureTableModel::FeatureTableModel(FeatureTable *featureTable, QObject *parent) : QAbstractTableModel(parent)
{
    m_roleNames.insert(Qt::DisplayRole, "display");

    m_featureTable = featureTable;
    if (LoadStatus::Loaded != m_featureTable->loadStatus())
    {
        // TODO: Load the feature table
    }

    connect(m_featureTable, &FeatureTable::queryFeaturesCompleted, this, &FeatureTableModel::queryFeaturesCompleted);
    queryAllFeatures();
}

int FeatureTableModel::rowCount(const QModelIndex &parent) const
{
    return m_featureCount;
}

int FeatureTableModel::columnCount(const QModelIndex &parent) const
{
    return m_attributeCount;
}

QVariant FeatureTableModel::data(const QModelIndex &index, int role) const
{
    if (m_featureCount <= index.row())
    {
        qWarning() << "Row index is invalid!";
        return QVariant();
    }
    if (m_attributeCount <= index.column())
    {
        qWarning() << "Column index is invalid!";
        return QVariant();
    }

    Feature *feature = m_features[index.row()];
    QString attributeName = feature->attributes()->attributeNames()[index.column()];
    switch (role)
    {
    case Qt::DisplayRole:
        return feature->attributes()->attributeValue(attributeName);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FeatureTableModel::roleNames() const
{
    return m_roleNames;
}

void FeatureTableModel::queryAllFeatures()
{
    QueryParameters queryAllFeaturesParameters;
    queryAllFeaturesParameters.setWhereClause("1=1");
    m_featureTable->queryFeatures(queryAllFeaturesParameters);
}

void FeatureTableModel::doneLoading(Error loadError)
{
    if (!loadError.isEmpty())
    {
        qCritical() << loadError.message();
        return;
    }
}

void FeatureTableModel::queryFeaturesCompleted(QUuid taskId, Esri::ArcGISRuntime::FeatureQueryResult *featureQueryResult)
{
    Q_UNUSED(taskId)

    if (nullptr == featureQueryResult)
    {
        return;
    }

    std::unique_ptr<FeatureQueryResult> featuresQueryResultPtr(featureQueryResult);
    FeatureIterator featureIterator = featuresQueryResultPtr->iterator();
    if (!featureIterator.hasNext())
    {
        qWarning() << "No features returned by this query!";
        return;
    }

    // Updated the whole model
    beginResetModel();

    // Delete old features
    if (!m_features.empty())
    {
        qDeleteAll(m_features);
    }

    // Update new features and counts
    m_features = featureIterator.features(this);
    m_featureCount = m_features.count();
    if (0 == m_featureCount)
    {
        m_attributeCount = 0;
    }
    else
    {
        m_attributeCount = m_features[0]->attributes()->attributeNames().count();
    }

    endResetModel();
}
