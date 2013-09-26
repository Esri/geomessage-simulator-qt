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

#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QProgressDialog>

#include "Geometry.h"
#include "Layer.h"
#include "GraphicsLayer.h"
#include "LocalServer.h"
#include "MarkerSymbol.h"
#include "GeometryEngine.h"
#include "LocalMapService.h"
#include "Geomessage.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleFillSymbol.h"

#include "MapController.h"

static const QString APP_CONFIG_PATH(":/Resources/appconfig.xml");
static const QString GPS_SIMULATION_FILE(":/Resources/Route_Archer.gpx");

MapController::MapController(Map* inputMap,
                             MapGraphicsView* inputGraphicsView,
                             QObject* parent) :
    QObject(parent),
    map(inputMap),
    mapGraphicsView(inputGraphicsView),
    buddiesLayerVisible(true),
    observationsLayerVisible(true),
    udpSocket(NULL),
    broadcastPort(45678),
    broadcastInterval(3000),
    simulator(),
    positionReportTimer(NULL),
    acceptPositionReports(true),
    acceptSpotReports(true),
    appConfigDialog(NULL),
    showOwnship(false),
    followOwnship(false),
    isMapReady(false),
    drawingOverlay(0),
    lastHeading(0.0),
    mouseState(MouseStateNone),
    visibilityInProgress(false)
{
  if (QFile::exists("./route.gpx"))
    simulator.setGpxFile("./route.gpx");
  else
    simulator.setGpxFile(GPS_SIMULATION_FILE);

  simulator.setTimerInterval(40); // Warning: jumpy at some intervals (ex. 100)
  simulator.setPlaybackMultiplier(25);
  connect(&simulator, SIGNAL(positionUpdateAvailable(QPointF, double)), this, SLOT(handlePositionAvailable(QPointF, double)));

  readAppConfig();

  // create the messaging socket connection and hook up message receiving
  udpSocket = new QUdpSocket(this);
  udpSocket->bind(broadcastPort, QUdpSocket::DontShareAddress | QUdpSocket::ReuseAddressHint);
  connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

  //Timer to send the position report
  positionReportTimer = new QTimer(this);
  positionReportTimer->setInterval(1000);
  connect(positionReportTimer, SIGNAL(timeout()), this, SLOT(sendPositionMessage()));
  handleToggleBroadcastPosition(true);
  handleToggleShowMe(true);
  handleToggleFollowMe(true);  

  chemLightColorStr = "None";
}

MapController::~MapController()
{
  if (appConfigDialog != NULL)
    delete appConfigDialog;

  if (drawingOverlay != NULL)
      delete drawingOverlay;
}

QByteArray MapController::createChemLightReport(Point& location, const QString& messageID, const QString& action)
{
  Q_UNUSED(action)

  if (qIsNaN(location.x()) || qIsNaN(location.y()))
    return QByteArray();

  QString strControlPoint = QString("%1,%2").arg(location.x()).arg(location.y());

  QByteArray data;
  QXmlStreamWriter chemLightReport(&data);
  chemLightReport.writeStartDocument();
  chemLightReport.writeStartElement("geomessages");

  Geomessage geomsg("chemlight", "update", messageID);
  geomsg.setProperty("_control_points", strControlPoint);
  geomsg.setProperty("_wkid", "3857");
  geomsg.setProperty("color", chemLightColorStr);

  geomsg.toXml(chemLightReport);

  chemLightReport.writeEndElement(); // geomessages
  chemLightReport.writeEndDocument();

  return data;
}

QByteArray MapController::createGenericReport(const QString& reportName, const QString& messageID, const QString& action)
{
  Q_UNUSED(action)
  Q_UNUSED(reportName)

  QByteArray reportData;
  QXmlStreamWriter genericReport(&reportData);

  genericReport.writeStartElement("geomessages");
    genericReport.writeStartElement("geomessage");
      // IMPORTANT: adds an attribute with the ID in order to allow filtering these messages from ownship
      genericReport.writeAttribute("id", messageID);
      genericReport.writeTextElement("_type", "position_report");
      genericReport.writeTextElement("_action", "update");
      genericReport.writeTextElement("_id", messageID);
    genericReport.writeEndElement(); // geomessage
  genericReport.writeEndElement(); // geomessages
  genericReport.writeEndDocument();

  return reportData;
}

