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

#include "dialog.h"
#include "QDebug"
#include "ui_dialog.h"
#include <QtXml>

#include "SymbolProperties.h"

const int ICON_LIST_DISPLAY_SIZE = 18;

/*!
 \brief Constructs the Dialog UI and sets up the Symbol Dictionary object.

 \sa on_btnReset_clicked()
*/
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    fwdIterator (QListIterator<QString> (paths)),
    bwdIterator (QListIterator<QString> (paths))
{
  ui->setupUi(this);

  //construction of objects for showing the symbols in a pop up dialog
  dlg = new QDialog(this,Qt::Dialog);
  layout = new QVBoxLayout(dlg);

  lblImage = new QLabel(dlg);
  lblSymbolName = new QLabel(dlg);
  lblSymbolId = new QLabel(dlg);
  lblStyleFile = new QLabel(dlg);
  lblCategory = new QLabel(dlg);
  lblTags = new QLabel(dlg);

  layout->addWidget(lblImage);
  layout->addWidget(lblSymbolName);
  layout->addWidget(lblSymbolId);
  layout->addWidget(lblStyleFile);
  layout->addWidget(lblCategory);
  layout->addWidget(lblTags);

  displaySize = ICON_LIST_DISPLAY_SIZE;

  ui->search->setIconSize(QSize(64, 64));
  ui->search->setStyleSheet("QListWidget::item { border: 1px solid #d9d9d9; border-top-color: transparent; border-bottom-color: transparent; }  QListWidget::item:hover { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1); border: 1px solid #bfcde4; }  QListWidget::item:selected { border: 1px solid #567dbc; }  QListWidget::item:selected:active{ background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6ea1f1, stop: 1 #567dbc); }  QListWidget::item:selected:!active { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6b9be8, stop: 1 #577fbf); }");

  handlePrevNext();

  ui->filters->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->filters->setSelectionBehavior(QAbstractItemView::SelectRows);

  ui->validValues->setSelectionMode(QAbstractItemView::MultiSelection);
  ui->validValues->setSelectionBehavior(QAbstractItemView::SelectRows);

  ui->keywords->setSelectionMode(QAbstractItemView::MultiSelection);
  ui->keywords->setSelectionBehavior(QAbstractItemView::SelectRows);

  ui->appliedKeywords->setSelectionMode(QAbstractItemView::MultiSelection);
  ui->appliedKeywords->setSelectionBehavior(QAbstractItemView::SelectRows);

  dictionary = SymbolDictionary(SymbolDictionary::DictionaryType::Mil2525C);

  on_btnReset_clicked();
}

/*!
 \brief Destroys the UI and the dictionary object

*/
Dialog::~Dialog()
{

  if(dlg)
    delete dlg;

  delete ui;
}

/*!
 \brief fetches valid values for the given \a filter

 This method demonstrates the usage of the \a validValuesForFilter method of the
 Search API. These API returns these values as a \a QStringList which are then added
 to a \a QListWidget for the user.

*/
void Dialog::displayFilterValues(const QString & filter)
{
  ui->validValues->clear();
  ui->validValues->addItems(dictionary.filters()[filter]);

  for(int i = 0; i < ui->validValues->count(); i++)
  {
    ui->validValues->item(i)->setCheckState(Qt::Unchecked);
  }
}

/*!
 \brief Displays the list of valid Values available for the selected filter
 Filter Values that have already been selected are shown as checked

 \note connects the itemClicked signal of fiters(ListWidget) to this SLOT
 \sa displayFilterValues()
*/
void Dialog::on_filters_itemClicked(QListWidgetItem* item)
{
    if (!item)
        return;

  currentFilter = item->text();
  displayFilterValues(currentFilter);
}

