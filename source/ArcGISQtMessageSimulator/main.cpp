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

#include <QtGui/QApplication>
#include "SimulatorController.h"
#include "SimulatorDialog.h"

#include <QTextStream>

int main(int argc, char *argv[])
{
  bool isGui = true;
  QString simulationFile;
  int port = SimulatorController::DEFAULT_BROADCAST_PORT;
  float frequency = 1;
  QString timeUnit = "second";
  QStringList timeOverrideFields;
  bool isVerbose = true;
  for (int i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-?"))
    {
      //Print help and exit
      QTextStream out(stdout);
      out << "Available command line parameters:" << endl;
      out << "  -?                     Print this help and exit" << endl;
      out << "  -c                     Console mode (no GUI)" << endl;
      out << "Parameters available only in console mode:" << endl;
      out << "  -p <port number>       Port number (console mode only; default is " << port << ")" << endl;
      out << "  -f <filename>          Simulation file" << endl;
      out << "  -q <frequency>         Frequency (messages per time unit); default is 1" << endl;
      out << "  -t <time unit>         Time unit for frequency; valid values are second," << endl <<
             "                         minute, hour, day, and week; default is second" << endl;
      out << "  -o <field1,...,fieldN> Override the value of these fields with the current" << endl <<
             "                         date/time" << endl;
      out << "  -s                     Silent mode; no verbose output" << endl;
      return 0;
    }
    else if (!strcmp(argv[i], "-c"))
    {
      isGui = false;
    }
    else if (!strcmp(argv[i], "-f"))
    {
      if ((i + 1) < argc)
      {
        simulationFile = QString(argv[++i]);
      }
    }
    else if (!strcmp(argv[i], "-p"))
    {
      if ((i + 1) < argc)
      {
        port = atoi(argv[++i]);
      }
    }
    else if (!strcmp(argv[i], "-q"))
    {
      if ((i + 1) < argc)
      {
        frequency = atoi(argv[++i]);
      }
    }
    else if (!strcmp(argv[i], "-t"))
    {
      if ((i + 1) < argc)
      {
        timeUnit = QString(argv[++i]);
      }
    }
    else if (!strcmp(argv[i], "-o"))
    {
      if ((i + 1) < argc)
      {
        timeOverrideFields = QString(argv[++i]).split(',');
      }
    }
    else if (!strcmp(argv[i], "-s"))
    {
      isVerbose = false;
    }
  }
  if (isGui)
  {
    QApplication a(argc, argv);
    SimulatorDialog w(isVerbose);
    w.show();
    return a.exec();
  }
  else
  {
    QCoreApplication a(argc, argv);
    SimulatorController controller;
    controller.setMessageFrequency(frequency, timeUnit);
    controller.setPort(port);
    controller.initializeSimulator(simulationFile);
    controller.setVerbose(isVerbose);
    controller.setTimeOverrideFields(timeOverrideFields);
    controller.startSimulation();
    return a.exec();
  }
}