QByteArray MapController::createPositionReport(const QString& action)
{
  Q_UNUSED(action)

  Point location = lastOwnshipPoint;
  QString strLocationControlPoint = QString("%1,%2").arg(location.x()).arg(location.y());
  double lat = location.y();
  double lon = location.x();

  QString mgrs = mapPointToMGRS(lastOwnshipPoint);

  QByteArray reportData;
  QXmlStreamWriter positionReport(&reportData);
  positionReport.writeStartDocument();
  positionReport.writeStartElement("geomessages");
    positionReport.writeStartElement("geomessage");

      // add an attribute with the ID in order to allow filtering the message
      positionReport.writeAttribute("id", userData.id);

      // positionReport.writeTextElement("_name", "position_report");
      positionReport.writeTextElement("_type", "position_report");
      positionReport.writeTextElement("_action", "update");
      positionReport.writeTextElement("_id", userData.id);
      positionReport.writeTextElement("_control_points", strLocationControlPoint);
      positionReport.writeTextElement("_wkid", "3857");
      positionReport.writeTextElement("sic", userData.code);

      // TODO: evaluate these remaining properties to see if still valid
      // symbol text modifiers
      positionReport.writeTextElement("quantity", "5");
      positionReport.writeTextElement("reinforcedorreduced", "+");
      positionReport.writeTextElement("staffcomments", "Comments");
      positionReport.writeTextElement("additionalinformation", "Additional Info");
      positionReport.writeTextElement("evaluation_rating", "A1");
      positionReport.writeTextElement("combateffectiness", "Excellent");
      positionReport.writeTextElement("signatureequipment", "Gun");
      positionReport.writeTextElement("higherformation", "XX");
      positionReport.writeTextElement("iff_sif", "iff");
      positionReport.writeTextElement("uniquedesignation", userData.name);
      positionReport.writeTextElement("type", "vehicle");
      positionReport.writeTextElement("dtg", QDateTime::currentDateTimeUtc().toString("dd' 'hhmmss'Z 'MMM' 'yyyy"));
      positionReport.writeTextElement("altitude_depth", "surface");
      positionReport.writeTextElement("location", mgrs);
      positionReport.writeTextElement("speed", "75");

      // position report attributes
      positionReport.writeTextElement("message_sent_time", QDateTime::currentDateTimeUtc().toString("ddhhmm'Z 'MMM' 'yy"));
      positionReport.writeTextElement("reporting_unit", userData.id);
      positionReport.writeTextElement("position", QString::number(lat, 'g', 4) + ", " + QString::number(lon, 'g', 4));
      positionReport.writeTextElement("heading", QString::number(lastHeading));
      positionReport.writeTextElement("altitude", "100m");
      positionReport.writeTextElement("engaged", userData.engaged);
      positionReport.writeTextElement("number_and_type", "1 Vehicle");
      positionReport.writeTextElement("category", "Land");

    positionReport.writeEndElement(); // geomessage
  positionReport.writeEndElement(); // geomessages
  positionReport.writeEndDocument();

  return reportData;
}

QByteArray MapController::createSpotReport(QVariant data)
{
  QObject* rawSpotReportData = qobject_cast<QObject*>(data.value<QObject*>());

  if (!rawSpotReportData)
    return QByteArray();

  //Get our current location to include in spot report
  QString reportingUnitLocationMGRS = mapPointToMGRS(lastOwnshipPoint);

  QVariant sizeData = rawSpotReportData->property("size");
  QVariant activityData = rawSpotReportData->property("activity");
  QVariant locationXData = rawSpotReportData->property("locationX");
  QVariant locationYData = rawSpotReportData->property("locationY");
  QVariant locationMGRSData = rawSpotReportData->property("locationMGRS");
  QVariant unitData = rawSpotReportData->property("unit");
  QVariant timeData = rawSpotReportData->property("time");
  QVariant equipmentData = rawSpotReportData->property("equipment");

  QString strLocationControlPoint = locationXData.toString() % "," % locationYData.toString();

  QString symbolID = "SFGP--------USG"; // default value in case lookup SIDC from equipment name fails

  // lookup SIDC from equipment name:
  QVariantMap symbolProps = symbolNameOrId2VariantMap(equipmentData.toString());
  if (symbolProps.count() > 0)
      symbolID = symbolProps.value("SymbolID").toString();

  QString messageID = QUuid::createUuid().toString();
  QString dtgTime(timeData.toString());
  if (dtgTime.compare("Now", Qt::CaseInsensitive) == 0)
    dtgTime = QDateTime::currentDateTimeUtc().toString("ddhhmm'Z 'MMMyy");

  QByteArray reportData;
  QXmlStreamWriter spotReport(&reportData);
  spotReport.writeStartDocument();
  spotReport.writeStartElement("geomessages");
    spotReport.writeStartElement("geomessage");
      spotReport.writeTextElement("_type", "position_report");
      spotReport.writeTextElement("_action", "update");
      spotReport.writeTextElement("_id", messageID);
      spotReport.writeTextElement("_control_points", strLocationControlPoint);
      spotReport.writeTextElement("_wkid", "3857");

      // symbol and text modifiers
      spotReport.writeTextElement("sic", symbolID);
      spotReport.writeTextElement("quantity", sizeData.toString());
      spotReport.writeTextElement("type", equipmentData.toString());
      spotReport.writeTextElement("dtg", dtgTime);
      spotReport.writeTextElement("additionalinformation", activityData.toString());
      spotReport.writeTextElement("altitudedepth", "surface");

      // spot report attributes
      spotReport.writeTextElement("observer_id", userData.id);
      spotReport.writeTextElement("message_sent_time", QDateTime::currentDateTimeUtc().toString("ddhhmm'Z 'MMM' 'yy"));
      spotReport.writeTextElement("reportingunitlocation", reportingUnitLocationMGRS);
      spotReport.writeTextElement("uniquedesignation", userData.name);

      // salute format attributes
      spotReport.writeTextElement("size", sizeData.toString());
      spotReport.writeTextElement("activity", activityData.toString());
      spotReport.writeTextElement("location", locationMGRSData.toString());
      spotReport.writeTextElement("unit", unitData.toString());
      spotReport.writeTextElement("time", dtgTime);
      spotReport.writeTextElement("equipment", equipmentData.toString());

    spotReport.writeEndElement(); // message
  spotReport.writeEndElement(); // messages
  spotReport.writeEndDocument();

  return reportData;
}

QString MapController::decimalDegreesToDMS(double coord)
{
  int lonDegrees = floor(coord);
  double degreesRemainder = coord - static_cast<double>(lonDegrees);
  int lonMinutes = floor(degreesRemainder * 60.0);
  double minutesRemainder = (degreesRemainder * 60) - static_cast<double>(lonMinutes);
  int lonSeconds = floor(minutesRemainder * 60);

  return QString::number(lonDegrees) + (char) 0x00B0 + " " + QString::number(lonMinutes) + "'" + " " + QString::number(lonSeconds) + "\"";
}

