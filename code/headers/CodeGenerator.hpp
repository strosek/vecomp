/* Declaration of class CodeGenerator */

#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

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
  OPC_PINTLN,
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
  CodeGenerator(const std::string& outputFileName, OutputType_t outputType);

  void writeClassFile();
  void translateSymbolsTable(const SymbolsTable& symbolsTable);
  void insertSymbol();
  void generateOperation(const std::string& mnemo, const std::string op);
  void generateOperation(const std::string& mnemo, const std::string op1,
                         const std::string op2);
private:
  OutputType_t outputType;

  std::ostringstream m_codeBuilder;
  unsigned int       m_labelCounter;
};

#endif /* CODEGENERATOR_HPP */

