/* Declaration of class ErrorReporter */

#ifndef ERROR_REPORTER_HPP
#define ERROR_REPORTER_HPP

#include <string>

// Forward declarations to avoid class dependency problems
class FileReader;
class Scanner;

#include "FileReader.hpp"
#include "TokenLexeme.hpp"
#include "Scanner.hpp"


class ErrorReporter
{
public:
  static ErrorReporter* getInstance();

  ErrorReporter& operator=(const ErrorReporter& rhs);

  void writeError(const std::string& message);
  void writeErrorWithPosition(const std::string& message);
  void writeLexicalError(int state, char currentChar,
                         const std::string& lexeme, int lineNo, int columnNo);
  void writeSyntaxError(const std::string& expectedLexeme);
  void writeSyntaxError(TokenType_t expectedToken);

  void setFileReader(FileReader* fileReader);
  void setScanner(Scanner* setScanner);
  void setMaxErrors(int maxErrors);
  unsigned int  getMaxErrors() const;
  unsigned int  getErrors() const;
  unsigned int  getWarnings() const;

private:
  static const int WIDTH_LEXEME =  15;
  static const int WIDTH_MESSAGE = 80;
  static const int WIDTH_LINE =   100;

  ErrorReporter();
  ErrorReporter(const ErrorReporter& source);
  ~ErrorReporter();

  unsigned int m_warnings;
  unsigned int m_errors;
  unsigned int m_maxErrors;
  FileReader*  m_fileReader;
  Scanner*     m_scanner;

  static ErrorReporter* m_instance;
};

#endif /* ERROR_REPORTER_HPP */