bool MapController::filterMessages(const QString& strMessages)
{
  QXmlStreamReader reader(strMessages);
  QXmlStreamAttributes attrs;

  while (reader.readNextStartElement() && !reader.atEnd() && !reader.hasError())
  {
    if (QStringRef::compare(reader.name(), "geomessage", Qt::CaseInsensitive) == 0)
    {
      attrs = reader.attributes();
      if (QStringRef::compare(attrs.value("id"), userData.id) == 0)
        return true;

      // Next filter out unsupported message types
      reader.readNext();
      while (!reader.atEnd())
      {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }

        if (reader.isStartElement())
        {
          if (QStringRef::compare(reader.name(), "_type", Qt::CaseInsensitive) == 0)
          {
            QString messageType = getReaderValue(reader);
            if ((QString::compare(messageType, "position_report", Qt::CaseInsensitive) == 0) ||
                (QString::compare(messageType, "spotrep", Qt::CaseInsensitive) == 0) ||
                (QString::compare(messageType, "situation_report", Qt::CaseInsensitive) == 0) ||
                (QString::compare(messageType, "gsrrep", Qt::CaseInsensitive) == 0))
            {
              return false;
            }
            else
            {
              qDebug() << "Skipping unsupported Message Type: " << messageType;
              return true;
            }
          }
          else
          {
            skipUnknownElement(reader);
          }
        }
        else
        {
          reader.readNext();
        }
      }

    }
  }

  return false;
}

void MapController::handleBasemapChange(QString name)
{
  qDebug() << "Basemap changed, name = " << name;

  // Implement if desired
}

void MapController::handleCenterOnSelfBtn()
{
  handleToggleFollowMe(true);
}

void MapController::handleChemLightSelected(QString color)
{
  Q_UNUSED(color);

  mouseState = MouseStateMenuClicked;

  chemLightColorStr = color;
}

void MapController::handleGetPointFromMap()
{
  mouseState = MouseStateMenuClicked;
}

void MapController::handleHomeClicked()
{
  if (!ownshipStartingMapPoint.isEmpty())
  {
    handleToggleFollowMe(false); // or it will just snap right back in simulation

    map->setExtent(originalExtent);
    map->setScale(originalScale);
    map->setRotation(0);
    map->panTo(ownshipStartingMapPoint);
  }
}

void MapController::mousePress(QMouseEvent mouseEvent)
{
  QPointF mousePoint = QPointF(mouseEvent.pos().x(), mouseEvent.pos().y());

  if (mouseEvent.button() == Qt::LeftButton)
  {
    handleMapMousePressLeft(mousePoint);
  }
  else if (mouseEvent.button() == Qt::RightButton)
  {
    handleMapMousePressRight(mousePoint);
  }
}

void MapController::handleMapMousePressLeft(QPointF mousePoint)
{
  if ((map) && (isMapReady))
  {
    Point mapPoint = map->toMapPoint(mousePoint.x(), mousePoint.y());

    previousMousePressPosScreen.setX(mousePoint.x());
    previousMousePressPosScreen.setY(mousePoint.y());

    previousMousePressPosMap.setX(mapPoint.x());
    previousMousePressPosMap.setY(mapPoint.y());

    qDebug() << "Right Click, Map Point = " << mapPoint.x() << ", " << mapPoint.y();

    if (mouseState == MouseStateMenuClicked)
        mouseState = MouseStateWaitingForMapPoint;
    else if (mouseState == MouseStateWaitingForMapPoint)
    {
      if (chemLightColorStr == "None")
        returnPoint(previousMousePressPosMap);
      else
        sendChemLightMessage(previousMousePressPosMap);
    }
  }
}

void MapController::handleMapMousePressRight(QPointF mousePoint)
{
  if ((map) && (isMapReady))
  {
    Point mapPoint = map->toMapPoint(mousePoint.x(), mousePoint.y());

    previousMousePressPosScreen.setX(mousePoint.x());
    previousMousePressPosScreen.setY(mousePoint.y());

    previousMousePressPosMap.setX(mapPoint.x());
    previousMousePressPosMap.setY(mapPoint.y());

    qDebug() << "Left Click, Map Point = " << mapPoint.x() << ", " << mapPoint.y();

    QList<Layer> layers = map->layers();

    foreach (Layer layer, layers)
    {
      QString name = layer.name();
      LayerType layerType = layer.type();
      LayerStatus status = layer.status();

      qDebug() << "Layer Name: " << name << ", Type: " << int(layerType);

      if ((status == LayerStatus::Initialized) &&
              (layerType ==  LayerType::ArcGISDynamicMapService))
      {
        ArcGISDynamicMapServiceLayer dynaMapLayer =
          static_cast<ArcGISDynamicMapServiceLayer>(layer);

        QString url = dynaMapLayer.url();
        qDebug() << "url: " << url;

        // Identify that point
        IdentifyParameters params;
        params.setGeometry(mapPoint);
        params.setMapExtent(map->extent());
        params.setSpatialReference(map->spatialReference());
        params.setMapHeight(map->height());
        params.setMapWidth(map->width());
        params.setLayerMode(IdentifyParameters::VISIBLE_LAYERS);
        int dpyY = QApplication::desktop()->physicalDpiY();
        params.setDPI(dpyY);

        IdentifyTask identifyTask(url);

        QList<IdentifyResult> results = identifyTask.executeAndWait(params);
        onIdentifyComplete(results);

        // if Async Desired:
        // connect(&identifyTask, SIGNAL(signalIdentifyResult(QList<IdentifyResult>)), this, SLOT(onIdentifyComplete(QList<IdentifyResult>)));
        // identifyTask.executeAsync(params);
      }
    }
  }
}

bool MapController::isDuplicateTimes(QTime time1, QTime time2)
{
  if (!(time1.isValid() && time2.isValid()))
    return time1.isValid() == time2.isValid();
  else
  {
    int absDiff = abs(time1.msecsTo(time2));
    return (50 > absDiff);
  }
}

void MapController::uiElementClicked()
{

}

