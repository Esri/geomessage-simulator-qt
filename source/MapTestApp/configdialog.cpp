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

#include <QtGui>

#include "configdialog.h"
#include "pages.h"

ConfigDialog::ConfigDialog(SampleMap *iMap)
{
    m_pMap = iMap;

    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(128);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;
    ConfigurationPage* confPage = new ConfigurationPage;
    connect(confPage,SIGNAL(ConfigurationChanged()),this, SLOT(handleConfigurationChanged()));
    pagesWidget->addWidget(confPage);
    pagesWidget->addWidget(new SymbolPage);

    QPushButton *closeButton = new QPushButton(tr("Close"));

    createIcons();
    contentsWidget->setCurrentRow(0);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Config Dialog"));
}

void ConfigDialog::handleConfigurationChanged()
{
    if(m_pMap)
    {
        const QVariant defaultValue;
        QSettings settings("ESRI","MapTestAppConfig");
        int insetmapdockEnumIndex = settings.value("insetmapdock", defaultValue).toInt();
        int insetmapsizeEnumIndex = settings.value("insetmapsize", defaultValue).toInt();
        int compassrosedockEnumIndex = settings.value("compassrosedock", defaultValue).toInt();
        int compassrosesizeEnumIndex = settings.value("compassrosesize", defaultValue).toInt();

        Q_UNUSED(compassrosedockEnumIndex)
        Q_UNUSED(compassrosesizeEnumIndex)

        // TODO: update compass rose
        //       if(m_pMap->IsMapCompassVisible())
        //       {
        //           m_pMap->ShowMapCompass(true,(MapElementDock)compassrosedockEnumIndex,(MapElementSize)compassrosesizeEnumIndex);
        //       }

       if(m_pMap->IsInsetMapVisible())
       {
           m_pMap->ShowInsetMap(true, (MapElementDock)insetmapdockEnumIndex,(MapElementSize)insetmapsizeEnumIndex);
       }
    }
}

void ConfigDialog::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    configButton->setIcon(QIcon(":/images/config.png"));
    configButton->setText(tr("Configuration"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *queryButton = new QListWidgetItem(contentsWidget);
    queryButton->setIcon(QIcon(":/images/query.png"));
    queryButton->setText(tr("Symbol"));
    queryButton->setTextAlignment(Qt::AlignHCenter);
    queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}
