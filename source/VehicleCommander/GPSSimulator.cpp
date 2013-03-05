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

#include "GPSSimulator.h"

// Default ctor.  To use simulation user must set gpx file the update interval
GPSSimulator::GPSSimulator() :
    m_PlaybackMultiplier(1),
    isStarted(false)
{
  timer = new QTimer(this);
  gpxReader = new QXmlStreamReader();
  connect(timer, SIGNAL(timeout()), this, SLOT(handleTimerEvent()));
}

//
// Populates the necessary components to run a gps simulation
//
GPSSimulator::GPSSimulator(QString fileName, int updateInterval) :
    m_TimerInterval(updateInterval),
    m_PlaybackMultiplier(1),
    isStarted(false)
{
  timer = new QTimer(this);
  gpxReader = new QXmlStreamReader();
  connect(timer, SIGNAL(timeout()), this, SLOT(handleTimerEvent()));

  if (!setGpxFile(fileName))
  {
    // raise error
    m_GpxFile.setFileName("");
  }
}

//
// dtor
//
GPSSimulator::~GPSSimulator()
{
  delete gpxReader;
  delete timer;
}

bool GPSSimulator::gotoNextPositionElement()
{
  while (!gpxReader->atEnd() && !gpxReader->hasError())
  {
    if (gpxReader->isStartElement())
    {
      if (gpxReader->name().compare("trkpt") == 0)
      {
        return true;
      }
    }

    gpxReader->readNext();
  }

  return false;
}

//
// GetNextPoint(QPointF&, QTime&) private method
//   - Convert the current gpx position to QPointF and QTime parmeters.
//
void GPSSimulator::getNextPoint(QPointF& point, QTime& time)
{
  if (!gotoNextPositionElement())
  {
    point.setX(0.0);
    point.setY(0.0);
    return;
  }

  // fetch the lat and lon attributes from the trkpt element
  QXmlStreamAttributes attrs = gpxReader->attributes();
  point.setY(attrs.value("lat").toString().toDouble());
  point.setX(attrs.value("lon").toString().toDouble());

  // if the new point is the same as the old point then trash it and try to get another.
  if (point == latestPoint)
  {
    gpxReader->readNext();
    getNextPoint(point, time);
  }

  // goto the start of the time child element
  gpxReader->readNextStartElement();

  while (gpxReader->name().compare("trkpt", Qt::CaseInsensitive) != 0 && !gpxReader->atEnd())
  {
    if (gpxReader->isStartElement())
    {
      if (gpxReader->name().compare("ele", Qt::CaseInsensitive) == 0)
      {
        // TODO: do something with the elevation
      }
      else if (gpxReader->name().compare("time", Qt::CaseInsensitive) == 0)
      {
        QString timeString = gpxReader->readElementText();
        int hours = timeString.section(":", 0, 0).right(2).toInt();
        int minutes = timeString.section(":", 1, 1).toInt();
        int seconds = timeString.section(":", 2, 2).left(2).toInt();
        time.setHMS(hours ,minutes, seconds, 0);
      }
    }

    gpxReader->readNext();
  }

  latestPoint.setX(point.x());
  latestPoint.setY(point.y());
}

//
// startSimulation() Public Method:
//   - Loads a GPX file into a stream reader
//   - Fetches the first 3 coordinates
//   - Starts a timer that performs interpolation and position updating
//
void GPSSimulator::startSimulation()
{
  // if the gpx file does not contain enough information to
  // interpolate on then cancel the simulation.
  if (!initializeInterpolationValues())
  {
    return;
  }

  // start the position update timer
  timer->start(m_TimerInterval);
  isStarted = true;
}

void GPSSimulator::pauseSimulation()
{
  timer->stop();
}

void GPSSimulator::resumeSimulation()
{
  timer->start();
}

bool GPSSimulator::isActive()
{
  return timer->isActive();
}

bool GPSSimulator::started()
{
  return isStarted;
}

//
// handleTimerEvent() Slot:
//   - increments the current time
//   - fetches new positions from the gpx file as necessary
//   - calculates and sets the current position and orientation
//
void GPSSimulator::handleTimerEvent()
{
  // update the current time
  currentTime = currentTime.addMSecs(timer->interval() * m_PlaybackMultiplier);

  // determine if a new position needs to be obtained from the gpx
  if (currentTime > segmentEndTime)
  {
    if (!updateInterpolationParameters())
    {
      gpxReader->clear();
      gpxReader->addData(gpxData);
      initializeInterpolationValues();

      return;
    }
  }

  // normalize the time across the current segment
  double val1 = static_cast<double>(segmentStartTime.msecsTo(currentTime));
  double val2 = static_cast<double>(segmentStartTime.msecsTo(segmentEndTime));
  double normalizedTime = val1 / val2;

  // get the interpolated position and orientation on the current
  // segment based on the normalized time.
  QPointF currentPosition = currentSegment.pointAt(normalizedTime);
  double currentOrientation = getInterpolatedOrientation(currentPosition, normalizedTime);

  emit positionUpdateAvailable(currentPosition, currentOrientation);
} // end HandleTimerEvent

