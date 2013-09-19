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

#include "samplemap.h"

#include "Point.h"
#include "ArcGISRuntime.h"
#include "ArcGISLocalTiledLayer.h"
#include "SpatialReference.h"
#include "MultiLayerSymbol.h"

#include <QGraphicsScene>
#include <QMessageBox>

static const QString PROP_MANAGING_LAYOUT = "managingLayout";
static const double INSETMAP_SCALE_FACTOR = 25.0;

SampleMap::SampleMap(QWidget *parent) :
    m_pMapCompass(0),
    m_pMapScalebar(0),
    QWidget(parent)
{
    try
    {
        // setup UI
        setUI();
    }
    catch (Exception ex)
    {
        this->showMaximized();
        this->setWindowTitle("MapTestApp");
        qDebug()<<"Exception: "<<QString(ex.what());
        return;
    }

    // use a grid layout
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setMargin(0);

    // set the stretch parameters, higher stretch number takes more space
    // set column stretch
    gridLayout->setColumnStretch(0,5);
    gridLayout->setColumnStretch(1,90);
    gridLayout->setColumnStretch(2,5);
    // set row stretch
    gridLayout->setRowStretch(0,5);
    gridLayout->setRowStretch(1,90);
    gridLayout->setRowStretch(2,5);

    // set the cell minimum size
    // lower left
    gridLayout->setColumnMinimumWidth(0, 200);
    gridLayout->setRowMinimumHeight(2, 100);

    // add widgets
    gridLayout->addWidget(m_pMapGraphicsView, 0, 0, -1, -1);
    m_pInsetMap.setProperty(PROP_MANAGING_LAYOUT.toStdString().c_str(), QVariant::fromValue(gridLayout));

    gridLayout->addWidget(m_pMapGraphicsViewInsetMap, 2, 0, 1, 1);

    // north arrow
    m_pMapCompass = new MapCompass();
    m_pMapCompass->setGraphicsView(m_pMapGraphicsView);

    // scale bar
    m_pMapScalebar = new MapScalebar();
    m_pMapScalebar->setGraphicsView(m_pMapGraphicsView);

    // set the layout
    this->setLayout(gridLayout);

    // hook up events
    if(m_pMap.isInitialized())
    {
        connect(&m_pMap, SIGNAL(mousePress(QMouseEvent)), this, SLOT(handleMousePress(QMouseEvent)));
        connect(&m_pMap, SIGNAL(mouseRelease(QMouseEvent)), this, SLOT(handleMouseRelease(QMouseEvent)));
        connect(&m_pMap, SIGNAL(mouseMove(QMouseEvent)), this, SLOT(handleMouseMove(QMouseEvent)));
        connect(&m_pMap, SIGNAL(mouseDoubleClick(QMouseEvent)), this, SLOT(handleMouseDoubleClick(QMouseEvent)));

        // OLD: connect(&m_pMap, SIGNAL(scaleChanged(double)),this, SLOT(handleMapScaleChanged(double)));
        connect(&m_pMap, SIGNAL(extentChanged()), this, SLOT(handleMapExtentChanged()));

        connect(&m_pMap, SIGNAL(mapReady()), this, SLOT(onMapReady()));

        m_pMap.grid().setType(GridType::Mgrs);
        m_pMap.grid().setVisible(false);

        messagGroupLayer = MessageGroupLayer(SymbolDictionaryType::Mil2525C);
        m_pMap.addLayer(messagGroupLayer);

        messageProcessor = messagGroupLayer.messageProcessor();

        dictionary = messageProcessor.symbolDictionary();

        QList<SymbolProperties> symbolPropList = dictionary.findSymbols();
        int symbolCount = symbolPropList.count();
        bool dictionaryWorks = (symbolCount > 0);

        if (!dictionaryWorks)
            QMessageBox::warning(m_pMapGraphicsView, "Failure", "Dictionary Did not initialize", "Advanced/Military Symbology will not work");
    }            

}

SampleMap::~SampleMap()
{

  if (m_pMapCompass)
    delete m_pMapCompass;

  if (m_pMapScalebar)
    delete m_pMapScalebar;
}

void SampleMap::onMapReady()
{
    firstExtent = m_pMap.extent();
    firstScale = m_pMap.scale();

    SpatialReference sr = m_pMap.spatialReference();

    qDebug() << "MapReady, Spatial Reference = " << sr.id();
}

