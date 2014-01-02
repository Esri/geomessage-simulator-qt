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

#ifndef SAMPLEMAP_H
#define SAMPLEMAP_H

#include <QtCore>
#include <QMouseEvent>
#include <QColor>
#include <QtPlugin>
#include <QObject>
#include <QGridLayout>
#include <QImage>

#include "Exception.h"
#include "Map.h"

#include "Geometry.h"
#include "GraphicsLayer.h"
#include "Layer.h"
#include "Map.h"
#include "MapGraphicsView.h"
#include "MarkerSymbol.h"
#include "Message.h"
#include "MessageGroupLayer.h"
#include "MessageProcessor.h"
#include "SymbolDictionary.h"

#include "mapcompass.h"
#include "mapscalebar.h"

enum MapElementSize
{
    MES_Small,
    MES_Medium,
    MES_Large
};

enum MapElementDock
{
    MED_LowerLeft,
    MED_TopLeft,
    MED_TopRight,
    MED_LowerRight
};

struct MilitarySymbolObject
{
    int Id;
    QString SymbolCode;
    QList<QPointF> ControlPoints;
};

Q_DECLARE_METATYPE(QGridLayout*)

class SampleMap : public QWidget
{
    Q_OBJECT

signals:
    void mousePress(QMouseEvent mouseEvent);
    void mouseMove(QMouseEvent mouseEvent);
    void mouseRelease(QMouseEvent mouseEvent);
    void mouseDoubleClick(QMouseEvent mouseEvent);       

public slots:

    void handleMousePress(QMouseEvent mouseEvent);
    void handleMouseMove(QMouseEvent mouseEvent);
    void handleMouseRelease(QMouseEvent mouseEvent);
    void handleMouseDoubleClick(QMouseEvent mouseEvent);

    void onMapReady();

private slots:
    void updateTimer();
    void handleMapExtentChanged();

public:

    SampleMap(QWidget *parent);
    virtual ~SampleMap();

    void resizeEvent(QResizeEvent* event);

    // Panning
    virtual void PanUp();
    virtual void PanDown();
    virtual void PanLeft();
    virtual void PanRight();

    // Center On
    virtual void CenterOn(QPointF point);

    // Zooming

    // Zoom-factor based API
    virtual void Zoom(double zoomFactor);
    virtual void Zoom(double zoomFactor, QPointF centerOnPoint);

    // Rotation
    virtual void Rotate(double rotationAngle);

    // Scale
    virtual void SetScale(double scale);
    virtual double GetScale();

    // Extent
    virtual void SetExtent(const QRectF& envelope);
    virtual QRectF GetExtent();

    // Grid
    virtual void SetGridVisible(bool visible);
    virtual bool GetGridVisible();

    // HitTest
    virtual QList<int> HitTest(int screenX, int screenY, int tolerance);

    // Screen to Map
    virtual const EsriRuntimeQt::Point ScreenToMap(const QPoint& pixelPoint);

    // Map to Screen
    virtual const QPoint MapToScreenPoint(const EsriRuntimeQt::Point& mapPoint);

    // Show/Hide scale bar
    virtual void ShowScalebar(bool visible);
    virtual bool IsScalebarVisible();

    // Drawing
    virtual void AddMapObjects(const QList<MilitarySymbolObject>& milObjects);
    virtual void DeleteMapObjects(const QList<int>& graphicIDs);

    //Compass
    virtual void ShowMapCompass(bool visible, MapElementSize mapElementSize=MES_Large);
    virtual bool IsMapCompassVisible();

    //Inset Map
    virtual void ShowInsetMap(bool visible, MapElementDock mapElementDock=MED_LowerLeft, MapElementSize mapElementSize=MES_Small);
    virtual bool IsInsetMapVisible();

    // Reset Map to original state
    virtual void Reset();

    // Load an XML file with symbol messages
    virtual void LoadMessageFile();

    //Test Repro Case for testing / demonstrating problems
    virtual void TestReproCase();

private:

    void setUI();
    void panMap(QString direction);
    void changeWidgetDock(QWidget* widget,const MapElementDock& mapElementDock);

    QString controlPoints2QString(QList<QPointF> pointFList);

    EsriRuntimeQt::Message MilitaryObject2UpdateMessage(const MilitarySymbolObject& mo);
    bool isMilMultipoint(const MilitarySymbolObject& mo);

    QString getPathSampleData();
    void debugNotImplemented();

    EsriRuntimeQt::MapGraphicsView* m_pMapGraphicsView;
    EsriRuntimeQt::MapGraphicsView* m_pMapGraphicsViewInsetMap;

    EsriRuntimeQt::Map m_pMap;
    EsriRuntimeQt::Map m_pInsetMap;

    MapCompass*  m_pMapCompass;
    MapScalebar* m_pMapScalebar;

    EsriRuntimeQt::SymbolDictionary dictionary;
    EsriRuntimeQt::MessageProcessor messageProcessor;
    EsriRuntimeQt::MessageGroupLayer messagGroupLayer;

    EsriRuntimeQt::GraphicsLayer graphicsLayer;
    QMap<int, EsriRuntimeQt::Graphic> graphicsMap;

    EsriRuntimeQt::Envelope firstExtent;
    double firstScale;
};
#endif // SAMPLEMAP_H
