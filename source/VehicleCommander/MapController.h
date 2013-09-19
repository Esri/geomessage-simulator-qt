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

#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <QObject>
#include <QUdpSocket>
#include <QMouseEvent>

#include "IdentifyTask.h"
#include "Geometry.h"
#include "GraphicsLayer.h"
#include "Layer.h"
#include "Map.h"
#include "MarkerSymbol.h"
#include "Message.h"
#include "MessageGroupLayer.h"
#include "MessageProcessor.h"
#include "SymbolDictionary.h"

#include "AppConfigDialog.h"
#include "GPSSimulator.h"

#include "simplegraphicoverlay.h"

using namespace EsriRuntimeQt;

class MapController : public QObject
{
  Q_OBJECT
public:
  static const double METERS_PER_MILE;
  static const int MILLISECONDS_PER_HOUR;

  explicit MapController(Map* inputMap, MapGraphicsView *inputGraphicsView, QObject* parent = 0);
  virtual ~MapController();
  void setMap(Map* newMap);
  void initController();
  void mapReady();

private:

  struct UserData
  {
    QString name;
    QString id;
    QString code;
    QString engaged;
  };

  // Methods
  static bool isDuplicateTimes(QTime time1, QTime time2);

  QByteArray createChemLightReport(Point& location, const QString& messageID, const QString& action);
  QByteArray createGenericReport(const QString& reportName, const QString& messageID, const QString& action);
  QByteArray createPositionReport(const QString& action);
  QByteArray createSpotReport(QVariant data);
  QString decimalDegreesToDMS(double coord);
  bool filterMessages(const QString& strMessages);

  void readAppConfig();
  void transmitMessages(QByteArray datagram);  
  void setMessageStream(const QString& stream);
  void setMessageStream(QXmlStreamReader& reader);
  void setMessageStream(QFile& file);
  bool readMessages(QXmlStreamReader& reader);
  bool readMessage(QXmlStreamReader& reader);
  void skipUnknownElement(QXmlStreamReader& reader);
  QString getReaderValue(QXmlStreamReader& reader);
  void readToElementEnd(QXmlStreamReader& reader);

  void handleMapMousePressLeft(QPointF mousePoint);
  void handleMapMousePressRight(QPointF mousePoint);

  void showHideMe(bool show, Point atPoint, double withHeading);
  Point MGRSToMapPoint(QString mgrs);
  QString mapPointToMGRS(Point point);

  void returnPoint(Point geometry);
  void sendChemLightMessage(Point pos);

  QVariantMap symbolNameOrId2VariantMap(QString nameOrId);

  // Attributes
  Map* map;
  MapGraphicsView* mapGraphicsView;
  SymbolDictionary dictionary;
  MessageGroupLayer messagGroupLayer;
  MessageProcessor messageProcessor;

  bool buddiesLayerVisible;
  GraphicsLayer buddiesLayer;
  bool observationsLayerVisible;
  GraphicsLayer observationsLayer;
  GraphicsLayer chemLightLayer;
  GraphicsLayer mouseClickGraphicLayer;

  Envelope originalExtent;
  double originalScale;

  QUdpSocket *udpSocket;
  int broadcastPort;
  int broadcastInterval;

  GPSSimulator simulator;

  QTimer* positionReportTimer;

  UserData userData;

  bool acceptPositionReports;
  bool acceptSpotReports;

  QString chemLightColorStr;

  AppConfigDialog* appConfigDialog;

  QPointF myPreviousLocation;
  QPoint previousMousePressPosScreen;
  Point previousMousePressPosMap;

  QStringList nonBasemapLayerNames;

  Point ownshipStartingMapPoint;
  Point lastOwnshipPoint;
  double lastHeading;
  bool showOwnship, followOwnship;
  bool isMapReady;

  SimpleGraphicOverlay* drawingOverlay;

  enum mouseStateEnum { MouseStateNone, MouseStateMenuClicked, MouseStateWaitingForMapPoint };
  mouseStateEnum mouseState;

signals:
  void clearChemLightUI();
  void headingChanged(QVariant newHeading);
  void newBasemapList(QVariant newList);
  void newPointCreated(QVariant lon, QVariant lat, QVariant mgrs);
  void positionChanged(QVariant newPosition);
  void speedChanged(QVariant newSpeed);
  void identifyComplete(QList<IdentifyResult> results);

public slots:
  void handleBasemapChange(QString name);
  void handleCenterOnSelfBtn();
  void handleChemLightSelected(QString color);
  void handleGetPointFromMap();
  void handleHomeClicked();
  void handleOpenCOA();
  void handleOpenMPK();
  void toggleLayerVisibility(QString layerName);
  void handlePan(QString direction);
  void handlePositionAvailable(QPointF pos, double orientation);
  void handleResetMap();
  void handleToggleBroadcastPosition(bool state);
  void handleToggleBuddies(bool state);
  void handleToggleEmergencyClicked(bool state);
  void handleToggleObservations(bool state);
  void handleToggleReceivePositionReports(bool state);
  void handleToggleReceiveSpotReports(bool state);
  void openAppConfigDialog();
  void handleToggleShowMe(bool state);
  void handleToggleFollowMe(bool state);
  void handleZoomIn();
  void handleZoomOut();
  void processPendingDatagrams();
  void sendSpotReport(QVariant data);
  void applyAppConfigSettings();
  void mousePress(QMouseEvent mouseEvent);
  void onIdentifyComplete(QList<IdentifyResult> results);

  /*!
    \brief Slot to tell MainView that a UI element was clicked, preventing it from running "identify"
  */
  void uiElementClicked();

private slots:
  void sendPositionMessage();
};

#endif // MAPCONTROLLER_H
