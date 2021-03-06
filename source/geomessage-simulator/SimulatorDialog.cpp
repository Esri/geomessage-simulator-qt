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

#include "SimulatorDialog.h"
#include "ui_SimulatorDialog.h"

#include <QFileDialog>
#include <QMessageBox>

SimulatorDialog::SimulatorDialog(bool isVerboseOutput, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimulatorDialog),
    m_numRows(0),
    m_paused(false),
    controller()
{
  controller.setVerbose(isVerboseOutput);
  ui->setupUi(this);

  setWindowTitle(windowTitle() + " " + SimulatorController::RELEASE);

  ui->btnStart->setEnabled(false);
  ui->btnPause->setEnabled(false);
  ui->btnStop->setEnabled(false);

  ui->spinBox_port->setValue(controller.getPort());

  connect(&controller, SIGNAL(readGeomessage(Geomessage)), this, SLOT(addGeomessageToTable(Geomessage)));
  connect(&controller, SIGNAL(advancedToGeomessage(int)), this, SLOT(selectGeomessageInTable(int)));
}

SimulatorDialog::~SimulatorDialog()
{
  delete ui;
}

void SimulatorDialog::addGeomessageToTable(Geomessage geomessage)
{
  // Create Table items.
  QTableWidgetItem *nameItem = new QTableWidgetItem(geomessage.name());
  QTableWidgetItem *idItem = new QTableWidgetItem(geomessage.messageId());
  QTableWidgetItem *actionItem = new QTableWidgetItem(geomessage.messageAction());
  QTableWidgetItem *symbolItem = new QTableWidgetItem(geomessage.symbolId());
  QTableWidgetItem *typeItem = new QTableWidgetItem(geomessage.type());

  {
    QMutexLocker locker(&messagesWidgetMutex);
    // Insert  a row.
    ui->messagesWidget->insertRow(m_numRows);

    // Add the row to the table.
    ui->messagesWidget->setItem(m_numRows, 0, nameItem);
    ui->messagesWidget->setItem(m_numRows, 1, idItem);
    ui->messagesWidget->setItem(m_numRows, 2, actionItem);
    ui->messagesWidget->setItem(m_numRows, 3, symbolItem);
    ui->messagesWidget->setItem(m_numRows, 4, typeItem);

    m_numRows++;
  }
}

void SimulatorDialog::selectGeomessageInTable(int index)
{
  {
    QMutexLocker locker(&messagesWidgetMutex);
    ui->messagesWidget->selectRow(index);
  }
}

void SimulatorDialog::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;
  default:
    break;
  }
}

void SimulatorDialog::on_btnPause_clicked()
{
  m_paused = !m_paused;
  if (m_paused)
  {
    ui->lblMessage->setText("Simulation Paused");
    ui->btnPause->setText("Continue Simulator");
    ui->btnFile->setEnabled(true);
    ui->btnFile->setText("Change Simulation File");
    controller.pauseSimulation();
  }
  else
  {
    ui->lblMessage->setText("Simulation In Progress..");
    ui->btnPause->setText("Pause Simulator");
    ui->btnFile->setEnabled(false);
    ui->btnFile->setText("Choose Simulation File");
    controller.unpauseSimulation();
  }
}

void SimulatorDialog::on_btnStop_clicked()
{
  ui->btnPause->setEnabled(false);
  ui->btnStop->setEnabled(false);
  ui->btnFile->setEnabled(true);
  ui->btnStart->setText("Start Simulator");
  ui->btnPause->setText("Pause Simulator");
  m_paused = false;
  ui->lblMessage->setText("Simulation Stopped...");
  controller.stopSimulation();
}

