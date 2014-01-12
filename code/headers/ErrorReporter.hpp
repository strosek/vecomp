/* Declaration of class ErrorReporter */

#ifndef ERROR_REPORTER_HPP
#define ERROR_REPORTER_HPP

#include <string>

// Forward declaration to avoid class dependency problems
class FileReader;

#include "FileReader.hpp"
#include "TokenLexeme.hpp"


class ErrorReporter
{
public:
  static ErrorReporter* getInstance();

  ErrorReporter& operator=(const ErrorReporter& rhs);

  void writeError(const std::string& message);
  void writeError(int line, int column, const std::string& lexeme, 
                  const std::string& message);
  void writeLexicalError(int state, char currentChar,
                         const std::string& lexeme, int lineNo, int columnNo);
  void writeSyntaxError(const std::string& expectedLexeme,
                        const std::string& actualLexeme, int line, int column);
  void writeSyntaxError(TokenType_t expectedToken, TokenType_t actualToken,
                        const std::string& actualLexeme, int line, int column);
  void endErrorsFile();

  void setFileReader(FileReader* fileReader);
  void setMaxErrors(int maxErrors);
  int  getMaxErrors() const;
  int  getErrors() const;
  int  getWarnings() const;

private:
  static const int WIDTH_NUMBER =   8;
  static const int WIDTH_LEXEME =  15;
  static const int WIDTH_MESSAGE = 80;
  static const int WIDTH_LINE =   100;

  ErrorReporter();
  ErrorReporter(const ErrorReporter& source);
  ~ErrorReporter();

  void writeErrorsFileHeader();
  void writeErrorsFileSeparator();

  int          m_warnings;
  int          m_errors;
  int          m_maxErrors;
  FileReader * m_fileReader;

  static ErrorReporter* m_instance;
};

#endif /* ERROR_REPORTER_HPP */

