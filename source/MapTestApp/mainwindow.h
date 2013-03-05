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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "samplemap.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void handleMousePress(QMouseEvent mouseEvent);

    void on_actionPan_Up_triggered();

    void on_actionPan_Down_triggered();

    void on_actionPan_Left_triggered();

    void on_actionPan_Right_triggered();

    void on_actionScale_bar_toggled(bool arg1);

    void on_actionGrid_toggled(bool arg1);

    void on_actionZoom_triggered();

    void on_actionZoom_2_triggered();

    void on_actionRotate_triggered();

    void on_actionRotateMinus_triggered();

    void on_actionAdd_Random_Graphics_triggered();

    void on_actionClear_All_triggered();

    void on_actionTest_Graphics_triggered();

    void on_actionInset_Map_ShowHide_toggled(bool visible);

    void on_actionCompass_ShowHide_toggled(bool arg1);

    void on_actionCompassDockActionGroup_activated(int arg1);

    void on_actionScaleBar_toggled(bool arg1);

    void on_actionReset_triggered();

    void on_actionConfig_triggered();

private:
    // overrides
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::MainWindow *ui;
    QSettings* m_pSettings;

    SampleMap* m_pSampleMap;
    QList<MilitarySymbolObject> qList2525;

    QPointF GetRandomPointF();
    void readAppConfig();

    int graphicsIDCounter;
    double heading;
    double defaultScale;
    QRectF defaultExtent;
    int totalGraphics;
};

#endif // MAINWINDOW_H