void SampleMap::handleMousePress(QMouseEvent mouseEvent)
{
    qDebug("Mouse Press Event");
    emit mousePress(mouseEvent);
}

void SampleMap::handleMouseMove(QMouseEvent mouseEvent)
{
    qDebug("Mouse Move Event");
    emit mouseMove(mouseEvent);
}

void SampleMap::handleMouseRelease(QMouseEvent mouseEvent)
{
    qDebug("Mouse Release Event");
    emit mouseRelease(mouseEvent);
}

void SampleMap::handleMouseDoubleClick(QMouseEvent mouseEvent)
{
    qDebug("Mouse Double Click");

    const int pixelTolerance = 3;
    HitTest(mouseEvent.pos().x(), mouseEvent.pos().y(), pixelTolerance);

    emit mouseDoubleClick(mouseEvent);
}

void SampleMap::updateTimer()
{
    if (m_pMap.isReady() && m_pMapCompass)
    {
        m_pMapCompass->UpdateRotation(m_pMap.rotation());
    }
}

void SampleMap::handleMapExtentChanged()
{
    qDebug()<< "MapExtentChanged()";

    if(!m_pInsetMap.isInitialized() || !m_pMap.isInitialized() || !m_pInsetMap.isReady() || !m_pMap.isReady())
        return;

    Envelope extent = m_pMap.extent();

    SpatialReference srMap = m_pInsetMap.spatialReference();
    SpatialReference srInsetMap = m_pInsetMap.spatialReference();

    int wkidMap = srMap.id();
    int wkidInsetMap = srInsetMap.id();

    if (wkidMap != wkidInsetMap)
    {
        qDebug() << "TODO: Project to InsetMap SR";
    }

    double scale = m_pMap.scale() * INSETMAP_SCALE_FACTOR;

    // IMPORTANT: below assumes that map and inset must be same Spatial Reference
    // If not, you will need to adjust the Scale Factor

    // update scalebar
    m_pMapScalebar->updateScalebar();

    qDebug()<< "Setting mapscale on inset map to " << scale*INSETMAP_SCALE_FACTOR << " from " << scale;
    m_pInsetMap.setScale(scale);
    m_pInsetMap.panTo(extent);
}

void SampleMap::panMap(QString direction)
{
    qDebug() << "panMap, direction=" << direction;

    Envelope extent = m_pMap.extent();

    double width = extent.width();
    double height = extent.height();

    double centerX = extent.centerX();
    double centerY = extent.centerY();

    const double PAN_INCREMENT = 0.25;

    if (direction.compare("Up") == 0)
        centerY += height * PAN_INCREMENT;
    else if (direction.compare("Down") == 0)
        centerY -= height * PAN_INCREMENT;
    else if (direction.compare("Left") == 0)
        centerX -= width * PAN_INCREMENT;
    else if (direction.compare("Right") == 0)
        centerX += width * PAN_INCREMENT;

    Envelope newExtent(Point(centerX, centerY), width, height);

    m_pMap.panTo(newExtent);
}

// Panning
void SampleMap::PanUp()
{
    if(!m_pMap.isInitialized())
        return;

    panMap("Up");
}

void SampleMap::PanDown()
{
    if(!m_pMap.isInitialized())
        return;

    panMap("Down");
}

void SampleMap::PanLeft()
{
    if(!m_pMap.isInitialized())
        return;

    panMap("Left");
}

void SampleMap::PanRight()
{
    if(!m_pMap.isInitialized())
        return;

    panMap("Right");
}

// Center On
void SampleMap::CenterOn(QPointF point)
{
    if(!m_pMap.isInitialized())
        return;

    Point mapPoint(point.x(), point.y());
    m_pMap.panTo(mapPoint);
}

// Zooming

// Zoom-factor based API
void SampleMap::Zoom(double zoomFactor)
{
    if(!m_pMap.isInitialized())
        return;

    m_pMap.zoom(zoomFactor);
}

void SampleMap::Zoom(double zoomFactor, QPointF centerOnPoint)
{
    if(!m_pMap.isInitialized())
        return;

    Point mapCenterOnPoint(centerOnPoint.x(), centerOnPoint.y());
    m_pMap.zoomToResolution(zoomFactor, mapCenterOnPoint);
}

