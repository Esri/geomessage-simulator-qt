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

#include "SimulatorController.h"

#include <QtCore/qmath.h>

const int SimulatorController::DEFAULT_BROADCAST_PORT = 45678;
const QString SimulatorController::PORT_SETTING_NAME = "port";
const QString SimulatorController::TAG_ROOT = "geomessages";
const QString SimulatorController::TAG_MESSAGES = "geomessages";
const QString SimulatorController::TAG_MESSAGE = "geomessage";
const QString SimulatorController::TAG_SIC = "sic";
const QString SimulatorController::TAG_NAME = "uniquedesignation";
const QString SimulatorController::TAG_ID = "_id";
const QString SimulatorController::TAG_ACTION = "_action";
const QString SimulatorController::TAG_TYPE = "_type";

SimulatorController::SimulatorController(QObject *parent) :
  QObject(parent),
  m_simulationStarted(false),
  m_simulationPaused(false),
  m_messageFrequency(1), // broadcasts per second
  m_messageThroughput(1), // messages per broadcast
  m_reachedEndOfFile(false),
  m_currentIndex(-1),
  settings("DefenseTemplates", "Message Simulator"),
  consoleOut(stdout),
  m_verbose(true),
  m_timer()
{
  m_udpSocket = new QUdpSocket(this);
}

void SimulatorController::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == m_timer.timerId())
  {
    QString payload;
    QString currentElementName;
    QString Name, messageID, messageAction, symbolID, type;
    QXmlStreamWriter streamWriter(&payload);

    streamWriter.writeStartElement(TAG_ROOT);
    for(int messageThroughput = 0 ; messageThroughput < m_messageThroughput ; messageThroughput++)
    {
      while (!m_inputReader.atEnd())
      {
        if(QStringRef::compare(m_inputReader.name(), TAG_MESSAGES) == 0)
        {
          m_inputReader.readNext();
          if(m_inputReader.atEnd())
          {
            m_reachedEndOfFile=true;
            m_inputReader.clear();
            m_inputFile.reset();
            m_inputReader.setDevice(&m_inputFile);
            m_currentIndex = 0;
          }
          continue;
        }
        else if (m_inputReader.isEndElement())
        {
          streamWriter.writeEndElement();
          if(QStringRef::compare(m_inputReader.name(), TAG_MESSAGE) == 0)
          {
            //Only emit readGeomessage if this is the first time we have read the message...
            if (m_reachedEndOfFile == false)
            {
              emit readGeomessage(Geomessage(Name, messageID, messageAction, symbolID, type));
            }
            //Only emit advancedToGeomessage when it's the last message in this set...
            if (messageThroughput + 1 == m_messageThroughput)
            {
              emit advancedToGeomessage(m_currentIndex);
            }
            m_currentIndex++;
            break;
          }

          m_inputReader.readNext();
        }
        else if (m_inputReader.isStartElement())
        {
          currentElementName = m_inputReader.name().toString();
          if (m_inputReader.prefix().toString().isEmpty())
          {
            streamWriter.writeStartElement(m_inputReader.name().toString());
          }
          else
          {
            streamWriter.writeStartElement(m_inputReader.prefix().toString()+ ":" + m_inputReader.name().toString());
          }
          if (m_inputReader.attributes().size() > 0)
          {
            streamWriter.writeAttributes(m_inputReader.attributes());
          }
        }
        else if (m_inputReader.isCharacters())
        {
          QString text = m_inputReader.text().toString();
          streamWriter.writeCharacters(text);
          if (TAG_NAME == currentElementName)
          {
            Name = text;
          }
          else if (TAG_ACTION == currentElementName)
          {
            messageAction = text;
          }
          else if (TAG_ID == currentElementName)
          {
            messageID = text;
          }
          else if (TAG_SIC == currentElementName)
          {
            symbolID = text;
          }
          else if (TAG_TYPE == currentElementName)
          {
            type = text;
          }
        }
        else if (m_inputReader.isComment())
        {
          /* leave not import */
        }
        else if (m_inputReader.isCDATA())
        {
          /* take cdata */
        }
        m_inputReader.readNext();
      }

      m_inputReader.readNext();
    }

    streamWriter.writeEndElement();

    QByteArray datagram;
    datagram += payload;
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, getPort());
    if (verbose())
    {
      consoleOut << payload << endl;
    }
  }
  else
  {
    QObject::timerEvent(event);
  }
}

