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

#include "SimulatorDialog.h"
#include "ui_SimulatorDialog.h"
#include "QtXml"
#include "QtDebug"
#include "QFileDialog"
#include <QMessageBox>
#include <math.h>

const int BROADCAST_PORT = 45678;

const QString SimulatorDialog::TAG_ROOT = "messages";
const QString SimulatorDialog::TAG_MESSAGES = "messages";
const QString SimulatorDialog::TAG_MESSAGE = "message";
const QString SimulatorDialog::TAG_SIC = "sic";
const QString SimulatorDialog::TAG_NAME = "UniqueDesignation";
const QString SimulatorDialog::TAG_ID = "_id";
const QString SimulatorDialog::TAG_ACTION = "_action";
const QString SimulatorDialog::TAG_TYPE = "_type";

SimulatorDialog::SimulatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulatorDialog),
    m_currentIndex(-1),
    m_simulationStarted(false),
    m_simulationPaused(false),
    m_messageFrequency(1), // broadcasts per second
    m_messageThroughput(1), // messages per broadcast
    m_numRows(0),
    m_reachedEndOfFile(false)
{
  ui->setupUi(this);
  //doSomething();

  m_udpSocket = new QUdpSocket(this);
  ui->btnStart->setEnabled(false);
  ui->btnPause->setEnabled(false);
  ui->btnStop->setEnabled(false);

  ui->messagesWidget->setSelectionMode(QAbstractItemView::MultiSelection);

}

SimulatorDialog::~SimulatorDialog()
{
  delete ui;
}

void SimulatorDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;
  default:
    break;
  }
}

void SimulatorDialog::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == m_timer.timerId())
  {
    QString payload;
    QString currentElementName;
    QString messageName, messageID, messageAction, symbolID, type;
    QXmlStreamWriter streamWriter(&payload);

    // Clear all the previous selectios.
    ui->messagesWidget->clearSelection();

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
            m_currentIndex=0;
            m_inputReader.clear();
            m_inputFile.reset();
            m_inputReader.setDevice(&m_inputFile);
          }
          continue;
        }
        else if (m_inputReader.isEndElement())
        {
          streamWriter.writeEndElement();
          if (QStringRef::compare(m_inputReader.name(), TAG_MESSAGE) == 0 && m_reachedEndOfFile == false)
          {
            // Create Table items.
            QTableWidgetItem *nameItem = new QTableWidgetItem(messageName);
            QTableWidgetItem *idItem = new QTableWidgetItem(messageID);
            QTableWidgetItem *actionItem = new QTableWidgetItem(messageAction);
            QTableWidgetItem *symbolItem = new QTableWidgetItem(symbolID);
            QTableWidgetItem *typeItem = new QTableWidgetItem(type);

            // Insert  a row.
            ui->messagesWidget->insertRow(m_numRows);

            // Add the row to the table.
            ui->messagesWidget->setItem(m_numRows, 0, nameItem);
            ui->messagesWidget->setItem(m_numRows, 1, idItem);
            ui->messagesWidget->setItem(m_numRows, 2, actionItem);
            ui->messagesWidget->setItem(m_numRows, 3, symbolItem);
            ui->messagesWidget->setItem(m_numRows, 4, typeItem);

            ui->messagesWidget->selectRow(m_numRows++);

            //m_numRows++;
            break;
          }
          if(QStringRef::compare(m_inputReader.name(), TAG_MESSAGE) == 0)
          {
            ui->messagesWidget->selectRow(m_currentIndex++);
            break;
          }

          m_inputReader.readNext();
        }
        else if (m_inputReader.isStartElement())
        {
          currentElementName = m_inputReader.name().toString();
          // handle old format
          currentElementName.replace("esri_", "_");

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
            messageName = text;
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
    // qDebug() << "Transmitted datagram";
    // qDebug() << datagram;
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::LocalHost/*::Broadcast*/, BROADCAST_PORT);
  }
  else
  {
    QDialog::timerEvent(event);
  }
}

void SimulatorDialog::initializeSimulator(const QString & fileName)
{
  QCursor c = cursor();
  setCursor(Qt::WaitCursor);

  ui->btnStart->setEnabled(false);
  ui->btnPause->setEnabled(false);
  ui->btnStop->setEnabled(false);
  ui->btnFile->setEnabled(true);

  if (false == loadSimulationFile(fileName))
  {
    ui->btnFile->setEnabled(true);
    setCursor(c);
    return;
  }

  setCursor(c);
  ui->btnStart->setEnabled(true);
  ui->dialFrequency->setValue(m_messageFrequency);
  ui->dialThroughput->setValue(m_messageThroughput);
  m_simulationPaused = false;
}

