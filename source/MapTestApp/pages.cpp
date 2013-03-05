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

 #include "pages.h"

 ConfigurationPage::ConfigurationPage(QWidget *parent)
     : QWidget(parent)
 {
    // Inset map
     QGroupBox* insetMapMainGroup = new QGroupBox(tr("Inset Map"));

    //QGroupBox *insetMapDockGroup = new QGroupBox(tr("Dock"));
    QRadioButton *insetMapTopLeftCheckBox = new QRadioButton(tr("Top Left"));
    QRadioButton *insetMapTopRightCheckBox = new QRadioButton(tr("Top Right"));
    QRadioButton *insetMapBottomRightCheckBox = new QRadioButton(tr("Bottom Right"));
    QRadioButton *insetMapBottomLeftCheckBox = new QRadioButton(tr("Bottom Left"));

    // set inset map dock checkbox
    const QVariant defaultValue;
    QSettings settings("ESRI","MapTestAppConfig");
    int insetmapdockEnumIndex = settings.value("insetmapdock", defaultValue).toInt();

    switch(insetmapdockEnumIndex)
    {
    case 0:
        insetMapBottomLeftCheckBox->setChecked(true);
        break;
    case 1:
        insetMapTopLeftCheckBox->setChecked(true);
        break;
    case 2:
        insetMapTopRightCheckBox->setChecked(true);
        break;
    case 3:
        insetMapBottomRightCheckBox->setChecked(true);
        break;
    default:
        insetMapBottomLeftCheckBox->setChecked(true);
        break;

    }


    QVBoxLayout* insetMapDockLayout = new QVBoxLayout;
    insetMapDockLayout->addWidget(insetMapTopLeftCheckBox);
    insetMapDockLayout->addWidget(insetMapTopRightCheckBox);
    insetMapDockLayout->addWidget(insetMapBottomRightCheckBox);
    insetMapDockLayout->addWidget(insetMapBottomLeftCheckBox);

    connect(insetMapTopLeftCheckBox,SIGNAL(clicked(bool)),this,SLOT(handleInsetMapTopLeftCheckBox(bool)));
    connect(insetMapTopRightCheckBox,SIGNAL(clicked(bool)),this,SLOT(handleInsetMapTopRightCheckBox(bool)));
    connect(insetMapBottomLeftCheckBox,SIGNAL(clicked(bool)),this,SLOT(handleInsetMapBottomLeftCheckBox(bool)));
    connect(insetMapBottomRightCheckBox,SIGNAL(clicked(bool)),this,SLOT(handleInsetMapBottomRightCheckBox(bool)));



    // size
    //QGroupBox* insetMapSizeGroup = new QGroupBox(tr("Size"));

    QLabel *insetMapSizeLabel = new QLabel(tr("Size:"));
    QComboBox *insetMapSizeCombo = new QComboBox;
    insetMapSizeCombo->addItem(tr("Small"));
    insetMapSizeCombo->addItem(tr("Medium"));
    insetMapSizeCombo->addItem(tr("Large"));

    connect(insetMapSizeCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(handleInsetMapIndexChanged(int)));

    int insetmapsizeindex = settings.value("insetmapsize", defaultValue).toInt();
    qDebug() << "Setting map size combo index to " << insetmapsizeindex;
    insetMapSizeCombo->setCurrentIndex(insetmapsizeindex);

    QHBoxLayout* insetMapSizeLayout = new QHBoxLayout;
    insetMapSizeLayout->addWidget(insetMapSizeLabel);
    insetMapSizeLayout->addWidget(insetMapSizeCombo);


    QVBoxLayout* configInsetMapLayout = new QVBoxLayout;
    configInsetMapLayout->addLayout(insetMapDockLayout);
    configInsetMapLayout->addLayout(insetMapSizeLayout);

    insetMapMainGroup->setLayout(configInsetMapLayout);

    // Compass Rose  
/*
    QGroupBox *compassRoseGroup = new QGroupBox(tr("Compass Rose"));
    QRadioButton *compassRoseTopLeftCheckBox = new QRadioButton(tr("Top Left"));
    QRadioButton *compassRoseTopRightCheckBox = new QRadioButton(tr("Top Right"));
    QRadioButton *compassRoseBottomRightCheckBox = new QRadioButton(tr("Bottom Right"));
    QRadioButton *compassRoseBottomLeftCheckBox = new QRadioButton(tr("Bottom Left"));

    int compassrosedockEnumIndex = settings.value("compassrosedock", defaultValue).toInt();

    switch(compassrosedockEnumIndex)
    {
    case 0:
        compassRoseBottomLeftCheckBox->setChecked(true);
        break;
    case 1:
        compassRoseTopLeftCheckBox->setChecked(true);
        break;
    case 2:
        compassRoseTopRightCheckBox->setChecked(true);
        break;
    case 3:
        compassRoseBottomRightCheckBox->setChecked(true);
        break;
    default:
        compassRoseBottomLeftCheckBox->setChecked(true);
        break;

    }

    QVBoxLayout* compassRoseLayout = new QVBoxLayout;
    compassRoseLayout->addWidget(compassRoseTopLeftCheckBox);
    compassRoseLayout->addWidget(compassRoseTopRightCheckBox);
    compassRoseLayout->addWidget(compassRoseBottomRightCheckBox);
    compassRoseLayout->addWidget(compassRoseBottomLeftCheckBox);

    connect(compassRoseTopLeftCheckBox,SIGNAL(clicked(bool)),this,SLOT(handleCompassRoseTopLeftCheckBox(bool)));
    connect(compassRoseTopRightCheckBox,SIGNAL(clicked(bool)),this,SLOT(handleCompassRoseTopRightCheckBox(bool)));
    connect(compassRoseBottomLeftCheckBox,SIGNAL(clicked(bool)),this,SLOT(handleCompassRoseBottomLeftCheckBox(bool)));
    connect(compassRoseBottomRightCheckBox,SIGNAL(clicked(bool)),this,SLOT(handleCompassRoseBottomRightCheckBox(bool)));


    QLabel *compassRoseSizeLabel = new QLabel(tr("Size:"));
    QComboBox *compassRoseSizeCombo = new QComboBox;
    compassRoseSizeCombo->addItem(tr("Small"));
    compassRoseSizeCombo->addItem(tr("Medium"));
    compassRoseSizeCombo->addItem(tr("Large"));

    int compassrosesizeindex = settings.value("compassrosesize", defaultValue).toInt();
    qDebug() << "Setting map size combo index to " << compassrosesizeindex;
    compassRoseSizeCombo->setCurrentIndex(compassrosesizeindex);

    connect(compassRoseSizeCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(handleCompassRoseIndexChanged(int)));

    QHBoxLayout* compassRoseSizeLayout = new QHBoxLayout;
    compassRoseSizeLayout->addWidget(compassRoseSizeLabel);
    compassRoseSizeLayout->addWidget(compassRoseSizeCombo);


    QVBoxLayout* configCompassRoseLayout = new QVBoxLayout;
    configCompassRoseLayout->addLayout(compassRoseLayout);
    configCompassRoseLayout->addLayout(compassRoseSizeLayout);
    compassRoseGroup->setLayout(configCompassRoseLayout);
*/

    QPushButton *applyButton = new QPushButton(tr("Apply"));
    connect(applyButton,SIGNAL(clicked()),this,SLOT(ApplyButtonPushed()));

    // main layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(configGroup);
    mainLayout->addWidget(insetMapMainGroup);
//    mainLayout->addWidget(compassRoseGroup);
    mainLayout->addWidget(applyButton);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
 }

 void ConfigurationPage::ApplyButtonPushed()
 {
     emit ConfigurationChanged();
 }

 void ConfigurationPage::handleCompassRoseIndexChanged(int index)
 {
     QSettings settings("ESRI","MapTestAppConfig");
     settings.setValue("compassrosesize",QVariant(index));
 }

 void ConfigurationPage::handleInsetMapIndexChanged(int index)
 {
     QSettings settings("ESRI","MapTestAppConfig");
     settings.setValue("insetmapsize",QVariant(index));
 }

 void ConfigurationPage::handleCompassRoseTopLeftCheckBox(bool value)
 {
     if(value)
     {
         QSettings settings("ESRI","MapTestAppConfig");
         settings.setValue("compassrosedock",QVariant(1));
     }
 }

 void ConfigurationPage::handleCompassRoseBottomLeftCheckBox(bool value)
 {
     if(value)
     {
         QSettings settings("ESRI","MapTestAppConfig");
         settings.setValue("compassrosedock",QVariant(0));
     }
 }

 void ConfigurationPage::handleCompassRoseBottomRightCheckBox(bool value)
 {
     if(value)
     {
         QSettings settings("ESRI","MapTestAppConfig");
         settings.setValue("compassrosedock",QVariant(3));
     }
 }

 void ConfigurationPage::handleCompassRoseTopRightCheckBox(bool value)
 {
     if(value)
     {
         QSettings settings("ESRI","MapTestAppConfig");
         settings.setValue("compassrosedock",QVariant(2));
     }
 }

 void ConfigurationPage::handleInsetMapTopLeftCheckBox(bool value)
 {
     if(value)
     {
         QSettings settings("ESRI","MapTestAppConfig");
         settings.setValue("insetmapdock",QVariant(1));
     }
 }

 void ConfigurationPage::handleInsetMapBottomLeftCheckBox(bool value)
 {
     if(value)
     {
         QSettings settings("ESRI","MapTestAppConfig");
         settings.setValue("insetmapdock",QVariant(0));
     }
 }

 void ConfigurationPage::handleInsetMapBottomRightCheckBox(bool value)
 {
     if(value)
     {
         QSettings settings("ESRI","MapTestAppConfig");
         settings.setValue("insetmapdock",QVariant(3));
     }
 }

 void ConfigurationPage::handleInsetMapTopRightCheckBox(bool value)
 {
     if(value)
     {
         QSettings settings("ESRI","MapTestAppConfig");
         settings.setValue("insetmapdock",QVariant(2));
     }
 }

 UpdatePage::UpdatePage(QWidget *parent)
     : QWidget(parent)
 {
     QGroupBox *updateGroup = new QGroupBox(tr("Package selection"));
     QCheckBox *systemCheckBox = new QCheckBox(tr("Update system"));
     QCheckBox *appsCheckBox = new QCheckBox(tr("Update applications"));
     QCheckBox *docsCheckBox = new QCheckBox(tr("Update documentation"));

     QGroupBox *packageGroup = new QGroupBox(tr("Existing packages"));

     QListWidget *packageList = new QListWidget;
     QListWidgetItem *qtItem = new QListWidgetItem(packageList);
     qtItem->setText(tr("Qt"));
     QListWidgetItem *qsaItem = new QListWidgetItem(packageList);
     qsaItem->setText(tr("QSA"));
     QListWidgetItem *teamBuilderItem = new QListWidgetItem(packageList);
     teamBuilderItem->setText(tr("Teambuilder"));

     QPushButton *startUpdateButton = new QPushButton(tr("Apply"));

     QVBoxLayout *updateLayout = new QVBoxLayout;
     updateLayout->addWidget(systemCheckBox);
     updateLayout->addWidget(appsCheckBox);
     updateLayout->addWidget(docsCheckBox);
     updateGroup->setLayout(updateLayout);

     QVBoxLayout *packageLayout = new QVBoxLayout;
     packageLayout->addWidget(packageList);
     packageGroup->setLayout(packageLayout);

     QVBoxLayout *mainLayout = new QVBoxLayout;
     mainLayout->addWidget(updateGroup);
     mainLayout->addWidget(packageGroup);
     mainLayout->addSpacing(12);
     mainLayout->addWidget(startUpdateButton);
     mainLayout->addStretch(1);
     setLayout(mainLayout);
 }

 SymbolPage::SymbolPage(QWidget *parent)
     : QWidget(parent)
 {
     QGroupBox *packagesGroup = new QGroupBox(tr("Symbol"));

     QLabel *sicLabel = new QLabel(tr("SIC:"));
     QLineEdit *sicEdit = new QLineEdit;

     connect(sicEdit,SIGNAL(textChanged(QString)),this,SLOT(handleSICChanged(QString)));

     QLabel *countLabel = new QLabel(tr("Count:"));
     QLineEdit *countEdit = new QLineEdit;

     connect(countEdit,SIGNAL(textChanged(QString)),this,SLOT(handleSymbolCountChanged(QString)));

     //QLabel *dateLabel = new QLabel(tr("Released after:"));
     //QDateTimeEdit *dateEdit = new QDateTimeEdit(QDate::currentDate());

     const QVariant defaultValue;
     QSettings settings("ESRI","MapTestAppConfig");
     int symbolcount = settings.value("symbolcount", defaultValue).toInt();
     QString symbolidcode = settings.value("symbolidcode", defaultValue).toString();

     sicEdit->setText(symbolidcode);
     countEdit->setText(QVariant(symbolcount).toString());


     QGridLayout *packagesLayout = new QGridLayout;
     packagesLayout->addWidget(sicLabel, 0, 0);
     packagesLayout->addWidget(sicEdit, 0, 1);
     packagesLayout->addWidget(countLabel, 1, 0);
     packagesLayout->addWidget(countEdit, 1, 1);
//     packagesLayout->addWidget(releasesCheckBox, 2, 0);
  //   packagesLayout->addWidget(upgradesCheckBox, 3, 0);
    // packagesLayout->addWidget(hitsSpinBox, 4, 0, 1, 2);
     packagesGroup->setLayout(packagesLayout);

     QVBoxLayout *mainLayout = new QVBoxLayout;
     mainLayout->addWidget(packagesGroup);
     mainLayout->addSpacing(12);
     //mainLayout->addWidget(startQueryButton);
     mainLayout->addStretch(1);
     setLayout(mainLayout);
 }

 void SymbolPage::handleSICChanged(QString sic)
 {
     // check length etc

     QSettings settings("ESRI","MapTestAppConfig");
     settings.setValue("symbolidcode",sic);
 }

 void SymbolPage::handleSymbolCountChanged(QString count)
 {
     QSettings settings("ESRI","MapTestAppConfig");
     settings.setValue("symbolcount",count);
 }
