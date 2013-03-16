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

#ifndef GEOMESSAGE_H
#define GEOMESSAGE_H

#include <QString>
#include <QVariantMap>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

/*!
 * \class Geomessage represents a geomessage with its values.
 */
class Geomessage
{
public:
  explicit Geomessage();
  explicit Geomessage(QString messageType, QString messageAction, QString messageId);
  explicit Geomessage(QString messageType, QString messageAction, QString messageId, QString uniqueDesignation,
                      QString symbolId);
  explicit Geomessage(QString messageType, QString messageAction, QString messageId, QString uniqueDesignation,
                      QString symbolId, QString wkid, QString controlPoints);

  // This Variant Map can be used directly with Runtime MessageProcessor
  QVariantMap toVariantMap();

  void setProperty(QString tag, QString value);
  QString getProperty(QString tag);

  // These methods are just for convenience & backward compatibility
  // They just call getProperty("Tag")
  QString messageType();
  QString messageAction();
  QString messageId();
  QString uniqueDesignation();
  QString symbolId();
  QString wkid();
  QString controlPoints();

  bool isEmpty() { return (m_properties.size() == 0); }

  // Serialization
  virtual void fromXml(QXmlStreamReader& reader);
  virtual void toXml(QXmlStreamWriter& writer) const;

  virtual QString toXml();
  virtual void fromXml(const QString& sXml);

  // Tag Names
  static const QString TAG_MESSAGE_ELEMENT;
  static const QString TAG_MESSAGE_TYPE;
  static const QString TAG_MESSAGE_ACTION;
  static const QString TAG_MESSAGE_ID;
  static const QString TAG_UNIQUE_DESIG;
  static const QString TAG_SIC;
  static const QString TAG_CONTROL_POINTS;
  static const QString TAG_WKID;

private:

  void skipUnknownElement(QXmlStreamReader& reader);
  QString getReaderValue(QXmlStreamReader& reader);
  void readToElementEnd(QXmlStreamReader& reader);
  bool readToTag(QXmlStreamReader& reader, const QString& tagName);

  // each message is simply a key(aka Tag)/value pair
  QVariantMap m_properties;
};

#endif // GEOMESSAGE_H
