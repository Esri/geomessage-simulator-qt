import QtQuick 1.1
import "StackObject.js" as StackObj

Item
{
  function addPanel(item)
  {
    if (!StackObj.isEmpty())
    {
      StackObj.top().visible = false
    }

    StackObj.push(item)
    item.visible = true
  }

  function removePanel()
  {
    if (!StackObj.isEmpty())
    {
      StackObj.pop().visible = false

      if (!StackObj.isEmpty())
      {
        StackObj.top().visible = true
      }
    }
  }

  function currentPanel()
  {
    if (!StackObj.isEmpty())
    {
      return StackObj.top()
    }
    else
    {
      return undefined
    }
  }

  function count()
  {
    return StackObj.count()
  }

  function getArray()
  {
    return StackObj.stackArray
  }

  Component.onCompleted:
  {
    console.log("Stack onCompleted")
  }
}
