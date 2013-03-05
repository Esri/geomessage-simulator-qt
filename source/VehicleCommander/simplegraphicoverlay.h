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

#ifndef SIMPLEGRAPHICOVERLAY_H
#define SIMPLEGRAPHICOVERLAY_H

#include <QGraphicsWidget>

#include "Map.h"
#include "Geometry.h"

using namespace EsriRuntimeQt;

class SimpleGraphicOverlay : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit SimpleGraphicOverlay(QGraphicsItem *parent = 0);
    
    void setImage(QImage imageIn);
    void setAngle(double rotationIn);
    void setPosition(Point point);
    void setMap(Map* pMap);
    void setVisible(bool visibleIn);

    QRectF boundingRect() const;

signals:
    
public slots:
    
protected :

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QImage image;
    double rotation;
    int screenX, screenY;
    bool visible;
    Map* m_pMap;

};

#endif // SIMPLEGRAPHICOVERLAY_H
