/* Definition of member functions of class SymboData */

#include "../headers/SymbolData.hpp"

#ifdef DEBUG
# include <iostream>
#endif

using namespace std;

SymbolData::SymbolData() :
  m_name(),
  m_type(TYPE_INVALID),
  m_dimensions(0),
  m_line(1),
  m_parameters(),
  m_isFunction(false),
  m_isConstant(false),
  m_scope()
{
}

NativeType_t SymbolData::getStringType(const std::string& typeString)
{
  NativeType_t type = TYPE_INVALID;

  if (typeString.compare(TYPESTRING_INTEGER) == 0)
  {
    type = TYPE_INTEGER;
  }
  else if (typeString.compare(TYPESTRING_FLOAT) == 0)
  {
    type = TYPE_FLOAT;
  }
  else if (typeString.compare(TYPESTRING_STRING) == 0)
  {
    type = TYPE_STRING;
  }
  else if (typeString.compare(TYPESTRING_BOOL) == 0)
  {
    type = TYPE_BOOL;
  }
  else if (typeString.compare(TYPESTRING_CHAR) == 0)
  {
    type = TYPE_CHAR;
  }
  else if (typeString.compare(TYPESTRING_VOID) == 0)
  {
    type = TYPE_VOID;
  }

  return type;
}

NativeType_t SymbolData::getLiteralType(TokenType_t token)
{
  NativeType_t type = TYPE_VOID;

  switch (token)
  {
  case TOKEN_DEC :
  case TOKEN_HEX :
  case TOKEN_OCT :
    type = TYPE_INTEGER;
    break;
  case TOKEN_FLOAT:
    type = TYPE_FLOAT;
    break;
  case TOKEN_CHARCONST :
    type = TYPE_CHAR;
    break;
  case TOKEN_STRING :
    type = TYPE_STRING;
    break;
  case TOKEN_LOGICCONST :
    type = TYPE_BOOL;
    break;
  default :
#ifdef DEBUG
    cout << "error: invalid literal type" << endl;
#endif
    break;
  } 

  return type;
}

NativeType_t SymbolData::getCharType(char typeChar)
{
  NativeType_t type = TYPE_INVALID;

  switch (typeChar)
  {
  case TYPECHAR_INTEGER :
    type = TYPE_INTEGER;
    break;
  case TYPECHAR_FLOAT :
    type = TYPE_FLOAT;
    break;
  case TYPECHAR_CHAR :
    type = TYPE_CHAR;
    break;
  case TYPECHAR_STRING :
    type = TYPE_STRING;
    break;
  case TYPECHAR_BOOL :
    type = TYPE_BOOL;
    break;
  case TYPECHAR_VOID :
    type = TYPE_VOID;
    break;
  case TYPECHAR_INVALID :
    type = TYPE_INVALID;
    break;
  default :
#ifdef DEBUG
    cout << "error: invalid char type" << endl;
#endif
    break;
  } 

  return type;
}

char SymbolData::getTypeChar(NativeType_t type)
{
  char typeChar = TYPECHAR_INVALID;

  switch (type)
  {
  case TYPE_VOID :
    typeChar = TYPECHAR_VOID;
    break;
  case TYPE_INTEGER:
    typeChar = TYPECHAR_INTEGER;
    break;
  case TYPE_CHAR:
    typeChar = TYPECHAR_CHAR;
    break;
  case TYPE_FLOAT :
    typeChar = TYPECHAR_FLOAT;
    break;
  case TYPE_STRING :
    typeChar = TYPECHAR_STRING;
    break;
  case TYPE_BOOL :
    typeChar = TYPECHAR_BOOL;
    break;
  case TYPE_INVALID :
    typeChar = TYPECHAR_INVALID;
    break;
  default :
#ifdef DEBUG
    cout << "error: invalid native type: " << type << endl;
#endif
    break;
  }

  return typeChar;
}

string SymbolData::getTypeString(NativeType_t type)
{
  string typeString;
  switch (type)
  {
  case TYPE_VOID :
    typeString = TYPESTRING_VOID;
    break;
  case TYPE_INTEGER :
    typeString = TYPESTRING_INTEGER;
    break;
  case TYPE_FLOAT :
    typeString = TYPESTRING_FLOAT;
    break;
  case TYPE_CHAR :
    typeString = TYPESTRING_CHAR;
    break;
  case TYPE_STRING :
    typeString = TYPESTRING_STRING;
    break;
  case TYPE_BOOL :
    typeString = TYPESTRING_BOOL;
    break;
  case TYPE_INVALID :
    typeString = TYPESTRING_INVALID;
    break;
  default :
#ifdef DEBUG
    cout << "error: invalid native type: " << type << endl;
#endif
    break;
  }

  return typeString;
}

void SymbolData::setName(const string& name)
{
  m_name = name;
}

void SymbolData::setDimensions(size_t dimensions)
{
  m_dimensions = dimensions;
}

void SymbolData::setLine(size_t line)
{
  m_line = line;
}

void SymbolData::setParameters(const string& parameters)
{
  m_parameters = parameters;
}

void SymbolData::addParameter(NativeType_t parameter)
{
  m_parameters.push_back(getTypeChar(parameter));
}

void SymbolData::setScope(string& scope)
{
  m_scope = scope;
}

void SymbolData::setType(NativeType_t type)
{
  m_type = type;
}

void SymbolData::setIsFunction(bool isFunction)
{
  m_isFunction = isFunction;
}

void SymbolData::setIsConstant(bool isConstant)
{
  m_isConstant = isConstant;
}

void SymbolData::reset()
{
  m_name.clear();
  m_type = TYPE_INVALID;
  m_dimensions = 0;
  m_line = 1;
  m_parameters.clear();
  m_isFunction = false;
  m_isConstant = false;
  m_scope.clear();
}

string SymbolData::getName() const
{
  return m_name;
}

NativeType_t SymbolData::getType() const
{
  return m_type;
}

size_t SymbolData::getDimensions() const
{
  return m_dimensions;
}

size_t SymbolData::getLine() const
{
  return m_line;
}

string SymbolData::getParameters() const
{
  return m_parameters;
}

string SymbolData::getScope() const
{
  return m_scope;
}

bool SymbolData::isFunction() const
{
  return m_isFunction;
}

bool SymbolData::isConstant() const
{
  return m_isConstant;
}

