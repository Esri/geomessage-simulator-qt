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

#ifndef SIMULATORDIALOG_H
#define SIMULATORDIALOG_H

#include <QDialog>
#include <QBasicTimer>
#include <QtNetwork>

namespace Ui {
  class SimulatorDialog;
}

/*!
 * \class	SimulatorDialog
 * \brief	Dialog for setting the simulator. 
 */
class SimulatorDialog : public QDialog {
  Q_OBJECT
public:
  /*!
   * \fn	SimulatorDialog::SimulatorDialog(QWidget *parent = 0);
   * \brief	Constructor.
   */
  SimulatorDialog(QWidget *parent = 0);
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
  void timerEvent(QTimerEvent *event);

private:

  /*!
   * \fn	void SimulatorDialog::doSomething();
   * \internal
   */
  void doSomething();
  /*!
   * \internal
   * \fn	void SimulatorDialog::initializeSimulator();
   * \brief this is where all the magic happens
   * 		  set up timer, socket; read message file, setup UI, etc
   */
  void initializeSimulator(const QString & file);
  bool loadSimulationFile(const QString & file);
  bool fileHasAnyMessages();

private:
  static const QString TAG_ROOT;
  static const QString TAG_MESSAGES;
  static const QString TAG_MESSAGE;
  static const QString TAG_SIC;
  static const QString TAG_NAME;
  static const QString TAG_ID;
  static const QString TAG_ACTION;
  static const QString TAG_TYPE;

  Ui::SimulatorDialog *ui;
  int m_currentIndex;
  bool m_simulationStarted;
  bool m_simulationPaused;
  int m_messageFrequency;
  int m_messageThroughput;
  QBasicTimer m_timer;
  QStringList m_messages;
  QUdpSocket *m_udpSocket;

  int m_numRows;
  QFile m_inputFile;
  QXmlStreamReader m_inputReader;
  bool m_reachedEndOfFile;

private slots:
  /*!
   * \fn	void SimulatorDialog::on_dialThroughput_valueChanged(int value);
   * \brief	Handles throughput value changes.
   */
  void on_btnFile_clicked();
  void on_dialThroughput_valueChanged(int value);
  /*!
   * \fn	void SimulatorDialog::on_dialFrequency_valueChanged(int value);
   * \brief	Handles frequency value changes.
   */
  void on_dialFrequency_valueChanged(int value);
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
};

#endif // SIMULATORDIALOG_H