//
// Populates all the internal values necessary to start the simulation.
//
bool GPSSimulator::initializeInterpolationValues()
{
  // fetch the first 3 points from the gpx feed to populate the
  // initial interpolation components.
  QPointF pt1;
  QPointF pt2;
  QPointF pt3;
  getNextPoint(pt1, segmentStartTime);
  getNextPoint(pt2, segmentEndTime);
  getNextPoint(pt3, nextSegmentEndTime);

  if (pt1.isNull() || pt2.isNull() || pt3.isNull())
  {
    return false;
  }

  // define the interpolation segments
  currentSegment.setPoints(pt1, pt2);
  nextSegment.setPoints(pt2, pt3);
  startOrientationDelta = 0;
  endOrientationDelta = currentSegment.angleTo(nextSegment);

  // normalize the orientation delta to be between -180 and 180
  if (endOrientationDelta > 180.0)
  {
    endOrientationDelta -= 360.0;
  }

  // define the current time as the first timestamp extracted from the gpx file
  currentTime = segmentStartTime;

  return true;
}

//
// implementation for smooth orientation transfer between segments.
// the smoothing is spread across the final 10% of the current segment
// and the first 10% of the next segment.
//
double GPSSimulator::getInterpolatedOrientation(const QPointF& currentPosition, double normalizedTime)
{
  QLineF segment;
  double transitionAngle;

  // interpolation of the first 10% of the segment
  if (normalizedTime < 0.1)
  {
    segment.setPoints(currentSegment.p1(), currentPosition);
    transitionAngle = ((startOrientationDelta / 2.0) * (((currentSegment.length() * 0.1) - segment.length()) / (currentSegment.length() * 0.1))) ;
    return currentSegment.angle() - transitionAngle;
  }
  // interpolation of the last 10% of the segment
  else if (normalizedTime > 0.9)
  {
    QPointF tempPt = currentSegment.pointAt(0.9);
    segment.setPoints(tempPt, currentPosition);
    transitionAngle = ((endOrientationDelta / 2.0) * segment.length()) / (currentSegment.length() * 0.1);

    return currentSegment.angle() + transitionAngle;
  }
  // no orientation interpolation needed, use the current segments angle
  else
  {
    return currentSegment.angle();
  }
}

//
// fetch the next coordinate in the gpx file and updates all the
// internal interpolation vars
//
bool GPSSimulator::updateInterpolationParameters()
{
  QPointF newPt;
  segmentStartTime = segmentEndTime;
  segmentEndTime = nextSegmentEndTime;
  getNextPoint(newPt, nextSegmentEndTime);

  // if there are no more points to get then notify simulation to start over
  if (newPt.isNull())
  {
    return false;
  }

  // discard the oldest segment and populate the newest segment.
  currentSegment = nextSegment;
  nextSegment.setPoints(currentSegment.p2(), newPt);

  // discard the oldest orientation delta and populate the newest
  startOrientationDelta = endOrientationDelta;
  endOrientationDelta = currentSegment.angleTo(nextSegment);

  // normalize the new orientationDelta to be between -180 and 180
  if (endOrientationDelta > 180.0)
  {
    endOrientationDelta -= 360.0;
  }

  return true;
}

//
// getter for the gpx file location
//
QString GPSSimulator::gpxFile()
{
  return m_GpxFile.fileName();
}

//
// setter for the gpx file location
//
bool GPSSimulator::setGpxFile(QString fileName)
{
  if (!QFile::exists(fileName))
    return false;

  if (m_GpxFile.isOpen())
    m_GpxFile.close();

  m_GpxFile.setFileName(fileName);

  if (!m_GpxFile.open(QFile::ReadOnly | QFile::Text))
    return false;

  gpxData = m_GpxFile.readAll();
  gpxReader->clear();
  gpxReader->addData(gpxData);
  m_GpxFile.close();

  isStarted = false;

  return true;
}

//
// getter for the simulation timers's polling interval
//
int GPSSimulator::timerInterval()
{
  return m_TimerInterval;
}

//
// setter for the simulation timers's polling interval
//
void GPSSimulator::setTimerInterval(int ms)
{
  m_TimerInterval = ms;
}

//
// getter for the playback multiplier
//
int GPSSimulator::playbackMultiplier()
{
  return m_PlaybackMultiplier;
}

//
// setter for the playback modifier.  Used if
// gpx timestamps are either too close or two far
//
void GPSSimulator::setPlaybackMultiplier(int val)
{
  m_PlaybackMultiplier = val;
}
