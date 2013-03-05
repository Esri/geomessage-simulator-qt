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

#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H

#include <QObject>

#include "DictionaryImageProvider.h"
#include "SymbolDictionary.h"

using namespace EsriRuntimeQt;

struct FilterObj
{
  FilterObj(QString name, QString value) : filterName(name), filterValue(value) {}

  QString filterName;
  QString filterValue;
};

class SearchController : public QObject
{
  Q_OBJECT
public:
  explicit SearchController(SymbolDictionary* inputDictionary, QObject* parent = 0);
  ~SearchController();

  void initController();

private:
  SymbolDictionary* dictionary;
  QMap<QString, QList<FilterObj> > spotReportCategories;
  QString currentSpotReportCategory;

  QStringList recentSpotReportSearchResults;

  void loadSpotReportCategories();
  void updateKeywordList();
  void updateSearch();

  QStringList userAppliedKeywords;
  QStringList currentSearchResults;
  QMap<QString, QStringList> currentFilters;

signals:
  void newCategoryList(QVariant newList);
  void newKeywordList(QVariant newList);
  void newRecentResultList(QVariant newList);
  void newSearchResultList(QVariant newList);
  void newUserAppliedKeywordList(QVariant newList);
  void newResultCount(QVariant count);

public slots:
  void handleApplyCategory(QVariant category);
  void handleApplyKeyword(QVariant keyword);
  void handleClearSearchParameters();
  void handleResultSelected(QVariant resultName);
  void handleSearch(QVariant searchText);
  void handleUpdateAffiliation(QVariant affiliation);
  void handleUserAppliedKeywordSelected(QVariant keyword);
};

#endif // SEARCHCONTROLLER_H
