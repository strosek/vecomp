/* Declaration of class CodeGenerator */

#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

struct classFormat
{

};

typedef enum
{
  
};

class CodeGenerator
{
public:
  CodeGenerator(const std::string& outputFileName);
  void writeClassFile();
private:
  struct classFormat m_classFormat;
};

#endif /* CODEGENERATOR_HPP */

