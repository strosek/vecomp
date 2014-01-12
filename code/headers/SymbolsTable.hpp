#ifndef SYMBOLSTABLE_HPP
#define SYMBOLSTABLE_HPP

#include "SymbolData.hpp"

#include <map>

class SymbolsTable
{
public:
  SymbolsTable();

  const SymbolData& search(std::string name, SymbolData data) const;

private:
  std::multimap<std::string, SymbolData> m_symbolsMap;
};

#endif /* SYMBOLSTABLE_HPP included */

