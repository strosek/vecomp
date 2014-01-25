/* Definition of member functions of class SymbolsTable */

#include "../headers/SymbolsTable.hpp"

#include <utility>

using namespace std;

SymbolsTable::SymbolsTable() :
  m_symbolsMap()
{
}

bool SymbolsTable::isPresent(string name, string parameters) const
{
  bool isFound = false;

  return isFound;
}

bool SymbolsTable::isPresent(string name, string scope, size_t dimensions,
                             NativeType_t type) const
{
  bool isFound = false;

  return isFound;
}

NativeType_t SymbolsTable::getType(string name, string scope) const
{
  NativeType_t type = TYPE_VOID;
  return type;
}

NativeType_t SymbolsTable::getFunctionType(string name, string parameters) const
{
  NativeType_t type = TYPE_VOID;
  return type;
}

size_t SymbolsTable::getDimensions(string name, string parameters) const
{
  size_t dimensions = 0;

  return dimensions;
}

void SymbolsTable::insert(string name, const SymbolData& data)
{
  m_symbolsMap.insert(make_pair(name, data));
}

