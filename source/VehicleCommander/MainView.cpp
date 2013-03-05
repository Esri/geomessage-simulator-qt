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

#include <QWidget>
#include <QMainWindow>
#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeProperty>
#include <QDeclarativeContext>
#include <QGraphicsWidget>
#include <QGraphicsLayoutItem>
#include <QGraphicsLinearLayout>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QFile>
#include <QDir>
#include <QString>
#include <QVariant>
#include <QStringList>

#include <math.h>

#include "ArcGISRuntime.h"
#include "Exception.h"
#include "Map.h"
#include "ArcGISLocalTiledLayer.h"
#include "ArcGISTiledMapServiceLayer.h"
#include "ArcGISLocalDynamicMapServiceLayer.h"
#include "SymbolDictionary.h"

#include "MainView.h"
#include "DictionaryImageProvider.h"

static const QString UI_OVERLAY_PATH("qrc:/Resources/qml/MainOverlay.qml");

MainView::MainView(QWidget* parent) :
  overlayWidget(new QGraphicsWidget()),
  map(0),
  mapController(0),
  searchController(0),
  overlayUI(0),
  QMainWindow(parent)
{
  this->showMaximized();

  try
  {
    setUI();
  }
  catch (Exception ex)
  {
    this->showMaximized();
    this->setWindowTitle("Vehicle Commander");
    qDebug()<<"Exception: "<<QString(ex.what());
    return;
  }

  this->setWindowTitle("Vehicle Commander Reference Application");
}

MainView::~MainView()
{
  if (mapController)
    delete mapController;
  if (searchController)
    delete searchController;
  if (overlayUI)
    delete overlayUI;
  if (engine)
    delete engine;
}

QString MainView::getPathSampleData()
{
  QString path = ArcGISRuntime::installDirectory();
  path.append("/sdk/samples/data");
  QDir dataDir(path); // using QDir to convert to correct file separator
  QString pathSampleData = dataDir.path() + QDir::separator();

  return pathSampleData;
}

