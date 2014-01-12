/* Declaration of class Symbol */

#ifndef FUNCTIONDATA_HPP
#define FUNCTIONDATA_HPP

#include "SymbolData.hpp"

#include <string>

class FunctionData : public SymbolData 
{
public:
  FunctionData();

private:
  std::string parameters;
};

#endif /* FUNCTIONDATA_HPP included */