void MapController::onIdentifyComplete(QList<IdentifyResult> results)
{
  int resultsCount = results.count();
  qDebug() << "identifyComplete, results = " << resultsCount;

  if (resultsCount == 0)
      return;

  qDebug() << "identifyComplete, results = " << resultsCount;

  emit identifyComplete(results);
}

void MapController::handlePan(QString direction)
{
  // disable the vehicle following if enabled so pan will work, and be in the correct direction
  if (followOwnship)
  {
      followOwnship = false;
      map->setRotation(0);
  }

  Envelope extent = map->extent();

  double width = extent.width();
  double height = extent.height();

  double centerX = extent.centerX();
  double centerY = extent.centerY();

  const double PAN_INCREMENT = 0.25;

  if (direction.compare("up") == 0)
      centerY += height * PAN_INCREMENT;
  else if (direction.compare("down") == 0)
      centerY -= height * PAN_INCREMENT;
  else if (direction.compare("left") == 0)
      centerX -= width * PAN_INCREMENT;
  else if (direction.compare("right") == 0)
      centerX += width * PAN_INCREMENT;

  Envelope newExtent(Point(centerX, centerY), width, height);

  map->panTo(newExtent);
}

void MapController::handlePositionAvailable(QPointF pos, double orientation)
{
  QDateTime positionReceivedTime = QDateTime::currentDateTime();
  Q_UNUSED(positionReceivedTime)

  if (!isMapReady)
      return;

  // if following....
  Point mapPoint = GeometryEngine::project(pos.x(), pos.y(), map->spatialReference());

  if (0) // if debug simulation data
  {
      qDebug() << "Simulator Data: " << QString::number(pos.y()) << ", " << QString::number(pos.y()) << ", Orientation: " << orientation;
      qDebug() << "         Delta: " << QString::number(mapPoint.x()) << ", " << QString::number(mapPoint.y());
  }

  double angle = (double) ((static_cast<int>(orientation + 90.0)) % 360);

  showHideMe(showOwnship, mapPoint, angle);

  if (followOwnship)
  {
    map->setRotation(angle);
    map->panTo(mapPoint);
  }

  if (!myPreviousLocation.isNull())
  {
    // calculate speed here if desired
  }

  myPreviousLocation = pos;

  QString lon = decimalDegreesToDMS(pos.x());
  QString lat = decimalDegreesToDMS(pos.y());
  QString latlon = lat + ", " + lon;
  Q_UNUSED(latlon)

  QString mgrs = mapPointToMGRS(mapPoint);

  emit headingChanged(QVariant(QString::number(angle) + (char) 0x00B0));
  emit positionChanged(QVariant(mgrs));
}

void MapController::handleResetMap()
{
  handleToggleFollowMe(false); // or it will just snap right back in simulation

  map->setExtent(originalExtent);
  map->setScale(originalScale);
  map->setRotation(0);
}

void MapController::handleToggleBroadcastPosition(bool state)
{
  //Connect the timer to emit position reports
  if (state)
    positionReportTimer->start(broadcastInterval);
  else
    positionReportTimer->stop();
}

void MapController::handleToggleBuddies(bool state)
{
  buddiesLayerVisible = state;

  if (!buddiesLayer.isNull())
    buddiesLayer.setVisible(buddiesLayerVisible);
}

void MapController::handleToggleEmergencyClicked(bool state)
{
  QByteArray genericReport;
  if (state)
  {
    userData.engaged = "Yes";
    genericReport = createGenericReport("position_report", userData.id, "HIGHLIGHT");
    positionReportTimer->setInterval(1000);
  }
  else
  {
    userData.engaged = "No";
    genericReport = createGenericReport("position_report", userData.id, "UN-HIGHLIGHT");
    positionReportTimer->setInterval(broadcastInterval);
  }

  QByteArray report = createPositionReport("UPDATE");
  transmitMessages(report);
  transmitMessages(genericReport);
}

void MapController::handleToggleObservations(bool state)
{
  observationsLayerVisible = state;

  if (!observationsLayer.isNull())
    observationsLayer.setVisible(observationsLayerVisible);
}

void MapController::handleToggleReceivePositionReports(bool state)
{
  acceptPositionReports = state;
}

void MapController::handleToggleReceiveSpotReports(bool state)
{
  acceptSpotReports = state;
}

void MapController::openAppConfigDialog()
{
  if (NULL == appConfigDialog)
    appConfigDialog = new AppConfigDialog(this, mapGraphicsView);
  appConfigDialog->show();
}

void MapController::handleToggleFollowMe(bool state)
{
  followOwnship = state;
}

void MapController::handleToggleShowMe(bool state)
{
  showOwnship = state;

  if (showOwnship && !simulator.started())
  {
    simulator.startSimulation();
  }
}

void MapController::handleOpenCOA()
{
  //TODO let user select a COA file; for now, use resource coa.xml
  QFile coaFile("./coa.xml");
  if (!coaFile.exists())
  {
    coaFile.setFileName(":/Resources/coa.xml");
    if (!coaFile.exists())
    {
      coaFile.setFileName("./coa.xml");
    }
  }
  if (coaFile.exists())
  {
    setMessageStream(coaFile);
  }
  else
  {
    QMessageBox messageBox;
    QFileInfo fileInfo(coaFile);
    messageBox.setText("COA file " + fileInfo.absoluteFilePath() + " not found!");
    messageBox.exec();
  }
}

void MapController::toggleLayerVisibility(QString layerName)
{
  Layer layer = map->layer(layerName);

  if (layer.status() == LayerStatus::Initialized)
  {
      layer.setVisible(!layer.visible());
  }
}