void MainView::setUI()
{
  // Remove the frame, border, and title bar of the window
  this->setWindowFlags(Qt::CustomizeWindowHint);

  map = Map::create(this);
  map->setWrapAroundEnabled(true);
  map->setShowingEsriLogo(false);

  QString dataPath = getPathSampleData();

  // TODO: put your own high res dataset path here if desired
  QString dataPathTpk = getPathSampleData() + "tpks" + QDir::separator();
  QString loadTpk = dataPathTpk + "Imagery";

  //////////////////////////////////////////////////////////////////
  // For a disconnected, simple low res test, this will use sdk/samples/data/tpk/Topographic.tpk
  // but you won't be able to see detailed symbology
  //  QString fallbackTpk = dataPathTpk + "Topographic.tpk";
  //  if (!(QFile(loadTpk).exists() || QDir(loadTpk).exists()))
  //      loadTpk = fallbackTpk;

  if ((QFile(loadTpk).exists() || QDir(loadTpk).exists()))
  {
    // use this local map
    ArcGISLocalTiledLayer tiledLayer(loadTpk);
    tiledLayer.setName("Basemap");
    map->addLayer(tiledLayer);
  }
  else
  {
    // use an online map
    ArcGISTiledMapServiceLayer tiledLayer("http://services.arcgisonline.com/ArcGIS/rest/services/NatGeo_World_Map/MapServer");
    // -or- ArcGISTiledMapServiceLayer tiledLayer("http://services.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer");

    map->addLayer(tiledLayer);
  }

  const bool LOAD_MPK = false;
  if (LOAD_MPK)
  {
    // using sdk/samples/data/mpks/USCitiesStates.mpk
    QString dataPathMpk = getPathSampleData() + "mpks" + QDir::separator() + "USCitiesStates.mpk";
    ArcGISLocalDynamicMapServiceLayer dynamicLayer = ArcGISLocalDynamicMapServiceLayer(dataPathMpk);
    dynamicLayer.setName("Cities");
    map->addLayer(dynamicLayer);
  }

  setCentralWidget(map);

  if (!map)
  {
    qCritical() << "Unable to create map.";
    return;
  }

  QObject::connect(map, SIGNAL(mapReady()), this, SLOT(onMapReady()));

  dictionary = SymbolDictionary(SymbolDictionary::Mil2525C);

  searchController = new SearchController(&dictionary);

  mapController = new MapController(map, this);

  // invoke the QML UI
  engine = new QDeclarativeEngine();
  imageProvider = new DictionaryImageProvider(&dictionary);
  engine->addImageProvider(QLatin1String("dictionary"), imageProvider);
  QDeclarativeComponent component(engine, QUrl(UI_OVERLAY_PATH));
  overlayUI = component.create();
  if (!overlayUI)
  {
    qDebug() << "Failed to load UI overlay";
    qDebug() << component.errorString();
    QMessageBox box;
    box.setText("Error:\n" + component.errorString());
    box.exec();
    return;
  }

  // Create a container for the UML UI and add it to the scene
  overlayWidget = new QGraphicsWidget();
  QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(overlayWidget);
  layout->setContentsMargins(0, 0, 0, 0);
  QGraphicsLayoutItem* qmlUILayout = qobject_cast<QGraphicsLayoutItem*>(overlayUI);
  layout->addItem(qmlUILayout);
  overlayWidget->setLayout(layout);
  map->scene()->addItem(overlayWidget);

  // Hook up the main overlay UI
  connect(overlayUI, SIGNAL(basemapChanged(QString)), mapController, SLOT(handleBasemapChange(QString)));
  connect(overlayUI, SIGNAL(homeClicked()), mapController, SLOT(handleHomeClicked()));
  connect(overlayUI, SIGNAL(chemLightSelected(QString)), mapController, SLOT(handleChemLightSelected(QString)));
  connect(overlayUI, SIGNAL(zoomInClicked()), mapController, SLOT(handleZoomIn()));
  connect(overlayUI, SIGNAL(zoomOutClicked()), mapController, SLOT(handleZoomOut()));
  connect(overlayUI, SIGNAL(panClicked(QString)), mapController, SLOT(handlePan(QString)));
  connect(overlayUI, SIGNAL(emergencyClicked(bool)), mapController, SLOT(handleToggleEmergencyClicked(bool)));
  connect(mapController, SIGNAL(clearChemLightUI()), overlayUI, SLOT(clearChemLightUI()));
  connect(mapController, SIGNAL(positionChanged(QVariant)), overlayUI, SLOT(updateLocation(QVariant)));
  connect(mapController, SIGNAL(speedChanged(QVariant)), overlayUI, SLOT(updateSpeed(QVariant)));
  connect(mapController, SIGNAL(speedChanged(QVariant)), overlayUI, SLOT(updateAltitude(QVariant)));
  connect(mapController, SIGNAL(headingChanged(QVariant)), overlayUI, SLOT(updateHeading(QVariant)));
  connect(mapController, SIGNAL(newBasemapList(QVariant)), overlayUI, SLOT(updateBasemapList(QVariant)));
  connect(mapController, SIGNAL(identifyComplete(QList<IdentifyResult>)),
          this, SLOT(showIdentifyPanel(QList<IdentifyResult>)));

  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateNorthArrow()));
  timer->start(1000 / 24);

  mapController->initController();

  // Hook up the main menu component UI
  mainMenuUI = overlayUI->findChild<QObject*>("mainMenu");
  if (mainMenuUI)
  {
    connect(mainMenuUI, SIGNAL(showBuddiesClicked(bool)), mapController, SLOT(handleToggleBuddies(bool)));
    connect(mainMenuUI, SIGNAL(receiveBuddyPositionsClicked(bool)), mapController, SLOT(handleToggleReceivePositionReports(bool)));
    connect(mainMenuUI, SIGNAL(exitClicked()), this, SLOT(close()));
    connect(mainMenuUI, SIGNAL(showObservationsClicked(bool)), mapController, SLOT(handleToggleObservations(bool)));
    connect(mainMenuUI, SIGNAL(receiveSpotReportsClicked(bool)), mapController, SLOT(handleToggleReceiveSpotReports(bool)));
    connect(mainMenuUI, SIGNAL(aboutMeClicked()), mapController, SLOT(openAppConfigDialog()));
    connect(mainMenuUI, SIGNAL(showMeToggled(bool)), mapController, SLOT(handleToggleShowMe(bool)));
    connect(mainMenuUI, SIGNAL(followMeToggled(bool)), mapController, SLOT(handleToggleFollowMe(bool)));
    connect(mainMenuUI, SIGNAL(openCOA()), mapController, SLOT(handleOpenCOA()));
    connect(mainMenuUI, SIGNAL(openMPK()), mapController, SLOT(handleOpenMPK()));
    connect(mainMenuUI, SIGNAL(toggleLayerVisibility(QString)), mapController, SLOT(toggleLayerVisibility(QString)));
    connect(mainMenuUI, SIGNAL(sendMyPositionToggled(bool)), mapController, SLOT(handleToggleBroadcastPosition(bool)));
    connect(mainMenuUI, SIGNAL(resetMapClicked()), mapController, SLOT(handleResetMap()));
    connect(mainMenuUI, SIGNAL(clicked()), mapController, SLOT(uiElementClicked()));
  }

  if (map)
  {
    // hook up the handler for the mouse click
    connect(map, SIGNAL(mouseLeftClick(QPointF)), mapController, SLOT(handleMapMousePressLeft(QPointF)));
    connect(map, SIGNAL(mouseRightClick(QPointF)), mapController, SLOT(handleMapMousePressRight(QPointF)));
  }

  // Hook up the spot report component UI
  spotReportUI = overlayUI->findChild<QObject*>("spotReport");
  if (spotReportUI)
  {
    connect(spotReportUI, SIGNAL(reportReady(QVariant)), mapController, SLOT(sendSpotReport(QVariant)));
    connect(spotReportUI, SIGNAL(getPointFromMapClicked()), mapController, SLOT(handleGetPointFromMap()));
    connect(mapController, SIGNAL(newPointCreated(QVariant, QVariant, QVariant)), spotReportUI, SLOT(updateLocation(QVariant, QVariant, QVariant)));
  }

  // Hook up the search component UI
  searchUI = overlayUI->findChild<QObject*>("search");
  if (searchUI)
  {
    connect(searchUI, SIGNAL(performSearch(QVariant)), searchController, SLOT(handleSearch(QVariant)));
    connect(searchUI, SIGNAL(updateAffiliation(QVariant)), searchController, SLOT(handleUpdateAffiliation(QVariant)));
    connect(searchUI, SIGNAL(updateCategory(QVariant)), searchController, SLOT(handleApplyCategory(QVariant)));
    connect(searchUI, SIGNAL(keywordSelected(QVariant)), searchController, SLOT(handleApplyKeyword(QVariant)));
    connect(searchUI, SIGNAL(resultSelected(QVariant)), searchController, SLOT(handleResultSelected(QVariant)));
    connect(searchUI, SIGNAL(userAppliedKeywordSelected(QVariant)), searchController, SLOT(handleUserAppliedKeywordSelected(QVariant)));
    connect(searchUI, SIGNAL(clearClicked()), searchController, SLOT(handleClearSearchParameters()));
    connect(searchController, SIGNAL(newResultCount(QVariant)), searchUI, SLOT(handleUpdateSearchCount(QVariant)));
    connect(searchController, SIGNAL(newCategoryList(QVariant)), searchUI, SLOT(handleUpdateCategoryList(QVariant)));
    connect(searchController, SIGNAL(newSearchResultList(QVariant)), searchUI, SLOT(handleUpdateResultList(QVariant)));
    connect(searchController, SIGNAL(newKeywordList(QVariant)), searchUI, SLOT(handleUpdateKeywordList(QVariant)));
    connect(searchController, SIGNAL(newUserAppliedKeywordList(QVariant)), searchUI, SLOT(handleUpdateUserAppliedKeywordList(QVariant)));
    connect(searchController, SIGNAL(newRecentResultList(QVariant)), searchUI, SLOT(handleUpdateRecentList(QVariant)));

    searchController->initController();
  }

  map->scene()->setSceneRect(0, 0, qmlUILayout->preferredSize().width(), qmlUILayout->preferredSize().height());

}

