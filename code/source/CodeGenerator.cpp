/* Definition of member functions of class CodeGenerator */

#include "../headers/CodeGenerator.hpp"

#include <sstream>

using namespace std;

CodeGenerator::CodeGenerator()
{
}

CodeGenerator::CodeGenerator(OutputType_t outputType) :
  m_outputType(outputType)
{
}

size_t CodeGenerator::getLastLabelNumber()
{
  return m_labels.size();
}

void CodeGenerator::setOutputType(OutputType_t type)
{
  m_outputType = type;
}

void CodeGenerator::setOutputFileName(const std::string& fileName)
{
  m_outputFileName = fileName;
}

void CodeGenerator::translateSymbolsTable(SymbolsTable& symbolsTable)
{
  for (multimap<string, SymbolData>::const_iterator it =
       symbolsTable.getSymbolsMap().cbegin();
       it != symbolsTable.getSymbolsMap().cend(); ++it)
  {
    ostringstream symbolBuilder;

    symbolBuilder << it->second.getName() << ",";

    if (it->second.isFunction())
    {
      symbolBuilder << "F,";
    }
    else
    {
      symbolBuilder << "V,";
    }

    switch (it->second.getType())
    {
    case TYPE_INTEGER :
      symbolBuilder << "E,";
      break;
    case TYPE_FLOAT :
      symbolBuilder << "R,";
      break;
    case TYPE_BOOL :
      symbolBuilder << "L,";
      break;
    case TYPE_STRING :
      symbolBuilder << "A,";
      break;
    case TYPE_VOID :
      symbolBuilder << "I,";
      break;
    case TYPE_INVALID :
    default :
      cout << "invalid symbol type: " << it->second.getType() << endl;
      break;
    }

    if (it->second.getDimensions() > 1)
    {
      symbolBuilder << it->second.getDimensions() << "," <<
          it->second.getDimensions() << ",";
    }
    else
    {
      symbolBuilder << it->second.getDimensions() << ",0,";
    }
    symbolBuilder << "#,";

#ifdef DEBUG
    cout << symbolBuilder.str() << endl;
#endif
    m_symbols.push_back(symbolBuilder.str());
  }
}

void CodeGenerator::addLabel()
{
  m_labels.push_back(m_operations.size());
}

void CodeGenerator::addOperation(const string& mnemo, const string& op1,
                                 const string& op2)
{
  ostringstream builder;

  builder << mnemo << " " << op1 << ", " << op2;
  builder.flush();

  m_operations.push_back(builder.str());
}

void CodeGenerator::writeObjectFile()
{
  ofstream outputFile(m_outputFileName);

  size_t nSymbols = m_symbols.size();
  for (size_t i = 0UL; i < nSymbols; ++i)
  {
    outputFile << m_symbols.at(i) << endl;
  }
 
  size_t nLabels = m_labels.size();
  for (size_t i = 0; i < nLabels; ++i)
  {
    outputFile << "_E" << i << ",I,I," << m_labels.at(i) << ",0,#," << endl;
  }

  outputFile << "@" << endl;

  size_t nOperations = m_operations.size();
  for (size_t i = 0; i < nOperations; ++i)
  {
    outputFile << i + 1 << " " << m_operations.at(i) << endl;
  }
}

