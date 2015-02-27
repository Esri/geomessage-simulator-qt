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

#ifndef GEOMESSAGE_H
#define GEOMESSAGE_H

#include <QString>

/*!
 * \class Geomessage represents a geomessage with its values.
 */
class Geomessage
{
public:
  explicit Geomessage();
  explicit Geomessage(QString name, QString messageId, QString messageAction, QString symbolId,
                      QString type);
  QString name();
  QString messageId();
  QString messageAction();
  QString symbolId();
  QString type();

private:
  QString m_name;
  QString m_messageId;
  QString m_messageAction;
  QString m_symbolId;
  QString m_type;
  
};

#endif // GEOMESSAGE_H