// Rotation
void SampleMap::Rotate(double rotationAngle)
{
    if(!m_pMap.isInitialized())
        return;

    m_pMap.setRotation(rotationAngle);
}

// Scale
void SampleMap::SetScale(double scale)
{
    if(!m_pMap.isInitialized())
        return;

    m_pMap.setScale(scale);
}

double SampleMap::GetScale()
{
    if(!m_pMap.isInitialized())
        return 0.0;

    return m_pMap.scale();
}

// Extent
void SampleMap::SetExtent(const QRectF& envelope)
{
    if(!m_pMap.isInitialized())
        return;

    envelope.center().x(), envelope.center().y();


//    m_pMap.setFullExtent(QPolygonF(envelope));
}

QRectF SampleMap::GetExtent()
{
    if(!m_pMap.isInitialized())
        return QRectF();

    Envelope extent = m_pMap.extent();

    return QRectF(extent.xMin(), extent.yMin(), extent.width(), extent.height());
}

// Grid
void SampleMap::SetGridVisible(bool visible)
{
    if(!m_pMap.isInitialized())
        return;

    m_pMap.grid().setVisible(visible);
}

bool SampleMap::GetGridVisible()
{
    if(!m_pMap.isInitialized())
        return false;

    return m_pMap.grid().visible();
}

// Screen to Map
const Point SampleMap::ScreenToMap(const QPoint& pixelPoint)
{
    if(!m_pMap.isInitialized())
        return QPointF();

    Point mapPoint = m_pMap.toMapPoint(pixelPoint.x(), pixelPoint.y());

    return mapPoint;
}

// Map to Screen
const QPoint SampleMap::MapToScreenPoint(const Point& mapPoint)
{
    if(!m_pMap.isInitialized())
        return QPoint();

    Point screenPoint = m_pMap.toScreenPoint(mapPoint);

    QPointF* pWindowPoint = new QPointF(screenPoint.x(), screenPoint.y());

    return pWindowPoint->toPoint();
}

// Show/Hide scale bar
void SampleMap::ShowScalebar(bool visible)
{
    if(!m_pMap.isInitialized() || !m_pMapScalebar)
        return;

    m_pMapScalebar->setVisible(visible);
}

bool SampleMap::IsScalebarVisible()
{
  if(!m_pMapScalebar)
    return false;
  else
    return m_pMapScalebar->isVisible();
}

// Drawing
void SampleMap::AddMapObjects(const QList<MilitarySymbolObject>& milObjects)
{
    if(!m_pMap.isInitialized())
        return;

    foreach(MilitarySymbolObject mo, milObjects)
    {
        if (mo.SymbolCode.length() < 15)
        {
            qWarning() << "Bad Symbol Id for " << QString::number(mo.Id);
            continue;
        }

        Message message = MilitaryObject2UpdateMessage(mo);

        qDebug()<<"Message Add: ID = " << message.id() << ", x=" << mo.ControlPoints[0].x() << ", y=" << mo.ControlPoints[0].y();

        messageProcessor.processMessage(message);

        Graphic graphic = messageProcessor.graphic(message.id());

        // TODO: we probably don't need this graphicsMap any longer (stored in MessageProcessor now)
        // add to ID graphic map
        graphicsMap.insert(mo.Id, graphic);
    }
}

void SampleMap::DeleteMapObjects(const QList<int>& graphicIDs)
{
    if(!m_pMap.isInitialized())
        return;

    foreach(int graphicID, graphicIDs)
    {
      if (graphicsMap.contains(graphicID))
      {
          QString sGraphicId = QString::number(graphicID);

          QVariantMap properties;
          properties["_type"] = "position_report";
          properties["_action"] = "remove";
          properties["_id"] = QVariant(sGraphicId);

          Message message;
          message.setProperties(properties);

          qDebug() << "Removing Graphic with ID:" << message.id() << " / " << sGraphicId; // should be the same

          messageProcessor.processMessage(message);
      }
    }
}

//Compass
void SampleMap::ShowMapCompass(bool visible, MapElementSize mapElementSize)
{
    if (m_pMap.isReady() && m_pMapCompass)
    {
        // update the scale/size
        switch(mapElementSize)
        {
        case MES_Small:
            m_pMapCompass->setScale(0.5);
            break;
        case MES_Medium:
            m_pMapCompass->setScale(0.75);
            break;
        case MES_Large:
            m_pMapCompass->setScale(1.0);
            break;
        default:
            m_pMapCompass->setScale(1.0);
            break;
        }

        // set visible
        m_pMapCompass->setVisible(visible);
    }
}

