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

#include <QtGui/QApplication>
#include "SimulatorController.h"
#include "SimulatorDialog.h"

#include <QTextStream>

int main(int argc, char *argv[])
{
  bool isGui = true;
  QString simulationFile;
  int frequency = 1;
  int throughput = 1;
  bool isVerbose = false;
  for (int i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-?"))
    {
      //Print help and exit
      QTextStream out(stdout);
      out << "Available command line parameters:" << endl;
      out << "  -?              Print this help and exit" << endl;
      out << "  -c              Console mode (no GUI)" << endl;
      out << "  -f <filename>   Simulation file (console mode only)" << endl;
      out << "  -q <frequency>  Frequency (broadcasts per second); default is 1; console mode only" << endl;
      out << "  -t <throughput> Throughput (messages per broadcast); default is 1; console mode only" << endl;
      out << "  -v              Verbose output (console mode only)" << endl;
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
        throughput = atoi(argv[++i]);
      }
    }
    else if (!strcmp(argv[i], "-v"))
    {
      isVerbose = true;
    }
  }
  if (isGui)
  {
    QApplication a(argc, argv);
    SimulatorDialog w;
    w.show();
    return a.exec();
  }
  else
  {
    QCoreApplication a(argc, argv);
    SimulatorController controller;
    controller.setMessageFrequency(frequency);
    controller.setMessageThroughput(throughput);
    controller.initializeSimulator(simulationFile);
    controller.setVerbose(isVerbose);
    controller.startSimulation();
    return a.exec();
  }
}
