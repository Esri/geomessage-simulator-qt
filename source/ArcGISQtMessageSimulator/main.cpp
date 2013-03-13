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
