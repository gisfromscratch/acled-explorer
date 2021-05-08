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
#ifndef FEATURETABLEMODEL_H
#define FEATURETABLEMODEL_H

#include<QAbstractTableModel>
#include <QObject>
#include <QUuid>

#include "Error.h"

namespace Esri
{
namespace ArcGISRuntime
{
class Feature;
class FeatureQueryResult;
class FeatureTable;
}
}

class FeatureTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit FeatureTableModel(Esri::ArcGISRuntime::FeatureTable *featureTable, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:

private slots:
    void doneLoading(Esri::ArcGISRuntime::Error loadError);
    void queryFeaturesCompleted(QUuid taskId, Esri::ArcGISRuntime::FeatureQueryResult *featureQueryResult);

private:
    void queryAllFeatures();

    QHash<int, QByteArray> m_roleNames;
    Esri::ArcGISRuntime::FeatureTable *m_featureTable = nullptr;
    QList<Esri::ArcGISRuntime::Feature*> m_features;
    int m_attributeCount = 0;
    qint64 m_featureCount = 0;
};

#endif // FEATURETABLEMODEL_H