void MapController::handleOpenMPK()
{
  //Give user a dialog to open an MPK
  QFileDialog dialog(mapGraphicsView);
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setNameFilter(tr("Map packages (*.mpk)"));

  QString mpkFileName = dialog.getOpenFileName();
  if (0 < mpkFileName.length())
  {
    LocalMapService localMapService = LocalMapService(mpkFileName);
    localMapService.startAndWait();

    ArcGISDynamicMapServiceLayer dynamicLocalServiceLayer =
        ArcGISDynamicMapServiceLayer(localMapService.urlMapService());
     map->addLayer(dynamicLocalServiceLayer);
  }
}

void MapController::handleZoomIn()
{
  map->zoom(0.5);
}

void MapController::handleZoomOut()
{
  map->zoom(2);
}

void MapController::initController()
{
  if (!map)
  {
    return;
  }

  if (!messageProcessor.isEmpty())
  {
    return;
  }

  messagGroupLayer = MessageGroupLayer(SymbolDictionaryType::Mil2525C);
  map->addLayer(messagGroupLayer);

  messageProcessor = messagGroupLayer.messageProcessor();

  dictionary = messageProcessor.symbolDictionary();

  QList<SymbolProperties> symbolPropList = dictionary.findSymbols();
  int symbolCount = symbolPropList.count();
  bool dictionaryWorks = (symbolCount > 0);

  if (!dictionaryWorks)
      QMessageBox::warning(mapGraphicsView, "Failure", "Dictionary Did not initialize", "Advanced/Military Symbology will not work");

  // Needed so map does not try to pan while we are following the vehicle (which gives a lagging appearance)
  map->setPanAnimationEnabled(false);

  map->addLayer(mouseClickGraphicLayer);
  map->addLayer(chemLightLayer);
  map->addLayer(viewshedGraphicLayer);

}

void MapController::processPendingDatagrams()
{
  while (udpSocket->hasPendingDatagrams())
  {
    QByteArray datagram;
    datagram.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(datagram.data(), datagram.size());
    QString strMessage(datagram.data());

    // we need to filter the message here. for now, only GPS messages
    if (filterMessages(strMessage))
      continue;

    // we need to parse the messages here,
    // break it into propertysets and push it to the message parser
    setMessageStream(strMessage);
  }
}

void MapController::returnPoint(Point point)
{
  if (point.isNull())
      return;

  mouseClickGraphicLayer.removeAll();

  SimpleMarkerSymbol smsSymbol(Qt::red, 16, SimpleMarkerSymbolStyle::Circle);
  Graphic mouseClickGraphic(point, smsSymbol);

  int id = mouseClickGraphicLayer.addGraphic(mouseClickGraphic);

  mouseClickGraphicLayer.select(id);

  QString mgrs = mapPointToMGRS(point);
  emit newPointCreated(point.x(), point.y(), mgrs);

  mouseState = MouseStateNone;
}

void MapController::readAppConfig()
{
  //First: if we have already saved user settings, we don't need to read appconfig.xml.
  applyAppConfigSettings();

  //If we could not find previously saved user settings, read them from appconfig.xml and save them
  if (0 == userData.id.length()
    || 0 == userData.name.length()
    || 0 == userData.code.length()
    || 0 == broadcastPort
    || 0 == broadcastInterval)
  {
    QFile file("./appconfig.xml");

    // if a local file doesn't exist use the one from the resource file
    if (!file.exists())
      file.setFileName(APP_CONFIG_PATH);

    if (!file.open(QFile::ReadOnly | QFile::Text))
      return;

    QXmlStreamReader reader(&file);
    while (!reader.atEnd() && !reader.hasError())
    {
      while (reader.readNextStartElement())
      {
        QString elementName = reader.name().toString();
        if (QString::compare(elementName, "user", Qt::CaseInsensitive) == 0)
        {
          QXmlStreamAttributes attrs = reader.attributes();
          if (0 == userData.name.length())
            userData.name = attrs.value("name").toString();
          if (0 == userData.id.length())
            userData.id = attrs.value("id").toString();
        }
        else if (QString::compare(elementName, "code", Qt::CaseInsensitive) == 0)
        {
          if (0 == userData.code.length())
            userData.code = reader.readElementText();
        }
        else if (QString::compare(elementName, "port", Qt::CaseInsensitive) == 0)
        {
          if (0 == broadcastPort)
            broadcastPort = reader.readElementText().toInt();
        }
        else if (QString::compare(elementName, "interval", Qt::CaseInsensitive) == 0)
        {
          if (0 == broadcastInterval)
            broadcastInterval = reader.readElementText().toInt();
        }
      }
    }

    file.close();

    //Save settings we just read from the file into the QSettings object
    QSettings settings("My Organization", "Vehicle Commander");
    settings.setValue("userId", userData.id);
    settings.setValue("username", userData.name);
    settings.setValue("userSIC", userData.code);
    settings.setValue("messagingPort", broadcastPort);
    settings.setValue("messagingInterval", broadcastInterval);
  }

  userData.engaged = "No";
}

void MapController::applyAppConfigSettings()
{
  const QVariant defaultValue;
  QSettings settings("My Organization", "Vehicle Commander");
  userData.id = settings.value("userId", defaultValue).toString();
  userData.name = settings.value("username", defaultValue).toString();
  userData.code = settings.value("userSIC", defaultValue).toString();
  broadcastPort = settings.value("messagingPort", defaultValue).toInt();
  broadcastInterval = settings.value("messagingInterval", defaultValue).toInt();

  //Apply these settings (e.g. change port and interval for socket)
  if (0 < broadcastPort && NULL != udpSocket)
    udpSocket->bind(broadcastPort, QUdpSocket::DontShareAddress | QUdpSocket::ReuseAddressHint);
  if (0 < broadcastInterval && NULL != positionReportTimer)
  {
    positionReportTimer->setInterval(broadcastInterval);
  }
}

