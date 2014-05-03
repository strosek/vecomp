/* Definition of member functions of class SymbolsTable */

#include "../headers/SymbolsTable.hpp"

#include <utility>
#include <iomanip>

using namespace std;

SymbolsTable::SymbolsTable() :
  m_symbolsMap()
{
}

bool SymbolsTable::exists(const string& name)
{
  return m_symbolsMap.find(name) != m_symbolsMap.end();
}

void SymbolsTable::checkDeclared(const string& name, const string& scope)
{
#ifdef DEBUG
  cout << "checking declared: " << name << ", with scope: " << scope << endl;
#endif
  if (exists(name))
  {
    bool isFound = false;
    bool isFoundFunction = false;
    m_searchRange = m_symbolsMap.equal_range(name);
    multimap<string, SymbolData>::iterator it;
    for (it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
      if (!it->second.isFunction() &&
          (it->second.getScope().compare(scope) == 0 ||
           it->second.getScope().compare(scope) == 0))
      {
        isFound = true;
      }
      else if (it->second.isFunction())
      {
        isFoundFunction = true;
      }
    }

    if (!isFound && isFoundFunction)
    {
      m_errorReporter->writeErrorWithPosition(
          "variable no declarada, existe funcion con el mismo nombre");
    }
  }
  else
  {
    m_errorReporter->writeErrorWithPosition("variable no declarada");
  }
}

void SymbolsTable::checkFunctionDeclared(const string& name,
                                         const string& parameters)
{
#ifdef DEBUG
  cout << "checking function declared: " << name << ", with params: " <<
      parameters << endl;
#endif
  if (exists(name))
  {
    bool isFound = false;
    bool isFoundDifferent = false;
    m_searchRange = m_symbolsMap.equal_range(name);
    for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
#ifdef DEBUG
      cout << "checking parameter list: " << it->second.getParameters() <<
        ", is function: " << boolalpha << it->second.isFunction() << endl;
#endif
      if (it->second.getParameters().compare(parameters) == 0 &&
          it->second.isFunction())
      {
#ifdef DEBUG
        cout << "function found" << endl;
#endif
        isFound = true;
      }
      else if (it->second.isFunction())
      {
        isFoundDifferent = true;
      }
    }

    if (!isFound && isFoundDifferent)
    {
      m_errorReporter->writeErrorWithPosition(
          "funcion declarada con diferente lista de parametros");
    }
    else if (!isFound && !isFoundDifferent)
    {
      m_errorReporter->writeErrorWithPosition(
          "identificador no es funcion");
    }
  }
  else
  {
    m_errorReporter->writeErrorWithPosition("funcion no declarada");
  }
}

void SymbolsTable::checkDeclarable(const string& name,
                                   const string& scope)
{
  if (exists(name))
  {
    bool isFound = false;

    m_searchRange = m_symbolsMap.equal_range(name);
    for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
      if (it->second.getScope().compare(scope) == 0 &&
          !it->second.isFunction())
      {
        isFound = true;
        m_errorReporter->writeErrorWithPosition(
            "variable ya declarada en este alcance");
      }
    }
  }
}

void SymbolsTable::checkFunctionDeclarable(const string& name,
                                           const string& parameters)
{
  if (exists(name))
  {
    bool isFound = false;

    m_searchRange = m_symbolsMap.equal_range(name);
    for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
      if (it->second.getParameters().compare(parameters) == 0 &&
          it->second.isFunction())
      {
        isFound = true;
        m_errorReporter->writeErrorWithPosition(
            "function ya declarada con esa lista de parametros");
      }
    }
  }
}

void SymbolsTable::checkModifiable(const string& name, const string& scope)
{
#ifdef DEBUG
  cout << "::: checking modifiable: " << name << endl;
#endif
  if (exists(name))
  {
    bool isFound = false;

    m_searchRange = m_symbolsMap.equal_range(name);
    for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
      if ( it->second.getScope().compare(scope) == 0 &&
          !it->second.isFunction() && it->second.isConstant())
      {
        isFound = true;
      }
    }

    if (!isFound)
    {
      for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
           it != m_searchRange.second && !isFound; ++it)
      {
        if ( it->second.getScope().compare("global") == 0 &&
            !it->second.isFunction() && it->second.isConstant())
        {
          isFound = true;
        }
      }
    }

    if (isFound)
    {
      string errorMessage = "no se puede modificar constante: ";
      errorMessage += name;
      m_errorReporter->writeErrorWithPosition(errorMessage);
    }
  }
}
        
