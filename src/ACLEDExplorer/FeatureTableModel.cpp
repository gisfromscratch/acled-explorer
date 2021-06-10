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

    // Emit the selection changed event
    emit featureSelectionChanged();
}

QList<Feature*> FeatureTableModel::selectedFeatures() const
{
    QList<Feature*> selectedFeatures;
    if (-1 != m_selectedFeatureIndex)
    {
        // Add the corresponding feature
        selectedFeatures.append(m_features[m_selectedFeatureIndex]);
    }
    return selectedFeatures;
}

void FeatureTableModel::setSpatialFilter(const Esri::ArcGISRuntime::Geometry& spatialFilter)
{
    m_spatialFilter = spatialFilter;

    // Query features using spatial filter
    queryFeaturesUsingSpatialFilter();
}

void FeatureTableModel::queryAllFeatures()
{
    QueryParameters queryAllFeaturesParameters;
    queryAllFeaturesParameters.setWhereClause("1=1");
    m_featureTable->queryFeatures(queryAllFeaturesParameters);
}

void FeatureTableModel::queryFeaturesUsingSpatialFilter()
{
    QueryParameters queryFeaturesParameters;
    queryFeaturesParameters.setGeometry(m_spatialFilter);
    m_featureTable->queryFeatures(queryFeaturesParameters);
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
        qWarning() << "Feature query result is invalid!";
        return;
    }

    // Updated the whole model
    beginResetModel();

    if (m_oidFieldName.isEmpty())
    {
        // Obtain the OID field name
        const QList<Field>& fields = m_featureTable->fields();
        for (const Field& field : fields)
        {
            switch (field.fieldType())
            {
                case FieldType::OID:
                    m_oidFieldName = field.name();
                    break;

                default:
                    break;
            }
        }
    }

    // Obtain the feature OID from the selected feature
    qint64 selectedOid = -1;
    if (-1 != m_selectedFeatureIndex && !m_oidFieldName.isEmpty())
    {
        AttributeListModel* attributes = m_features[m_selectedFeatureIndex]->attributes();
        if (attributes->containsAttribute(m_oidFieldName))
        {
            selectedOid = attributes->attributeValue(m_oidFieldName).toLongLong();
        }
    }

    // Delete old features
    if (!m_features.empty())
    {
        qDeleteAll(m_features);
        m_features.clear();
    }

    // Update new features and counts
    std::unique_ptr<FeatureQueryResult> featuresQueryResultPtr(featureQueryResult);
    FeatureIterator featureIterator = featuresQueryResultPtr->iterator();
    if (featureIterator.hasNext())
    {
        m_features = featureIterator.features(this);
        m_featureCount = m_features.count();
        if (0 == m_featureCount)
        {
            m_attributeNames.clear();
            m_selectedFeatureIndex = -1;
        }
        else
        {
            m_attributeNames = m_features[0]->attributes()->attributeNames();

            // Find the last selected feature
            m_selectedFeatureIndex = -1;
            qint64 featureIndex = 0;
            const QList<Feature*>& features = m_features;
            for (const Feature* feature : features)
            {
                qint64 oid = feature->attributes()->attributeValue(m_oidFieldName).toLongLong();
                if (selectedOid == oid)
                {
                    m_selectedFeatureIndex = featureIndex;
                    qDebug() << "OID found";
                    break;
                }
                featureIndex++;
            }
        }
    }
    else
    {
        m_attributeNames.clear();
        m_selectedFeatureIndex = -1;
    }

    m_attributeCount = m_attributeNames.count();

    endResetModel();
}
