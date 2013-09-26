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

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include <QGraphicsWidget>
#include <QtDeclarative/QDeclarativeEngine>

#include "Map.h"
#include "SymbolDictionary.h"

#include "MapController.h"
#include "SearchController.h"

using namespace EsriRuntimeQt;

class MainView : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainView(QWidget *parent = 0);
  ~MainView();

protected:
  void keyPressEvent(QKeyEvent* event);
  void resizeEvent(QResizeEvent* event);

private:
  static const QString MIL2525C_DIRECTORY;

  Map map;
  MapGraphicsView* mapGraphicsView;

  MapController* mapController;
  SearchController* searchController;
  DictionaryImageProvider* imageProvider;
  SymbolDictionary dictionary;

  QDeclarativeEngine* engine;
  QGraphicsWidget* overlayWidget;
  QObject* overlayUI;
  QObject* mainMenuUI;
  QObject* spotReportUI;
  QObject* searchUI;

  void setUI();
  QString getPathSampleData();

private slots:
  void showIdentifyPanel(QList<IdentifyResult>);
  void updateNorthArrow();
  void onMapReady();
};

#endif // MAINVIEW_H
