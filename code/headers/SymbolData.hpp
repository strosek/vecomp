/* Declaration of class Symbol */

#ifndef SYMBOLDATA_HPP
#define SYMBOLDATA_HPP

#include <string>

typedef enum
{
  TYPE_INTEGER,
  TYPE_FLOAT,
  TYPE_CHAR,
  TYPE_STRING,
  TYPE_BOOL,
  TYPE_VOID
} NativeType_t;

const char TYPECHAR_INTEGER = 'i';
const char TYPECHAR_FLOAT =   'f';
const char TYPECHAR_CHAR =    'c';
const char TYPECHAR_STRING =  's';
const char TYPECHAR_BOOL =    'b';
const char TYPECHAR_VOID =    'v';
const char TYPECHAR_INVALID = '~';

class SymbolData
{
public:
  SymbolData();

  void setDimensions(size_t dimensions);
  void setLine(size_t line);
  void setParameters(std::string& parameters);
  void setScope(std::string& scope);
  void setType(NativeType_t type);

  NativeType_t getType() const;
  size_t       getDimensions() const;
  size_t       getLine() const;
  std::string  getParameters() const;
  std::string  getScope() const;
private:
  NativeType_t type;
  size_t       dimensions;
  size_t       line;
  std::string  parameters;
  std::string  scope;
};

#endif /* SYMBOL_HPP included */

