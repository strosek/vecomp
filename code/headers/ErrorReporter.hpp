/* Declaration of class ErrorReporter */

#ifndef ERROR_REPORTER_HPP
#define ERROR_REPORTER_HPP

#include <fstream>

// Forward declaration to avoid class dependency problems
class FileReader;
#include "FileReader.hpp"
#include "TokenLexeme.hpp"


class ErrorReporter
{
public:
  static ErrorReporter* getInstance(FileReader* fileReader);
  static ErrorReporter* getInstance(FileReader* fileReader,
                                    const std::string& outFileName);

  void writeError(const std::string& message);
  void writeLexicalError(int state, char currentChar, const std::string& line,
                         const std::string& lexeme, int lineNo, int columnNo);
  void writeSyntaxError(const std::string& expectedLexeme,
                        const std::string& actualLexeme, int line, int column);
  void writeSyntaxError(TokenType_t expectedToken, TokenType_t actualToken,
                        int line, int column);

  void setMaxErrors(int maxErrors);
  int  getMaxErrors() const;
  int  getErrors() const;
  int  getWarnings() const;
private:
  static const int WIDTH_NUMBER =   8;
  static const int WIDTH_LEXEME =  15;
  static const int WIDTH_MESSAGE = 80;
  static const int WIDTH_LINE =   100;

  ErrorReporter(FileReader* fileReader);
  ErrorReporter(FileReader* fileReader, const std::string& outFileName);
  ErrorReporter(const ErrorReporter& source);
  ~ErrorReporter();
  ErrorReporter& operator=(const ErrorReporter& rhs);

  void writeErrorsFileHeader();

  std::ofstream m_errorOut;
  std::string   m_outFileName;
  int           m_warnings;
  int           m_errors;
  int           m_maxErrors;
  FileReader*   m_fileReader;

  static ErrorReporter* m_instance;
};

#endif /* ERROR_REPORTER_HPP */

