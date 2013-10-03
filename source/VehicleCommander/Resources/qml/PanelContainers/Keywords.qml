import QtQuick 1.1
import "../Controls"
import "../Panels/Search"

Item
{
  signal keywordSelected(variant value)
  signal userAppliedKeywordSelected(variant value)

  property Stack stack
  property Panel panel

  id: keywords
  panel: keywordsPanel

  KeywordsPanel
  {
    id: keywordsPanel
    anchors.fill: parent
    stack: keywords.stack
    addKeywordsPanel: addKeywordsPanel
    Component.onCompleted: userAppliedKeywordSelected.connect(keywords.userAppliedKeywordSelected)
  }

  AddKeywordsPanel
  {
    id: addKeywordsPanel
    stack: keywords.stack
    Component.onCompleted: keywordSelected.connect(keywords.keywordSelected)
  }
}
