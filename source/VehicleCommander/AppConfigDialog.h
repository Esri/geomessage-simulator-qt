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

#ifndef APPCONFIGDIALOG_H
#define APPCONFIGDIALOG_H

#include <QDialog>

//Forward declaration
class MapController;

namespace Ui {
  class AppConfigDialog;
}

class AppConfigDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AppConfigDialog(MapController* mapController, QWidget *parent = 0);
  virtual ~AppConfigDialog();
  void show();

private:
  Ui::AppConfigDialog *ui;
  MapController* mapController;

private slots:
    void on_pushButton_clicked();
    void on_buttonBox_accepted();
};

#endif // APPCONFIGDIALOG_H
