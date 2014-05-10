/* Declaration of class CodeGenerator */

#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

#include <string>
#include <vector>

#include "SymbolsTable.hpp"

typedef enum
{
  PL0 = 0,
  JBC
} OutputType_t;

typedef enum
{
  OPC_END = 0,
  OPC_RETURN,
  OPC_ADD,
  OPC_SUBS,
  OPC_MULT,
  OPC_DIV,
  OPC_MOD,
  OPC_EXP,
  OPC_MINUS,
  OPC_LT,
  OPC_GT,
  OPC_LE,
  OPC_GE,
  OPC_DIF,
  OPC_EQ,
  OPC_AND,
  OPC_OR,
  OPC_NOT,
  OPC_CLS,
  OPC_READ,
  OPC_PRINT,
  OPC_PRINTLN,
} Pl0OperationCode_t;

const std::string MNE_OPR = "OPR";
const std::string MNE_LIT = "LIT";
const std::string MNE_LOD = "LOD";
const std::string MNE_STO = "STO";
const std::string MNE_JMP = "JMP";
const std::string MNE_JMC = "JMC";
const std::string MNE_CAL = "CAL";

class CodeGenerator
{
public:
  CodeGenerator();
  CodeGenerator(OutputType_t outputType);

  void   writeObjectFile();
  size_t getLastLabelNumber();
  void   setOutputType(OutputType_t type);
  void   setOutputFileName(const std::string& fileName);
  void   translateSymbolsTable(SymbolsTable& symbolsTable);
  void   addLabel();
  void   addOperation(const std::string& mnemo, const std::string& op1,
                      const std::string& op2);
  void   addOperation(const std::string& mnemo, const std::string& op1,
                      int op2);
private:
  OutputType_t             m_outputType;
  std::string              m_outputFileName;
  std::vector<std::string> m_symbols;
  std::vector<int>         m_labels;
  std::vector<std::string> m_operations;
};

#endif /* CODEGENERATOR_HPP */