void MapController::sendChemLightMessage(Point pos)
{
  Qt::GlobalColor chemlightColor = Qt::green;

  if (chemLightColorStr == "red")
    chemlightColor = Qt::red;
  else
    if (chemLightColorStr == "blue")
      chemlightColor = Qt::blue;
    if (chemLightColorStr == "yellow")
        chemlightColor = Qt::yellow;

  SimpleMarkerSymbol smsSymbol(chemlightColor, 20, SimpleMarkerSymbolStyle::Circle);
  Graphic mouseClickGraphic(pos, smsSymbol);

  int id = this->chemLightLayer.addGraphic(mouseClickGraphic);
  Q_UNUSED(id)

  QString messageID;
  QByteArray report;

  // create a new chem light
  messageID = QUuid::createUuid().toString();
  report = createChemLightReport(pos, messageID, "update");

  transmitMessages(report);
  emit clearChemLightUI();

  chemLightColorStr = "None";
  mouseState = MouseStateNone;
}

void MapController::sendPositionMessage()
{
  QByteArray report = createPositionReport("UPDATE");
  transmitMessages(report);
}

void MapController::sendSpotReport(QVariant data)
{
  mouseClickGraphicLayer.removeAll();

  QByteArray report = createSpotReport(data);
  transmitMessages(report);
}

void MapController::setMap(Map* newMap)
{
  if (map)
  {
    disconnect(map, SIGNAL(layerAdded(QString, LayerType)), this, SLOT(handleLayerAdded(QString, LayerType)));
    disconnect(map, SIGNAL(layerRemoved(QString,LayerType)), this, SLOT(handleLayerRemoved(QString, LayerType)));

    buddiesLayer.removeAll();
    chemLightLayer.removeAll();
    observationsLayer.removeAll();
    mouseClickGraphicLayer.removeAll();
  }

  map = newMap;
  initController();
}

void MapController::transmitMessages(QByteArray datagram)
{
  const bool SUPPRESS_MESSAGES = false; // true;

  if (SUPPRESS_MESSAGES)
    return;

  if (!datagram.isEmpty() && !datagram.isNull())
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, broadcastPort);
}

double inline RandomValue()
{
  return (double)::rand() / ((double)(RAND_MAX));
} // end RandomValue

void MapController::skipUnknownElement(QXmlStreamReader& reader)
{
  reader.readNext();
  while (!reader.atEnd())
  {
    if (reader.isEndElement())
    {
      reader.readNext();
      break;
    }

    if (reader.isStartElement())
    {
      skipUnknownElement(reader);
    }
    else
    {
      reader.readNext();
    }
  }
}

QString MapController::getReaderValue(QXmlStreamReader& reader)
{
  QString text = reader.readElementText();
  if (reader.isEndElement())
      reader.readNext();

  return text;
}

void MapController::readToElementEnd(QXmlStreamReader& reader)
{
  reader.readNext();
  while (!reader.atEnd())
  {
    if (reader.isEndElement())
    {
      reader.readNext();
      break;
    }
    reader.readNext();
  }
}

void MapController::setMessageStream(QFile& file)
{
  file.open(QFile::ReadOnly | QFile::Text);
  QXmlStreamReader reader(&file);
  setMessageStream(reader);
}

void MapController::setMessageStream(const QString& stream)
{
  QXmlStreamReader reader(stream);
  setMessageStream(reader);
}

void MapController::setMessageStream(QXmlStreamReader& reader)
{
  while (!reader.atEnd() && !reader.hasError())
  {
    if (reader.isStartElement())
    {
      // Handle Runtime Message or GeoMessage
      if ((QStringRef::compare(reader.name(), "geomessages", Qt::CaseInsensitive) == 0) ||
        (QStringRef::compare(reader.name(), "messages", Qt::CaseInsensitive) == 0))
      {
        if (!readMessages(reader))
        {
          readToElementEnd(reader);
          continue;
        }
      } else { skipUnknownElement(reader); } // end if message
    } else { reader.readNext(); } // end if startElement
  } // end of while
}

bool MapController::readMessages(QXmlStreamReader& reader)
{
  reader.readNext();
  while (!reader.atEnd())
  {
    if (reader.isEndElement())
    {
      reader.readNext();
      break;
    }
    if (reader.isStartElement())
    {
      // Handle Runtime Message or GeoMessage
      if ((QStringRef::compare(reader.name(), "geomessage", Qt::CaseInsensitive) == 0) ||
          (QStringRef::compare(reader.name(), "message", Qt::CaseInsensitive) == 0))
      {
        if (!readMessage(reader))
        {
          readToElementEnd(reader);
          continue;
        }
      }
      else
      {
        skipUnknownElement(reader);
      }
    } // end if isStartElement
    else
    {
      reader.readNext();
    } // end of if start element
  }

  return true;
}

bool MapController::readMessage(QXmlStreamReader& reader)
{
  if (!isMapReady)
      return false;

  // each message is simply a key/value pair
  QVariantMap properties;
  QString name;
  QString value;
  reader.readNext();
  while (!reader.atEnd())
  {
    if (reader.isEndElement())
    {
      reader.readNext();
      break;
    }
    if (reader.isStartElement())
    {
      // get the name of the element
      name = reader.name().toString();
      value = getReaderValue(reader);
      properties.insert(name, QVariant(value));
    } // end if isStartElement
    else
    {
      reader.readNext();
    } // end of if start element
  }

  if (properties.count() > 0)
  {
    Message message;

    // force every message type (except chemlight) to position_report for runtime
    if (QString::compare(properties["_type"].toString(), "chemlight", Qt::CaseInsensitive) == 0)
    {
      qDebug("Chemlight");
    }
    else
    {
      properties["_type"] = "position_report";
    }

    message.setProperties(properties);

    if (0) // debug desired for sent messages
    {
      qDebug() << "New Message, Properties: ";
      for (QVariantMap::const_iterator i = properties.begin(); i != properties.end(); ++i)
      {
          QString key = i.key();
          QVariant value = i.value();
          qDebug() << "Key: " << key << ", Value: " << value.toString();
      }
    }

    try
    {
      // MESSAGES PROCESSED HERE:
      messageProcessor.processMessage(message);
    }
    catch (EsriRuntimeQt::Exception e)
    {
      qWarning() << "Process Message Failed: ";
      qWarning() << "Exception: " << e.what();
    }

  }

  return true;
}

