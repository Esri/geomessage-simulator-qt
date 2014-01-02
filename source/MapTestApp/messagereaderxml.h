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
#ifndef MESSAGEREADERXML_H
#define MESSAGEREADERXML_H

#include "MessageProcessor.h"

#include <QFile>
#include <QVariantMap>
#include <QXmlStreamReader>

/**
 * @brief The MessageReaderXml class - loads an XML message file and passes the
 * elements to the Runtime Message Processor (which must be properly initialized
 * prior to using it with this class)
 */
class MessageReaderXml
{
public:
  explicit MessageReaderXml();

  bool LoadMessageFile(EsriRuntimeQt::MessageProcessor messageProcessor,
                       QString fileName);

private:

  void addMessage(QVariantMap properties);
  void setMessageStream(QFile& file);
  void setMessageStream(const QString& stream);
  void setMessageStream(QXmlStreamReader& reader);
  bool readMessages(QXmlStreamReader& reader);
  bool readMessage(QXmlStreamReader& reader);
  void skipUnknownElement(QXmlStreamReader& reader);
  QString getReaderValue(QXmlStreamReader& reader);
  void readToElementEnd(QXmlStreamReader& reader);

  EsriRuntimeQt::MessageProcessor m_messageProcessor;

  bool m_msgLoaded;

};

#endif // MESSAGEREADERXML_H
