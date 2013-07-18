/*
 | Copyright 2012 Esri
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

#include "simplegraphicoverlay.h"

SimpleGraphicOverlay::SimpleGraphicOverlay(QGraphicsItem *parent) :
    QGraphicsWidget(parent)
{
    rotation = 0.0;
    screenX = -1;
    screenY = -1;
    m_pMap = 0;
    visible = true;

    setAttribute(Qt::WA_NoBackground);
    setAutoFillBackground(false);
}

QRectF SimpleGraphicOverlay::boundingRect() const
{
  if (m_pMap)
    return QRect(0, 0, m_pMap->width(), m_pMap->height());

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
    if (!m_pMap)
        return;

    Point screenPoint = m_pMap->toScreenPoint(position);

    screenX = (int)screenPoint.x();
    screenY = (int)screenPoint.y();

    // TODO: handle case when wraparound enabled and point returned is offscreen
    // qDebug() << "Draw Ownship @ Map Point(" << position.X() << ", " << position.Y() << ")";
}

void SimpleGraphicOverlay::setMap(Map* pMap)
{
    m_pMap = pMap;

    QGraphicsScene* scene = m_pMap->scene();
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

    if ((!visible) || (!m_pMap) || image.isNull() || (screenX == -1) || (screenY == -1))
        return;

    // These are not needed because the image is always placed at the center of the display
    // however if needed anywhere else uncomment these lines and place the image accordingly:
    // double mapWidth  = m_pMap->width();
    // double mapHeight = m_pMap->height();
    // qDebug() << "Draw Ownship @ (" << screenX << ", " << screenY << ")";

    double mapRotation = m_pMap->rotation();

    QSize size = image.size();
    double halfImageWidth = size.width() / 2.0;
    double halfImageHeight = size.height() / 2.0;

    // move to the center of the image
    painter->translate(screenX, screenY);

    painter->rotate(rotation - mapRotation);

    painter->drawImage(QPoint(-halfImageWidth, -halfImageHeight), image);
}
