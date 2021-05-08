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
    m_roleNames.insert(Qt::UserRole + 1, "featureState");

    m_featureTable = featureTable;
    if (LoadStatus::Loaded != m_featureTable->loadStatus())
    {
        // TODO: Load the feature table
    }

    connect(m_featureTable, &FeatureTable::queryFeaturesCompleted, this, &FeatureTableModel::queryFeaturesCompleted);
    queryAllFeatures();
}

QVariant FeatureTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // Attribute names for the column header
    if (Qt::Orientation::Horizontal == orientation)
    {
        if (m_attributeCount <= section)
        {
            qWarning() << "Section index is invalid!";
            return QVariant();
        }

        return m_attributeNames[section];
    }

    // OBJECTIDs for the row header
    if (Qt::Orientation::Vertical == orientation)
    {
        if (m_featureCount <= section)
        {
            qWarning() << "Section index is invalid!";
            return QVariant();
        }

        return section;
    }

    return QVariant();
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
    case Qt::UserRole + 1:
        return (index.row() == m_selectedFeatureIndex) ? QString("selected") : QString("");
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FeatureTableModel::roleNames() const
{
    return m_roleNames;
}

void FeatureTableModel::setSelectedFeature(qint64 selectedFeatureIndex)
{
    qint64 lastSelectedFeatureIndex = m_selectedFeatureIndex;
    m_selectedFeatureIndex = selectedFeatureIndex;
    if (-1 != lastSelectedFeatureIndex)
    {
        // Update all cells from the last selected feature
        emit dataChanged(index(lastSelectedFeatureIndex, 0), index(lastSelectedFeatureIndex, m_attributeCount - 1));
    }
    if (-1 != m_selectedFeatureIndex)
    {
        // Update all cells from the new selected feature
        emit dataChanged(index(m_selectedFeatureIndex, 0), index(m_selectedFeatureIndex, m_attributeCount - 1));
    }
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
        m_attributeNames.clear();
    }
    else
    {
        m_attributeNames = m_features[0]->attributes()->attributeNames();
    }
    m_attributeCount = m_attributeNames.count();
    m_selectedFeatureIndex = -1;

    endResetModel();
}
