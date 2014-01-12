/* Declaration of class Symbol */

#ifndef VARIABLEDATA_HPP
#define VARIABLEDATA_HPP 

#include "SymbolData.hpp"

#include <string>

class VariableData : public SymbolData 
{
public:
  FunctionData();

  int getDimensions() const;

private:
  int m_nDimensions;
};

#endif /* VARIABLEDATA_HPP included */

