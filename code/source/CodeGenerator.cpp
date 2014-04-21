/* Definition of member functions of class CodeGenerator */

#include "../headers/CodeGenerator.hpp"

#include <sstream>

using namespace std;

CodeGenerator::CodeGenerator(const string& outputFileName,
                             OutputType_t outputType)
{
  m_outputFileName = outputFileName;
  m_outputType = outputType;
}

void CodeGenerator::writeObjectFile()
{
  ofstream outputFile;
  outputFile << m_symbolsTable;
  // TODO: write labels and @ symbol
  for (int i = 0; i < m_operations.size(); ++i)
  {
    outputFile << m_operations.at(i);
  }
}

void CodeGenerator::translateSymbolsTable(const SymbolsTable& symbolsTable)
{
}

void CodeGenerator::generateOperation(const string& mnemo, const string& op1,
                                      const string& op2)
{
  ostringstream builder;

  builder << mnemo << " " << op1 << ", " << op2;
  builder.flush();

  m_operations.push_back(builder.str());
}

