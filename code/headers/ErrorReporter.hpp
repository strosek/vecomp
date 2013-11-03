/* Declaration of class ErrorReporter */

#ifndef ERROR_REPORTER_HPP
#define ERROR_REPORTER_HPP

#include <fstream>

class ErrorReporter
{
public:
  ErrorReporter* getInstance();
  ErrorReporter* getInstance(const std::string& outFileName);

  void writeError(const std::string& message);
  void writeLexicalError(int state, char currentChar, const std::string& line,
                         const std::string& lexeme, int lineNo, int columnNo);
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
  ErrorReporter(const std::string& outFileName);
  ErrorReporter(const ErrorReporter& source);
  ~ErrorReporter();
  ErrorReporter& operator=(const ErrorReporter& rhs);

  void writeErrorsFileHeader();

  std::ofstream m_errorOut;
  std::string   m_outFileName;
  int           m_warnings;
  int           m_errors;
  int           m_maxErrors;

  ErrorReporter* m_instance;
};

#endif /* ERROR_REPORTER_HPP */