bool SimulatorDialog::loadSimulationFile(const QString &fileName)
{
  m_messages.clear();
  ui->messagesWidget->clear();
  ui->messagesWidget->clearContents();
  ui->messagesWidget->setRowCount(0);
  m_numRows = 0;
  ui->btnFile->setEnabled(false);
  ui->lblMessage->setText("File Loaded");
  ui->btnStart->setText("Start Simulator");

  if(m_inputFile.isOpen())
    m_inputFile.close();
  m_inputFile.setFileName(fileName);

  //Check file for at least one message
  if (!fileHasAnyMessages())
  {
    QMessageBox::warning(this, "Empty Message File",
                         m_inputFile.fileName() + " is an empty message file");
    return false;
  }
  else
  {
    QStringList labels;
    ui->messagesWidget->setColumnCount(5);
    labels << "Name" <<"Message ID" << "Message Action" << "Symbol ID" << "Type";
    ui->messagesWidget->setHorizontalHeaderLabels(labels);

    ui->btnFile->setEnabled(true);
    ui->btnStart->setEnabled(true);
    return true;
  }
}

bool SimulatorDialog::fileHasAnyMessages()
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

void SimulatorDialog::on_btnPause_clicked()
{
  m_simulationPaused = !m_simulationPaused;
  qDebug() << "Pause clicked " << m_simulationPaused;

  if(m_simulationPaused)
  {
    ui->lblMessage->setText("Simulation Paused");
    ui->btnPause->setText("Continue Simulator");
    ui->btnFile->setEnabled(true);
    ui->btnFile->setText("Change Simulation File");
    m_timer.stop();
  }
  else
  {
    ui->lblMessage->setText("Simulation In Progress..");
    ui->btnPause->setText("Pause Simulator");
    ui->btnFile->setEnabled(false);
    ui->btnFile->setText("Choose Simulation File");
    int msec = (int) floor((1.0 / m_messageFrequency * 1000) + 0.5);
    m_timer.start(msec, this);
  }
}

void SimulatorDialog::on_btnStop_clicked()
{
  ui->btnPause->setEnabled(false);
  ui->btnStop->setEnabled(false);
  ui->btnFile->setEnabled(true);
  ui->btnStart->setText("Start Simulator");
  ui->btnPause->setText("Pause Simulator");
  m_simulationPaused = false;
  ui->lblMessage->setText("Simulation Stoped...");

  // Clear out the XML reader and close the input file.
  m_inputReader.clear();
  m_inputFile.close();

  m_reachedEndOfFile = false;
  m_currentIndex = -1;
  m_timer.stop();
  m_simulationStarted = false;
}

void SimulatorDialog::on_btnStart_clicked()
{
  ui->btnPause->setEnabled(true);
  ui->btnStop->setEnabled(true);
  ui->btnFile->setEnabled(false);

  if(false == m_simulationStarted)
  {
    ui->btnStart->setText("Restart Simulator");
  }

  ui->lblMessage->setText("Simulation In Progress...");
  ui->messagesWidget->clear();
  ui->messagesWidget->clearContents();
  ui->messagesWidget->setRowCount(0);
  m_numRows = 0;

  QStringList labels;
  ui->messagesWidget->setColumnCount(5);
  labels << "Name" <<"Message ID" << "Message Action" << "Symbol ID" << "Type";
  ui->messagesWidget->setHorizontalHeaderLabels(labels);


  // Open the file for reading.
  if(m_inputFile.isOpen())
    m_inputFile.reset();
  else
    m_inputFile.open(QIODevice::ReadOnly | QIODevice::Text);
  // Initialize the XML reader.
  m_inputReader.clear();
  m_inputReader.setDevice(&m_inputFile);
  m_reachedEndOfFile = false;

  ui->btnPause->setText("Pause Simulator");
  m_simulationPaused = false;
  m_simulationStarted = true;
  m_currentIndex = 0;
  int msec = (int) floor((1.0 / m_messageFrequency * 1000) + 0.5);
  m_timer.start(msec, this);
}

void SimulatorDialog::on_dialFrequency_valueChanged(int value)
{
  m_messageFrequency = value;
  m_timer.stop();
  if((true == m_simulationStarted) && (false == m_simulationPaused))
  {
    int msec = (int) floor((1.0 / m_messageFrequency * 1000) + 0.5);
    m_timer.start(msec, this);
  }
}

void SimulatorDialog::on_dialThroughput_valueChanged(int value)
{
  m_messageThroughput = value;
}

void SimulatorDialog::on_btnFile_clicked()
{
  QFileDialog dialog(this);
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setNameFilter(tr("Report Simulation files (*.xml)"));

  QStringList fileNames;
  if (dialog.exec())
  {
    fileNames = dialog.selectedFiles();
    initializeSimulator(fileNames.at(0));
  }
}
