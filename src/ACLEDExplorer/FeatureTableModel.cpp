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

    }
}

int FeatureTableModel::rowCount(const QModelIndex &parent) const
{
    return 12;
}

int FeatureTableModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant FeatureTableModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        return QString("%1, %2").arg(index.column()).arg(index.row());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FeatureTableModel::roleNames() const
{
    return m_roleNames;
}

void FeatureTableModel::doneLoading(Error loadError)
{
    if (!loadError.isEmpty())
    {
        qCritical() << loadError.message();
        return;
    }


}
