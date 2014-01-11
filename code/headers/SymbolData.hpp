/* Declaration of class Symbol */

#ifndef SYMBOLDATA_HPP
#define SYMBOLDATA_HPP

typedef enum
{
  TYPE_INTEGER,
  TYPE_FLOAT,
  TYPE_CHAR,
  TYPE_STRING,
  TYPE_BOOL,
  TYPE_VOID
} NativeType_t;

class Symbol
{
public:
  SymbolData();
private:
  std::string  name;
  std::string  scope;
  NativeType_t type;
  size_t       line;
};

#endif /* SYMBOL_HPP included */

