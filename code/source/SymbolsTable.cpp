/* Definition of member functions of class SymbolsTable */

#include "../headers/SymbolsTable.cpp"

#include <utility>

SymbolsTable::SymbolsTable() :
  m_symbolsMap()
{
}

bool SymbolsTable::isPresent(string name, string parameters) const
{
  isFound = false;

  return isFound;
}

bool SymbolsTable::isPresent(string name, string scope, string dimensions,
                             NativeType_t type) const
{
  isFound = false;

  return isFound;
}

size_t SymbolsTable::getType() const
{
}

size_t SymbolsTable::getDimensions() const
{
}

void SymbolsTable::insert(string name, const SymbolData& data)
{
  m_symbolsMap.insert(make_pair(name, data));
}

