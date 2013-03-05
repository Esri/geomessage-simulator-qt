var stackArray = new Array()

function push(item)
{
  stackArray.push(item)
}

function pop()
{
  return stackArray.pop()
}

function top()
{
  return stackArray[stackArray.length - 1]
}

function isEmpty()
{
  return stackArray.length == 0
}

function count()
{
  return stackArray.length
}
