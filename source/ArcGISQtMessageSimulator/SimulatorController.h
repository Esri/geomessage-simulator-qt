/*******************************************************************************
 * Copyright 2013 Esri
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

#ifndef SIMULATOR_CONTROLLER_H
#define SIMULATOR_CONTROLLER_H

#include <QBasicTimer>
#include <QFile>
#include <QSettings>
#include <QTimerEvent>
#include <QUdpSocket>
#include <QXmlStreamReader>

#include "Geomessage.h"

#ifdef WINDOWS
  //Disable VCC warning C4290, which arises from declaring a method to throw a specific type.
  #pragma warning(disable : 4290)
#endif

class SimulatorController : public QObject
{
  Q_OBJECT
public:
  static const int DEFAULT_BROADCAST_PORT;

  explicit SimulatorController(QObject *parent = 0);
  /*!
   * \fn	bool SimulatorController::initializeSimulator(const QString&)
   * \brief	Initializes the simulator.
   * \param     file the simulation file.
   * \throws    an error message if the simulator could not be initialized.
   */
  void initializeSimulator(const QString & file) throw(QString);
  void startSimulation();
  void pauseSimulation();
  void unpauseSimulation();
  void stopSimulation();

  /*!
   * \brief Sets the number of broadcasts per second. Equivalent to
   *        setMessageFrequency(newFrequency, "second");
   * \param the number of broadcasts per second.
   */
  void setMessageFrequency(float newFrequency);

  /*!
   * \brief Sets the number of messages per time unit. Then the application will send
   *        <newFrequency> messages per <newTimeUnit>.
   * \param newFrequency the number of broadcasts per time unit.
   * \param newTimeUnit the time unit. Valid values are second, minute
   *        hour, day, and week; default is second.
   */
  void setMessageFrequency(float newFrequency, QString newTimeUnit);

  /*!
   * \brief Returns the number of broadcasts per second.
   * \return the number of broadcasts per second.
   */
  float messageFrequency();

  /*!
   * \brief Sets the number of messages per broadcast.
   * \param newThroughput the number of messages per broadcast.
   * \deprecated Sending more than one message per broadcast is something that ArcGIS GeoEvent Processor
   *             cannot handle. Therefore, it is not recommended that you leave the throughput at its
   *             default value of 1.
   */
  void setMessageThroughput(int newThroughput);
  int messageThroughput();
  void setPort(int newPort);
  int getPort();
  bool simulationStarted();
  void setVerbose(bool verbose);
  bool verbose();

protected:
  void timerEvent(QTimerEvent *event);

private:
  static const QString PORT_SETTING_NAME;
  static const QString TAG_ROOT;
  static const QString TAG_MESSAGES;
  static const QString TAG_MESSAGE;
  static const QString TAG_SIC;
  static const QString TAG_NAME;
  static const QString TAG_ID;
  static const QString TAG_ACTION;
  static const QString TAG_TYPE;

  QBasicTimer m_timer;
  QFile m_inputFile;
  QXmlStreamReader m_inputReader;
  bool m_reachedEndOfFile;
  float m_messageFrequency;
  /*!
   * \brief m_messageThroughput should be left at its default value of 1. Otherwise, ArcGIS GeoEvent
   *        Processor cannot handle it.
   */
  int m_messageThroughput;
  QSettings settings;
  bool m_simulationStarted;
  bool m_simulationPaused;
  int m_currentIndex;
  QUdpSocket *m_udpSocket;
  bool m_verbose;
  QTextStream consoleOut;

  QString loadSimulationFile(const QString & file);
  bool fileHasAnyMessages();
  static int getSeconds(const QString* unit);

signals:
  void readGeomessage(Geomessage geomessage);
  void advancedToGeomessage(int index);
  
public slots:
  
};

#endif // SIMULATOR_CONTROLLER_H
