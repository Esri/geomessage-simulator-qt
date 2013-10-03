/*******************************************************************************
 * Copyright 2012 Esri
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

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtGui>
#include <QListWidgetItem>
#include <QLabel>
#include <QVBoxLayout>

#include "SymbolDictionary.h"

using namespace EsriRuntimeQt;

namespace Ui {
  class Dialog;
}

/*!
    \class Dialog

    \brief The Dialog class provides the a reference implementation for
    the Search API portion of the ArcGIS SDK for Qt.

    Feel free to browse through this class and get to know the various methods
    available on the Search API. This sample also shows you how to implement
    pagination, how to visually indicate to the user

    Finally, this sample demonstrates how your user can get to a symbol with no typing
    and just a few taps. You can minimize even these by priming the Search with
    pre-selected filters. In any case, the user needs to have no previous knowledge
    of the 2525C standard, or the Appendices, or 15 character symbol id codes.
*/
class Dialog : public QDialog
{
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = 0);
  ~Dialog();

private:

  void displayFilterValues(const QString & filter);
  void handlePrevNext();
  QStringList inPlaceSubtract(const QStringList & all, const QStringList & remove );
  void addWidgetItem(const QString & name, bool append=true);
  void setupKeywordsUI();

  QVariantMap symbolNameOrId2VariantMap(QString nameOrId);

  Ui::Dialog *ui;
  SymbolDictionary dictionary;
  QString currentFilter;

  QStringList currentKeywords;
  QMap<QString, QStringList> currentFilters;

  QList<QListWidgetItem*> listItems;

  QStringList paths;
  QListIterator < QString > fwdIterator;
  QListIterator < QString > bwdIterator;
  int displaySize;

  QDialog* dlg ;
  QLabel* lblImage ;
  QLabel* lblSymbolName;
  QLabel* lblSymbolId;
  QLabel* lblStyleFile;
  QLabel* lblCategory;
  QLabel* lblTags;
  QVBoxLayout* layout;

private slots:
  void on_search_itemClicked(QListWidgetItem* item);
  void on_btnReset_clicked();
  void on_filters_itemClicked(QListWidgetItem* item);
  void on_validValues_itemClicked(QListWidgetItem* item);
  void on_keywords_itemClicked(QListWidgetItem* item);
  void on_appliedKeywords_itemClicked(QListWidgetItem* item);
  void on_btnSearch_clicked();
  void on_btnNext_clicked();
  void on_btnPrevious_clicked();
};

#endif // DIALOG_H