bool SampleMap::IsMapCompassVisible()
{
    if(!m_pMapCompass)
        return false;

    return m_pMapCompass->isVisible();
}

//Inset Map
void SampleMap::ShowInsetMap(bool visible, MapElementDock mapElementDock, MapElementSize mapElementSize)
{
    if(m_pMapGraphicsViewInsetMap)
    {
        // update location if needed
        changeWidgetDock(m_pMapGraphicsViewInsetMap, mapElementDock);

        // update size if needed
        switch(mapElementSize)
        {
        case MES_Small:
            m_pMapGraphicsViewInsetMap->setFixedSize(75,75);
            break;
        case MES_Medium:
            m_pMapGraphicsViewInsetMap->setFixedSize(125,125);
            break;
        case MES_Large:
            m_pMapGraphicsViewInsetMap->setFixedSize(200,200);
            break;
        default:
            break;
        }

        // update visibility
        if(visible)
        {
            m_pMapGraphicsViewInsetMap->show();
        }
        else
        {
            m_pMapGraphicsViewInsetMap->hide();
        }
    }
}

bool SampleMap::IsInsetMapVisible()
{
    if(!m_pInsetMap.isInitialized())
        return false;

    return m_pMapGraphicsViewInsetMap->isVisible();
}

QString SampleMap::getPathSampleData()
{
  QString path = ArcGISRuntime::installDirectory();
  path.append("/sdk/samples/data");
  QDir dataDir(path); // using QDir to convert to correct file separator
  QString pathSampleData = dataDir.path() + QDir::separator();

  return pathSampleData;
}

void SampleMap::setUI()
{
    qDebug() << "ArcMapImpl::setUI";

    // set to openGL rendering (or won't work on Windows)
    EsriRuntimeQt::ArcGISRuntime::setRenderEngine(EsriRuntimeQt::RenderEngine::OpenGL);

    m_pMapGraphicsView =
        EsriRuntimeQt::MapGraphicsView::create(m_pMap, this);

    m_pMap.setWrapAroundEnabled(true);

// TODO: figure out where this property went
//    m_pMap.setShowingEsriLogo(false);

    m_pMapGraphicsViewInsetMap =
        EsriRuntimeQt::MapGraphicsView::create(m_pInsetMap, this);

// TODO: figure out where this property went
//    m_pInsetMap.setShowingEsriLogo(false);

    // set this so inset will do finer grained zooming
    m_pInsetMap.setZoomSnapEnabled(false);

    if ((!m_pMap.isInitialized()) || (!m_pInsetMap.isInitialized()))
    {
      qCritical() << "Unable to create map.";
      return;
    }

    QString dataPathTpk = getPathSampleData() + "tpks" + QDir::separator();

    // TODO: put your own high res dataset path here if desired
    QString highResTpk = dataPathTpk + "Imagery";

    // Low-res because this will be in the inset map
    QString lowResTpk = dataPathTpk + "Topographic.tpk";

    QString mainMapTpk = highResTpk;

    // try high-res 1st and if it doesn't work revert back to SDK/low res one
    if (!(QFile(highResTpk).exists() || QDir(highResTpk).exists()))
        mainMapTpk = lowResTpk;

    ArcGISLocalTiledLayer tiledLayer(mainMapTpk);
    m_pMap.addLayer(tiledLayer);

    ArcGISLocalTiledLayer overviewMapLayer(lowResTpk);
    m_pInsetMap.addLayer(overviewMapLayer);
    m_pInsetMap.setWrapAroundEnabled(true);

    // set inset map attributes
    if(m_pInsetMap.isInitialized())
    {
      // don't allow inset map to recieve mouse input
      m_pMapGraphicsViewInsetMap->setDisabled(true);
    }

    // hook up north arrow
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    timer->start(1000 / 30);
}

void SampleMap::debugNotImplemented()
{
    qDebug("Not Implemented.");
}

