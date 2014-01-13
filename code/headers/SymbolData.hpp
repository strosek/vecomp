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

protected:
  std::string  name;
  std::string  scope;
  NativeType_t type;
  size_t       line;
};

#endif /* SYMBOL_HPP included */

