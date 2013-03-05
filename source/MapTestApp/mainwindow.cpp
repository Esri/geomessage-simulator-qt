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

#include <QSettings>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"

static const QString APP_CONFIG_PATH(":/Resources/appconfig.xml");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pSampleMap(new SampleMap(parent))
{
    ui->setupUi(this);

    setCentralWidget(m_pSampleMap);
    this->showMaximized();
    this->setWindowTitle("MapTestApp");

    // hook up some events
    if(m_pSampleMap)
    {
        connect(m_pSampleMap, SIGNAL(mousePress(QMouseEvent)), this, SLOT(handleMousePress(QMouseEvent)));
    }

    graphicsIDCounter = 0;
    totalGraphics = 0;
    heading = 0.0;

    if(m_pSampleMap)
    {
        defaultExtent = m_pSampleMap->GetExtent();
        defaultScale = m_pSampleMap->GetScale();
    }

    // load settings
    m_pSettings = new QSettings("ESRI","MapTestAppConfig");

    readAppConfig();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_pSampleMap;
}

void MainWindow::handleMousePress(QMouseEvent mouseEvent)
{
    Q_UNUSED(mouseEvent)
}

void MainWindow::on_actionPan_Up_triggered()
{
    if (m_pSampleMap != NULL)
    {
        m_pSampleMap->PanUp();
    }
    else
    {
        qDebug("IVendorNeutralMap pointer is null");
    }
}

void MainWindow::on_actionPan_Down_triggered()
{
    if (m_pSampleMap != NULL)
    {
        m_pSampleMap->PanDown();
    }
    else
    {
        qDebug("IVendorNeutralMap pointer is null");
    }
}

void MainWindow::on_actionPan_Left_triggered()
{
    if (m_pSampleMap != NULL)
    {
        m_pSampleMap->PanLeft();
    }
    else
    {
        qDebug("IVendorNeutralMap pointer is null");
    }
}

void MainWindow::on_actionPan_Right_triggered()
{
    if (m_pSampleMap != NULL)
    {
        m_pSampleMap->PanRight();
    }
    else
    {
        qDebug("IVendorNeutralMap pointer is null");
    }
}

void MainWindow::on_actionScale_bar_toggled(bool visible)
{
    if (m_pSampleMap != NULL)
    {
        m_pSampleMap->ShowScalebar(visible);
    }
    else
    {
        qDebug("IVendorNeutralMap pointer is null");
    }
}

void MainWindow::on_actionGrid_toggled(bool visible)
{
    if (m_pSampleMap != NULL)
    {
        m_pSampleMap->SetGridVisible(visible);
    }
    else
    {
        qDebug("IVendorNeutralMap pointer is null");
    }
}

void MainWindow::on_actionZoom_triggered()
{
    if (m_pSampleMap != NULL)
    {
        m_pSampleMap->Zoom(0.5);
    }
    else
    {
        qDebug("IVendorNeutralMap pointer is null");
    }
}

void MainWindow::on_actionZoom_2_triggered()
{
    if (m_pSampleMap != NULL)
    {
        m_pSampleMap->Zoom(1.5);
    }
    else
    {
        qDebug("IVendorNeutralMap pointer is null");
    }
}

void MainWindow::on_actionRotate_triggered()
{
    if (m_pSampleMap != NULL)
    {
        heading += 90.0;
        if(heading > 360.0)
        {
            heading -= 360.0;
        }
        m_pSampleMap->Rotate(heading);
    }
    else
    {
        qDebug("IVendorNeutralMap pointer is null");
    }
}

void MainWindow::on_actionRotateMinus_triggered()
{
    if (m_pSampleMap != NULL)
    {
        heading -= 90.0;
        if(heading < 0.0)
        {
            heading += 360.0;
        }
        m_pSampleMap->Rotate(heading);

    }
    else
    {
        qDebug("IVendorNeutralMap pointer is null");
    }
}

void MainWindow::on_actionAdd_Random_Graphics_triggered()
{
    // load dialog and get desired count

    // foreach count add graphics to map via map interface

    QList<MilitarySymbolObject> list;

    const int NUMBER2ADD = 500;

    double width = m_pSampleMap->GetExtent().width() * .05; // % width

    for(int x = 0 ; x < NUMBER2ADD ; x++)
    {
        // for now, generate a few test cases
        int caseSwitch = x % 20;

        MilitarySymbolObject mo;
        mo.Id = graphicsIDCounter++;
        mo.SymbolCode.clear();

        switch (caseSwitch)
        {
        case 0 :
            {
            // Create a Multipoint (Arrow)
            mo.SymbolCode = QString("GFGPOLAV------X");
            QList<QPointF> gpointlist;
            QPointF point1 = GetRandomPointF();

            gpointlist.append(point1);
            QPointF point2(point1.x() - width, point1.y()); //  = GetRandomPointF();
            gpointlist.append(point2);
            QPointF point3(point1.x() - width, point1.y() - width); // = GetRandomPointF();
            gpointlist.append(point3);

            mo.ControlPoints = gpointlist;
            break;
            }
        case 1 :
            {
            // Create a Multipoint
            mo.SymbolCode = QString("GFGPGLB-------X");
            QList<QPointF> gpointlist;
            QPointF point1 = GetRandomPointF();
            gpointlist.append(point1);
            QPointF point2(point1.x() - width, point1.y() - width); //  = GetRandomPointF();
            gpointlist.append(point2);
            QPointF point3(point1.x() - width, point1.y() + width); //  = GetRandomPointF();
            gpointlist.append(point3);

            mo.ControlPoints = gpointlist;
            break;
            }
        case 17 :
            {
            // Create a Multipoint (Area)
            mo.SymbolCode = QString("GNGPSAN-------X");
            QList<QPointF> gpointlist;
            QPointF point1 = GetRandomPointF();
            gpointlist.append(point1);
            QPointF point2(point1.x() + width, point1.y()); //  = GetRandomPointF();
            gpointlist.append(point2);
            QPointF point3(point1.x() - width, point1.y() - width); // = GetRandomPointF();
            gpointlist.append(point3);
            //gpointlist.append(point1);

            mo.ControlPoints = gpointlist;
            break;
            }
       case 18 :
           {
            // Create a single point
            QPointF point = GetRandomPointF();
            mo.ControlPoints.append(point);
            mo.SymbolCode = QString("GHMPNF--------X");
            break;
            }
        case 19 :
            {
            // Create a single point
            QPointF point = GetRandomPointF();
            mo.ControlPoints.append(point);
            mo.SymbolCode = QString("SHGPUCATM--E---");
            break;
            }
        default :
            {
            // Create a single point
            QPointF point = GetRandomPointF();
            mo.ControlPoints.append(point);
            mo.SymbolCode = QString("SFGPUCE---AA---");
            }
        }

        if (mo.SymbolCode.length() > 0)
        {
            list.append(mo);
            qList2525.append(mo);
            totalGraphics++;
        }
    }

    // send list off to the map
    if(m_pSampleMap != NULL)
    {
        m_pSampleMap->AddMapObjects(list);

        QString tempString;
        tempString.append("Total Graphics : ");
        tempString.append(QVariant(totalGraphics).toString());
        this->setWindowTitle(tempString);
    }
}