/*!
 \brief modifies the search parameter by adding or removing a filter value
 \note connects the itemClicked signal of validValues(ListWidget) to this SLOT

 \param item
*/
void Dialog::on_validValues_itemClicked(QListWidgetItem* item)
{
  QString currentFilterValue = item->text();
  if(item->isSelected())
  {
    item->setCheckState(Qt::Checked);

    QStringList listKeyWords = dictionary.keywords();

    // add filter
    currentFilters[currentFilter].append(currentFilterValue);
  }
  else
  {
    item->setCheckState(Qt::Unchecked);

    // removeFilter
    if (currentFilters.keys().contains(currentFilter))
        currentFilters[currentFilter].removeAll(currentFilterValue);
  }

  setupKeywordsUI();
  on_btnSearch_clicked();
}

/*!
 \brief modifies the search parameter by adding a keyword

 \note connects the itemClicked signal of keywords(ListWidget) to this SLOT
 \param item
*/
void Dialog::on_keywords_itemClicked(QListWidgetItem* item)
{
  if(item->isSelected())
  {
    //applied keywords
    // Add keyword to the list of applied keywords
    this->currentKeywords.append(item->text());

    setupKeywordsUI();
    on_btnSearch_clicked();
  }
}

/*!
 \brief modifies the search parameter by removing a keyword

 \note connects the itemClicked signal of appliedKeywords(ListWidget) to this SLOT
 \param item
*/
void Dialog::on_appliedKeywords_itemClicked(QListWidgetItem* item)
{
  if(item->isSelected())
  {
    //Removes keyword from the list of applied keywords
    currentKeywords.removeAll(item->text());

    setupKeywordsUI();
    on_btnSearch_clicked();
  }
}


/*!
 \brief fetches the Previous set of symbol results - demonstrates pagination

 \note connects the clicked signal of btnPrevious to this SLOT
*/
void Dialog::on_btnPrevious_clicked()
{
  ui->search->clear();
  fwdIterator = bwdIterator;
  for(int i = 0; i < displaySize; i++)
  {
    if (bwdIterator.hasPrevious())
      addWidgetItem(bwdIterator.previous(), false);
  }

  handlePrevNext();
}

/*!
 \brief fetches the Next set of symbol results - demonstrates pagination

 \note connects the clicked signal of btnNext to this SLOT
*/
void Dialog::on_btnNext_clicked()
{
  ui->search->clear();
  bwdIterator = fwdIterator;

  for(int i = 0; i < displaySize; i++)
  {
    if (fwdIterator.hasNext())
    {
      addWidgetItem(fwdIterator.next(), true);
    }
  }
  handlePrevNext();
}

/*!
 \brief Invokes search on the dictionary and displays the first results
 \note connects the clicked signal of btnSearch to this SLOT
*/
void Dialog::on_btnSearch_clicked()
{
    paths.clear();

    QList<SymbolProperties> symbolList = dictionary.findSymbols(currentKeywords, currentFilters);

    QStringList listKeyWords = dictionary.keywords();

    ui->keywords->clear();
    ui->keywords->addItems(listKeyWords);

    QListIterator<SymbolProperties> it(symbolList);
    while (it.hasNext())
    {
        SymbolProperties symbolProps = it.next();

        if (symbolProps.name().size() > 0)
        {
          // qDebug() << "Adding Symbol: " << symbolProps.name();
          paths.append(symbolProps.name());
        }
    }

  fwdIterator = QListIterator<QString> (paths);
  bwdIterator = QListIterator<QString> (paths);
  ui->lblResults->setText(QString("%1 symbols found").arg(paths.count()));

  on_btnNext_clicked();
}


/*!
 \internal
 \brief helper method to add a symbol item image to the display

 \note adds widget items to the search widget List

 \param name
 \param append
*/
void Dialog::addWidgetItem(const QString & name, bool append)
{
  QListWidgetItem *item ;

  QImage image = dictionary.symbolImage(name, 64, 64);
  if (image.isNull())
  {
    item = new QListWidgetItem(name);
  }
  else
  {
    item = new QListWidgetItem(QIcon(QPixmap::fromImage(image)), name);
  }

  if(append)
    ui->search->insertItem(ui->search->count(), item);
  else
    ui->search->insertItem(0, item);

  item->setSizeHint(QSize(75, 100));
  item->setToolTip(name);
}


