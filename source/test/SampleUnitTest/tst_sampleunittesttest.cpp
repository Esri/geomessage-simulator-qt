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

#include <QString>
#include <QtTest>
#include <iostream>

#include "Geomessage.h"

// Qt Geomessage Unit Test Class
class SampleUnitTestTest : public QObject
{
    Q_OBJECT
    
public:
    SampleUnitTestTest();
    
private Q_SLOTS:
    void testCaseFromXml();
    void testCaseToXml();
    void testCaseProperties();
    void testCaseGetSetProperties();
};

SampleUnitTestTest::SampleUnitTestTest()
{
}

void SampleUnitTestTest::testCaseFromXml()
{
  QString sampleMessage;
  QTextStream msgOut(&sampleMessage);

  msgOut << "<geomessage>"
    << "<_type>position_report</_type>"
    << "<_action>UPDATE</_action>"
    << "<_id>{8a1387f6-4059-423e-bb21-e3a67caa866b}</_id>"
    << "<_wkid>4326</_wkid>"
    << "<sic>SFGPEVAL-------</sic>"
    << "<_control_points>70.4572000156,34.4275000144</_control_points>"
    << "<uniquedesignation>3A2-002</uniquedesignation>"
    << "<quantity>1</quantity>"
    << "<direction>359</direction>"
    << "<type>M1126</type>"
    << "<datetimevalid>2012-07-26 00:00:00</datetimevalid>"
    << "<speed>100</speed>"
    << "<owningunit>2/3/A/2</owningunit>"
    << "<status911>0</status911>"
    << "<fuel_state>100</fuel_state>"
    << "<rel_info>0</rel_info>"
  << "</geomessage>";

  Geomessage geoMsg;
  geoMsg.fromXml(sampleMessage);

  QString expected = "position_report";
  QCOMPARE(geoMsg.messageType(), expected);
  expected = "UPDATE";
  QCOMPARE(geoMsg.messageAction(), expected);
  expected = "{8a1387f6-4059-423e-bb21-e3a67caa866b}";
  QCOMPARE(geoMsg.messageId(), expected);
  expected = "3A2-002";
  QCOMPARE(geoMsg.uniqueDesignation(), expected);
  expected = "SFGPEVAL-------";
  QCOMPARE(geoMsg.symbolId(), expected);
  expected = "70.4572000156,34.4275000144";
  QCOMPARE(geoMsg.controlPoints(), expected);
  expected = "4326";
  QCOMPARE(geoMsg.wkid(), expected);
  expected = "0";
  QCOMPARE(geoMsg.getProperty("status911"), expected);
}

void SampleUnitTestTest::testCaseToXml()
{
  QString messageType = "chemlight";
  QString messageAction = "UPDATE";
  QString messageId = "1234567890-0987654321";
  QString uniqueDesignation = "99999-9999-999999";
  QString symbolId = "Howitzer F";
  QString wkid = "1234";
  QString controlPoints = "10.123,-89.9876";

  Geomessage geoMsg(messageType, messageAction, messageId, uniqueDesignation,
                    symbolId, wkid, controlPoints);

  QString sampleMessage = geoMsg.toXml();

  QString expectedMessage;
  QTextStream msgOut(&expectedMessage);

  msgOut << "<geomessage><_action>UPDATE</_action>"
   << "<_control_points>10.123,-89.9876</_control_points>"
   << "<_id>1234567890-0987654321</_id><_type>chemlight</_type>"
   << "<_wkid>1234</_wkid><sic>Howitzer F</sic>"
   << "<uniquedesignation>99999-9999-999999</uniquedesignation>"
   << "</geomessage>";

  // If you want to output the XML returned:
  // std::cout << sampleMessage.toStdString().c_str() << std::endl;

  QCOMPARE(sampleMessage, expectedMessage);
}

void SampleUnitTestTest::testCaseProperties()
{
  QString messageType = "chemlight";
  QString messageAction = "UPDATE";
  QString messageId = "1234567890-0987654321";
  QString uniqueDesignation = "99999-9999-999999";
  QString symbolId = "Howitzer F";
  QString wkid = "1234";
  QString controlPoints = "10.123,-89.9876";

  Geomessage geoMsg(messageType, messageAction, messageId, uniqueDesignation,
                    symbolId, wkid, controlPoints);

  QString actual = geoMsg.messageType();
  QCOMPARE(actual, messageType);

  actual = geoMsg.messageAction();
  QCOMPARE(actual, messageAction);

  actual = geoMsg.messageId();
  QCOMPARE(actual, messageId);

  actual = geoMsg.uniqueDesignation();
  QCOMPARE(actual, uniqueDesignation);

  actual = geoMsg.symbolId();
  QCOMPARE(actual, symbolId);

  actual = geoMsg.controlPoints();
  QCOMPARE(actual, controlPoints);

  actual = geoMsg.wkid();
  QCOMPARE(actual, wkid);
}

void SampleUnitTestTest::testCaseGetSetProperties()
{
  Geomessage geoMsg;

  QString expectedTag = "TestProperty";
  QString expectedValue = "TestValue";

  geoMsg.setProperty(expectedTag, expectedValue);

  QString actualValue = geoMsg.getProperty(expectedTag);
  QCOMPARE(actualValue, expectedValue);

  // Test Setting New Value
  QString expectedValue2 = "TestValue2";
  geoMsg.setProperty(expectedTag, expectedValue2);

  // Test getting non-existant key/tag
  actualValue = geoMsg.getProperty("BOGUS_TAG");

  QVERIFY(actualValue.size() == 0);
}

QTEST_APPLESS_MAIN(SampleUnitTestTest)

#include "tst_sampleunittesttest.moc"