void SimulatorDialog::on_btnStart_clicked()
{
  ui->btnPause->setEnabled(true);
  ui->btnStop->setEnabled(true);
  ui->btnFile->setEnabled(false);

  if (!controller.simulationStarted())
  {
    ui->btnStart->setText("Restart Simulator");
  }

  ui->lblMessage->setText("Simulation In Progress...");
  {
    QMutexLocker locker(&messagesWidgetMutex);
    ui->messagesWidget->clear();
    ui->messagesWidget->clearContents();
    ui->messagesWidget->setRowCount(0);
    m_numRows = 0;

    QStringList labels;
    ui->messagesWidget->setColumnCount(5);
    labels << "Name" <<"Message ID" << "Message Action" << "Symbol ID" << "Type";
    ui->messagesWidget->setHorizontalHeaderLabels(labels);
  }

  controller.startSimulation();
  ui->btnPause->setText("Pause Simulator");
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

    QCursor c = cursor();
    setCursor(Qt::WaitCursor);

    ui->btnStart->setEnabled(false);
    ui->btnPause->setEnabled(false);
    ui->btnStop->setEnabled(false);
    ui->btnFile->setEnabled(true);

    {
      QMutexLocker locker(&messagesWidgetMutex);
      ui->messagesWidget->clear();
      ui->messagesWidget->clearContents();
      ui->messagesWidget->setRowCount(0);
      m_numRows = 0;
      ui->btnFile->setEnabled(false);
      ui->lblMessage->setText("File Loaded");
      ui->btnStart->setText("Start Simulator");

      try
      {
        controller.initializeSimulator(fileNames.at(0));
        setCursor(c);
        ui->btnStart->setEnabled(true);
        ui->spinBox_frequency->setValue(controller.messageFrequency());

        QStringList labels;
        ui->messagesWidget->setColumnCount(5);
        labels << "Name" <<"Message ID" << "Message Action" << "Symbol ID" << "Type";
        ui->messagesWidget->setHorizontalHeaderLabels(labels);

        ui->listWidget_timeOverrideFields->clear();
        QStringList fieldNames = controller.fieldNames();
        ui->listWidget_timeOverrideFields->addItems(fieldNames);
        for (int i = 0; i < fieldNames.length(); i++)
        {
          ui->listWidget_timeOverrideFields->item(i)->setFlags(ui->listWidget_timeOverrideFields->item(i)->flags() | Qt::ItemIsUserCheckable);
          ui->listWidget_timeOverrideFields->item(i)->setCheckState(Qt::Unchecked);
        }
        {
          QMutexLocker locker(&checkedFieldsMutex);
          checkedFields = QStringList();
        }

        ui->btnFile->setEnabled(true);
        ui->btnStart->setEnabled(true);
      }
      catch (QString error)
      {
        QMessageBox::warning(this, "Simulator Not Initialized", error);
        ui->btnFile->setEnabled(true);
        setCursor(c);
      }
    }
  }
}

void SimulatorDialog::on_spinBox_port_valueChanged(int newPort)
{
  controller.setPort(newPort);
}

void SimulatorDialog::on_spinBox_frequency_valueChanged(int newFrequency)
{
  Q_UNUSED(newFrequency);
  updateMessageFrequency();
}

void SimulatorDialog::on_comboBox_timeUnit_currentIndexChanged(int index)
{
  Q_UNUSED(index);
  updateMessageFrequency();
}

void SimulatorDialog::on_spinBox_timeCount_valueChanged(int newTimeCount)
{
  Q_UNUSED(newTimeCount);
  updateMessageFrequency();
}

void SimulatorDialog::updateMessageFrequency()
{
  controller.setMessageFrequency(
        ui->spinBox_frequency->value(),
        ui->spinBox_timeCount->value(),
        ui->comboBox_timeUnit->currentText());
}

void SimulatorDialog::on_listWidget_timeOverrideFields_itemChanged(QListWidgetItem *item)
{
  QMutexLocker locker(&checkedFieldsMutex);
  if (Qt::Checked == item->checkState())
  {
    checkedFields.append(item->text());
  }
  else
  {
    checkedFields.removeAll(item->text());
  }
  controller.setTimeOverrideFields(checkedFields);
}