void SimulatorController::initializeSimulator(const QString & fileName) throw (QString)
{
  QString error = loadSimulationFile(fileName);
  if (NULL == error)
  {
    m_simulationPaused = false;
  }
  else
  {
      throw error;
  }
}

QString SimulatorController::loadSimulationFile(const QString &fileName)
{
  if(m_inputFile.isOpen())
    m_inputFile.close();

  m_inputFile.setFileName(fileName);

  //Check file for at least one message
  if (!fileHasAnyMessages())
  {
    return QString(m_inputFile.fileName() + " is an empty message file");
  }
  else
  {
    return NULL;
  }
}

bool SimulatorController::fileHasAnyMessages()
{
  // Open the file for reading.
  if(m_inputFile.isOpen())
    m_inputFile.reset();
  else
    m_inputFile.open(QIODevice::ReadOnly | QIODevice::Text);

  // Initialize the XML reader.
  m_inputReader.clear();
  m_inputReader.setDevice(&m_inputFile);

  bool hasMessage = false;
  while (!m_inputReader.atEnd())
  {
    m_inputReader.readNext();
    if (m_inputReader.isStartElement() && 0 == QStringRef::compare(m_inputReader.name(), TAG_MESSAGE))
    {
      hasMessage = true;
      break;
    }
  }

  m_inputReader.clear();
  m_inputFile.reset();
  m_inputReader.setDevice(&m_inputFile);

  return hasMessage;
}

void SimulatorController::startSimulation()
{
  m_currentIndex = 0;
  // Open the file for reading.
  if(m_inputFile.isOpen())
    m_inputFile.reset();
  else
    m_inputFile.open(QIODevice::ReadOnly | QIODevice::Text);
  // Initialize the XML reader.
  m_inputReader.clear();
  m_inputReader.setDevice(&m_inputFile);
  m_reachedEndOfFile = false;

  m_simulationPaused = false;
  m_simulationStarted = true;
  int msec = (int) floor((1.0 / m_messageFrequency * 1000) + 0.5);
  m_timer.start(msec, this);
}

void SimulatorController::pauseSimulation()
{
  m_simulationPaused = true;
  m_timer.stop();
}

void SimulatorController::unpauseSimulation()
{
  int msec = (int) floor((1.0 / m_messageFrequency * 1000) + 0.5);
  m_timer.start(msec, this);
}

void SimulatorController::stopSimulation()
{
  m_simulationPaused = false;

  // Clear out the XML reader and close the input file.
  m_inputReader.clear();
  m_inputFile.close();

  m_reachedEndOfFile = false;
  m_currentIndex = -1;
  m_timer.stop();
  m_simulationStarted = false;
}

void SimulatorController::setMessageFrequency(int newFrequency)
{
  m_messageFrequency = newFrequency;
  m_timer.stop();
  if((true == m_simulationStarted) && (false == m_simulationPaused))
  {
    int msec = (int) floor((1.0 / m_messageFrequency * 1000) + 0.5);
    m_timer.start(msec, this);
  }
}

int SimulatorController::messageFrequency()
{
  return m_messageFrequency;
}

void SimulatorController::setMessageThroughput(int newThroughput)
{
  m_messageThroughput = newThroughput;
}

int SimulatorController::messageThroughput()
{
  return m_messageThroughput;
}

void SimulatorController::setPort(int newPort)
{
  settings.setValue(PORT_SETTING_NAME, newPort);
}

int SimulatorController::getPort()
{
  return settings.value(PORT_SETTING_NAME, DEFAULT_BROADCAST_PORT).toInt();
}

bool SimulatorController::simulationStarted()
{
  return m_simulationStarted;
}

void SimulatorController::setVerbose(bool verbose)
{
  m_verbose = verbose;
}

bool SimulatorController::verbose()
{
  return m_verbose;
}
