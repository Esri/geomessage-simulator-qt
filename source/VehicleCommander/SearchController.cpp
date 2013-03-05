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

#include "SearchController.h"
#include "DictionaryImageProvider.h"

SearchController::SearchController(SymbolDictionary* inputDictionary, QObject* parent) :
  dictionary(inputDictionary),
  QObject(parent)

{
  loadSpotReportCategories();
}

SearchController::~SearchController()
{
}

void SearchController::handleApplyCategory(QVariant category)
{
  if (category.toString().compare(currentSpotReportCategory) == 0)
    return;

  // Remove the old category filters
  QList<FilterObj> oldFilters = spotReportCategories[currentSpotReportCategory];
  foreach(FilterObj filter, oldFilters)
  {
    if (filter.filterName.compare("Keyword") == 0 &&
        !userAppliedKeywords.contains(filter.filterValue))
    {
        userAppliedKeywords.removeAll(filter.filterValue);
    }
    else
    {
        currentFilters[filter.filterName].removeAll(filter.filterValue);
    }

    }

  // Apply the new catagory filters
  QList<FilterObj> newFilters = spotReportCategories[category.toString()];
  foreach(FilterObj filter, newFilters)
  {
    if (filter.filterName.compare("Keyword") == 0)
    {
        userAppliedKeywords.append(filter.filterValue);
    }
    else
    {
        currentFilters[filter.filterName].append(filter.filterValue);
    }
  }

  currentSpotReportCategory = category.toString();

  updateKeywordList();
}

void SearchController::handleApplyKeyword(QVariant keyword)
{
  userAppliedKeywords.append(keyword.toString());

  updateKeywordList();

  emit newUserAppliedKeywordList(userAppliedKeywords);
}

void SearchController::handleClearSearchParameters()
{
  userAppliedKeywords.clear();

  updateKeywordList();

  currentSpotReportCategory = "";

  emit newUserAppliedKeywordList(userAppliedKeywords);

  QStringList results; // = dictionary->search();
  emit newResultCount(QString::number(results.count()));
}

void SearchController::handleResultSelected(QVariant resultName)
{
  // Remove the result if it is somewhere in the list and
  // replace it in the back of the list.
  recentSpotReportSearchResults.removeOne(resultName.toString());

  if (recentSpotReportSearchResults.count() > 8)
    recentSpotReportSearchResults.removeFirst();

  recentSpotReportSearchResults.append(resultName.toString());
  emit newRecentResultList(recentSpotReportSearchResults);

  handleClearSearchParameters();
}

void SearchController::handleSearch(QVariant searchText)
{
  qDebug() << "SearchText: " << searchText;

  updateSearch();

  if (currentSearchResults.size() > 0)
    emit newSearchResultList(currentSearchResults);

  emit newResultCount(QString::number(currentSearchResults.count()));
}

void SearchController::handleUpdateAffiliation(QVariant affiliation)
{
    userAppliedKeywords.clear();

    if (affiliation.toString().compare("All") != 0)
        userAppliedKeywords.append(affiliation.toString());

    updateSearch();
}

void SearchController::handleUserAppliedKeywordSelected(QVariant keyword)
{
    userAppliedKeywords.append(keyword.toString());

    updateKeywordList();
}

void SearchController::initController()
{
  QStringList categoriesList = spotReportCategories.keys();
  emit newCategoryList(categoriesList);

  updateKeywordList();
}