NativeType_t SymbolsTable::getVariableType(const string& name,
                                           const string& scope)
{
#ifdef DEBUG
  cout << "::: retrieving type of variable: " << name << endl;
#endif
  NativeType_t type = TYPE_INVALID;

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
#ifdef DEBUG
        cout << "::: found variable with type: " <<
            SymbolData::getTypeString(type) << endl;
#endif
      }
    }
    if (!isFound)
    {
      for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
           it != m_searchRange.second && !isFound; ++it)
      {
        if (it->second.getScope().compare("global") == 0)
        {
          isFound = true;
          type = it->second.getType();
#ifdef DEBUG
          cout << "::: found variable with type: " <<
              SymbolData::getTypeString(type) << endl;
#endif
        }
      }
    }
  }
  else
  {
#ifdef DEBUG
    cout << "::: variable not found" << endl;
#endif
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

size_t SymbolsTable::getDimensions(const string& name, const string& scope)
{
  size_t dimensions = 0;

  if (exists(name))
  {
    m_searchRange = m_symbolsMap.equal_range(name);

    bool isFound = false;
    for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
         it != m_searchRange.second && !isFound; ++it)
    {
      if (it->second.getScope().compare(scope) == 0)
      {
#ifdef DEBUG
        cout << "found dimensioned variable: " << name << ", with scope: " <<
            scope << ", dimensions: " << it->second.getDimensions() << endl;
#endif
        isFound = true;
        dimensions = it->second.getDimensions();
      }
    }

    if (!isFound)
    {
      for (multimap<string, SymbolData>::iterator it = m_searchRange.first;
           it != m_searchRange.second && !isFound; ++it)
      {
        if (it->second.getScope().compare("global") == 0)
        {
          isFound = true;
          dimensions = it->second.getDimensions();
        }
      }
    }
  }
#ifdef DEBUG
    cout << "returning nDimensions: " << dimensions << endl;
#endif

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
  if (data.isFunction())
  {
    checkFunctionDeclarable(name, data.getParameters());
  }
  else
  {
    checkDeclarable(name, data.getScope());
  }

  m_symbolsMap.insert(make_pair(name, data));
}

void SymbolsTable::setErrorReporter(ErrorReporter* errorReporter)
{
  m_errorReporter = errorReporter;
}

multimap<string, SymbolData>& SymbolsTable::getSymbolsMap()
{
  return m_symbolsMap;
}

ostream& operator<<(ostream& out, const SymbolsTable& symbolsTable)
{
  static const size_t WIDTH_IDEN =   12;
  static const size_t WIDTH_NUMBER =  5;
  static const size_t WIDTH_TYPE =   12;
  static const size_t WIDTH_PARAMS =  8;
  static const size_t WIDTH_BOOL =    6;

  out << "------------------------------------------------------------------\n";
  out << setw(WIDTH_IDEN) << "NAME" <<
      setw(WIDTH_IDEN) << "SCOPE" <<
      setw(WIDTH_TYPE) << "TYPE" <<
      setw(WIDTH_BOOL) << "FUNC" <<
      setw(WIDTH_PARAMS) << "PARAMS" <<
      setw(WIDTH_NUMBER) << "DIM" <<
      setw(WIDTH_BOOL) << "CONST" <<
      setw(WIDTH_NUMBER) << "LN" << "\n";
  out << "------------------------------------------------------------------\n";

  for (multimap<string, SymbolData>::const_iterator it = 
       symbolsTable.m_symbolsMap.cbegin();
       it != symbolsTable.m_symbolsMap.cend(); ++it)
  {
    out << setw(WIDTH_IDEN) << it->second.getName() <<
        setw(WIDTH_IDEN) << it->second.getScope() <<
        setw(WIDTH_TYPE) << SymbolData::getTypeString(it->second.getType()) <<
        setw(WIDTH_BOOL) << boolalpha << it->second.isFunction() <<
        setw(WIDTH_PARAMS) << it->second.getParameters() <<
        setw(WIDTH_NUMBER) << it->second.getDimensions() <<
        setw(WIDTH_BOOL) << boolalpha << it->second.isConstant() <<
        setw(WIDTH_NUMBER) << it->second.getLine();
    out << "\n";
  }
  out.flush();

  return out;
}

