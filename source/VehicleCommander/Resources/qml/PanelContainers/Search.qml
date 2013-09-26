import QtQuick 1.1
import "../Controls"
import "../Panels/Search"

Item
{
  signal performSearch(variant text)
  signal resultSelected(variant value)
  signal updateAffiliation(variant value)
  signal updateCategory(variant value)
  signal keywordSelected(variant value)
  signal userAppliedKeywordSelected(variant value)
  signal clearClicked()

  function clearUIContent()
  {
    textSearchPanel.clearText()
    browse.panel.affiliationPanel.selectedContent = "None"
    browse.panel.categoryPanel.selectedContent = "None"
    browse.panel.keywordsPanel.selectedContent = "0"
  }

  function handleResultSelected(value)
  {
    clearUIContent()
    resultSelected(value)
  }

  function handleClearClicked()
  {
    clearUIContent()
    clearClicked()
  }

  function handleUpdateUserAppliedKeywordList(newList)
  {
    browse.panel.keywordsPanel.userAppliedKeywordList = newList
  }

  function handleUpdateKeywordList(newList)
  {
    browse.panel.keywordsPanel.addKeywordsPanel.keywordList = newList
  }

  function handleUpdateResultList(newList)
  {
    resultsPanel.resultList = newList
  }

  function handleUpdateCategoryList(newList)
  {
    browse.panel.categoryPanel.categoryList = newList
  }

  function handleUpdateSearchCount(viewCount)
  {
    browse.panel.viewLabelText = viewCount + " Results"
  }

  function handleUpdateRecentList(newList)
  {
    recentPanel.recentList = newList
  }

  property Stack stack
  property Panel panel

  id: search
  objectName: "search"
  panel: searchPanel

  SearchPanel
  {
    id: searchPanel
    anchors.fill: parent
    stack: search.stack
    browsePanel: browse.panel
    textSearchPanel: textSearchPanel
    recentPanel: recentPanel
  }

  Browse
  {
    id: browse
    stack: search.stack
    resultsPanel: resultsPanel
    anchors.fill: parent
    Component.onCompleted:
    {
      performSearch.connect(search.performSearch)
      updateAffiliation.connect(search.updateAffiliation)
      updateCategory.connect(search.updateCategory)
      keywordSelected.connect(search.keywordSelected)
      userAppliedKeywordSelected.connect(search.userAppliedKeywordSelected)
      clearClicked.connect(search.handleClearClicked)
    }
  }

  TextSearchPanel
  {
    id: textSearchPanel
    stack: search.stack
    resultsPanel: resultsPanel
    anchors.fill: parent
    Component.onCompleted: performSearch.connect(search.performSearch)
  }

  RecentPanel
  {
    id: recentPanel
    stack: search.stack
    Component.onCompleted: recentSelected.connect(search.resultSelected)
  }

  ResultsPanel
  {
    id: resultsPanel
    stack: search.stack
    Component.onCompleted: resultSelected.connect(search.handleResultSelected)
  }
}