void SearchController::loadSpotReportCategories()
{
  QList<FilterObj> groundEquipmentVehicles;
  groundEquipmentVehicles.append(FilterObj("Appendix", "Appendix A"));
  groundEquipmentVehicles.append(FilterObj("StyleFile", "C2 UEI Ground Track Equipment"));
  groundEquipmentVehicles.append(FilterObj("Keyword", "Appendix A"));
  groundEquipmentVehicles.append(FilterObj("Keyword", "Warfighting Symbols"));
  groundEquipmentVehicles.append(FilterObj("Keyword", "Equipment"));
  groundEquipmentVehicles.append(FilterObj("Keyword", "Ground Track"));
  groundEquipmentVehicles.append(FilterObj("Keyword", "Ground Vehicle"));
  spotReportCategories["Ground Vehicles"] = groundEquipmentVehicles;

  QList<FilterObj> groundEquipmentWeapons;
  groundEquipmentWeapons.append(FilterObj("Appendix", "Appendix A"));
  groundEquipmentWeapons.append(FilterObj("StyleFile", "C2 UEI Ground Track Equipment"));
  groundEquipmentWeapons.append(FilterObj("Keyword", "Appendix A"));
  groundEquipmentWeapons.append(FilterObj("Keyword", "Warfighting Symbols"));
  groundEquipmentWeapons.append(FilterObj("Keyword", "Equipment"));
  groundEquipmentWeapons.append(FilterObj("Keyword", "Ground Track"));
  groundEquipmentWeapons.append(FilterObj("Keyword", "Weapon"));
  spotReportCategories["Ground Weapons"] = groundEquipmentWeapons;

  QList<FilterObj> groundEquipmentSpecialEquipment;
  groundEquipmentSpecialEquipment.append(FilterObj("Appendix", "Appendix A"));
  groundEquipmentSpecialEquipment.append(FilterObj("StyleFile", "C2 UEI Ground Track Equipment"));
  groundEquipmentSpecialEquipment.append(FilterObj("Keyword", "Appendix A"));
  groundEquipmentSpecialEquipment.append(FilterObj("Keyword", "Warfighting Symbols"));
  groundEquipmentSpecialEquipment.append(FilterObj("Keyword", "Equipment"));
  groundEquipmentSpecialEquipment.append(FilterObj("Keyword", "Ground Track"));
  groundEquipmentSpecialEquipment.append(FilterObj("Keyword", "Special"));
  spotReportCategories["Ground Special Equipment"] = groundEquipmentSpecialEquipment;

  QList<FilterObj> groundInstallations;
  groundInstallations.append(FilterObj("Appendix", "Appendix A"));
  groundInstallations.append(FilterObj("StyleFile", "C2 UEI Ground Track Installations.style"));
  groundInstallations.append(FilterObj("Keyword", "Appendix A"));
  groundInstallations.append(FilterObj("Keyword", "Warfighting Symbols"));
  groundInstallations.append(FilterObj("Keyword", "Ground Track"));
  groundInstallations.append(FilterObj("Keyword", "Installation"));
  spotReportCategories["Ground Installations"] = groundInstallations;

  QList<FilterObj> groundCombatUnits;
  groundCombatUnits.append(FilterObj("Appendix", "Appendix A"));
  groundCombatUnits.append(FilterObj("StyleFile", "C2 UEI Ground Track Units.style"));
  groundCombatUnits.append(FilterObj("Keyword", "Appendix A"));
  groundCombatUnits.append(FilterObj("Keyword", "Warfighting Symbols"));
  groundCombatUnits.append(FilterObj("Keyword", "Ground Track"));
  groundCombatUnits.append(FilterObj("Keyword", "Unit"));
  groundCombatUnits.append(FilterObj("Keyword", "Combat"));
  spotReportCategories["Ground Combat Units"] = groundCombatUnits;

  QList<FilterObj> groundCombatSupportUnits;
  groundCombatSupportUnits.append(FilterObj("Appendix", "Appendix A"));
  groundCombatSupportUnits.append(FilterObj("StyleFile", "C2 UEI Ground Track Units.style"));
  groundCombatSupportUnits.append(FilterObj("Keyword", "Appendix A"));
  groundCombatSupportUnits.append(FilterObj("Keyword", "Warfighting Symbols"));
  groundCombatSupportUnits.append(FilterObj("Keyword", "Ground Track"));
  groundCombatSupportUnits.append(FilterObj("Keyword", "Unit"));
  groundCombatSupportUnits.append(FilterObj("Keyword", "Support"));
  spotReportCategories["Ground Combat Support Units"] = groundCombatSupportUnits;

  QList<FilterObj> groundCombatServiceSupportUnits;
  groundCombatServiceSupportUnits.append(FilterObj("Appendix", "Appendix A"));
  groundCombatServiceSupportUnits.append(FilterObj("StyleFile", "C2 UEI Ground Track Units.style"));
  groundCombatServiceSupportUnits.append(FilterObj("Keyword", "Appendix A"));
  groundCombatServiceSupportUnits.append(FilterObj("Keyword", "Warfighting Symbols"));
  groundCombatServiceSupportUnits.append(FilterObj("Keyword", "Ground Track"));
  groundCombatServiceSupportUnits.append(FilterObj("Keyword", "Unit"));
  groundCombatServiceSupportUnits.append(FilterObj("Keyword", "Service Support"));
  spotReportCategories["Ground Combat Service Support Units"] = groundCombatServiceSupportUnits;
}

void SearchController::updateKeywordList()
{
  updateSearch();
  QStringList keywordList = dictionary->keywords();

  emit newKeywordList(keywordList);
}

void SearchController::updateSearch()
{
  QList<SymbolProperties> symbolList = dictionary->findSymbols(userAppliedKeywords, currentFilters);

  QStringList listKeyWords = dictionary->keywords();

  currentSearchResults.clear();

  QListIterator<SymbolProperties> it(symbolList);
  while (it.hasNext())
  {
    SymbolProperties symbolProps = it.next();

    currentSearchResults.append(symbolProps.name());

    // qDebug() << "Adding Symbol: " << symbolProps.name();
  }

  emit newResultCount(QString::number(symbolList.count()));
}
