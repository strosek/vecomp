/* Definition of member functions of class SymboData */

#include "../headers/SymbolData.hpp"

#ifdef DEBUG
# include <iostream>
#endif

using namespace std;

SymbolData::SymbolData() :
  m_type(),
  m_dimensions(0),
  m_line(1),
  m_parameters(),
  m_isFunction(false),
  m_scope()
{
}

void SymbolData::setDimensions(size_t dimensions)
{
  m_dimensions = dimensions;
}

void SymbolData::setLine(size_t line)
{
  m_line = line;
}

void SymbolData::setParameters(string& parameters)
{
  m_parameters = parameters;
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

string SymbolData::getTypeString(NativeType_t type)
{
  string typeString;
  switch (type)
  {
  case TYPE_VOID :
    typeString = "nada";
    break;
  case TYPE_INTEGER :
    typeString = "entero";
    break;
  case TYPE_FLOAT :
    typeString = "real";
    break;
  case TYPE_CHAR :
    typeString = "caracter";
    break;
  case TYPE_STRING :
    typeString = "alfabetico";
    break;
  case TYPE_BOOL :
    typeString = "logico";
    break;
  default :
#ifdef DEBUG
    cout << "typo de dato invalido" << endl;
#endif
    break;
  }

  return typeString;
}

