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

#include "Geomessage.h"

const QString Geomessage::TAG_MESSAGE_ELEMENT = "geomessage";
const QString Geomessage::TAG_MESSAGE_TYPE = "_type";
const QString Geomessage::TAG_MESSAGE_ACTION = "_action";
const QString Geomessage::TAG_MESSAGE_ID = "_id";
const QString Geomessage::TAG_SIC = "sic";
const QString Geomessage::TAG_UNIQUE_DESIG = "uniquedesignation";
const QString Geomessage::TAG_CONTROL_POINTS = "_control_points";
const QString Geomessage::TAG_WKID = "_wkid";

Geomessage::Geomessage()
{
}

Geomessage::Geomessage(QString messageType, QString messageAction, QString messageId)
{
  m_properties[TAG_MESSAGE_TYPE] = messageType;
  m_properties[TAG_MESSAGE_ACTION] = messageAction;
  m_properties[TAG_MESSAGE_ID] = messageId;
}

Geomessage::Geomessage(QString messageType, QString messageAction, QString messageId, QString uniqueDesignation,
                       QString symbolId)
{
  m_properties[TAG_MESSAGE_TYPE] = messageType;
  m_properties[TAG_MESSAGE_ACTION] = messageAction;
  m_properties[TAG_MESSAGE_ID] = messageId;
  m_properties[TAG_UNIQUE_DESIG] = uniqueDesignation;
  m_properties[TAG_SIC] = symbolId;
}

Geomessage::Geomessage(QString messageType, QString messageAction, QString messageId, QString uniqueDesignation,
                       QString symbolId, QString wkid, QString controlPoints)
{
  m_properties[TAG_MESSAGE_TYPE] = messageType;
  m_properties[TAG_MESSAGE_ACTION] = messageAction;
  m_properties[TAG_MESSAGE_ID] = messageId;
  m_properties[TAG_UNIQUE_DESIG] = uniqueDesignation;
  m_properties[TAG_SIC] = symbolId;
  m_properties[TAG_CONTROL_POINTS] = controlPoints;
  m_properties[TAG_WKID] = wkid;
}

QVariantMap Geomessage::toVariantMap()
{
  return m_properties;
}

void Geomessage::setProperty(QString tag, QString value)
{
  if (m_properties.contains(tag))
    m_properties[tag] = value;
  else
    m_properties.insert(tag, QVariant(value));
}

QString Geomessage::getProperty(QString tag)
{
  if (m_properties.contains(tag))
    return m_properties[tag].toString();
  else
    return QString();
}

QString Geomessage::messageType()
{
  if (m_properties.contains(TAG_MESSAGE_TYPE))
    return m_properties[TAG_MESSAGE_TYPE].toString();
  else
    return QString();
}

QString Geomessage::messageAction()
{
  if (m_properties.contains(TAG_MESSAGE_ACTION))
    return m_properties[TAG_MESSAGE_ACTION].toString();
  else
    return QString();
}

QString Geomessage::messageId()
{
  if (m_properties.contains(TAG_MESSAGE_ID))
    return m_properties[TAG_MESSAGE_ID].toString();
  else
    return QString();
}

QString Geomessage::uniqueDesignation()
{
  if (m_properties.contains(TAG_UNIQUE_DESIG))
    return m_properties[TAG_UNIQUE_DESIG].toString();
  else
    return QString();
}

QString Geomessage::symbolId()
{
  if (m_properties.contains(TAG_SIC))
    return m_properties[TAG_SIC].toString();
  else
    return QString();
}

QString Geomessage::wkid()
{
  if (m_properties.contains(TAG_WKID))
    return m_properties[TAG_WKID].toString();
  else
    return QString();
}

QString Geomessage::controlPoints()
{
  if (m_properties.contains(TAG_CONTROL_POINTS))
    return m_properties[TAG_CONTROL_POINTS].toString();
  else
    return QString();
}

QString Geomessage::toXml()
{
  QString sXml;
  QXmlStreamWriter streamWriter(&sXml);

  toXml(streamWriter);

  return sXml;
}

void Geomessage::fromXml(const QString& sXml)
{ 
  QXmlStreamReader streamReader(sXml);
  streamReader.readNextStartElement();
  fromXml(streamReader);
}

void Geomessage::toXml(QXmlStreamWriter& writer) const
{
  writer.writeStartElement(TAG_MESSAGE_ELEMENT);

  QVariantMap::const_iterator it = m_properties.begin();
  while(it != m_properties.end())
  {
    QString tag = it.key();
    QVariant value = it.value();

    writer.writeTextElement(tag, value .toString());

    ++it;
  }

  writer.writeEndElement(); // TAG_MESSAGE_ELEMENT
}

void Geomessage::fromXml(QXmlStreamReader& reader)
{
  QString name;
  QString value;

  m_properties.clear();

  if (!readToTag(reader, TAG_MESSAGE_ELEMENT))
    return;

  reader.readNext();
  while (!reader.atEnd())
  {
    if (reader.isEndElement())
    {
        reader.readNext();
        break;
    }

    if (reader.isStartElement())
    {
      // element name = Tag = Key in Variant Map
      name = reader.name().toString();
      // TODO: figure out if case matters for tag name & convert case here if it does
      value = getReaderValue(reader);
      m_properties.insert(name, QVariant(value));
    }
    else
    {
      reader.readNext();
    }
  }
}

/* -------------------- serialization helper methods ---------------------- */
// TODO: Move these to a helper or ISerialization class

void Geomessage::skipUnknownElement(QXmlStreamReader& reader)
{
  reader.readNext();
  while (!reader.atEnd())
  {
    if (reader.isEndElement())
    {
      reader.readNext();
      break;
    }

    if (reader.isStartElement())
    {
      skipUnknownElement(reader);
    }
    else
    {
      reader.readNext();
    }
  }
}

QString Geomessage::getReaderValue(QXmlStreamReader& reader)
{
  QString text = reader.readElementText();
  if (reader.isEndElement())
      reader.readNext();

  return text;
}

void Geomessage::readToElementEnd(QXmlStreamReader& reader)
{
  reader.readNext();
  while (!reader.atEnd())
  {
    if (reader.isEndElement())
    {
      reader.readNext();
      break;
    }
    reader.readNext();
  }
}

bool Geomessage::readToTag(QXmlStreamReader& reader, const QString& tagName)
{
  bool tagFound = false;
  if (QStringRef::compare(reader.name(), tagName, Qt::CaseInsensitive) != 0)
  {
    reader.readNext();
    while (!reader.atEnd())
    {
      if (reader.isEndElement())
      {
          reader.readNext();
          break;
      }

      if (reader.isStartElement())
      {
        if (QStringRef::compare(reader.name(), tagName, Qt::CaseInsensitive) == 0)
        {
          tagFound = true;
          break;
        }
        else
        {
          skipUnknownElement(reader);
        }
      }
      else
      {
        reader.readNext();
      }
    }
  }
  else
  {
    tagFound = true;
  }

  return tagFound;
} // end readToTag