QString SampleMap::controlPoints2QString(QList<QPointF> pointFList)
{
    QString temp;

    // build string of coordinates
    foreach(QPointF pointF, pointFList)
    {
        QVariant vX(pointF.x());
        temp.append(vX.toString());
        temp.append(',');
        QVariant vY(pointF.y());
        temp.append(vY.toString());
        temp.append(';');
    }

    // remove last semicolon
    temp.remove(temp.size()-1,1);

    return temp;
}

// HitTest
QList<int> SampleMap::HitTest(int screenX, int screenY, int tolerance)
{
    QList<int> list;

    if(!m_pMap.isInitialized())
        return list;

    QList<Layer> layers = messagGroupLayer.getLayers();

    foreach (Layer layer,  layers)
    {
        GraphicsLayer glayer(layer);
        if (!glayer.isNull())
        {
            // There is a know bug where this doesn't work
            // when 2 maps are visible (in this case the inset map)
            list = glayer.graphicIDs(screenX, screenY, tolerance);
            if (list.count() > 0)
            {
                qDebug() << "Hit test has " << list.count() << " hits.";
                foreach (int Id, list)
                {
                    qDebug() << "Hit test: " << Id;
                    if (glayer.isGraphicSelected(Id))
                        glayer.unselect(Id);
                    else
                        glayer.select(Id);
                }

                break;
            }
        }
    }

    return list;
}

void SampleMap::changeWidgetDock(QWidget* widget,const MapElementDock& mapElementDock)
{
    if(!widget)
        return;

    // get parent layout of compass widget
    QGridLayout* layout = widget->property(PROP_MANAGING_LAYOUT.toStdString().c_str()).value<QGridLayout*>();

    if(layout)
    {
        // remove compass widget
        layout->removeWidget(widget);

        // add widget
        switch(mapElementDock)
        {
        case MED_LowerLeft:
            layout->addWidget(widget,layout->rowCount()-1,0,1,1);
            break;
        case MED_LowerRight:
            layout->addWidget(widget,layout->rowCount()-1,layout->columnCount()-1,1,1);
            break;
        case MED_TopLeft:
            layout->addWidget(widget,0,0,1,1);
            break;
        case MED_TopRight:
            layout->addWidget(widget,0,layout->columnCount()-1,1,1);
            break;
        default:
            break;
        }
    }
}

bool SampleMap::isMilMultipoint(const MilitarySymbolObject& mo)
{
    QString sic = mo.SymbolCode;

    if (sic.length() < 10)
        return false;

    if ((sic[0] == 'G') && (mo.ControlPoints.length() > 1))
        return true;

    return false;
}

Message SampleMap::MilitaryObject2UpdateMessage(const MilitarySymbolObject& mo)
{
    Message message;

    QVariantMap properties;

    QString messageID = QString::number(mo.Id);
    QString sic = mo.SymbolCode;

    properties["_id"] = QVariant(messageID);
    properties["_type"] = "position_report";
    properties["_action"] = "update";
    properties["sic"] = QVariant(sic);
    properties["uniquedesignation"] = messageID;

    // TODO: Verify WKID of the MilitarySymbolObject before adding this back in
    // properties["_wkid"] = "3857"; // = Web Wercator

    QString cps = controlPoints2QString(mo.ControlPoints);
    properties["_control_points"] = QVariant(cps);

    if (0) // debug message
    {
        qDebug() << "New Message, Properties: ";
        for (QVariantMap::const_iterator i = properties.begin(); i != properties.end(); ++i)
        {
            QString key = i.key();
            QVariant value = i.value();
            qDebug() << "Key: " << key << ", Value: " << QString(value.toString());
        }
    }

    bool wellFormedMessage = true;
    // TODO: check message for correctness (Valid SIDC, etc.)

    if (wellFormedMessage)
        message.setProperties(properties);

    return message;
}

void SampleMap::Reset()
{
    if (m_pMap.isInitialized())
    {
        m_pMap.setExtent(firstExtent);
        m_pMap.setScale(firstScale);
        m_pMap.setRotation(0.0);
    }
}

void SampleMap::TestReproCase()
{
   QMessageBox::warning(m_pMapGraphicsView, "Test Case", "Insert Your Test Repro Case Here", "To Show Problems");
}

void SampleMap::resizeEvent(QResizeEvent* event)
{
  Q_UNUSED(event)

  qDebug() << "Widget resized.";

  if (m_pMapScalebar)
    m_pMapScalebar->updateScalebar();
}
