/*
 | Copyright 2012-2013 Esri
 |
 | Licensed under the Apache License, Version 2.0 (the "License");
 | you may not use this file except in compliance with the License.
 | You may obtain a copy of the License at
 |
 |    http://www.apache.org/licenses/LICENSE-2.0
 |
 | Unless required by applicable law or agreed to in writing, software
 | distributed under the License is distributed on an "AS IS" BASIS,
 | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 | See the License for the specific language governing permissions and
 | limitations under the License.
 */

#include <QPainter>

#include "simplegraphicoverlay.h"

SimpleGraphicOverlay::SimpleGraphicOverlay(QGraphicsItem *parent) :
    QGraphicsWidget(parent), m_pMapGraphicsView(0)
{
    rotation = 0.0;
    screenX = -1;
    screenY = -1;
    visible = true;

    setAttribute(Qt::WA_NoBackground);
    setAutoFillBackground(false);
}

QRectF SimpleGraphicOverlay::boundingRect() const
{
  if (m_pMapGraphicsView)
    return QRect(0, 0, m_pMapGraphicsView->width(), m_pMapGraphicsView->height());

  return QRectF();
}

void SimpleGraphicOverlay::setImage(QImage imageIn)
{
    image = imageIn;
}

void SimpleGraphicOverlay::setAngle(double rotationIn)
{
    rotation = rotationIn;
}

void SimpleGraphicOverlay::setPosition(Point position)
{
    if (!m_pMapGraphicsView)
        return;

    Point screenPoint = m_pMapGraphicsView->map().toScreenPoint(position);

    screenX = (int)screenPoint.x();
    screenY = (int)screenPoint.y();

    // TODO: handle case when wraparound enabled and point returned is offscreen
    // qDebug() << "Draw Ownship @ Map Point(" << position.X() << ", " << position.Y() << ")";
}

void SimpleGraphicOverlay::setGraphicsView(MapGraphicsView* pGraphicView)
{
    m_pMapGraphicsView = pGraphicView;

    QGraphicsScene* scene = m_pMapGraphicsView->scene();
    scene->addItem(this);
}

void SimpleGraphicOverlay::setVisible(bool visibleIn)
{
    visible = visibleIn;
}

void SimpleGraphicOverlay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if ((!visible) || (!m_pMapGraphicsView) || image.isNull() || (screenX == -1) || (screenY == -1))
        return;

    // These are not needed because the image is always placed at the center of the display
    // however if needed anywhere else uncomment these lines and place the image accordingly:
    // double mapWidth  = m_pMap->width();
    // double mapHeight = m_pMap->height();
    // qDebug() << "Draw Ownship @ (" << screenX << ", " << screenY << ")";

    double mapRotation = m_pMapGraphicsView->map().rotation();

    QSize size = image.size();
    double halfImageWidth = size.width() / 2.0;
    double halfImageHeight = size.height() / 2.0;

    // move to the center of the image
    painter->translate(screenX, screenY);

    painter->rotate(rotation - mapRotation);

    painter->drawImage(QPoint(-halfImageWidth, -halfImageHeight), image);
}
