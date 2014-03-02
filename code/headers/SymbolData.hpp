/* Declaration of class Symbol */

#ifndef SYMBOLDATA_HPP
#define SYMBOLDATA_HPP

#include <string>

#include "TokenLexeme.hpp"

typedef enum
{
  TYPE_INTEGER,
  TYPE_FLOAT,
  TYPE_CHAR,
  TYPE_STRING,
  TYPE_BOOL,
  TYPE_VOID,
  TYPE_INVALID,
} NativeType_t;

const char TYPECHAR_INTEGER = 'i';
const char TYPECHAR_FLOAT =   'f';
const char TYPECHAR_CHAR =    'c';
const char TYPECHAR_STRING =  's';
const char TYPECHAR_BOOL =    'b';
const char TYPECHAR_VOID =    'v';
const char TYPECHAR_INVALID = '~';

const std::string TYPESTRING_INTEGER = "entero";
const std::string TYPESTRING_FLOAT =   "real";
const std::string TYPESTRING_CHAR =    "caracter";
const std::string TYPESTRING_STRING =  "alfabetico";
const std::string TYPESTRING_BOOL =    "logico";
const std::string TYPESTRING_VOID =    "nada";
const std::string TYPESTRING_INVALID = "invalido";

class SymbolData
{
public:
  SymbolData();

  static std::string  getTypeString(NativeType_t type);
  static NativeType_t getStringType(const std::string& typeString);
  static char         getTypeChar(NativeType_t type);
  static NativeType_t getCharType(char typeChar);
  static NativeType_t getLiteralType(TokenType_t typeChar);

  void setName(const std::string& name);
  void setDimensions(size_t dimensions);
  void setLine(size_t line);
  void setParameters(const std::string& parameters);
  void addParameter(NativeType_t parameter);
  void setScope(std::string& scope);
  void setType(NativeType_t type);
  void setIsFunction(bool isFunction);
  void setIsConstant(bool isConstant);
  void reset();

  std::string  getName() const;
  NativeType_t getType() const;
  size_t       getDimensions() const;
  size_t       getLine() const;
  std::string  getParameters() const;
  std::string  getScope() const;
  bool         isFunction() const;
  bool         isConstant() const;

private:
  std::string  m_name;
  NativeType_t m_type;
  size_t       m_dimensions;
  size_t       m_line;
  std::string  m_parameters;
  bool         m_isFunction;
  bool         m_isConstant;
  std::string  m_scope;
};

#endif /* SYMBOL_HPP included */