QPointF MainWindow::GetRandomPointF()
{
    if(m_pSampleMap != NULL)
    {
        QRectF envelope = m_pSampleMap->GetExtent();

        qreal tempx = ((envelope.right() - envelope.left()) * ((qreal)qrand() / (qreal)RAND_MAX)) + envelope.left();
        qreal tempy = ((envelope.top() - envelope.bottom()) * ((qreal)qrand() / (qreal)RAND_MAX)) + envelope.bottom();

        return QPointF(tempx, tempy);
    }

    return QPointF(0.0,0.0);
}

void MainWindow::readAppConfig()
{
    QFile file("./appconfig.xml");

    if (!file.exists())
      file.setFileName(APP_CONFIG_PATH);

    if (!file.open(QFile::ReadOnly | QFile::Text))
      return;

    QXmlStreamReader reader(&file);
    while (!reader.atEnd() && !reader.hasError())
    {
      while (reader.readNextStartElement())
      {
          //reader.readNext();

        QString elementName = reader.name().toString();

        if (QString::compare(elementName, "appconfig", Qt::CaseInsensitive) == 0)
        {
            continue;
        }
        QString elementText = reader.readElementText();
        qDebug() << "setting : " << elementName << "-" << elementText;
        m_pSettings->setValue(elementName,elementText);

      }
    }

    file.close();
}

void MainWindow::on_actionInset_Map_ShowHide_toggled(bool visible)
{
    if(m_pSampleMap)
    {
        const QVariant defaultValue;
        QSettings settings("ESRI","MapTestAppConfig");
        int insetmapdockEnumIndex = settings.value("insetmapdock", defaultValue).toInt();
        int insetmapsizeEnumIndex = settings.value("insetmapsize", defaultValue).toInt();

        m_pSampleMap->ShowInsetMap(visible,(MapElementDock)insetmapdockEnumIndex,(MapElementSize)insetmapsizeEnumIndex);
    }
}

void MainWindow::on_actionClear_All_triggered()
{
    if(m_pSampleMap)
    {
        QList<int> listIDs;
        foreach(MilitarySymbolObject mo, qList2525)
        {
            listIDs.append(mo.Id);
            totalGraphics--;

            QString tempString;
            tempString.append("MapTestApp : ");
            tempString.append(QVariant(totalGraphics).toString());
            this->setWindowTitle(tempString);

        }

        if(listIDs.count() > 0)
        {
            m_pSampleMap->DeleteMapObjects(listIDs);
        }
    }
}

void MainWindow::on_actionCompass_ShowHide_toggled(bool visible)
{
    if(m_pSampleMap)
    {
        const QVariant defaultValue;
        QSettings settings("ESRI","MapTestAppConfig");
        int compassrosedockEnumIndex = settings.value("compassrosedock", defaultValue).toInt();
        Q_UNUSED(compassrosedockEnumIndex)
        int compassrosesizeEnumIndex = settings.value("compassrosesize", defaultValue).toInt();

        m_pSampleMap->ShowMapCompass(visible, (MapElementSize)compassrosesizeEnumIndex);
    }
}

void MainWindow::on_actionCompassDockActionGroup_activated(int arg1)
{
    qDebug() << "Compass Dock action : " << arg1;
}

void MainWindow::on_actionScaleBar_toggled(bool visible)
{
    if(m_pSampleMap)
    {
        m_pSampleMap->ShowScalebar(visible);
    }
}

void MainWindow::on_actionReset_triggered()
{
    if(m_pSampleMap)
    {
        m_pSampleMap->Reset();
    }
}

void MainWindow::on_actionConfig_triggered()
{
    // open configuration dialog
    ConfigDialog dialog(m_pSampleMap);
    dialog.exec();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
  // allow close with Esc Key
  if (event->key() == Qt::Key_Escape)
    this->close();
  else
    QMainWindow::keyPressEvent(event);
}

void MainWindow::on_actionTest_Graphics_triggered()
{
    // TODO: add test method to interface for
    m_pSampleMap->TestReproCase();
}
