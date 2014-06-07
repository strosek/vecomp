/* Definition of member functions of class CodeGenerator */

#include "../headers/CodeGenerator.hpp"

#include <sstream>

using namespace std;

CodeGenerator::CodeGenerator() :
  m_outputType(),
  m_outputFileName(),
  m_symbols(),
  m_labels(),
  m_operations(),
  m_parametersStack(),
  m_mainLabelNo(1)
{
}

CodeGenerator::CodeGenerator(OutputType_t outputType) :
  m_outputType(outputType),
  m_outputFileName(),
  m_symbols(),
  m_labels(),
  m_operations(),
  m_parametersStack(),
  m_mainLabelNo(1)
{
}

size_t CodeGenerator::getLastLabelNumber()
{
  return m_labels.size();
}

size_t CodeGenerator::getLastOperationNumber()
{
  return m_operations.size();
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

void CodeGenerator::addMainLabel()
{
  if (m_operations.size() == 0)
  {
    m_labels.push_back(1);
  }
  else
  {
    m_labels.push_back(m_operations.size());
  }
  m_mainLabelNo = getLastLabelNumber();
}

void CodeGenerator::setLabelValue(size_t labelNo, int value)
{
  m_labels.at(labelNo - 1) = value;
}

void CodeGenerator::addOperation(const string& mnemo, const string& op1,
                                 const string& op2)
{
  ostringstream builder;

  builder << mnemo << " " << op1 << ", " << op2;
  builder.flush();

  m_operations.push_back(builder.str());
}

void CodeGenerator::addOperation(const string& mnemo, const string& op1,
                                 int op2)
{
  ostringstream builder;

  builder << mnemo << " " << op1 << ", " << op2;
  builder.flush();

  m_operations.push_back(builder.str());
}

void CodeGenerator::addParameter(const string& parameter)
{
  m_parametersStack.push(parameter);
}

void CodeGenerator::generateParameters()
{
  while (!m_parametersStack.empty())
  {
    addOperation(MNE_STO, m_parametersStack.top(), "0");
    m_parametersStack.pop();
  }
}

void CodeGenerator::writeObjectFile()
{
  ofstream outputFile(m_outputFileName);

  size_t nSymbols = m_symbols.size();
  for (size_t i = 1UL; i <= nSymbols; ++i)
  {
    outputFile << m_symbols.at(i - 1) << endl;
  }
 
  size_t nLabels = m_labels.size();
  for (size_t i = 1UL; i <= nLabels; ++i)
  {
#ifdef DEBUG
    cout << "::: mainlabel: " << m_mainLabelNo << " i: " << i << endl;
#endif
    if (i != m_mainLabelNo)
    {
      outputFile << "_E" << i;
    }
    else
    {
      outputFile << "_P";
    }
    outputFile << ",I,I," << m_labels.at(i - 1) << ",0,#," << endl;
  }

  outputFile << "@" << endl;

  size_t nOperations = m_operations.size();
  for (size_t i = 0; i < nOperations; ++i)
  {
    outputFile << i + 1 << " " << m_operations.at(i) << endl;
  }
}

