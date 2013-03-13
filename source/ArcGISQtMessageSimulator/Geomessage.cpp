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

#include "Geomessage.h"

Geomessage::Geomessage()
{
}

Geomessage::Geomessage(QString name, QString messageId, QString messageAction, QString symbolId,
                       QString type)
{
  this->m_name = name;
  this->m_messageId = messageId;
  this->m_messageAction = messageAction;
  this->m_symbolId = symbolId;
  this->m_type = type;
}

QString Geomessage::name()
{
  return m_name;
}

QString Geomessage::messageId()
{
  return m_messageId;
}

QString Geomessage::messageAction()
{
  return m_messageAction;
}

QString Geomessage::symbolId()
{
  return m_symbolId;
}

QString Geomessage::type()
{
  return m_type;
}
