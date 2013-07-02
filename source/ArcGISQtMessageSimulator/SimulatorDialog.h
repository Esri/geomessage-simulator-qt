/*******************************************************************************
 * Copyright 2012-2013 Esri
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

#ifndef SIMULATORDIALOG_H
#define SIMULATORDIALOG_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QMutex>

#include "Geomessage.h"
#include "SimulatorController.h"

namespace Ui {
  class SimulatorDialog;
}

/*!
 * \class	SimulatorDialog
 * \brief	Dialog for setting the simulator. 
 */
class SimulatorDialog : public QMainWindow {
  Q_OBJECT
public:
  /*!
   * \fn	SimulatorDialog::SimulatorDialog(QWidget *parent = 0);
   * \brief	Constructor.
   */
  SimulatorDialog(bool isVerboseOutput = true, QWidget *parent = 0);
  /*!
   * \fn	SimulatorDialog::~SimulatorDialog();
   * \brief	Desstructor
   */
  ~SimulatorDialog();

protected:
  /*!
   * \fn	void SimulatorDialog::changeEvent(QEvent *e);
   * \brief	Auto-generated Change event.
   */
  void changeEvent(QEvent *e);
  /*!
   * \fn	void SimulatorDialog::timerEvent(QTimerEvent *event);
   * \brief	Timer event, to broadcast messages periodically
   */
//  void timerEvent(QTimerEvent *event);

private:
  Ui::SimulatorDialog *ui;
  bool m_paused;
  int m_numRows;
  SimulatorController controller;
  QMutex messagesWidgetMutex;
  QStringList checkedFields;
  QMutex checkedFieldsMutex;

private slots:
  void on_btnFile_clicked();
  /*!
   * \fn	void SimulatorDialog::on_btnStart_clicked();
   * \brief	Starts or Restarts the simulator
   */
  void on_btnStart_clicked();
  /*!
   * \fn	void SimulatorDialog::on_btnStop_clicked();
   * \brief	Stops the simulator
   */
  void on_btnStop_clicked();
  /*!
   * \fn	void SimulatorDialog::on_btnPause_clicked();
   * \brief	Pauses the simulator if started or continues it, if already paused
   */
  void on_btnPause_clicked();
  void on_spinBox_port_valueChanged(int newPort);
  void on_spinBox_frequency_valueChanged(int newFrequency);
  void on_comboBox_timeUnit_currentIndexChanged(int index);

  void addGeomessageToTable(Geomessage geomessage);
  void selectGeomessageInTable(int index);
  void on_listWidget_timeOverrideFields_itemChanged(QListWidgetItem *item);
};

#endif // SIMULATORDIALOG_H
