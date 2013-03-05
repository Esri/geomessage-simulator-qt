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

#include "DictionaryImageProvider.h"

DictionaryImageProvider::DictionaryImageProvider() :
    QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
  dictionary = 0;
}

DictionaryImageProvider::DictionaryImageProvider(SymbolDictionary* inputDictionary) :
    QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
  dictionary = inputDictionary;
}

void DictionaryImageProvider::setDictionary(SymbolDictionary* newDictionary)
{
  dictionary = newDictionary;
}

QImage DictionaryImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
  if (!dictionary)
      return QImage();

  QString formattedID(id);
  formattedID = formattedID.remove('\n');

  int width = 150;
  int height = 150;
  if (requestedSize.isValid())
  {
    width = requestedSize.width();
    height = requestedSize.height();
  }

  QImage image = dictionary->symbolImage(formattedID, width, height);

  bool successful = !image.isNull();
  if (!successful)
    return QImage();

  if (size)
    *size = image.size();

  QImage requestedImage;
  if (requestedSize.width() > 0 && requestedSize.height() > 0)
    requestedImage = image.scaled(requestedSize);
  else
    requestedImage = image.copy(0, 0, image.width(), image.height());

  return requestedImage;
}