void MapController::showHideMe(bool show, Point atPoint, double withHeading)
{
  if ((!isMapReady) || (mapGraphicsView == 0))
    return;

  // set these on the first positioning
  if (ownshipStartingMapPoint.isEmpty())
  {
    originalExtent = map->extent();
    originalScale = map->scale();
    ownshipStartingMapPoint = atPoint; // originalExtent.center();
  }

  if (drawingOverlay == 0)
  {
    drawingOverlay = new SimpleGraphicOverlay();

    QPixmap ownshipPixmap(":/Resources/icons/Ownship.png");
    QImage ownshipImage = ownshipPixmap.toImage();
    drawingOverlay->setImage(ownshipImage);
    drawingOverlay->setGraphicsView(mapGraphicsView);
  }

  drawingOverlay->setVisible(show);
  if (show)
  {
    drawingOverlay->setPosition(atPoint);
    drawingOverlay->setAngle(withHeading);
  }

  lastOwnshipPoint = atPoint;
  lastHeading = withHeading;
}

void MapController::mapReady()
{
  isMapReady = true;

  // Zoom into on start
  map->setScale(5000000.0);

  SpatialReference sr = map->spatialReference();

  qDebug() << "MapReady, Spatial Reference = " << sr.id();

  // IMPORTANT: turn on/off grid by deafult
  const bool DEFAULT_GRID_ON = true;
  if (DEFAULT_GRID_ON)
  {
      map->grid().setType(GridType::Mgrs);
      map->grid().setVisible(true);
  }
}

Point MapController::MGRSToMapPoint(QString mgrs)
{
  Point returnPoint;

  if (!isMapReady) // no SR until map ready
      return returnPoint;

  SpatialReference sr = map->spatialReference();
  if (sr.id() < 0)
  {
      qDebug() << "FAIL: No SR in mapPointToMGRS";
      return returnPoint;
  }

  QStringList mgrss;
  mgrss.append(mgrs);

  QList<Point> points;
  points = sr.fromMilitaryGrid(mgrss, MgrsConversionMode::Automatic);

  if (points.length() < 1)
    return returnPoint;

  returnPoint = points.at(0);

  if (0)
    qDebug() << "MGRSToMapPoint: " << QString::number(returnPoint.x()) << ", " << QString::number(returnPoint.y());;

  return returnPoint;
}

QString MapController::mapPointToMGRS(Point point)
{
  QString returnMgrs;

  if (!isMapReady) // no SR until map ready
      return returnMgrs;

  SpatialReference sr = map->spatialReference();
  if (sr.id() < 0)
  {
      qDebug() << "FAIL: No SR in mapPointToMGRS";
      return returnMgrs;
  }

  QList<Point> coordinates;
  coordinates.append(point);

  const MgrsConversionMode method = MgrsConversionMode::Automatic;
  const int digits = 5;
  QStringList mgrss = sr.toMilitaryGrid(method, digits, false, true, coordinates);

  if (mgrss.length() < 1)
      return returnMgrs;

  returnMgrs = mgrss.at(0);

  if (0)
      qDebug() << "mapPointToMGRS = " << returnMgrs;

  return returnMgrs;
}

// returns symbol properties as a slightly more friendly property set
QVariantMap MapController::symbolNameOrId2VariantMap(QString nameOrId)
{
  QVariantMap vMap;

  SymbolProperties symbol(nameOrId, dictionary);

  int key_size = symbol.keywords().size();
  int val_size = symbol.values().size();
  Q_UNUSED(key_size)
  Q_UNUSED(val_size)

  QHash<QString, QString> values = symbol.values();
  QStringList keywords = symbol.keywords();

  QHashIterator<QString, QString> i(values);
  while (i.hasNext())
  {
    i.next();
    qDebug() << i.key() << ": " << i.value();
    vMap[i.key()] = i.value();
  }

  QString keywordString;
  QStringListIterator j(keywords);
  while (j.hasNext())
  {
      keywordString.append(j.next());
      keywordString.append(";");
  }
  vMap["Tags"] = keywordString;

  return vMap;
}