void MainView::updateNorthArrow()
{
  QMetaObject::invokeMethod(overlayUI, "updateMapRotation", Q_ARG(QVariant, map->rotation()));
}

void MainView::showIdentifyPanel(QList<IdentifyResult> resultsList)
{
  QObject* idPanel = overlayUI->findChild<QObject*>("idPanel");
  QVariantList myList;
  foreach (IdentifyResult result, resultsList)
  {
    QVariantMap map;
    map["layerID"] = result.layerId();
    map["layerName"] = result.layerName();
    map["displayFieldName"] = result.displayFieldName();
    QVariantMap attributes;
    QList<QString> keys = result.attributes().keys();
    foreach (QString key, keys)
    //If you just use the attribute as is, the encoding will be wrong and
    //non-Latin characters will not print correctly. That's why we use
    //toByteArray and then fromUtf8.
    attributes[key] = QString::fromUtf8(result.attributes()[key].toByteArray());
    map["attributes"] = attributes;
    map["attributeKeys"] = qVariantFromValue<QStringList>(keys);
    //    map["geometry"] = qVariantFromValue<Geometry>(result.geometry());
    myList.append(map);
  }

  idPanel->setProperty("resultsList", qVariantFromValue(myList));
}

void MainView::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Escape)
    this->close();
  else
    QMainWindow::keyPressEvent(event);
}

void MainView::resizeEvent(QResizeEvent* event)
{
  if (map)
  {
    QRectF sceneRect = map->sceneRect();
    overlayWidget->setGeometry(sceneRect);

    // TODO: add any resizing logic here needed to adjust a scalebar or compass
  }

  QMainWindow::resizeEvent(event);
}

void MainView::onMapReady()
{
  // IMPORTANT: some geometry / spatial reference operations can not take place
  // until the map is valid and has a valid spatial reference
  // so you will need to wait for this event to start processing

  qDebug() << "Map is now ready";

  if (mapController)
  {
     mapController->mapReady();
  }
}