/*!
 \internal
 \brief handles enable/disable logic of the Next and Previous buttons

*/
void Dialog::handlePrevNext()
{
  ui->btnNext->setEnabled(fwdIterator.hasNext());
  ui->btnPrevious->setEnabled(bwdIterator.hasPrevious());
}

/*!
 \internal

 \brief The dictionary returns keywords in the order of their frequency.
  this method ensures that priority order of keywords is maaintained

 \param all
 \param remove
 \return QStringList
*/
QStringList Dialog::inPlaceSubtract(const QStringList & all, const QStringList & remove )
{
  QStringList ret;
  foreach(QString s, all)
  {
    if(! remove.contains(s))
      ret.append(s);
  }
  return ret;
}

/*!
 \brief initializes the dialog back to its original state.

 \note if the user provided contents in a "Filters.xml" file, the dialog will use
 those values, else it will reset to a 'blank' search
*/
void Dialog::on_btnReset_clicked()
{
  currentKeywords.clear();
  currentFilters.clear();

  ui->filters->clear();
  ui->validValues->clear();
  ui->keywords->clear();
  ui->appliedKeywords->clear();
  ui->search->clear();
  ui->lblResults->setText("Results");

  if(!dictionary.isNull())
  {
    ui->filters->setVisible(true);
    ui->validValues->setVisible(true);
    ui->filters->addItems(dictionary.filters().keys());

    QListWidgetItem* item = ui->filters->itemAt(0,0);
    ui->filters->setCurrentItem(item, QItemSelectionModel::SelectCurrent);
    on_filters_itemClicked(item);

    setupKeywordsUI();
    on_btnSearch_clicked();
  }
  else
  {
    ui->filters->setEnabled(false);
    ui->validValues->setEnabled(false);
    ui->keywords->setEnabled(false);
    ui->appliedKeywords->setEnabled(false);
    ui->search->setEnabled(false);
    ui->lblResults->setText("Invalid Symbol Dictionary");
    ui->btnReset->setEnabled(false);
    ui->btnSearch->setEnabled(false);
  }
}

/*!
 \internal
 \brief helper method to set up the UI for the Available keywords and the ones already applied
*/
void Dialog::setupKeywordsUI()
{
  ui->appliedKeywords->clear();
  ui->appliedKeywords->addItems(currentKeywords);
}

// returns symbol properties as a slightly more friendly property set
QVariantMap Dialog::symbolNameOrId2VariantMap(QString nameOrId)
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

/*!
 \brief Launches a small dialog-box showing info about the currently selected symbol.

 This method demonstrates the usage of the \a getSymbolInfo method of the
 Search API. These API returns these values as a \a QVariantMap with Key-Value pairs
 that refer to each of the \a SymbolInfos flags passed in.

 The method grabs this information from the API and then displays it to the user
 in a dialog box.
*/
void Dialog::on_search_itemClicked(QListWidgetItem* item)
{
  QString symbolName = item->text();
  QVariantMap vMap = symbolNameOrId2VariantMap(symbolName);

  if (vMap.count() == 0)
    return;

  QImage image = dictionary.symbolImage(symbolName, 150, 150);
  lblImage->setPixmap(QPixmap::fromImage(image));

  lblSymbolId->setText("Symbol Id : " % vMap.value("SymbolID").toString() % "\n");
  lblSymbolName->setText("Symbol Name : " % symbolName % "\n");
  lblStyleFile->setText("Stylefile : " % vMap.value("StyleFile").toString() % "\n");
  lblCategory->setText("Category : " % vMap.value("Category").toString() % "\n");
  lblTags->setText("Keywords : " % vMap.value("Tags").toStringList().join("\n") % "\n");

  dlg->setModal(true);
  dlg->setWindowTitle("Symbol Info");

  dlg->show();
}
