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

#include "AppConfigDialog.h"
#include "ui_AppConfigDialog.h"

#include <QSettings>
#include <QHostInfo>

#include "MapController.h"

AppConfigDialog::AppConfigDialog(MapController* mapController, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppConfigDialog)
{
  this->mapController = mapController;
  ui->setupUi(this);
}

void AppConfigDialog::show()
{
  //Read settings
  const QVariant defaultValue;
  QSettings settings("My Organization", "Vehicle Commander");
  ui->lineEdit_uniqueId->setText(settings.value("userId", defaultValue).toString());
  ui->lineEdit_username->setText(settings.value("username", defaultValue).toString());
  ui->lineEdit_sic->setText(settings.value("userSIC", defaultValue).toString());
  ui->lineEdit_port->setText(settings.value("messagingPort", defaultValue).toString());
  ui->lineEdit_interval->setText(settings.value("messagingInterval", defaultValue).toString());

  QDialog::show();
}

AppConfigDialog::~AppConfigDialog()
{
  delete ui;
}

void AppConfigDialog::on_buttonBox_accepted()
{
  //Save settings in dialog to QSettings object
  QSettings settings("My Organization", "Vehicle Commander");
  settings.setValue("userId", ui->lineEdit_uniqueId->text());
  settings.setValue("username", ui->lineEdit_username->text());
  settings.setValue("userSIC", ui->lineEdit_sic->text());
  settings.setValue("messagingPort", ui->lineEdit_port->text().toInt());
  settings.setValue("messagingInterval", ui->lineEdit_interval->text().toInt());

  mapController->applyAppConfigSettings();
}

void AppConfigDialog::on_pushButton_clicked()
{
  //Generate a new unique ID and add to UI but don't save it yet (let the OK button save it)
  ui->lineEdit_uniqueId->setText(QUuid::createUuid().toString());
  ui->lineEdit_username->setText(QHostInfo::localHostName());
}