void MapController::handleVisibilityAnalysisClicked()
{ 
  // IMPORTANT: Assumes gpk is in same folder as .exe
  QString currentPath = QCoreApplication::applicationDirPath();

  QFileInfo gpkFile(currentPath + QDir::separator() + "FastVisibilityByDistance.gpk");

  if (!gpkFile.exists())
  {
    QString msg = "Could not find required GPK at: " + gpkFile.absoluteFilePath();
    qDebug() << "IMPORTANT: " << msg;
    QMessageBox messageBox;
    messageBox.setText(msg);
    messageBox.exec();
    return;
  }

  if (visibilityInProgress)
  {
    qDebug() << "Visibility/Viewshed Already In Progress...";
    return;
  }
  visibilityInProgress = true;

  QProgressDialog* m_progressDialog = new QProgressDialog();
  // set up progress bar
  m_progressDialog->setBaseSize(280, 20);
  m_progressDialog->setRange(0, 0);
  m_progressDialog->setValue(0);
  m_progressDialog->setCancelButton(0);
  m_progressDialog->setLabelText("Computing the viewshed...");
  m_progressDialog->setVisible(true);

  if (viewshedService.isEmpty())
  {
    m_progressDialog->setLabelText("Starting Viewshed Service...");
    viewshedService = EsriRuntimeQt::LocalGeoprocessingService(gpkFile.absoluteFilePath());
    viewshedService.setServiceType(EsriRuntimeQt::GPServiceType::SubmitJobWithMapServerResult);
    viewshedService.startAndWait();
    m_progressDialog->setLabelText("Computing the viewshed...");
  }

  // the ownship is the viewshed point
  EsriRuntimeQt::Point viewshedPoint = lastOwnshipPoint;

  EsriRuntimeQt::SpatialReference srMap = map->spatialReference();
  if (!EsriRuntimeQt::GeometryEngine::within(viewshedPoint, map->extent(), srMap))
    qDebug() << "Warning: Viewshed point not on the map.";

  if (!(viewshedLayer.isEmpty() || viewshedLayer.isNull()))
  {
    // remove the previous viewshed layer's outputs
    map->removeLayer("Viewshed");
  }

  viewshedGraphicLayer.removeAll();

  EsriRuntimeQt::SimpleMarkerSymbol sms(QColor("Red"), 14, EsriRuntimeQt::SimpleMarkerSymbolStyle::X);
  EsriRuntimeQt::Graphic viewshedPointGraphic(viewshedPoint, sms);
  viewshedGraphicLayer.addGraphic(viewshedPointGraphic);

  QString viewshedUrl = viewshedService.url();

  // IMPORTANT: The remaining parameters are dependent on the
  // GPK used
  QString modelName = "Fast%20Visibility%20By%20Distance";
  geoprocessor = EsriRuntimeQt::Geoprocessor(viewshedUrl + "/" + modelName);

  geoprocessor.setProcessSR(srMap);
  geoprocessor.setOutSR(srMap);

  QList<EsriRuntimeQt::GPParameter> gpInputParams;

  EsriRuntimeQt::GPFeatureRecordSetLayer gpInputViewshedPoint("Observer");
  gpInputViewshedPoint.setSpatialReference(srMap);
  gpInputViewshedPoint.setGeometryType(GeometryType::Point);
  gpInputViewshedPoint.addGraphic(viewshedPointGraphic);

  // TODO: make this a setting on the menu
  EsriRuntimeQt::GPLinearUnit gpInputViewshedDistance("Radius");
  gpInputViewshedDistance.setUnits("esriMeters");
  double viewshedDistance = 5000;
  gpInputViewshedDistance.setDistance(viewshedDistance);

  GPDouble gpInputHeight("ObserverHeight");
  double observerHeight = 2.0;
  gpInputHeight.setValue(observerHeight);

  gpInputParams.append(gpInputViewshedPoint);
  gpInputParams.append(gpInputViewshedDistance);
  gpInputParams.append(gpInputHeight);

  // execute the geoprocessing request
  connect(&geoprocessor, SIGNAL(gpSubmitJobComplete(EsriRuntimeQt::GPJobResource)), this, SLOT(onSubmitJobComplete(EsriRuntimeQt::GPJobResource)));
  connect(&geoprocessor, SIGNAL(gpError(EsriRuntimeQt::ServiceError)), this, SLOT(onGpError(EsriRuntimeQt::ServiceError)));

  geoprocessor.submitJob(gpInputParams);

  m_progressDialog->setVisible(false);
}

void MapController::onSubmitJobComplete(const EsriRuntimeQt::GPJobResource& jobResource)
{
  JobStatus gpStatus = jobResource.jobStatus();

  QString jobID = jobResource.jobID();
  qDebug() << "GP SubmitJobComplete - gpStatus: " << int(gpStatus) << ", Job ID:" << jobID;
  int i = 0;
  const int MAX_TIMEOUT_COUNT = 40;
  while ((gpStatus != JobStatus::Succeeded) &&
         (gpStatus != JobStatus::Failed) &&
         (i < MAX_TIMEOUT_COUNT))
  {
      GPJobResource jobResourceNext = geoprocessor.jobStatusAndWait(jobID);
      gpStatus = jobResourceNext.jobStatus();

      qDebug() << "GP Job in progress(jobStatusAndWait) - gpStatus: " << int(gpStatus) << ", Job ID:" << jobID;

      if (gpStatus == JobStatus::Succeeded)
      {
        qDebug() << "GP Job Succeeded - adding results to map";

        viewshedLayer = ArcGISDynamicMapServiceLayer(geoprocessor.url(), jobResourceNext);
        viewshedLayer.setName("Viewshed");
        map->addLayer(viewshedLayer);

        break;
      }

      const int sleepMilliSecs = 500;
// Workaround/Note: QThread::msleep/sleep not public until Qt5
#ifdef _WIN32
  ::Sleep(sleepMilliSecs);
#else
  usleep(sleepMilliSecs * (10*10*10)); // convert to usec (10^-6) to msec (10^-3)
#endif

      i++;
  }

  disconnect(&geoprocessor, SIGNAL(gpSubmitJobComplete(EsriRuntimeQt::GPJobResource)), this, SLOT(onSubmitJobComplete(EsriRuntimeQt::GPJobResource)));
  disconnect(&geoprocessor, SIGNAL(gpError(EsriRuntimeQt::ServiceError)), this, SLOT(onGpError(EsriRuntimeQt::ServiceError)));

  visibilityInProgress = false;
}

void MapController::onGpError(const EsriRuntimeQt::ServiceError& error)
{
  qDebug() << "GP Error! Message=" << error.message();

  disconnect(&geoprocessor, SIGNAL(gpSubmitJobComplete(EsriRuntimeQt::GPJobResource)), this, SLOT(onSubmitJobComplete(EsriRuntimeQt::GPJobResource)));
  disconnect(&geoprocessor, SIGNAL(gpError(EsriRuntimeQt::ServiceError)), this, SLOT(onGpError(EsriRuntimeQt::ServiceError)));

  visibilityInProgress = false;
}
