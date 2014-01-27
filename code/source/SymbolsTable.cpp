/* Definition of member functions of class SymbolsTable */

#include "../headers/SymbolsTable.hpp"

#include <utility>

using namespace std;

SymbolsTable::SymbolsTable() :
  m_symbolsMap()
{
}

bool SymbolsTable::isPresent(const string& name, const string& parameters)
{
  bool isFound = false;

  if (m_symbolsMap.find(name) != m_symbolsMap.end())
  {
    m_searchRange = m_symbolsMap.equal_range(name);
    for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
      if (it->second.getParameters().compare(parameters) == 0)
      {
        isFound = true;
      }
    }
  }

  return isFound;
}

bool SymbolsTable::isPresent(const string& name, const string& scope,
                             size_t dimensions, NativeType_t type)
{
  bool isFound = false;

  if (m_symbolsMap.find(name) != m_symbolsMap.end())
  {
    m_searchRange = m_symbolsMap.equal_range(name);
    for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
      if (it->second.getScope().compare(scope) == 0 &&
          it->second.getDimensions() == dimensions  &&
          it->second.getType() == type)
      {
        isFound = true;
      }
    }
  }

  return isFound;
}

NativeType_t SymbolsTable::getType(const string& name,
                                   const string& scope)
{
  NativeType_t type = TYPE_VOID;

  if (m_symbolsMap.find(name) != m_symbolsMap.end())
  {
    m_searchRange = m_symbolsMap.equal_range(name);

    bool isFound = false;
    for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
      if (it->second.getScope().compare(scope) == 0)
      {
        isFound = true;
        type = it->second.getType();
      }
    }
  }

  return type;
}

NativeType_t SymbolsTable::getFunctionType(const string& name,
                                           const string& parameters)
{
  NativeType_t type = TYPE_VOID;

  if (m_symbolsMap.find(name) != m_symbolsMap.end())
  {
    m_searchRange = m_symbolsMap.equal_range(name);

    bool isFound = false;
    for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
      if (it->second.getParameters().compare(parameters) == 0)
      {
        isFound = true;
        type = it->second.getType();
      }
    }
  }

  return type;
}

size_t SymbolsTable::getDimensions(const string& name,
                                   const string& scope)
{
  size_t dimensions = 0;

  if (m_symbolsMap.find(name) != m_symbolsMap.end())
  {
    m_searchRange = m_symbolsMap.equal_range(name);

    bool isFound = false;
    for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
      if (it->second.getScope().compare(scope) == 0)
      {
        isFound = true;
        dimensions = it->second.getDimensions();
      }
    }
  }

  return dimensions;
}

size_t SymbolsTable::getFunctionDimensions(const string& name,
                                           const string& parameters)
{
  size_t dimensions = 0;

  if (m_symbolsMap.find(name) != m_symbolsMap.end())
  {
    m_searchRange = m_symbolsMap.equal_range(name);

    bool isFound = false;
    for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
      if (it->second.getParameters().compare(parameters) == 0)
      {
        isFound = true;
        dimensions = it->second.getDimensions();
      }
    }
  }

  return dimensions;
}

void SymbolsTable::insert(const string& name, const SymbolData& data)
{
  m_symbolsMap.insert(make_pair(name, data));
}

