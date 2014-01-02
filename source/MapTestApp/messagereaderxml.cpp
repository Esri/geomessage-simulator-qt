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

#include "messagereaderxml.h"

#include <QMessageBox>

MessageReaderXml::MessageReaderXml()
{
  m_msgLoaded = false;
}

bool MessageReaderXml::LoadMessageFile(EsriRuntimeQt::MessageProcessor messageProcessor,
                                       QString fileName)
{
  bool success = false;

  m_messageProcessor = messageProcessor;

  QFile messageFile(fileName);
  if (messageFile.exists() && !m_messageProcessor.isEmpty())
  {
    setMessageStream(messageFile);
    success = m_msgLoaded;
  }
  else
  {
    QMessageBox::warning(NULL, "Message File Not Found", messageFile.fileName());
    success = false;
  }

  return success;
}

void MessageReaderXml::addMessage(QVariantMap properties)
{
  if (properties.count() > 0)
  {
    EsriRuntimeQt::Message message;
    message.setProperties(properties);

    m_messageProcessor.processMessage(message);

    m_msgLoaded = true;
  }
}

void MessageReaderXml::setMessageStream(QFile& file)
{
  file.open(QFile::ReadOnly | QFile::Text);
  QXmlStreamReader reader(&file);
  setMessageStream(reader);
}

void MessageReaderXml::setMessageStream(const QString& stream)
{
  QXmlStreamReader reader(stream);
  setMessageStream(reader);
}

void MessageReaderXml::setMessageStream(QXmlStreamReader& reader)
{
  while (!reader.atEnd() && !reader.hasError())
  {
    if (reader.isStartElement())
    {
      if ((QStringRef::compare(reader.name(), "geomessages", Qt::CaseInsensitive) == 0) ||
          (QStringRef::compare(reader.name(), "messages", Qt::CaseInsensitive) == 0))
      {
        if (!readMessages(reader))
        {
          readToElementEnd(reader);
          continue;
        }
      } else { skipUnknownElement(reader); } // end if message
    } else { reader.readNext(); } // end if startElement
  } // end of while
}

bool MessageReaderXml::readMessages(QXmlStreamReader& reader)
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
      if ((QStringRef::compare(reader.name(), "geomessage", Qt::CaseInsensitive) == 0) ||
          (QStringRef::compare(reader.name(), "message", Qt::CaseInsensitive) == 0))
      {
        if (!readMessage(reader))
        {
          readToElementEnd(reader);
          continue;
        }
      }
      else
      {
        skipUnknownElement(reader);
      }
    } // end if isStartElement
    else
    {
      reader.readNext();
    } // end of if start element
  }

  return true;
}

bool MessageReaderXml::readMessage(QXmlStreamReader& reader)
{
  // each message is simply a key/value pair
  QVariantMap properties;
  QString name;
  QString value;
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
      // get the name of the element
      name = reader.name().toString();
      value = getReaderValue(reader);
      properties.insert(name, QVariant(value));
    } // end if isStartElement
    else
    {
      reader.readNext();
    } // end of if start element
  }

  // properties are now set & can be added to MessageProcessor
  addMessage(properties);

  return true;
}

void MessageReaderXml::skipUnknownElement(QXmlStreamReader& reader)
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

QString MessageReaderXml::getReaderValue(QXmlStreamReader& reader)
{
  QString text = reader.readElementText();
  if (reader.isEndElement())
      reader.readNext();

  return text;
}

void MessageReaderXml::readToElementEnd(QXmlStreamReader& reader)
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
