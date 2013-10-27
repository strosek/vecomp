/* Declaration of class ErrorReporter */

#ifndef ERROR_REPORTER_HPP
#define ERROR_REPORTER_HPP

#include <fstream>

class ErrorReporter
{
public:
  ErrorReporter();
  ErrorReporter(const std::string& outFileName);

  void writeError(const std::string& message);
  void writeLexicalError(int state, char currentChar, const std::string& line,
                         const std::string& lexeme, int lineNo, int columnNo);
private:
  static const int WIDTH_NUMBER =   8;
  static const int WIDTH_LEXEME =  15;
  static const int WIDTH_MESSAGE = 80;
  static const int WIDTH_LINE =   100;

  void writeErrorsFileHeader();

  std::ofstream m_errorOut;
  std::string   m_outFileName;
};

#endif /* ERROR_REPORTER_HPP */

