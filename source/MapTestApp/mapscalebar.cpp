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

#include "mapscalebar.h"
#include <QtGui>

MapScalebar::MapScalebar(QGraphicsItem *parent) : QGraphicsWidget(parent), m_pMapGraphicsView(0)
{
    // set defaults
    maxTick = 500000.0; //meters
    pixelLength = 100.0;
    pixelMaxLength = 250;

    setAttribute(Qt::WA_NoBackground);
    // setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    setAutoFillBackground(false);
}

void MapScalebar::setGraphicsView(EsriRuntimeQt::MapGraphicsView* pGraphicView)
{
    m_pMapGraphicsView = pGraphicView;

    QGraphicsScene* scene = m_pMapGraphicsView->scene();
    scene->addItem(this);
}

void MapScalebar::updateScalebar()
{
    update();
}

void MapScalebar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (!m_pMapGraphicsView || !m_pMapGraphicsView->map().isReady())
        return;

    if (m_pMapGraphicsView->map().layerCount() == 0)
      return;

    double mapResolution = m_pMapGraphicsView->map().resolution();
    if (mapResolution <= 0.0)
      return;

    // skip if too small to draw
    if (scene()->height() < 100)
      return;

    // calculate maxTick
    double temp = mapResolution * pixelMaxLength;
    maxTick = RoundToPlace(temp);

    // calculate pixel length of graphic
    pixelLength = maxTick / mapResolution;

    //
    // qDebug()<< "maxTick = " << maxTick;
    // qDebug()<< "ESRIRuntimeQT::Unit = " << m_pMapGraphicsView->map().layer(0).spatialReference().unit().displayName();

    QLinearGradient linearGradient(QPointF(40, 40), QPointF(80, 80));
        linearGradient.setColorAt(0.1, Qt::red);
        linearGradient.setColorAt(0.5, Qt::magenta);
        linearGradient.setColorAt(0.9, Qt::blue);

    painter->setBrush(*(new QBrush(linearGradient)));

    const int OFFSET_FROM_BOTTOM = 20;
    setPos((scene()->width()/2) - (pixelLength/2), scene()->height() - OFFSET_FROM_BOTTOM);

    painter->drawRect(0, 0, pixelLength, 10);
    painter->drawText(pixelLength+5, 0, 200, 14, Qt::AlignLeft|Qt::AlignVCenter, QString().setNum(maxTick/1000) + " km");
}

int MapScalebar::RoundToPlace(int number)
{
    int places = QString().setNum(number).length() - 1;

    int i = 1;

    if(places <= 0)
        return number;

    while(places > 0)
    {
        i *= 10;
        places--;
    }

    return number - (number % i);
}
