/*******************************************************************************
 * Copyright 2012-2013 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 ******************************************************************************/

#ifndef MAPSCALEBAR_H
#define MAPSCALEBAR_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsWidget>

#include "MapGraphicsView.h"

using namespace EsriRuntimeQt;

class MapScalebar : public QGraphicsWidget
{
    Q_OBJECT
public:
    MapScalebar(QGraphicsItem *parent = 0);

    void setGraphicsView(MapGraphicsView* pGraphicView);
    void updateScalebar();

signals:

public slots:

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:    
    int RoundToPlace(int);

    double maxTick;
    double pixelLength;
    double pixelMaxLength;

    MapGraphicsView* m_pMapGraphicsView;
};

#endif // MAPSCALEBAR_H
