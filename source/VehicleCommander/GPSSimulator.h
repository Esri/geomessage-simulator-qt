/*
 | Copyright 2012 Esri
 |
 | Licensed under the Apache License, Version 2.0 (the "License");
 | you may not use this file except in compliance with the License.
 | You may obtain a copy of the License at
 |
 |    http://www.apache.org/licenses/LICENSE-2.0
 |
 | Unless required by applicable law or agreed to in writing, software
 | distributed under the License is distributed on an "AS IS" BASIS,
 | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 | See the License for the specific language governing permissions and
 | limitations under the License.
 */

#ifndef GPSSIMULATOR_H
#define GPSSIMULATOR_H

#include <QDomDocument>
#include <QXmlStreamReader>
#include <QDomElement>
#include <QTimer>
#include <QFile>
#include <QLineF>
#include <QTime>
#include <QPointF>

class GPSSimulator : public QObject
{
  Q_OBJECT

public:

    GPSSimulator();
    GPSSimulator(QString fileName, int updateInterval = 20);
    ~GPSSimulator();

    void startSimulation();
    void pauseSimulation();
    void resumeSimulation();

    bool isActive();
    bool started();

    QString gpxFile();
    bool setGpxFile(QString fileName);

    int timerInterval();
    void setTimerInterval(int milliseconds);

    int playbackMultiplier();
    void setPlaybackMultiplier(int multiplier);

private:
    QFile m_GpxFile;
    QByteArray gpxData;
    QXmlStreamReader* gpxReader;
    QTimer* timer;
    int m_TimerInterval;
    int m_PlaybackMultiplier;
    QLineF currentSegment;
    QLineF nextSegment;
    QPointF latestPoint;
    double startOrientationDelta;
    double endOrientationDelta;
    QTime currentTime;
    QTime segmentStartTime;
    QTime segmentEndTime;
    QTime nextSegmentEndTime;
    bool isStarted;

    bool gotoNextPositionElement();
    void getNextPoint(QPointF& point, QTime& time);
    bool updateInterpolationParameters();
    bool initializeInterpolationValues();

    double getInterpolatedOrientation(const QPointF& currentPosition, double normalizedTime);

private slots:
    void handleTimerEvent();

signals:
    void positionUpdateAvailable(QPointF pos, double orientation);

};

#endif // GPSSIMULATOR_H
