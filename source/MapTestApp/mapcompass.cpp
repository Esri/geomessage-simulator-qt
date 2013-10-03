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

#include "mapcompass.h"

static const QString COMPASS_FILE = ":/Resources/icons/North-Arrow.png";

MapCompass::MapCompass(QGraphicsItem *parent) :
  QGraphicsWidget(parent), m_pMapGraphicsView(0)
{
    QString fileName = QString(COMPASS_FILE);
    QImage loadedImage;

    if(!loadedImage.load(fileName))
    {
        qDebug("Failed to load MapCompass image.");
        return;
    }

    scaleValue = 1.0;

    image = loadedImage;
    setAttribute(Qt::WA_NoBackground);

    setAutoFillBackground(false);
}

void MapCompass::setGraphicsView(MapGraphicsView* pGraphicView)
{
    m_pMapGraphicsView = pGraphicView;

    QGraphicsScene* scene = m_pMapGraphicsView->scene();
    scene->addItem(this);
}

void MapCompass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  if (!m_pMapGraphicsView || !m_pMapGraphicsView->map().isReady())
      return;

    Q_UNUSED(option)
    Q_UNUSED(widget)

    double mapWidth  = m_pMapGraphicsView->map().width();
    double mapHeight = m_pMapGraphicsView->map().height();

    QSize size = image.size();
    double halfImageWidth = size.width() / 2.0;
    double halfImageHeight = size.height() / 2.0;

    // IMPORTANT: code below assumes it will be docked on bottom right of the map

    // if scaled, need to factor in below
    //    painter->scale(scaleValue,scaleValue);

    // move to the center of the image
    painter->translate(mapWidth - halfImageWidth, mapHeight - halfImageWidth);

    painter->rotate(rotateValue);

    painter->drawImage(QPoint(-halfImageWidth, -halfImageHeight), image);
}

QSize MapCompass::sizeHint() const
{
    return image.size();
}

void MapCompass::UpdateRotation(double value)
{
    rotateValue = 360.0 - value;
    update();

    // qDebug() << "MapCompass Rotate to: " << rotateValue;
}

void MapCompass::setScale(double scale)
{
    scaleValue = scale;
}
